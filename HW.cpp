#include <iostream>
#include <cmath>
using namespace std;

// 單一項(節點)結構 //struct 強調 Term 裡的資料成員是公用的
struct Term  
{
    int coef;   // 係數
    int exp;    // 指數
    Term* link; // 下一個節點指標
};

// 可用空間鏈結串列
Term* avail = nullptr;

// 多項式類
class Polynomial
{
public:
    Polynomial();                       // 建構子
    Polynomial(const Polynomial& a);    // 複製建構子
    ~Polynomial();                      // 解構子

    const Polynomial& operator=(const Polynomial& a); // 賦值運算子重載
    Polynomial operator+(const Polynomial& b) const;  // 加法運算
    Polynomial operator-(const Polynomial& b) const;  // 減法運算
    Polynomial operator*(const Polynomial& b) const;  // 乘法運算
    float Eval(float x) const;          // 計算多項式值

    friend istream& operator>>(istream& is, Polynomial& x);         // 輸入運算子重載
    friend ostream& operator<<(ostream& os, const Polynomial& x);   // 輸出運算子重載

private:
    Term* head;                         // 頭節點指標
    void insertTerm(int coef, int exp); // 插入單項式(添加新項)
    static Term* getNode();             // 從可用空間取得節點
    static void returnNode(Term* node); // 將節點歸還到可用空間
};

// 建構子：建立頭節點 // 初始化為空環狀鏈結串列 
Polynomial::Polynomial()
{
    head = getNode();            // 從可用空間取得一個頭節點
    head->link = head;           // 形成循環
}

// 複製建構子
Polynomial::Polynomial(const Polynomial& a) : Polynomial()
{
    Term* current = a.head->link;
    while (current != a.head)    // 處理多項式每一項 //遍歷 a 的所有有效節點，直到重新到達頭節點 a.head
    {
        insertTerm(current->coef, current->exp);
        current = current->link; // 移動到下一個節點
    }
}

// 解構子：釋放所有節點
Polynomial::~Polynomial()
{
    Term* current = head->link;
    while (current != head)
    {
        Term* temp = current;
        current = current->link;
        returnNode(temp);
    }
    returnNode(head);
}

// 賦值運算子重載
const Polynomial& Polynomial::operator=(const Polynomial& a)
{
    if (this != &a)          // 避免對自身進行賦值（如 p1 = p1）
    {
        this->~Polynomial(); // 清除當前多項式物件 *this(自己) 的舊內容，包括所有的節點記憶體
        
        // 為當前物件重新建立頭節點 // 重新初始化為空環狀鏈結串列
        head = getNode();
        head->link = head;

        //複製另一個多項式的內容
        Term* current = a.head->link;
        while (current != a.head)
        {
            insertTerm(current->coef, current->exp);
            current = current->link;
        }
    }
    return *this;             // 返回當前物件
}

// 從可用空間取得節點
Term* Polynomial::getNode()
{
    if (avail) 
    {
        Term* temp = avail;   // 從可用空間取出一個節點
        avail = avail->link;  // 更新可用空間
        return temp;
    }
    return new Term;          // 若可用空間為空，透過 new Term 分配新節點(向系統申請記憶體，並初始化)
}

// 將節點歸還到可用空間
void Polynomial::returnNode(Term* node)
{
    node->link = avail;       // 將節點放回可用空間
    avail = node;
    //node = 0;
}

// 插入單項式(添加新項)，按指數降序排列
void Polynomial::insertTerm(int coef, int exp)
{
    Term* current = head;
    while (current->link != head && current->link->exp > exp) // && 下一項指數 > 新項指數
    {
        current = current->link;        // 移動到下一項
    }

    // 若下一項指數 !> 新項指數，表示找到插入位置
    // 檢查新項在此插入位置，是否需要被合併 (下一項指數 = 新項指數)
    if (current->link != head && current->link->exp == exp)
    {
        current->link->coef += coef;    // 合併同類項 (下一項係數 + 新項係數)
        if (current->link->coef == 0)   // 合併後，若係數為 0，刪除該項
        {
            Term* temp = current->link; // 記錄要刪除的項(節點)
            current->link = temp->link; // 將鏈結指向刪除項(節點)的下一項
            returnNode(temp);
        }
    }
    else
    {
        Term* temp = getNode();         // 從可用空間或記憶體中分配一個新節點(項)
        temp->coef = coef;              // 設置新項的係數
        temp->exp = exp;
        temp->link = current->link;     // 先將新項的下一項設置為 current 的下一項
        current->link = temp;           // 再更新 current->link，使其指向新項
    }
}

// 加法運算
Polynomial Polynomial::operator+(const Polynomial& b) const
{
    Polynomial result;
    Term* p1 = head->link;
    Term* p2 = b.head->link;

    while (p1 != head || p2 != b.head)   // 只要有一多項式不為空就可以算
    {
        if (p1 != head && (p2 == b.head || p1->exp > p2->exp))
        {
            result.insertTerm(p1->coef, p1->exp);
            p1 = p1->link;
        }
        else if (p2 != b.head && (p1 == head || p2->exp > p1->exp))
        {
            result.insertTerm(p2->coef, p2->exp);
            p2 = p2->link;
        }
        else                             // p1指數 = p2指數
        {
            result.insertTerm(p1->coef + p2->coef, p1->exp);
            p1 = p1->link;
            p2 = p2->link;
        }
    }
    return result;
}
// 加法運算(效果同上)
/*Polynomial Polynomial::operator+(const Polynomial& b) const
{
    Polynomial result;
    Term* p1 = head->link;   //aCurrent
    Term* p2 = b.head->link; //bCurrent

    while (p1 != head || p2 != b.head)
    {
        if (p1->exp == p2->exp)
        {
            result.insertTerm(p1->coef+p2->coef, p1->exp);
            p1 = p1->link;
            p2 = p2->link;
        }
        else if (p1->exp > p2->exp)
        {
            result.insertTerm(p1->coef, p1->exp);
            p1 = p1->link;
        }
        else
        {
            result.insertTerm(p2->coef, p2->exp);
            p2 = p2->link;
        }
    }
    while (p1 != head) 
    {
        result.insertTerm(p1->coef, p1->exp);
        p1 = p1->link;
    }
    while (p2 != b.head) 
    {
        result.insertTerm(p2->coef, p2->exp);
        p2 = p2->link;
    }
    return result;
}*/

// 減法運算
Polynomial Polynomial::operator-(const Polynomial& b) const
{
    Polynomial result;
    Term* p1 = head->link;
    Term* p2 = b.head->link;

    while (p1 != head || p2 != b.head)
    {
        if (p1 != head && (p2 == b.head || p1->exp > p2->exp))      // 自己(p1)-0=自己
        {
            result.insertTerm(p1->coef, p1->exp);
            p1 = p1->link;
        }
        else if (p2 != b.head && (p1 == head || p2->exp > p1->exp)) // 0-自己(p2)=-自己
        {
            result.insertTerm(-p2->coef, p2->exp);
            p2 = p2->link;
        }
        else
        {
            result.insertTerm(p1->coef - p2->coef, p1->exp);
            p1 = p1->link;
            p2 = p2->link;
        }
    }
    return result;
}

// 乘法運算
Polynomial Polynomial::operator*(const Polynomial& b) const
{
    Polynomial result;
    Term* p1 = head->link;

    while (p1 != head)
    {
        Term* p2 = b.head->link;
        while (p2 != b.head)
        {
            result.insertTerm(p1->coef * p2->coef, p1->exp + p2->exp);
            p2 = p2->link;
        }
        p1 = p1->link;
    }
    return result;
}

// 計算多項式值
float Polynomial::Eval(float x) const
{
    float result = 0;
    Term* current = head->link;

    while (current != head)
    {
        result += current->coef * pow(x, current->exp);
        current = current->link;
    }
    return result;
}

// 輸入運算子
istream& operator>>(istream& is, Polynomial& x)
{
    int n;
    cout << "輸入非零項的數量: ";
    is >> n;
    for (int i = 0; i < n; ++i)
    {
        int coef, exp;
        cout << "輸入係數與指數: ";
        is >> coef >> exp;
        x.insertTerm(coef, exp);
    }
    return is;
}

// 輸出運算子
ostream& operator<<(ostream& os, const Polynomial& x)
{
    Term* current = x.head->link;
    bool firstTerm = true;                              // 用於檢查是否是第一個項
    while (current != x.head)
    {
        if (!firstTerm && current->coef > 0) os << "+"; // 非第一項且係數為正，添加 '+'
        if (current->coef != 1 || current->exp == 0)    // 係數不為 1 或 指數為 0 時，顯示係數
        {
            os << current->coef;
        }
        if (current->exp > 0)     // 如果指數大於 0，顯示 x
        {
            os << "x";
            if (current->exp > 1) // 指數大於 1 時顯示次方
            {
                os << "^" << current->exp;
            }
        }

        current = current->link;
        firstTerm = false;         // 更新為非第一項
    }
    return os;
}

int main()
{
    Polynomial p1, p2;

    cout << "輸入第一個多項式:" << endl;
    cin >> p1;
    cout << endl << "輸入第二個多項式:" << endl;
    cin >> p2;

    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial prod = p1 * p2;

    cout << "第一個多項式: " << p1 << endl;
    cout << "第二個多項式: " << p2 << endl;
    cout << "和: " << sum << endl;
    cout << "差: " << diff << endl;
    cout << "積: " << prod << endl;

    float x;
    cout << "計算多項式值 (輸入 x 的值): ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}