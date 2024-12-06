#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <cctype>
#include <sstream>

using namespace std;

#define TOIDA 80
#define TRUE 1
#define FALSE 0

// Khai báo stack chứa các toán hạng
struct Stack 
{
    int top;
    double nodes[TOIDA];
};

// Kiểm tra stack có rỗng 
int rong(struct Stack *ps) 
{
    return ps->top == -1 ? TRUE : FALSE;
}

// Thêm phần tử vào stack
void them(struct Stack *ps, double x) 
{
    if (ps->top == TOIDA - 1) 
        cout << "Stack bị đầy!" << endl;
    else 
        ps->nodes[++(ps->top)] = x;
}

// Lấy phần tử từ stack
double lay(struct Stack *ps) 
{
    if (rong(ps)) 
    {
        cout << "Stack bị rỗng!" << endl;
        return 0;
    } 
    else 
        return ps->nodes[ps->top--];    
}

// Kiểm tra ký tự có phải là số không
bool laSo(char c) 
{
    return (c >= '0' && c <= '9') || c == '.';  
}

// Kiểm tra ký tự có phải là toán tử không
bool laToanTu(char c) 
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Kiểm tra tính hợp lệ của biểu thức 
bool kiemTraKyTuHopLe(const string& bieuThuc) 
{
    for (char c : bieuThuc) 
    {
        if (!laSo(c) && !laToanTu(c) && c != '(' && c != ')' && !isspace(c)) 
            return false; 
    }
    return true;
}

// Hàm kiểm tra tính hợp lệ của dấu ngoặc
bool kiemTraNgoac(const string& bieuThuc) 
{
    struct Stack ngoac;
    ngoac.top = -1;
    for (char c : bieuThuc) 
    {
        if (c == '(') 
            them(&ngoac, c);
        else if (c == ')') 
        {
            if (rong(&ngoac)) 
                return false; 
            lay(&ngoac);
        }
    }
    return rong(&ngoac); 
}

// Hàm kiểm tra độ ưu tiên của toán tử
int uuTien(char toanTu) 
{
    if (toanTu == '+' || toanTu == '-') return 1;
    if (toanTu == '*' || toanTu == '/') return 2;
    if (toanTu == '^') return 3;
    return 0;
}

// Hàm tính toán một phép toán
double thucHienPhepToan(double a, double b, char toanTu) 
{
    switch (toanTu) 
    {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0)
                throw runtime_error("Lỗi: chia cho 0!");
            return a / b;
        case '^': return pow(a, b);
        default: throw runtime_error("Lỗi: Toán tử không hợp lệ!");
    }
}

// Hàm chuyển đổi từ Infix sang Postfix
string chuyenInfixSangPostfix(const string& bieuThuc) 
{
    struct Stack toanTu;
    toanTu.top = -1;
    string postfix = "";
    string so = "";  
    for (size_t i = 0; i < bieuThuc.length(); i++) 
    {
        char c = bieuThuc[i];
        if (isspace(c)) continue;
        if (laSo(c)) 
            so += c;  
        else 
        {
            if (!so.empty()) 
            {
                postfix += so + " ";
                so = "";
            }          
            if (c == '(') 
                them(&toanTu, c);
            else if (c == ')') 
            {
                while (!rong(&toanTu) && toanTu.nodes[toanTu.top] != '(') 
                {
                    postfix += toanTu.nodes[toanTu.top];
                    postfix += " ";
                    lay(&toanTu);
                }
                lay(&toanTu); 
            } 
            else if (laToanTu(c)) 
            {
                while (!rong(&toanTu) && uuTien(toanTu.nodes[toanTu.top]) >= uuTien(c)) 
                {
                    postfix += toanTu.nodes[toanTu.top];
                    postfix += " ";
                    lay(&toanTu);
                }
                them(&toanTu, c);
            }
        }
    }
    if (!so.empty()) 
    {
        postfix += so + " ";  
    }
    while (!rong(&toanTu)) 
    {
        postfix += toanTu.nodes[toanTu.top];
        postfix += " ";
        lay(&toanTu);
    }
    return postfix;
}

double tinhGiaTriPostfix(const string& postfix) 
{
    struct Stack giaTri;
    giaTri.top = -1;
    string so = "";
    for (size_t i = 0; i < postfix.length(); i++) 
    {
        char c = postfix[i];

        if (isspace(c)) 
        {
            if (!so.empty()) 
            {
                them(&giaTri, stod(so));
                so = "";
            }
        } 
        else if (laSo(c)) 
        {
            so += c;
        } 
        else if (laToanTu(c)) 
        {
            if (giaTri.top < 1) 
            {
                throw runtime_error("Lỗi: Biểu thức không hợp lệ!");
            }
            double b = lay(&giaTri);
            double a = lay(&giaTri);
            double ketQua = thucHienPhepToan(a, b, c);
            cout << "Thực hiện: " << a << " " << c << " " << b << " = " << ketQua << endl;
            them(&giaTri, ketQua);
        }
    }
    if (giaTri.top != 0) 
    {
        throw runtime_error("Lỗi: Biểu thức không hợp lệ!");
    }
    return lay(&giaTri);
}

int main() 
{
    string bieuThuc;
    cout << "==================== GIAO DIỆN CHƯƠNG TRÌNH ====================" << endl;
    cout << "Chào mừng bạn đến với chương trình tính toán biểu thức toán học!" << endl;
    cout << "Hướng dẫn: Nhập biểu thức toán học theo cú pháp sau:" << endl;
    cout << "  - Sử dụng các số từ 0 đến 9"<<endl;
    cout << "  - Sử dụng các toán tử: +, -, *, /, ^ (cộng, trừ, nhân, chia, lũy thừa)" << endl;
    cout << "  - Các biểu thức có thể bao gồm dấu ngoặc đơn ( )." << endl;
    cout << "  - Ví dụ hợp lệ: (3 + 5) * 2, 4 + 3.5 * 2, 3 ^ 2" << endl;
    cout << "==================== BẮT ĐẦU NHẬP ====================" << endl;    
    cout << "Nhập biểu thức toán học: ";
    getline(cin, bieuThuc);
    if (!kiemTraKyTuHopLe(bieuThuc)) 
    {
        cout << "Lỗi: Biểu thức chứa ký tự không hợp lệ!" << endl;
        return 1;
    }
    if (!kiemTraNgoac(bieuThuc)) 
    {
        cout << "Lỗi: Biểu thức có dấu ngoặc không hợp lệ!" << endl;
        return 1;
    }
    string postfix = chuyenInfixSangPostfix(bieuThuc);
    cout << "Biểu thức Postfix: " << postfix << endl;
    try 
    {
        double ketQua = tinhGiaTriPostfix(postfix);
        cout << "Kết quả cuối cùng: " << ketQua << endl;
    } 
    catch (const exception& e) 
    {
        cout << "Lỗi: " << e.what() << endl;
    }
    return 0;
}
