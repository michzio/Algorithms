#include<cstdio>
#include<cstdlib>
#include<string>

/**
 * Klasa będąca zbiorem elementów dowolnego typu
 **/

class Set
{
    struct IncorrectSizeException : public std::exception
    {
        std::string s;
        IncorrectSizeException(std::string ss) : s(ss) {}
        ~IncorrectSizeException() throw () {}
        const char* what() const throw() { return s.c_str(); }
    };
    
    //rozmiar przestrzeni wartości zbioru
    unsigned int size;
    //liczba elementów aktualnie w zbiorze
    unsigned int count;
    
    //zbiór reprezentowany jako tablica dynamiczna wartości typu bool
    //oznaczają one czy dany element jest w zbiorze TRUE,
    //czy nie jest w zbiorze FALSE
    bool *set_element;
    
    
public:
    Set(unsigned int n) : size(n), count(0) { set_element = new bool[n];}
    ~Set() { delete [] set_element; }
    unsigned int ElementsCount();
    
    //metody dla zbioru
    void Insert(int id);
    bool Contains(int id) const;
    void Remove(int id);
    Set Sum(const Set &s) const;
    Set Difference(const Set &s) const;
    Set Intersection(const Set &s) const;
    bool Inclusion(const Set &s) const;
    
    //przeciążone operatory
    Set operator+(Set const &);
    Set operator-(Set const &);
    Set operator*(Set const &);
    bool operator==(Set const &);
    bool operator<=(Set const &);
    bool &operator[](int id);
    bool &operator[](int id) const;
    
    //metoda wypisuje elementy zbioru
    void print();
};


/**
 * Zwraca bieżącą ilość elementów w zbiorze
 **/
unsigned int Set::ElementsCount()
{
    return count;
}

/**
 * Wstawia element o id do zbioru oznaczając element 
 * tablicy pod indeksem id jako TRUE
 **/
void Set::Insert(int id)
{
    set_element[id] = true;
    count++;
}

/**
 * Sprawdza czy element o id znajduje się w zbiorze
 **/
bool Set::Contains(int id) const
{
    return set_element[id];
}

/**
 * Usuwa element o id ze zbioru oznaczając element 
 * tablicy pod indeksem id jako FALSE
 **/
void Set::Remove(int id)
{
    set_element[id] = false;
    count--;
}

/**
 * Suma dwóch zbiorów
 **/
Set Set::Sum(Set const &s) const
{
    if(size != s.size)
        throw IncorrectSizeException("Sumowane zbiory mają różny rozmiar!");
        
    Set sum(size);
    
    for(int i=0; i<size; i++)
    {
        sum[i] = set_element[i] || s[i];
    }
    
    sum.count = count + s.count;
    
    return sum;
}

/**
 * Przecięcie dwóch zbiorów
 **/
Set Set::Intersection(const Set &s) const
{
    if(size != s.size)
        throw IncorrectSizeException("Przecinane zbiory mają różny rozmiar!");
    
    Set inter(size);
    
    int new_count = 0;
    
    for(int i=0; i<size; i++)
    {
        if(set_element[i] && s[i]) {
            inter[i] = set_element[i] && s[i];
            new_count++;
        } else {
            inter[i] = false;
        }
    }
    
    inter.count = new_count;
    
    return inter;
}

/**
 * Różnica dwóch zbiorów
 **/
Set Set::Difference(const Set &s) const
{
    if(size != s.size)
        throw IncorrectSizeException("Odejmowane zbiory mają różny rozmiar!");
    
    Set diff(size);
    
    for(int i=0; i<size; i++)
    {
        diff[i] = set_element[i] && (!s[i]);
    }
    
    diff.count = count - s.count;
    
    return diff;
}

/**
 * Inkluzja zbioru przekazanaego jako argument  w bieżącym
 **/
bool Set::Inclusion(const Set &s) const
{
    if(size != s.size)
        throw IncorrectSizeException("Operacja inkluzji na zbiorach o różnych rozmiarach!");
    
    for(int i=0; i<size; i++)
    {
        if(s[i] && !set_element[i])
        {
            return false;
            
        }
    }
    
    return true;
}

/**
 * Operator sumy zbiorów
 **/
Set Set::operator+(Set const &s)
{
    return this->Sum(s);
}

/**
 * Operator różnicy zbiorów
 **/
Set Set::operator-(Set const &s)
{
    return this->Difference(s);
}

/**
 * Operator iloczynu (przecięcia zbiorów)
 **/
Set Set::operator*(Set const &s)
{
    return this->Intersection(s);
}

/**
 * Operator sprawdzania równości zbiorów
 **/
bool Set::operator==(Set const &s)
{
    if(size != s.size)
        throw IncorrectSizeException("Porównywanie zbiorów o różnym rozmiarze!");
    
    for(int i=0; i< size; i++)
    {
        if(s[i] != set_element[i])
            return false;
    }
    
    return true;
}

/**
 * Operator inkluzji (zawierania) sprawdza czy bieżący zbiór
 * zawiera się w tym przekazanym jako argument
 **/
bool Set::operator<=(Set const &s)
{
    return s.Inclusion(*this);
}

/**
 * Operator dostępu do elementu zbioru o indeksie w wewnętrznej tablicy
 * równym id
 **/
bool &Set::operator[](int id)
{
    return set_element[id];
}

/**
 * Operator dostępu do elementu zbioru o indeksie w wewnętrznej tablicy
 * równym id
 **/
bool &Set::operator[](int id) const
{
    return set_element[id];
}


void Set::print(void)
{
    printf("{ ");
    for(int idx=0; idx < size; idx++)
    {
        if(set_element[idx])
            printf("%d ", idx);
    }
    
    printf("}\n");
}

int main(int argc, char **argv)
{
    printf("Utworzenie 2 zbiorów s1 i s2.\n");
    
    Set s1(100);
    Set s2(100);
    Set s3(100);
    
    printf("******************** ZBIORY ********************\n\n");
    printf("Wypełnienie co 5 indeksu zbioru s1: 0, 5, 10,...\n");
    for(int i=0; i<100; i = i+5)
        s1.Insert(i);
    s1.print();
    printf("\n");
    
    printf("Wypełnienie co 10 indeksu zbioru s2: 0, 10,...\n");
    for(int i=0; i<100; i = i+10)
        s2.Insert(i);
    s2.print();
    printf("\n");
    
    if(s1 == s2) {
        printf("Zbiory s1 i s2 są równe.\n");
    } else {
        printf("Zbiory s1 i s2 nie są równe.\n");
    }
    printf("\n");
    
    printf("Wypełnienie co 5 idenksu zbioru s3: 0, 5, 10,...\n");
    for(int i=0; i<100; i= i+5)
        s3[i] = true;
    s3.print();
    printf("\n");
    
    if(s1 == s3) {
        printf("Zbiory s1 i s3 są równe.\n");
    } else {
        printf("Zbiory s1 i s3 nie są równe.\n");
    }
    printf("\n");
    
    if(s3 == s2) {
        printf("Zbiory s2 i s3 są równe.\n");
    } else {
        printf("Zbiory s2 i s3 nie są równe.\n");
    }
    printf("\n");
    
    printf("Różnica zbiorów s4 = s1 - s2\n");
    
    Set s4 = s1 - s2;
    
    s4.print();
    printf("\n");
    
    printf("Iloczyn zbiorów s5 = s4 * s2\n");
    Set s5 = s4*s2;
    s5.print();
    printf("\n");
    
    printf("Iloczyn zbiorów s6 = s2*s1\n");
    Set s6 = s1*s2;
    s6.print();
    printf("\n");
    
    printf("Suma zbiorów s7 = s4+s2\n");
    Set s7 = s4+s2;
    s7.print();
    printf("\n");
    
    printf("Zawieranie zbiorów s4 <= s1\n");
    if(s4 <= s1) {
        printf("Zbiór s4 zawiera się w s1\n");
    } else {
         printf("Zbiór s4 NIE zawiera się w s1\n");
    };
    printf("\n");
    
    printf("Zawieranie zbiorów s2 <= s1\n");
    if(s2 <= s1) {
        printf("Zbiór s2 zawiera się w s1\n");
    } else {
        printf("Zbiór s2 NIE zawiera się w s1\n");
    };
    
    printf("Zawieranie zbiorów s1 <= s2\n");
    if(s1 <= s2) {
        printf("Zbiór s1 zawiera się w s2\n");
    } else {
        printf("Zbiór s1 NIE zawiera się w s2\n");
    };
    printf("\n");
    
    
    return 0;
}
