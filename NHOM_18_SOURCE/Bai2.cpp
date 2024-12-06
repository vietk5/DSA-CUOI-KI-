#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// Cấu trúc để lưu thông tin tệp
struct File {
    string name;
    double size;
};

// Cấu trúc cho nút của cây
struct TreeNode {
    File data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(File f) : data(f), left(nullptr), right(nullptr) {}
};

// Hàm chuyển đổi kích thước tệp từ chuỗi sang MB
int convertSizeToMB(const string& sizeStr) {
    stringstream ss(sizeStr);
    int size;
    string unit;

    ss >> size >> unit;

    if (unit == "MB") {
        return size;
    }
    else if (unit == "GB") {
        return size * 1024;
    }
    else if (unit == "KB") {
        return size / 1024;
    }

    return size;
}

// Hàm thêm phần tử vào cây (đệ quy)
TreeNode* insertRecursive(TreeNode* root, File f) {
    if (root == nullptr) {
        return new TreeNode(f);
    }
    if (f.name < root->data.name) {
        root->left = insertRecursive(root->left, f);
    }
    else {
        root->right = insertRecursive(root->right, f);
    }
    return root;
}

// Hàm thêm phần tử vào cây (không đệ quy)
TreeNode* insertIterative(TreeNode* root, File f) {
    TreeNode* newNode = new TreeNode(f);
    if (root == nullptr) {
        return newNode;
    }

    TreeNode* current = root;
    TreeNode* parent = nullptr;

    while (current != nullptr) {
        parent = current;
        if (f.name < current->data.name) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    if (f.name < parent->data.name) {
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }

    return root;
}

// Hàm xóa phần tử khỏi cây (đệ quy)
TreeNode* deleteNodeRecursive(TreeNode* root, string fileName) {
    if (root == nullptr) {
        return root;
    }

    if (fileName < root->data.name) {
        root->left = deleteNodeRecursive(root->left, fileName);
    }
    else if (fileName > root->data.name) {
        root->right = deleteNodeRecursive(root->right, fileName);
    }
    else {
        if (root->left == nullptr) {
            TreeNode* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            TreeNode* temp = root->left;
            delete root;
            return temp;
        }

        TreeNode* temp = root->right;
        while (temp && temp->left != nullptr) {
            temp = temp->left;
        }
        root->data = temp->data;
        root->right = deleteNodeRecursive(root->right, temp->data.name);
    }
    return root;
}

// Hàm xóa phần tử khỏi cây (không đệ quy)
TreeNode* deleteNodeIterative(TreeNode* root, string fileName) {
    TreeNode* parent = nullptr;
    TreeNode* current = root;

    while (current != nullptr && current->data.name != fileName) {
        parent = current;
        if (fileName < current->data.name) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    if (current == nullptr) {
        return root;
    }

    if (current->left == nullptr || current->right == nullptr) {
        TreeNode* child = current->left ? current->left : current->right;

        if (current == root) {
            delete current;
            return child;
        }

        if (parent->left == current) {
            parent->left = child;
        }
        else {
            parent->right = child;
        }
        delete current;
    }
    else {
        TreeNode* temp = current->right;
        TreeNode* tempParent = current;

        while (temp->left != nullptr) {
            tempParent = temp;
            temp = temp->left;
        }

        current->data = temp->data;

        if (tempParent->left == temp) {
            tempParent->left = temp->right;
        }
        else {
            tempParent->right = temp->right;
        }
        delete temp;
    }

    return root;
}

// Hàm tính chiều cao của cây (đệ quy)
int height(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    return max(leftHeight, rightHeight) + 1;
}

// Hàm chính
int main() {
    TreeNode* root = nullptr;
    int n;
    cout << "So luong tep: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        File f;
        string sizeStr;
        cout << "Ten tep " << (i + 1) << " : ";
        cin >> f.name;
        cout << "Kich thuoc tep" << (i + 1) << " : ";
        cin >> ws;
        getline(cin, sizeStr);
        f.size = convertSizeToMB(sizeStr);
        root = insertRecursive(root, f);
    }

    int choice;
    do {
        cout << "\nChon thao tac:\n";
        cout << "1. Them tep (De quy)\n";
        cout << "2. Them tep (Khong de quy)\n";
        cout << "3. Xoa tep (de quy)\n";
        cout << "4. Xoa tep (Khong de quy)\n";
        cout << "5. Tinh chieu cao cua cay\n";
        cout << "0. Thoat\n";
        cout << "Lua chon: ";
        cin >> choice;

        if (choice == 1) {
            File f;
            string sizeStr;
            cout << "Ten tep: ";
            cin >> f.name;
            cout << "Kich thuoc tep: ";
            cin >> ws;
            getline(cin, sizeStr);
            f.size = convertSizeToMB(sizeStr);
            root = insertRecursive(root, f);
        }
        else if (choice == 2) {
            File f;
            string sizeStr;
            cout << "Ten tep: ";
            cin >> f.name;
            cout << "Kich thuoc tep: ";
            cin >> ws;
            getline(cin, sizeStr);
            f.size = convertSizeToMB(sizeStr);
            root = insertIterative(root, f);
        }
        else if (choice == 3) {
            string fileName;
            cout << "Ten tep can xoa: ";
            cin >> fileName;
            root = deleteNodeRecursive(root, fileName);
        }
        else if (choice == 4) {
            string fileName;
            cout << "Ten tep can xoa: ";
            cin >> fileName;
            root = deleteNodeIterative(root, fileName);
        }
        else if (choice == 5) {
            cout << "Chieu cao cua cay: " << height(root) << endl;
        }
    } while (choice != 0);

    return 0;
}
