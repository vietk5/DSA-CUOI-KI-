using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace BinarySearchTree
{
    public partial class MainForm : Form
    {
        // Lớp đại diện một nút trong cây
        public class FileNode
        {
            public string FileName { get; set; }
            public long FileSize { get; set; }
            public FileNode Left { get; set; }
            public FileNode Right { get; set; }
            public int Height { get; set; }

            public FileNode(string fileName, long fileSize)
            {
                FileName = fileName;
                FileSize = fileSize;
                Height = 1;
            }

            // So sánh hai nút dựa trên tên và kích thước tệp
            public override bool Equals(object obj)
            {
                if (obj is FileNode other)
                {
                    return this.FileName == other.FileName && this.FileSize == other.FileSize;
                }
                return false;
            }

            // Tạo mã băm dựa trên tên và kích thước tệp
            public override int GetHashCode()
            {
                return FileName.GetHashCode() ^ FileSize.GetHashCode();
            }
        }

        // Lớp quản lý cây tìm kiếm nhị phân (AVL Tree)
        public class FileSystemTree
        {
            private FileNode root;

            // Thêm một tệp mới vào cây
            public void InsertFile(string fileName, long fileSize)
            {
                root = InsertFileRecursive(root, fileName, fileSize);
            }

            // Hàm thêm nút (đệ quy)
            private FileNode InsertFileRecursive(FileNode node, string fileName, long fileSize)
            {
                if (node == null)
                    return new FileNode(fileName, fileSize);

                // Logic so sánh
                if (fileSize < node.FileSize || (fileSize == node.FileSize && string.Compare(fileName, node.FileName) < 0))
                    node.Left = InsertFileRecursive(node.Left, fileName, fileSize);
                else if (fileSize > node.FileSize || (fileSize == node.FileSize && string.Compare(fileName, node.FileName) > 0))
                    node.Right = InsertFileRecursive(node.Right, fileName, fileSize);
                else
                    return node;

                // Cập nhật chiều cao của nút
                UpdateHeight(node);

                // Kiểm tra cân bằng và thực hiện xoay nếu cần
                int balance = GetBalance(node);

                // Trường hợp mất cân bằng: xoay phải
                if (balance > 1 && (fileSize < node.Left.FileSize || (fileSize == node.Left.FileSize && string.Compare(fileName, node.Left.FileName) < 0)))
                    return RightRotate(node);


                // Trường hợp mất cân bằng: xoay trái
                if (balance < -1 && (fileSize > node.Right.FileSize || (fileSize == node.Right.FileSize && string.Compare(fileName, node.Right.FileName) > 0)))
                    return LeftRotate(node);

                // Mất cân bằng: xoay trái-phải
                if (balance > 1 && (fileSize > node.Left.FileSize || (fileSize == node.Left.FileSize && string.Compare(fileName, node.Left.FileName) > 0)))
                {
                    node.Left = LeftRotate(node.Left);
                    return RightRotate(node);
                }

                // Mất cân bằng: xoay phải-trái
                if (balance < -1 && (fileSize < node.Right.FileSize || (fileSize == node.Right.FileSize && string.Compare(fileName, node.Right.FileName) < 0)))
                {
                    node.Right = RightRotate(node.Right);
                    return LeftRotate(node);
                }

                return node;
            }

            // Cập nhật chiều cao của nút
            private void UpdateHeight(FileNode node)
            {
                node.Height = 1 + Math.Max(GetHeight(node.Left), GetHeight(node.Right));
            }

            // Lấy chiều cao của nút
            private int GetHeight(FileNode node)
            {
                return node == null ? 0 : node.Height;
            }

            // Tính toán hệ số cân bằng của nút
            private int GetBalance(FileNode node)
            {
                return node == null ? 0 : GetHeight(node.Left) - GetHeight(node.Right);
            }

            // Xoay phải
            private FileNode RightRotate(FileNode y)
            {
                FileNode x = y.Left;
                FileNode T2 = x.Right;

                x.Right = y;
                y.Left = T2;

                UpdateHeight(y);
                UpdateHeight(x);

                return x;
            }

            // Xoay trái
            private FileNode LeftRotate(FileNode x)
            {
                FileNode y = x.Right;
                FileNode T2 = y.Left;

                y.Left = x;
                x.Right = T2;

                UpdateHeight(x);
                UpdateHeight(y);

                return y;
            }

            // Xóa một tệp khỏi cây
            public void DeleteFile(string fileName)
            {
                var fileToDelete = SearchFilesByName(fileName);
                if (fileToDelete.Count == 0)
                {
                    MessageBox.Show("Tên tệp không tồn tại.", "Thông báo");
                    return;
                }

                root = DeleteFileRecursive(root, fileName);
            }

            // Hàm xóa nút (đệ quy)
            private FileNode DeleteFileRecursive(FileNode node, string fileName)
            {
                if (node == null)
                    return node;

                if (fileName.CompareTo(node.FileName) < 0)
                    node.Left = DeleteFileRecursive(node.Left, fileName);
                else if (fileName.CompareTo(node.FileName) > 0)
                    node.Right = DeleteFileRecursive(node.Right, fileName);
                else
                {
                    if (node.Left == null || node.Right == null)
                    {
                        return node.Left ?? node.Right;
                    }
                    else
                    {
                        FileNode minNode = MinValueNode(node.Right);
                        node.FileName = minNode.FileName;
                        node.FileSize = minNode.FileSize;
                        node.Right = DeleteFileRecursive(node.Right, minNode.FileName);
                    }
                }

                UpdateHeight(node);

                int balance = GetBalance(node);


                if (balance > 1 && GetBalance(node.Left) >= 0)
                    return RightRotate(node);
                    
                if (balance > 1 && GetBalance(node.Left) < 0)
                {
                    node.Left = LeftRotate(node.Left);
                    return RightRotate(node);
                }

                if (balance < -1 && GetBalance(node.Right) <= 0)
                    return LeftRotate(node);

                if (balance < -1 && GetBalance(node.Right) > 0)
                {
                    node.Right = RightRotate(node.Right);
                    return LeftRotate(node);
                }

                return node;
            }

            // Lấy nút có giá trị nhỏ nhất từ cây con
            private FileNode MinValueNode(FileNode node)
            {
                while (node.Left != null)
                    node = node.Left;
                return node;
            }

            // Trả về chiều cao của cây
            public int GetHeight()
            {
                return GetHeightRecursive(root);
            }

            // Hàm đệ quy lấy chiều cao cây
            private int GetHeightRecursive(FileNode node)
            {
                if (node == null)
                    return 0;

                int leftHeight = GetHeightRecursive(node.Left);
                int rightHeight = GetHeightRecursive(node.Right);

                return Math.Max(leftHeight, rightHeight) + 1;
            }

            // Duyệt cây theo thứ tự kích thước tệp
            public List<FileNode> GetFilesBySize()
            {
                List<FileNode> files = new List<FileNode>();
                GetFilesBySizeRecursive(root, files);
                return files;
            }

            private void GetFilesBySizeRecursive(FileNode node, List<FileNode> files)
            {
                if (node != null)
                {
                    GetFilesBySizeRecursive(node.Left, files);
                    files.Add(node);
                    GetFilesBySizeRecursive(node.Right, files);
                }
            }

            // Tìm kiếm tệp theo tên
            public List<FileNode> SearchFilesByName(string searchValue)
            {
                List<FileNode> matchingFiles = new List<FileNode>();
                SearchFilesByNameRecursive(root, searchValue.ToLower(), matchingFiles);
                return matchingFiles;
            }

            private void SearchFilesByNameRecursive(FileNode node, string searchValue, List<FileNode> matchingFiles)
            {
                if (node != null)
                {
                    if (node.FileName.ToLower().Contains(searchValue))
                    {
                        matchingFiles.Add(node);
                    }
                    SearchFilesByNameRecursive(node.Left, searchValue, matchingFiles);
                    SearchFilesByNameRecursive(node.Right, searchValue, matchingFiles);
                }
            }




            public FileNode GetRoot() => root;
        }

        private FileSystemTree fileSystemTree;
        private Label lblTreeHeight;
        private ListBox lstFiles;


        public MainForm()
        {
            InitializeComponent();
            fileSystemTree = new FileSystemTree();
            this.ClientSize = new Size(950, 700);

            lblTreeHeight = new Label();
            lblTreeHeight.Location = new Point(13, 73);
            lblTreeHeight.Size = new Size(200, 30);
            lblTreeHeight.Text = "Chiều cao cây: 0";
            this.Controls.Add(lblTreeHeight);

            lstFiles = new ListBox();
            lstFiles.Location = new Point(13, 110);
            lstFiles.Size = new Size(200, 200);
            this.Controls.Add(lstFiles);
        }

        private void RefreshFileList()
        {
            lstFiles.Items.Clear();
            List<FileNode> files = fileSystemTree.GetFilesBySize();
            foreach (var file in files)
            {
                lstFiles.Items.Add($"{file.FileName} - {FormatFileSize(file.FileSize)}");
            }
        }


        private void btnAddFile_Click(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(txtFileName.Text) && ParseFileSize(txtFileSize.Text, out long fileSize))
            {
                if (fileSystemTree.SearchFilesByName(txtFileName.Text).Find(f => f.FileName == txtFileName.Text) != null)
                {
                    MessageBox.Show("Tên tệp đã tồn tại. Vui lòng thử lại với tên khác.");
                    return;
                }

                fileSystemTree.InsertFile(txtFileName.Text, fileSize);

                lblTreeHeight.Text = "Chiều cao cây: " + fileSystemTree.GetHeight();
                RefreshFileList(); // Cập nhật danh sách tệp
                Invalidate(); // Làm mới cây tệp trên giao diện
                MessageBox.Show("Thêm tệp thành công.");
            }
            else
            {
                MessageBox.Show("Vui lòng nhập đầy đủ thông tin tệp hợp lệ.");
            }
        }



        private bool ParseFileSize(string sizeString, out long fileSize)
        {
            sizeString = sizeString.Trim().ToUpper();
            string[] parts = sizeString.Split(' ');

            if (parts.Length == 2 && long.TryParse(parts[0], out long sizeValue))
            {
                switch (parts[1])
                {
                    case "KB":
                        fileSize = sizeValue * 1024;
                        return true;
                    case "MB":
                        fileSize = sizeValue * 1024 * 1024;
                        return true;
                    case "GB":
                        fileSize = sizeValue * 1024 * 1024 * 1024;
                        return true;
                    default:
                        fileSize = 0;
                        return false;
                }
            }

            fileSize = 0;
            return false;
        }

        private string FormatFileSize(long fileSize)
        {
            if (fileSize >= 1024 * 1024 * 1024)
                return (fileSize / (1024.0 * 1024.0 * 1024.0)).ToString("0.##") + " GB";
            if (fileSize >= 1024 * 1024)
                return (fileSize / (1024.0 * 1024.0)).ToString("0.##") + " MB";
            if (fileSize >= 1024)
                return (fileSize / 1024.0).ToString("0.##") + " KB";
            return fileSize + " Bytes";
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            DrawTree(e.Graphics, fileSystemTree.GetRoot(), ClientSize.Width / 2, 80, 150);
            lblTreeHeight.Text = "Chiều cao cây: " + fileSystemTree.GetHeight();
        }

        private void DrawTree(Graphics g, FileNode node, int x, int y, int offset)
        {
            if (node == null) return;

            g.FillEllipse(Brushes.LightBlue, x - 20, y, 40, 40);
            g.DrawString(node.FileName, this.Font, Brushes.Black, x - 20, y);

            if (node.Left != null)
            {
                g.DrawLine(Pens.Black, x, y + 20, x - offset, y + 80);
                DrawTree(g, node.Left, x - offset, y + 80, offset / 2);
            }

            if (node.Right != null)
            {
                g.DrawLine(Pens.Black, x, y + 20, x + offset, y + 80);
                DrawTree(g, node.Right, x + offset, y + 80, offset / 2);
            }
        }

        private void btnSearchByName_Click(object sender, EventArgs e)
        {
            string searchValue = Microsoft.VisualBasic.Interaction.InputBox(
                "Nhập tên tệp cần tìm:",
                "Tìm kiếm tệp",
                "");

            if (!string.IsNullOrEmpty(searchValue))
            {
                List<FileNode> matchingFiles = fileSystemTree.SearchFilesByName(searchValue);

                if (matchingFiles.Count > 0)
                {
                    string result = "Tệp tìm thấy:\n";
                    foreach (var file in matchingFiles)
                    {
                        result += $"{file.FileName} - {FormatFileSize(file.FileSize)}\n";
                    }
                    MessageBox.Show(result, "Kết quả tìm kiếm");
                }
                else
                {
                    MessageBox.Show("Không tìm thấy tệp nào phù hợp.", "Thông báo");
                }
            }
        }

        private void btnDeleteFile_Click(object sender, EventArgs e)
        {
            string fileName = Microsoft.VisualBasic.Interaction.InputBox(
                "Nhập tên tệp cần xóa:",
                "Xóa tệp",
                "");

            if (string.IsNullOrEmpty(fileName)) return;

            var matchingFiles = fileSystemTree.SearchFilesByName(fileName);

            if (matchingFiles.Count == 0)
            {
                MessageBox.Show("Tệp không hợp lệ.", "Thông báo");
                return;
            }

            fileSystemTree.DeleteFile(fileName);

            if (fileSystemTree.SearchFilesByName(fileName).Count == 0)
            {
                lblTreeHeight.Text = "Chiều cao cây: " + fileSystemTree.GetHeight();
                RefreshFileList();
                Invalidate();
                MessageBox.Show("Xóa tệp thành công.");
            }
            else
            {
                MessageBox.Show("Tệp không hợp lệ.", "Thông báo");
            }
        }

    }
}