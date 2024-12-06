// #include <sstream>
// #include <iostream>
// #include <fstream>
// #include <string>
// #include <vector>
// #include <algorithm>
// #include <chrono>
// #include <thread>

#include <conio.h>
#include <bits/stdc++.h>

#define M 62989

using namespace std;

// Cấu trúc từ
struct Word
{
    string word; // Lưu từ tiếng Anh
    string type; // Lưu loại của từ
    string mean; // Lưu nghĩa của từ

    void Nhap()
    {
        cout << "Nhap tu tieng anh: ";
        getline(cin, word);
        cout << "Nhap loai tu: ";
        getline(cin, type);
        cout << "Nhap nghia tu moi (dung '-' de phan cach): ";
        getline(cin, mean);
        stringstream formattedMean;
        stringstream ss(mean);
        string meanPart;
        while (getline(ss, meanPart, '-'))
        {
            formattedMean << "- " << meanPart << "\n";
        }
        mean = formattedMean.str();
    }
};

// Cấu trúc node cho danh sách liên kết
struct Node
{
    Word word;
    Node *next;
};

// Tạo Node
Node *CreateNode(Word w)
{
    Node *p = new Node;
    p->word = w;
    p->next = NULL;
    return p;
}

// Khởi tạo bảng băm
void Init(Node *tudien[])
{
    // for (int i =0; i<M ; i++){
    //     tudien[i] = NULL;
    // }
    fill(tudien, tudien + M, nullptr);
}

// Hàm băm
int hashh(const string &en)
{
    // Duyệt từ đầu đến cuối từ tiếng anh
    // mỗi kí tự đổi ra mã ascii * (vị trí của kí tự đó trong từ) => tính tổng => modunlus M
    int h = 0;
    for (int i = 0; i < en.length(); i++)
    {
        h += int(tolower(en[i])) * (i + 1);
    }
    return h % M;
}

// Tạo hashtable tudien từ file .txt
void loadFromTextFile(Node *tudien[], const string &filetxt)
{
    ifstream file(filetxt);
    if (!file.is_open())
    {
        cout << "Khong the mo file .txt de doc cac tu.\n";
        return;
    }
    // Đọc dô từng dòng, mỗi dòng bao gồm 3 phần tuTiengAnh loaiTu nghia
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        Word w;
        // Lấy word
        getline(ss, w.word, ' ');
        // Lấy loại từ (type)
        getline(ss, w.type, ' ');
        // Lấy mean
        getline(ss, w.mean);
        // Định dạng lại nghĩa, thêm "- " trước mỗi nghĩa, mỗi từ có thể có nhiều nghĩa
        stringstream formattedMean;
        stringstream meanStream(w.mean);
        string meanPart;
        while (getline(meanStream, meanPart, '-'))
        {
            formattedMean << "- " << meanPart << "\n";
        }
        w.mean = formattedMean.str();

        // Thêm từ vào bảng băm
        int h = hashh(w.word);
        Node *p = CreateNode(w);
        if (tudien[h] == nullptr)
        {
            tudien[h] = p;
        }
        else
        {
            Node *current = tudien[h];
            while (current->next != nullptr)
            {
                current = current->next;
            }
            current->next = p;
        }
    }
    file.close();
    cout << "*** Da tai tu dien tu file .txt ***\n";
}

// Lưu một từ vào file .bin
void writeWordToFile(ofstream &file, const Word &w)
{
    size_t length;
    // Lưu word
    length = w.word.size();
    file.write(reinterpret_cast<const char *>(&length), sizeof(length));
    file.write(w.word.c_str(), length);

    // Lưu loại từ
    length = w.type.size();
    file.write(reinterpret_cast<const char *>(&length), sizeof(length));
    file.write(w.type.c_str(), length);

    // Lưu nghĩa từ
    length = w.mean.size();
    file.write(reinterpret_cast<const char *>(&length), sizeof(length));
    file.write(w.mean.c_str(), length);
}

// Lưu bảng băm vào file nhị phân
void saveHashTableToBinaryFile(Node *tudien[], const string &filename)
{
    ofstream file(filename, ios::binary);
    if (!file.is_open())
    {
        cout << "Khong the mo file de ghi.\n";
        return;
    }
    for (int i = 0; i < M; i++)
    {
        Node *current = tudien[i];
        size_t bucketSize = 0;
        // Đếm số phần tử trong bucket
        Node *temp = current;
        while (temp != nullptr)
        {
            bucketSize++;
            temp = temp->next;
        }

        // Ghi kích thước bucket
        file.write(reinterpret_cast<const char *>(&bucketSize), sizeof(bucketSize));

        // Ghi từng từ trong bucket
        while (current != nullptr)
        {
            writeWordToFile(file, current->word);
            current = current->next;
        }
    }
    file.close();
    cout << "\033[31mDa luu tu dien vao file nhi phan.\033[0m\n";
}

// Đọc bảng băm từ file nhị phân
void loadHashTableFromBinaryFile(Node *tudien[], const string &filename)
{
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        cout << "Khong the mo file.\n";
        return;
    }

    for (int i = 0; i < M; i++)
    {
        size_t bucketSize;
        file.read(reinterpret_cast<char *>(&bucketSize), sizeof(bucketSize));

        for (size_t j = 0; j < bucketSize; j++)
        {
            Word w;
            size_t length;

            // Đọc từ tiếng anh
            file.read(reinterpret_cast<char *>(&length), sizeof(length));
            w.word.resize(length);
            file.read(&w.word[0], length);

            // Đọc loại từ
            file.read(reinterpret_cast<char *>(&length), sizeof(length));
            w.type.resize(length);
            file.read(&w.type[0], length);

            // Đọc nghĩa từ
            file.read(reinterpret_cast<char *>(&length), sizeof(length));
            w.mean.resize(length);
            file.read(&w.mean[0], length);

            // Thêm từ vào bảng băm
            Node *p = CreateNode(w);
            if (tudien[i] == nullptr)
            {
                tudien[i] = p;
            }
            else
            {
                Node *current = tudien[i];
                while (current->next != nullptr)
                {
                    current = current->next;
                }
                current->next = p;
            }
        }
    }
    file.close();
    cout << "\033[31mDa tai tu dien thanh cong. \033[0m\n";
}

// Thêm từ vào bảng băm
void addWord(Node *tudien[], const Word &w, const string &filename)
{
    int h = hashh(w.word);
    Node *current = tudien[h];

    // Kiểm tra trùng lặp
    while (current != nullptr)
    {
        if (current->word.word == w.word)
        {
            cout << "\033[31mTu nay da ton tai trong tu dien.\033[0m\n";
            return;
        }
        current = current->next;
    }
    // Thêm từ mới
    Node *p = CreateNode(w);
    p->next = tudien[h];
    tudien[h] = p;
    cout << "\n";
    // Lưu cập nhật vào file
    cout << "\033[31mDa them tu vao tu dien.\n\033[0m";
    saveHashTableToBinaryFile(tudien, filename);
}
// Tìm kiếm từ trong bảng băm
void searchWord(Node *tudien[], const string &eng)
{
    // Chuyển từ cần tìm kiếm thành chữ thường
    string searchKey = eng;
    transform(searchKey.begin(), searchKey.end(), searchKey.begin(), ::tolower);

    int h = hashh(searchKey);
    Node *current = tudien[h];

    // Tìm kiếm trong bucket
    while (current != nullptr)
    {
        if (current->word.word == searchKey)
        {
            cout << "\nWord : " << current->word.word << endl;
            cout << "Type : " << current->word.type << endl;
            // cout << "Mean :\n"
            //      << current->word.mean << endl;
            cout << "Mean :\n"
                 << "\033[34m" << current->word.mean << "\033[0m" << endl;
            return;
        }
        current = current->next;
    }
    cout << "\n\033[31mKhong tim thay tu '" << eng << "' trong tu dien.\n\033[0m";
}
// Xóa từ ra khỏi từ điển
void deleteWord(Node *tudien[], const string &eng, const string &filename)
{
    string searchKey = eng;
    transform(searchKey.begin(), searchKey.end(), searchKey.begin(), ::tolower);

    int h = hashh(searchKey);
    Node *current = tudien[h];
    Node *prev = nullptr;

    while (current != nullptr)
    {
        if (current->word.word == searchKey)
        {
            if (prev == nullptr) // Xóa ở đầu bucket
            {
                tudien[h] = current->next;
            }
            else // Xóa ở giữa hoặc cuối bucket
            {
                prev->next = current->next;
            }
            delete current; // Giải phóng bộ nhớ
            cout << "\n\033[31mDa xoa tu '" << eng << "' khoi tu dien.\n\033[0m";
            saveHashTableToBinaryFile(tudien, filename);
            return;
        }
        prev = current;
        current = current->next;
    }
    cout << "\n\033[31mKhong tim thay tu '" << eng << "' de xoa.\033[0m";
}

// Chỉnh sửa từ
void editWord(Node *tudien[], const string &eng, const string &filename)
{
    string searchKey = eng;
    transform(searchKey.begin(), searchKey.end(), searchKey.begin(), ::tolower);
    int h = hashh(searchKey); // Băm từ khóa
    Node *current = tudien[h];
    Node *prev = nullptr;
    // Tìm kiếm từ cần chỉnh sửa
    while (current != nullptr)
    {
        if (current->word.word == searchKey)
        {
            // Xóa node hiện tại khỏi bucket
            if (prev == nullptr) // Nếu node cần xóa ở đầu bucket
            {
                tudien[h] = current->next;
            }
            else // Nếu node cần xóa ở giữa hoặc cuối bucket
            {
                prev->next = current->next;
            }
            delete current; // Giải phóng bộ nhớ của node cũ
            // Tạo từ mới và thêm lại vào bảng băm
            Node *newWord = new Node();
            cout << "\n\033[32mTim thay tu trong tu dien. Hay nhap thong tin moi:\033[0m\n";
            newWord->word.Nhap();
            // Băm lại từ mới và thêm vào bảng
            string newKey = newWord->word.word;
            transform(newKey.begin(), newKey.end(), newKey.begin(), ::tolower);
            int newHash = hashh(newKey);
            newWord->next = tudien[newHash];
            tudien[newHash] = newWord; // Thêm vào đầu bucket mới
            cout << "\n\033[32mDa cap nhat thong tin cua tu.\033[0m\n";
            // Lưu bảng băm vào file nhị phân
            saveHashTableToBinaryFile(tudien, filename);
            return;
        }
        prev = current;
        current = current->next;
    }
    cout << "\n\033[32mKhong tim thay tu '" << eng << "' de chinh sua.\033[0m\n";
}

// Lưu bảng băm vào file .txt
void saveHashTableToTextFile(Node *tudien[], const string &filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "Khong the mo file .txt de ghi.\n";
        return;
    }

    for (int i = 0; i < M; i++)
    {
        Node *current = tudien[i];
        while (current != nullptr)
        {
            // Ghi từ, loại từ và nghĩa
            file << current->word.word << " "
                 << current->word.type << " "
                 << current->word.mean;
            current = current->next;
        }
    }
    file.close();
    // cout << "\033[31mDa luu tu dien vao file .txt.\033[0m\n";
}

// Thanh laoding
void loadingBar()
{
    const int barWidth = 50;
    cout << "\033[31mLoading: [\033[0m";
    for (int i = 0; i <= barWidth; ++i)
    {
        cout << "\033[31m=\033[0m";
        cout.flush();
        for (volatile int j = 0; j < 10000000; ++j)
        {
        }
    }
    cout << "\033[31m] Done!\033[0m\n";
}
void wait(int seconds)
{
    auto start = chrono::steady_clock::now();
    while (chrono::steady_clock::now() - start < chrono::seconds(seconds))
    {
    }
}

// Hiện mật khẩu

string matkhau()
{
    string pwinput; // Mật khẩu nhập vào
    char ch;
    while (true)
    {
        ch = _getch(); // Lấy từng ký tự người dùng nhập
        if (ch == 13)
        { // Nếu người dùng nhấn Enter (mã ASCII 13)
            break;
        }
        else if (ch == 8)
        { // Nếu người dùng nhấn Backspace
            if (pwinput.length() > 0)
            {
                pwinput.pop_back(); // Xóa ký tự cuối
                cout << "\b \b";    // Xóa dấu '*'
            }
        }
        else
        {
            pwinput.push_back(ch); // Thêm ký tự vào mật khẩu
            cout << "*";           // In ra dấu '*'
        }
    }

    cout << endl; // In một dòng mới sau khi nhập mật khẩu
    return pwinput;
}

int main()
{
    Node *tudien[M];
    Init(tudien);

    string filetxt = "input.txt";
    string filebin = "InputTuDien.bin";

    // thanh chạy loading 
    loadingBar();
    cout << "\033[33mTao tu dien thanh cong !!!\n\033[0m";
    wait(2);

    // dọc dữ liệu từ file .txt, tạo bảng băm từ điển
    loadFromTextFile(tudien, filetxt);

    // lưu từ điển vào file .bin
    saveHashTableToBinaryFile(tudien, filebin);

    // Tải dữ liệu từ file .bin
    loadHashTableFromBinaryFile(tudien, filebin);

    string pw = "dsabai04";
    string pwinput = "";
    int choice;
    while (true)
    {
        system("cls");
        cout << "\033[32m------------PHAN MEM TU DIEN------------\033[0m\n";
        cout << "1. Tra cuu tu.\n";
        cout << "2. Them tu vao tu dien.\n";
        cout << "3. Chinh sua tu trong tu dien.\n";
        cout << "4. Xoa tu trong tu dien.\n";
        cout << "0. Thoat chuong trinh.\n";
        cout << "\033[32m---------------------------------------\033[0m\n";
        cout << "\033[33mNhap lua chon : \033[0m";

        cin >> choice;
        cin.ignore();

        if (choice == 0)
        {
            cout << "\033[31mThoat chuong trinh. \033[0m\n";
            break;
        }

        switch (choice)
        {
        case 1:
        {
            string tuCanTim;
            cout << "Nhap tu can tim: ";
            getline(cin, tuCanTim);
            searchWord(tudien, tuCanTim);
            break;
        }
        case 2:
        {
            cout << "Nhap password xac minh: ";
            pwinput = matkhau();
            if (pw == pwinput)
            {
                Word w;
                w.Nhap();
                addWord(tudien, w, filebin);
                saveHashTableToTextFile(tudien, "output.txt");
            }
            else
            {
                cout << "Ban khong duoc phep them tu vao tu dien.\n";
            }
            break;
        }
        case 3:
        {
            cout << "Nhap password xac minh: ";
            pwinput = matkhau();
            if (pw == pwinput)
            {
                string tuCanChinhSua;
                cout << "Nhap tu can chinh sua: ";
                getline(cin, tuCanChinhSua);
                editWord(tudien, tuCanChinhSua, filebin);
                saveHashTableToTextFile(tudien, "output.txt");
            }
            else
            {
                cout << "\nBan khong duoc phep chinh sua tu trong tu dien.\n";
            }
            break;
        }
        case 4:
        {
            cout << "Nhap password xac minh: ";
            pwinput = matkhau();
            if (pw == pwinput)
            {
                cout << "Nhap tu tieng anh can xoa: ";
                string xoaTu;
                getline(cin, xoaTu);
                deleteWord(tudien, xoaTu, filebin);
                saveHashTableToTextFile(tudien, "output.txt");
            }
            else
            {
                cout << "Ban khong duoc phep xoa tu trong tu dien.\n";
            }
            break;
        }
        default:
        {
            cout << "\033[31mLua chon khong hop le. Vui long nhap lai!\033[0m\n";
            break;
        }
        }
        cout << "\nNhan Enter de tiep tuc...";
        cin.ignore();
    }
    return 0;
}
