#include <iostream>
using namespace std;

struct monomial{
    double coeff; //coefficient pri x
    int rank; //stepen'
    monomial* next; //sleduushiy
    monomial(double c, int r, monomial* next = nullptr); //kostructor monoma
    monomial(monomial* mon); // kopiruyshiy kostructor
};

monomial::monomial(double c, int r, monomial* next): coeff(c), rank(r), next(next)
{}

monomial::monomial(monomial *mon): coeff(mon->coeff), rank(mon->rank), next(mon->next)
{}

class Polynomial{
public:
    Polynomial(); //konstructor

    void addLastMonom(double c, int r);
    void print();

    Polynomial operator *(int number) const; //umnozhenie na chislo
    Polynomial operator +(const Polynomial& m) const; //slozhenie monomov
    Polynomial operator -(const Polynomial& m) const; // vichitanie

    double operator ()(double x) const;

    Polynomial operator *(const Polynomial& m) const; // umnozhenie
private:
    monomial* head;
    double epsilon = 0.000001;
};

Polynomial::Polynomial() {
    this->head = nullptr;
}


void Polynomial::addLastMonom(double c, int r){
    monomial* new_monomial = new monomial(c, r, nullptr);
    if (this->head == nullptr)
    {
        this->head = new_monomial;
    }
    else
    {
        monomial* last = this->head;
        while(last->next != nullptr)
            last = last->next;
        last->next = new_monomial;
    }
}

Polynomial Polynomial::operator+(const Polynomial &m) const{
    if (m.head == nullptr)
        return *(new Polynomial(*this));
    if (this->head == nullptr)
        return *(new Polynomial(m));

    Polynomial* result = new Polynomial();
    monomial* currentMon1 = this->head;
    monomial* currentMon2 = m.head;
    while (currentMon1 != nullptr && currentMon2 != nullptr)
    {
        double coeff = 0.0;
        int rank = 0;
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
        result->addLastMonom(coeff, rank);
    }
    return *result;
}

Polynomial Polynomial::operator -(const Polynomial &m) const{
    return *this + m * (-1);
}

Polynomial Polynomial::operator*(int number) const {
    Polynomial* result = new Polynomial();

    if (this->head != nullptr)
        for (monomial* currentMon = this->head; currentMon != nullptr; currentMon = currentMon->next)
            result->addLastMonom(currentMon->coeff * number, currentMon->rank);

    return *result;
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
    pol2.addLastMonom(3, 3);
    pol2.addLastMonom(3, 1);
    pol2.print();
    cout << endl;
    cout << endl;
    Polynomial pol3 = pol + pol2;
    pol3.print();*/
    Polynomial pol = Polynomial();
    pol.addLastMonom(11, 11);
    pol.addLastMonom(4, 4);
    pol.addLastMonom(2, 2);
    pol.addLastMonom(1, 1);
    Polynomial pol1 = pol;
    pol = pol * 2;
    pol.print();
    cout << endl;
    pol1.print();
    return 0;
}