#include<cmath>
#include<cstdlib>
#include<cstring>
#include<cstdio>

//lista jednokierunkowa parametryzowane typem T
template<typename T>
struct Node
{
    T element;
    Node *next;
};

//tablica haszująca parametryzowane typem T
template<typename T>
class HashTable
{
    const int DEFAULT_SIZE = 100;
    
    Node<T> *tableOfLists;
    int size;
    //funkcja haszująca - zwraca numer indeksu tablicy
    //na podstawie przekazanej wartości elementu
    //jezeli dla dwóch roznych elementów zwróci tę samą
    //wartość to umieszamy elementy na liscie
    int hashFunction(T element);
    void initNULL(void);
public:
    HashTable() {
        //alokowanie HashTable z domyślną liczbę pozycji
        size = DEFAULT_SIZE;
        tableOfLists = new Node<t>[size];
        initNull();
    }
    HashTable(int s) : size(s) {
        //allokowanie tablicy list do przechowywania elementów
        tableOfLists = new Node<T>[size];
        initNULL();
    }
    void insert(T element);
    bool search(T element);
    
    //destruktor
    ~HashTable() {
        delete [] tableOfLists;
    }
};