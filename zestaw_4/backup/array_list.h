/**
 * Plik zawiera deklarację listy 'ArrayList' opartej o tablice.
 * Rozmiar tablicy opakowanej przez ArrayList jest ustalany w konstruktorze
 * Lista oparta o tablice realizuje proste operacje 
 * Dodawania elementu na początek listy
 * Dodawanie elementu w dowolne miejsce listy
 * Dodawania elementu na koniec listy
 * Pobieranie wartości pierwszego elementu listy
 * Usuwanie pierwszego elementu listy
 * Usuwanie dowolnego elementu listy 
 * Wypisywanie aktualnego rozmiaru listy 
 **/

#include<cstdio>

#define MAX_CAPACITY 1000000  // 10^6 - maksymalna pojemność tablicy
namespace ADT {
    template<typename T>
    struct ArrayIterator;
    
    template<typename T>
    class ArrayList;
}


/**
 * klasa ArrayList jest parametryzowane typem T, a więc na liście można 
 * przechowywać dane dowolnego typu ustalonego podczas tworzenia obiektu ArrayList
 **/
template<typename T>
class ADT::ArrayList
{
    //maksymalna ustalona pojemność tablicy
    int capacity;
    //tablica przechowująca elementy listy
    T *array;
    //bieżący rozmiar tablicy array
    int size;
public:
    //konstruktory
    //1) domyślny ustala 0 pojemność tablicy nie alokuje składowej T*array
    //2) pobierający argument int c ustala pojemność tablicy capacity = c oraz
    //   alokuje składową T*array odpowiedniego rozmiaru c
    ArrayList() : capacity(0), size(0) {}
    ArrayList(int c) : capacity(c), size(0) {
        array = new T[c];
        fprintf(stderr, "Utworzenie listy opertej na tablicy o pojemności %d.\n", c);
    }
    
    //metody:
    ArrayIterator<T> begin();
    ArrayIterator<T> end();
    ArrayIterator<T> add(ArrayIterator<T> position, const T& element);
    ArrayIterator<T> add_front(const T& element);
    ArrayIterator<T> add_back(const T& element);
    T& first();
    T& element_at(int i);
    T& element_at(ArrayIterator<T> position);
    T& last();
    ArrayIterator<T> remove(ArrayIterator<T> position);
    ArrayIterator<T> remove_first();
    int count(void);
    
    //destruktor zwalnia tablice
    ~ArrayList() {
        fprintf(stderr, "Destruktor ArrayList.\n");
        if(capacity > 0)
            delete [] array;
    }
    
private:
    void enlargeCapacity(void);
    void logArray(void);
};

template<typename T>
struct ADT::ArrayIterator {
    //wskaźnik na element listy (tablicy)
    T* element;
    
    //konstruktor iteratora
    // 1) domyślny, wskaźnik na null
    ArrayIterator() : element(0) {}
    // 2) konstruktor ustawiający wskaźnik T*element na konkretny element tablicy
    ArrayIterator(T *e) :element(e) {}
    
    //przeciążenie operatorów
    
    //1) indirection operator - zwraca referencje do wartości elementu typu T
    //  na którą wskazuje bieżący wskaźnik T *element
    T& operator*() const
    {
        return this->element;
    }
    
    //2) structure dereference operator - wykorzystywane gdyby T było typu
    // posiadającego składowe jak np. struct, class, użycie itr->field
    T* operator->() const
    {
        return element;
    }
    
    //3) operator ++ preinkrementacji iterator ++itr
    ArrayIterator<T>& operator++() {
        
        element++;
        return *this;
    }
    
    //4) operator ++ postinkrementacji iterator itr++
    ArrayIterator<T> operator++(int) {
        ArrayIterator<T> tmp(*this);
        element++;
        return tmp;
    }
    
    //5) operator porównania ==
    bool operator==(const ArrayIterator<T> &otherIterator) {
        return element = otherIterator.element;
    }
    
    //6) operator !=
    bool operator!=(const ArrayIterator<T> &otherIterator) {
        return element != otherIterator.element;
    }
    
    //Iterator na nastepny element listy (tablicy)
    ArrayIterator<T> next() const
    {
        if(element)
            return ArrayIterator<T>(element +1);
        else
            return ArrayIterator<T>(0);
    }
    
};