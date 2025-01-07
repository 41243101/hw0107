#include <iostream>
#include <cmath>
using namespace std;

// ��@��(�`�I)���c //struct �j�� Term �̪���Ʀ����O���Ϊ�
struct Term  
{
    int coef;   // �Y��
    int exp;    // ����
    Term* link; // �U�@�Ӹ`�I����
};

// �i�ΪŶ��쵲��C
Term* avail = nullptr;

// �h������
class Polynomial
{
public:
    Polynomial();                       // �غc�l
    Polynomial(const Polynomial& a);    // �ƻs�غc�l
    ~Polynomial();                      // �Ѻc�l

    const Polynomial& operator=(const Polynomial& a); // ��ȹB��l����
    Polynomial operator+(const Polynomial& b) const;  // �[�k�B��
    Polynomial operator-(const Polynomial& b) const;  // ��k�B��
    Polynomial operator*(const Polynomial& b) const;  // ���k�B��
    float Eval(float x) const;          // �p��h������

    friend istream& operator>>(istream& is, Polynomial& x);         // ��J�B��l����
    friend ostream& operator<<(ostream& os, const Polynomial& x);   // ��X�B��l����

private:
    Term* head;                         // �Y�`�I����
    void insertTerm(int coef, int exp); // ���J�涵��(�K�[�s��)
    static Term* getNode();             // �q�i�ΪŶ����o�`�I
    static void returnNode(Term* node); // �N�`�I�k�٨�i�ΪŶ�
};

// �غc�l�G�إ��Y�`�I // ��l�Ƭ��������쵲��C 
Polynomial::Polynomial()
{
    head = getNode();            // �q�i�ΪŶ����o�@���Y�`�I
    head->link = head;           // �Φ��`��
}

// �ƻs�غc�l
Polynomial::Polynomial(const Polynomial& a) : Polynomial()
{
    Term* current = a.head->link;
    while (current != a.head)    // �B�z�h�����C�@�� //�M�� a ���Ҧ����ĸ`�I�A���쭫�s��F�Y�`�I a.head
    {
        insertTerm(current->coef, current->exp);
        current = current->link; // ���ʨ�U�@�Ӹ`�I
    }
}

// �Ѻc�l�G����Ҧ��`�I
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

// ��ȹB��l����
const Polynomial& Polynomial::operator=(const Polynomial& a)
{
    if (this != &a)          // �קK��ۨ��i���ȡ]�p p1 = p1�^
    {
        this->~Polynomial(); // �M����e�h�������� *this(�ۤv) ���¤��e�A�]�A�Ҧ����`�I�O����
        
        // ����e���󭫷s�إ��Y�`�I // ���s��l�Ƭ��������쵲��C
        head = getNode();
        head->link = head;

        //�ƻs�t�@�Ӧh���������e
        Term* current = a.head->link;
        while (current != a.head)
        {
            insertTerm(current->coef, current->exp);
            current = current->link;
        }
    }
    return *this;             // ��^��e����
}

// �q�i�ΪŶ����o�`�I
Term* Polynomial::getNode()
{
    if (avail) 
    {
        Term* temp = avail;   // �q�i�ΪŶ����X�@�Ӹ`�I
        avail = avail->link;  // ��s�i�ΪŶ�
        return temp;
    }
    return new Term;          // �Y�i�ΪŶ����šA�z�L new Term ���t�s�`�I(�V�t�ΥӽаO����A�ê�l��)
}

// �N�`�I�k�٨�i�ΪŶ�
void Polynomial::returnNode(Term* node)
{
    node->link = avail;       // �N�`�I��^�i�ΪŶ�
    avail = node;
    //node = 0;
}

// ���J�涵��(�K�[�s��)�A�����ƭ��ǱƦC
void Polynomial::insertTerm(int coef, int exp)
{
    Term* current = head;
    while (current->link != head && current->link->exp > exp) // && �U�@������ > �s������
    {
        current = current->link;        // ���ʨ�U�@��
    }

    // �Y�U�@������ !> �s�����ơA��ܧ�촡�J��m
    // �ˬd�s���b�����J��m�A�O�_�ݭn�Q�X�� (�U�@������ = �s������)
    if (current->link != head && current->link->exp == exp)
    {
        current->link->coef += coef;    // �X�֦P���� (�U�@���Y�� + �s���Y��)
        if (current->link->coef == 0)   // �X�֫�A�Y�Y�Ƭ� 0�A�R���Ӷ�
        {
            Term* temp = current->link; // �O���n�R������(�`�I)
            current->link = temp->link; // �N�쵲���V�R����(�`�I)���U�@��
            returnNode(temp);
        }
    }
    else
    {
        Term* temp = getNode();         // �q�i�ΪŶ��ΰO���餤���t�@�ӷs�`�I(��)
        temp->coef = coef;              // �]�m�s�����Y��
        temp->exp = exp;
        temp->link = current->link;     // ���N�s�����U�@���]�m�� current ���U�@��
        current->link = temp;           // �A��s current->link�A�Ϩ���V�s��
    }
}

// �[�k�B��
Polynomial Polynomial::operator+(const Polynomial& b) const
{
    Polynomial result;
    Term* p1 = head->link;
    Term* p2 = b.head->link;

    while (p1 != head || p2 != b.head)   // �u�n���@�h���������ŴN�i�H��
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
        else                             // p1���� = p2����
        {
            result.insertTerm(p1->coef + p2->coef, p1->exp);
            p1 = p1->link;
            p2 = p2->link;
        }
    }
    return result;
}
// �[�k�B��(�ĪG�P�W)
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

// ��k�B��
Polynomial Polynomial::operator-(const Polynomial& b) const
{
    Polynomial result;
    Term* p1 = head->link;
    Term* p2 = b.head->link;

    while (p1 != head || p2 != b.head)
    {
        if (p1 != head && (p2 == b.head || p1->exp > p2->exp))      // �ۤv(p1)-0=�ۤv
        {
            result.insertTerm(p1->coef, p1->exp);
            p1 = p1->link;
        }
        else if (p2 != b.head && (p1 == head || p2->exp > p1->exp)) // 0-�ۤv(p2)=-�ۤv
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

// ���k�B��
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

// �p��h������
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

// ��J�B��l
istream& operator>>(istream& is, Polynomial& x)
{
    int n;
    cout << "��J�D�s�����ƶq: ";
    is >> n;
    for (int i = 0; i < n; ++i)
    {
        int coef, exp;
        cout << "��J�Y�ƻP����: ";
        is >> coef >> exp;
        x.insertTerm(coef, exp);
    }
    return is;
}

// ��X�B��l
ostream& operator<<(ostream& os, const Polynomial& x)
{
    Term* current = x.head->link;
    bool firstTerm = true;                              // �Ω��ˬd�O�_�O�Ĥ@�Ӷ�
    while (current != x.head)
    {
        if (!firstTerm && current->coef > 0) os << "+"; // �D�Ĥ@���B�Y�Ƭ����A�K�[ '+'
        if (current->coef != 1 || current->exp == 0)    // �Y�Ƥ��� 1 �� ���Ƭ� 0 �ɡA��ܫY��
        {
            os << current->coef;
        }
        if (current->exp > 0)     // �p�G���Ƥj�� 0�A��� x
        {
            os << "x";
            if (current->exp > 1) // ���Ƥj�� 1 ����ܦ���
            {
                os << "^" << current->exp;
            }
        }

        current = current->link;
        firstTerm = false;         // ��s���D�Ĥ@��
    }
    return os;
}

int main()
{
    Polynomial p1, p2;

    cout << "��J�Ĥ@�Ӧh����:" << endl;
    cin >> p1;
    cout << endl << "��J�ĤG�Ӧh����:" << endl;
    cin >> p2;

    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial prod = p1 * p2;

    cout << "�Ĥ@�Ӧh����: " << p1 << endl;
    cout << "�ĤG�Ӧh����: " << p2 << endl;
    cout << "�M: " << sum << endl;
    cout << "�t: " << diff << endl;
    cout << "�n: " << prod << endl;

    float x;
    cout << "�p��h������ (��J x ����): ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}