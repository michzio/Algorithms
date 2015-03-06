#include<cmath>
#include<cstdlib>
#include<cstring>
#include<cstdio>

#include "hash_table.h"
/**
 * Funkcja inicjalizuje wewnętrzną tablice list NULL'ami
 **/
template<typename T>
void HashTable<T>::initNULL(void)
{
    //petla po wszystkich elementach tablicy
    for(int i=0; i<size; i++)
    {
        //inicjalizacja elementów tablicy nullami
        tableOfLists[i] = NULL;
    }
}

/**
 * Funkcja odszukuje element przekazany jako argument 
 * w HashTable używając funkcji haszującej, a następnie
 * przechodząc w pętli liste. Następnie gdy odnajdzie 
 * przekazany element zwraca TRUE w przeciwnym wypadku 
 * zwraca FALS
 **/
template<typename T>
bool HashTable<T>::search(T element)
{
    //wyznaczenie indeksu tablicy funkcja haszujaca
    int idx = hashFunction(element);
    
    //pobranie listy dla danej komórki tablicy
    Node<T> *node = tableOFLists[idx];
    while(node != NULL) {
        if(node->element == element)
            return true;
        //przeszukujemy kolejny element listy
        node = node->next;
    }
    
    //nie znaleziono elementu w tablicy haszujacej
    return false;
}

/**
 * Funkcja wstawiajaca element przekazany jako argument
 * do HashTable używając funkcji haszujacej. Jezeli 
 * pod dana pozycja znajduje sie juz jakis element wtedy 
 * wstawiamy element na koniec listy znajdujacej sie pod
 * zadaną komórką -> przechodzimy w petli na koniec listy
 * @param element - element wstawiany do tablicy haszujacej
 **/
template<typename T>
void HashTable<T>::insert(T element)
{
    //sprawdzamy czy elementu nie ma juz w tablicy
    if(search(element)) return;
    
    //w przeciwnym wypadku wstawiamy element do tablicy haszujacej
    int idx = hashFunction(T element);
    //pobieramy wskaźnik na liste znajdujący się na pozycji idx
    Node<T> *oldList = tableOfLists[idx];
    tableOfLists[idx] = new Node<T>;
    tableOfLists[idx]->element = element;
    tableOfLists[idx]->next = oldList;
}

int main(int argc, char **argv)
{
    
    return 0;
}