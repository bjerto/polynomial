#include <iostream>
#include <cmath>

using namespace std;

struct monomial{
    double coeff; //coefficient pri x
    int rank; //stepen'
    monomial* next; //sleduushiy
    monomial(double c, int r, monomial* next = nullptr); //kostructor monoma
    monomial(monomial* mon); // kopiruyshiy kostructor
    monomial* deepCopy();
};

monomial::monomial(double c, int r, monomial* next): coeff(c), rank(r), next(next)
{}

monomial::monomial(monomial *mon): coeff(mon->coeff), rank(mon->rank), next(mon->next)
{}

monomial *monomial::deepCopy() {
    monomial* newMon = new monomial(this);
    if (this->next != nullptr)
        newMon->next = newMon->next->deepCopy();
    return newMon;
}

monomial* addMonoms(monomial* monom1, monomial* monom2, double epsilon){
    monomial* result = new monomial(0, 0, nullptr); // vremenniy element
    monomial* currentMonRes = result;
    monomial* currentMon1 = monom1;
    monomial* currentMon2 = monom2;

    while (currentMon1 != nullptr && currentMon2 != nullptr)
    {
        double coeff;
        int rank;
        if (currentMon1->rank == currentMon2->rank)
        {
            coeff = currentMon2->coeff + currentMon1->coeff;
            rank = currentMon1->rank;
            currentMon1 = currentMon1->next;
            currentMon2 = currentMon2->next;
        }
        else
        {
            if (currentMon1->rank > currentMon2->rank)
            {
                coeff = currentMon1->coeff;
                rank = currentMon1->rank;
                currentMon1 = currentMon1->next;
            }
            else
            {
                coeff = currentMon2->coeff;
                rank = currentMon2->rank;
                currentMon2 = currentMon2->next;
            }
        }
        if (coeff >= epsilon)
        {
            currentMonRes->next = new monomial(coeff, rank, nullptr);
            currentMonRes = currentMonRes->next;
        }
    }
    if (currentMon1 != nullptr)
        currentMonRes->next = currentMon1->deepCopy();
    if (currentMon2 != nullptr)
        currentMonRes->next = currentMon2->deepCopy();

    result = result->next;

    return result;
}

monomial* multiplyMonom(monomial* monom, int n, double epsilon){
    monomial* result = new monomial(0, 0, nullptr); // vremenniy element
    monomial* currentMonRes = result;
    for (monomial* currentMon = monom; currentMon != nullptr; currentMon = currentMon->next)
    {
        double coeff = currentMon->coeff * n;
        if (coeff >= epsilon)
        {
            currentMonRes->next = new monomial(coeff, currentMon->rank, nullptr);
            currentMonRes = currentMonRes->next;
        }
    }
    result = result->next;
    return result;
}

monomial* multiplyMonoms(monomial* monom1, monomial* monom2, double epsilon){
    monomial* result = nullptr;
    for(monomial* currentMon1 = monom1; currentMon1 != nullptr; currentMon1 = currentMon1->next)
    {
        monomial* temp = new monomial(0, 0, nullptr); // vremenniy element
        monomial* currentTemp = temp;
        for (monomial* currentMon2 = monom2; currentMon2 != nullptr; currentMon2 = currentMon2->next)
        {
            double coeff = currentMon1->coeff * currentMon2->coeff;
            int rank = currentMon1->rank + currentMon2->rank;
            if (coeff >= epsilon)
            {
                currentTemp->next = new monomial(coeff, rank, nullptr);
                currentTemp = currentTemp->next;
            }
        }
        temp = temp->next;
        result = addMonoms(result, temp, epsilon);
        delete temp;
    }
    return result;
}

class Polynomial{
public:
    Polynomial(); //konstructor
    Polynomial(const Polynomial& m); //konstructor
    Polynomial(monomial* monom); //konstructor
    ~Polynomial();

    void addLastMonom(double c, int r);
    void addLastMonom(monomial* monom);

    void print();

    Polynomial& operator =(const Polynomial& m);
    Polynomial operator *(int number) const; //umnozhenie na chislo
    Polynomial operator +(const Polynomial& m) const; //slozhenie monomov
    Polynomial operator -(const Polynomial& m) const; // vichitanie
    Polynomial operator *(const Polynomial& m) const; // umnozhenie

private:
    monomial* head;
    double getValue(int x);
    void clean();
    double epsilon = 0.000001;
};

Polynomial::Polynomial() {
    this->head = nullptr;
}

Polynomial::Polynomial(const Polynomial& m) {
    this->head = m.head->deepCopy();
}


Polynomial::Polynomial(monomial *monom) {
    this->head = monom;
}

Polynomial::~Polynomial() {
    this->clean();
}

void Polynomial::addLastMonom(double c, int r){
    monomial* newMonomial = new monomial(c, r, nullptr);
    this->addLastMonom(newMonomial);
}

void Polynomial::addLastMonom(monomial *monom) {
    if (this->head == nullptr)
    {
        this->head = monom;
    }
    else
    {
        monomial* last = this->head;
        while(last->next != nullptr)
            last = last->next;
        last->next = monom;
    }
}

Polynomial Polynomial::operator+(const Polynomial &m) const{
    if (m.head == nullptr)
        return *(new Polynomial(*this));
    if (this->head == nullptr)
        return *(new Polynomial(m));

    Polynomial* result = new Polynomial(addMonoms(m.head, this->head, this->epsilon));
    return *result;
}

Polynomial Polynomial::operator -(const Polynomial &m) const{
    return *this + m * (-1);
}

Polynomial Polynomial::operator *(int number) const {
    Polynomial* result = new Polynomial();

    if (this->head != nullptr)
        result->head = multiplyMonom(this->head, number, this->epsilon);
    return *result;
}

Polynomial Polynomial::operator*(const Polynomial &m) const {
    Polynomial* newPol = new Polynomial();
    if (this->head == nullptr || m.head == nullptr)
        return *newPol;
    newPol->head = multiplyMonoms(this->head, m.head, this->epsilon);
    return *newPol;
}

void Polynomial::print() {
    if (this->head == nullptr)
    {
        cout << 0;
        return;
    }
    cout << this->head->coeff << "x^" << this->head->rank;
    monomial* temp = this->head->next;
    while(temp != nullptr)
    {
        cout << " + " << temp->coeff << "x^" << temp->rank;
        temp = temp->next;
    }
}

Polynomial& Polynomial::operator =(const Polynomial &m) {
    this->clean();
    this->head = m.head->deepCopy();
    return *this;
}

void Polynomial::clean() {
    monomial* currentMon = this->head;
    while(currentMon != nullptr)
    {
        monomial* nextMon = currentMon->next;
        delete currentMon;
        currentMon = nextMon;
    }
    this->head = nullptr;
}

double Polynomial::getValue(int x) {
    double result = 0.0;
    for(monomial* currentMon = this->head; currentMon != nullptr; currentMon = currentMon->next)
        result += currentMon->coeff * pow(x, currentMon->rank);
    return result;
}




int main() {
    /*Polynomial pol = Polynomial();
    pol.addLastMonom(11, 11);
    pol.addLastMonom(4, 4);
    pol.addLastMonom(2, 2);
    pol.addLastMonom(1, 1);
    pol.print();
    cout << endl;
    Polynomial pol2 = Polynomial();
    pol2.addLastMonom(9, 9);
    pol2.addLastMonom(-1, 4);
    pol2.print();
    cout << endl;
    cout << endl;
    Polynomial pol3 = pol + pol2;
    pol3.print();*/

    /*Polynomial pol = Polynomial();
    pol.addLastMonom(11, 11);
    pol.addLastMonom(4, 4);
    pol.addLastMonom(2, 2);
    pol.addLastMonom(1, 1);
    Polynomial pol1 = Polynomial();
    pol1.head = pol.head->deepCopy();
    pol = pol * 2;
    pol.print();
    cout << endl;
    pol1.print();*/

    Polynomial pol = Polynomial();
    pol.addLastMonom(11, 11);
    pol.addLastMonom(2, 2);
    pol.print();
    cout << endl;
    Polynomial pol2 = Polynomial();
    pol2.addLastMonom(9, 9);
    pol2.addLastMonom(2, 4);
    pol2.print();
    cout << endl;
    cout << endl;
    Polynomial pol3 = pol * pol2;
    pol3.print();

    return 0;
}