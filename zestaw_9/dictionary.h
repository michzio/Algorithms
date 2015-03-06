#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <string>


namespace ADT {
    template<typename T>
    class Dictionary;
}

template<typename T>
class ADT::Dictionary
{
private:
    static const int DICTIONARY_MAX_SIZE = 120;
    
    //uporzadkowana tablica przechowujaca
    //słowa (stringi) przypisane do indeksów (kluczy) w słowniku
    //zostaje załadowana na poczatku zbiorem wszystkich mozliwych
    //slow - zbiór Uniwersalny
    static T words[DICTIONARY_MAX_SIZE];
    static int noOfWords; //liczba wczytanych słów
    
    
    //zmienne 2xlong long int przechowujace indeksy
    //elementów aktualnie znajdujacych sie w słowniku
    //WEKTOR WŁASNY - okresla zbior A bedacy pewnym podzbiorem
    //zbioru uniwersalnego U
    //poszczególne bity ustawione na 1 reprezentuja dany indeks
    // np 0000...0000 0000...00001 -> 0
    //    0000...0000 0000...00010 -> 1
    //    0000...0001 0000...00000 -> 61
    //zbiory indeksów
    //np. 0000...0001 0000...00011 -> {0,1,61}
    long long int dictionary127_64, dictionary63_0;
    
public:
   
    //metody statyczne dotyczace zbioru uniwersalnego words
    //(odwzorowanie indekstów (kluczy) na słowa (T np. string)
    static void loadUniversalSet(T *U, int size);
    static int findKey(const T& word);
    
    //metody obietkowe praca na biezacym słowniku
    //tj. zbiorze bedacym podzbiorem zbioru uniwersalnego
    //reprezentowanym przez 2 x long long int
    void insert(T word);
    void insert_key(int key);
    void remove(int key);
    bool lookup(int key);
    Dictionary<T> sum(const Dictionary<T>& otherDictionary) const;
    Dictionary<T> complement(void) const;
    Dictionary<T> XOR(const Dictionary<T>& otherDictionary) const;
    Dictionary<T> intersection(const Dictionary<T>& otherDictionary) const;
    void print(void) const;
};












#endif //DICTIONARY_H