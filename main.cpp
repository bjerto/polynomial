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

    Polynomial operator +=(const Polynomial& m); //slozhenie monomov
    Polynomial operator *(const Polynomial& m); // umnozhenie
    Polynomial operator *(int number); //umnozhenie na chislo

    Polynomial operator -(const Polynomial& m); // vichitanie
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

Polynomial Polynomial::operator +=(const Polynomial&  m) {
    if (m.head == nullptr)
        return *this;
    if (this->head == nullptr)
        return m;

    monomial* currentMon1 = this->head;
    monomial* currentMon2 = m.head;
    while (currentMon1 != nullptr && currentMon2 != nullptr)
    {
        if (currentMon1->rank == currentMon2->rank)
        {
            currentMon1->coeff += currentMon2->coeff;
            currentMon1 = currentMon1->next;
            currentMon2 = currentMon2->next;
        }
        else
        {
            if (currentMon1->rank > currentMon2->rank)
            {
                monomial *new_mon = new monomial(currentMon2);
                new_mon->next = currentMon1->next;
                currentMon1->next = new_mon;
                currentMon1 = new_mon->next;
            }
            else
            {
                monomial *new_mon = new monomial(currentMon1);
                new_mon->next = currentMon2->next;
                currentMon2->next = new_mon;
                currentMon1 = new_mon->next;
            }
        }
    }
    return *this;
}

Polynomial Polynomial::operator -(const Polynomial &m) {
    if (m.head == nullptr)
        return *this;
    if (this->head == nullptr)
        return m;
    monomial* currentMon1 = this->head;
    monomial* currentMon2 = m.head;
    while (currentMon1 != nullptr && currentMon2 != nullptr)
    {
        if (currentMon1->rank == currentMon2->rank)
        {
            currentMon1->coeff -= currentMon2->coeff;
            currentMon1 = currentMon1->next;
            currentMon2 = currentMon2->next;
        }
        else {
            if (currentMon1->rank > currentMon2->rank)
                currentMon1 = currentMon1->next;
            else
                currentMon2 = currentMon2->next;
        }

    }
    return *this;

}
Polynomial Polynomial::operator*(int number) {
    monomial* currentMonom = this->head;
    while (currentMonom != nullptr)
    {
        currentMonom->coeff *= number;
        currentMonom = currentMonom->next;
    }
    return *this;
}

void Polynomial::print() {
    cout << this->head->coeff << "x^" << this->head->rank;
    monomial* temp = this->head->next;
    while(temp != nullptr)
    {
        cout << " + " << temp->coeff << "x^" << temp->rank;
        temp = temp->next;
    }
}


int main() {
    Polynomial pol = Polynomial();
    pol.addLastMonom(1, 1);
    pol.addLastMonom(2, 2);
    pol.addLastMonom(4, 4);
    pol.addLastMonom(11, 11);
    pol.print();
    cout << endl;
    Polynomial pol2 = Polynomial();
    pol2.addLastMonom(3, 1);
    pol2.addLastMonom(3, 3);
    pol2.addLastMonom(-1, 4);
    pol2.addLastMonom(9, 9);
    pol2.print();
    cout << endl;
    cout << endl;
    pol += pol2;
    pol.print();
    return 0;
}