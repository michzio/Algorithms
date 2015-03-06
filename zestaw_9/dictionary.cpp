#include "dictionary.h"

#include <algorithm>

namespace ADT {
    
template<typename T>
int Dictionary<T>::noOfWords = 0;
template<typename T>
T Dictionary<T>::words[DICTIONARY_MAX_SIZE];

//Funkcja wczytuje nowy slownik(zbiór uniwersalny) i go sortuje
template<typename T>
 void Dictionary<T>::loadUniversalSet(T *U, int size)
    {
    
        //1. sprawdzenie czy dodawana tablica slow nie
        //   przekracz dopuszczalnego rozmiaru
        noOfWords = (size <= DICTIONARY_MAX_SIZE) ? size : DICTIONARY_MAX_SIZE;
        
        //2. przekopiowanie tablicy U do statycznej words
        for(int i=0; i<noOfWords; i++)
        {
            words[i] = U[i];
        }
        
        //3. posortuj tablice słów words zanim zostanie użyta
        //   w tablicy statycznej T words[] aby umozliwic pozniej
        //   wyszukiwania indeksu dla słowa w czasie logN
        std::sort(words, words+noOfWords);
        
    }


/***
 * Funkcja findKey() zwraca klucz dla zadanego slowa 
 * który jest indeksem w tablicy words. Jezeli nie znajdzie
 * slowa w calym slowniku (zbiór uniwersalny) to zwraca 
 * -1
 **/
template<typename T>
int Dictionary<T>::findKey(const T& word)
{
    //zakładamy ze elementu nie ma w tablicy klucz -1
    int key = -1;
    
    //odszukanie indeksu dla elementu word w tablicy slownika
    T *element = std::lower_bound(words, words+noOfWords, word);
    
    //sprawdzenie czy dopasowano element
    if( !(word < *element) && element != words+noOfWords)
    {
        //operacja na wskaźnikach (pointer arithmetic)
        key = element - words;
    }
    //zwrócenie klucza
    return key;
}
    
/**
 * funkcja wstawia do bieżącego słownika (zbioru bedacego podzbiorem
 * zdefiniowanego universum -> static T words[noOfWords])
 * nowe słowo. 
 * 1. znajduje klucz dla przekazanego jako argument word slowa  
 *    w tablicy slownika (klucz jest indeksem pod ktorym znajduje sie
 *    poszukiwane slowo, UWAGA! słowa w tablicy sa posortowane 
 *    przeszukiwanie tablicy wykonywane jest za pomoca binary search 
 *    w czasie log_2(N) 
 * 2. Klucz wstawiany jest do biezacego słownika (podzbioru uniwersum) 
 *    bazujacego na 2 x long long int za pomoca funkcji 
 *    insert_key(int) - funkacja ta stosuje operatory binarne
 **/
template<typename T>
    void Dictionary<T>::insert(T word)
    {
        int key = findKey(word);
        if(key >= 0)
            insert_key(key);
        else
            std::cout << "Error: próbujesz dodać do zbioru słowo spoza zakresu" << std::endl;
    }

/***
 * Dodaje klucz do zbioru Dictionary bazujacego na wektorze wlasnym 
 * w postaci 2 x long long int 
 * 1. poszczególne bity oznaczaja numer indeksu poczawszy od bitu
 *    najmniej znaczacego w dictionary63_0  mamy kolejne indeksy
 *     Jezeli wartosc indeksu przekracza 60 np. 100 to jest on
 *     ustawiany w zmiennej dictionary127_64 na pozycji (key - 61)
 *     również począwszy od najmniej znaczacego bitu. 
 * 2. w celu ustawienia wlasciwego bitu najpierw ustalamy wartosc 
 *    indeksu czy key > 60 jezeli nie to bit ustawiamy w
 *    dictionary63_0 jezeli tak to w dictionary127_64
 *    bit ustawiamy na odpowiednia pozycje poprzez utworzenie 
 *    zmiennej tymczsowej (long long int 1)<< offset gdzie bit 
 * poczatkowy przesuwamy o offset = key lub key -61
 *   nastepnie stosujemy operator OR | do ustawienia bitu na
 * odpowiedniej pozycji
 **/
template<typename T>
    void Dictionary<T>::insert_key(int key)
    {
        long long int bit_to_set;
        
        //1. test czy klucz ma wartosc wieksza niz 63
        if(key > 60) {
            //ustaw bit w dictionary127_64 na pozycji przesunietej
            //o offset = key - 61;
            int offset = key - 61;
            bit_to_set = ((long long int) 1) << offset;
            //ustawienie wlasciwego bitu operatorem OR
            dictionary127_64 = dictionary127_64 | bit_to_set;
        } else {
            //ustaw bit w dictionary63_0 na pozycji przesunietej
            //o offset = key
            bit_to_set = ((long long int) 1) << key;
            //ustawienie wlasciwego bitu operatorem OR
            dictionary63_0 = dictionary63_0 | bit_to_set;
        }
    }
/***
 * Metoda usuwa ze zbioru wskazany klucz
 **/
template<typename T>
    void Dictionary<T>::remove(int key)
    {
        long long int bit_to_unset;
        
        //1. sprawdzamy czy key > 60 tj. czy jest przechowywany
        //   w dictionary127_64 czy dictionary63_0 zmiennej
        if(key > 60)
        {
            //klucz przechowywany w dictionary127_64
            //2. obliczamy offest klucza
            int offset = key - 61;
            //tymczasowa zmienna z pozycja bita który chcemy usunac
            bit_to_unset = ((long long int) 1) << offset;
            //3. w celu usuniecia bita wskazywanego przez bit_to_unset
            //   XOR ujemy ta zmienna ze zmienna dictionary127_64
            dictionary127_64 = dictionary127_64 ^ bit_to_unset;
        } else {
            //klucz przechowywany w dicitionary63_0
            //tymczasowa zmiena z pozycja bita który chcemy usunac
            bit_to_unset = ((long long int) 1) << key;
            //3. w celu usuniecia bita wskazywanego przez bit_to_unset
            //   XOR ujemy ta zmienna ze zmienna dictionary63_0
            dictionary63_0 = dictionary63_0 ^ bit_to_unset;
        }
    }

/***
 * Metoda sprawdza czy w zbiorze istnieje element o wskazanym kluczu
 **/
template<typename T>
    bool Dictionary<T>::lookup(int key)
    {
        long long int bit_to_check;
        
        //1. sprawdzamy czy klucz znajduje sie w
        //   dictionary127_64 czy dictionary63_0
        if(key >60)
        {
            //2. tworzenie zmiennej offset
            int offset = key - 61;
            //tymczasowa zmienna z pozycja klucza która chcemy sprawdzic
            bit_to_check = ((long long int) 1) << offset;
            //sprawdzanie na wskazywanej przez bit_to_check pozycji cos
            // sie znajduje stosujemy w tym celu operator & (AND)
            return ((dictionary127_64 & bit_to_check) != 0);
        } else
        {
            //tymczasowa zmienna z pozycja klucza którą chcemy sprawdzic
            bit_to_check = ((long long int)1) << key;
            //sprawdzamy czy na wskazywanej przez bit_to_check pozycji
            // mamy ustawiony bit, uzywamy operatora AND (&)
            return ((dictionary63_0 & bit_to_check) != 0);
        }
        
    }

/***
 * Metoda tworzy nowy slownik ze zbiorem bedacym suma biezacego i przekazanego
 * metodzie jako argument otherDictionary
 **/
template<typename T>
Dictionary<T> Dictionary<T>::sum(const Dictionary<T>& otherDictionary) const
    {
        //1. nowy slownik ze zbiorem bedacym suma biezacego i other
        Dictionary<T> sumDictionary;
        //2. ustawiamy sume w nowo utworzonym slowniku dla
        //   pierwszej zmiennej long long int operatorem OR |
        sumDictionary.dictionary127_64 =
                dictionary127_64 | otherDictionary.dictionary127_64;
        //3. analogicznie ustawiamy dla drugiej zmiennej operatorem OR |
        sumDictionary.dictionary63_0 =
            dictionary63_0 | otherDictionary.dictionary63_0;

        //zwracamy słownik z suma przez wartosc
        return sumDictionary;
    }
    
/***
 * Metoda tworzy nowy slownik ze zbiorem bedacym dopelnieniem biezacego
 **/
template<typename T>
    Dictionary<T> Dictionary<T>::complement(void) const
    {
        Dictionary<T> complementDictionary;
        //dopelnienie zbioru tworzymy poprzez XOR owanie biezacego
        // z liczba long long int ktora ma ustawione wszystkie bity na 1
        //oprócz pierwszego
        
        long long int b = ((long long int)1)<<61;
        b-=1;
        
        complementDictionary.dictionary127_64 =
                        dictionary127_64 ^ b;
        //analogicznie XORujemu druga zmienna long long int
        complementDictionary.dictionary63_0 =
                        dictionary63_0 ^ b;
        
        return complementDictionary;
    }

/***
 * Metoda tworzy nowy slownik zawierajacy zbiór bedacy różnicą symetryczna
 * zbiorów biezacego i przekazanego jako otherDictionary
 **/
template<typename T>
Dictionary<T> Dictionary<T>::XOR(const Dictionary<T>& otherDictionary) const
    {
        Dictionary<T> xorDictionary;
        
        //stosujemy poprostu dla kazdej zmiennej reprezentujacej
        //wektor wlasny slownika (zbioru) operator XOR
        xorDictionary.dictionary127_64 =
                dictionary127_64 ^ otherDictionary.dictionary127_64;
        xorDictionary.dictionary63_0 =
                dictionary63_0 ^ otherDictionary.dictionary63_0;
        
        return xorDictionary;
    }

/***
 * Metoda towrzy nowy słownik zawierajacy zbiór bedacy przecieciem (iloczyn)
 * dwóch zbiorów: biezacego i przekazanego jako argument otherDictionary
 **/
template<typename T>
Dictionary<T> Dictionary<T>::intersection(const Dictionary<T>& otherDictionary) const
{
    Dictionary<T> intersectionDictionary;
    
    //nowy słownik z przecieciem uzyskujemy za pomoca operatora AND &
    //podobnie jak we wczesniejszych funkcjach
    intersectionDictionary.dictionary127_64 =
        dictionary127_64 & otherDictionary.dictionary127_64;
    intersectionDictionary.dictionary63_0 =
            dictionary63_0 & otherDictionary.dictionary63_0;
    
    return intersectionDictionary;
}

/***
 * metoda wypisuje zawartosc biezacego slownika
 **/
template<typename T>
    void Dictionary<T>::print(void) const
    {
        long long int tempB = dictionary127_64;
        long long int tempA = dictionary63_0;
        
        int i=0;
        while(tempA !=0)
        {
            
            if(tempA%2 == 1)
            {
                if( i<noOfWords)
                    std::cout << words[i] << ", ";
            }
            tempA/=2; i++;
        }
        
        i = 61;
        while(tempB !=0)
        {
            
            if(tempB%2 == 1)
            {
                if(i < noOfWords)
                    std::cout << words[i] << ", ";
            }
            tempB/=2; i++;
        }
        
        std::cout << std::endl;
    }
    
} //namespace ADT

using namespace std;

int main(int argc, char **argv)
{
    /* wczytanie zbioru uniwersalnego słownika */
    int dictSize = 0;
    
    scanf("%d", &dictSize);
    
    //cout << dictSize << endl;
    
    string *U = new string[dictSize];
    
    for(int i=0; i<dictSize; i++)
    {
        cin >> U[i];
        //cout << U[i] << endl;
    }
    
    ADT::Dictionary<string>::loadUniversalSet(U, dictSize);
    
    /* wczytywanie poszczególnych zbiorów */
    int noOfDicts = 0;
    string element;
    
    scanf("%d", &noOfDicts);
    //cout << noOfDicts << endl;
    
    //utworzenie noOfDicts obiektów Dictionary<string>
    ADT::Dictionary<string> *dict = new ADT::Dictionary<string>[noOfDicts];
    
    cout << "**** ZAWARTOŚĆ KOLEJNYCH ZBIORÓW *****" << endl;
    for(int i = 0; i<noOfDicts; i++)
    {
        int cardinality = 0;
        //wczytanie mocy itego zbioru
        scanf("%d", &cardinality);
        //cout << cardinality << endl;
        for(int j = 0; j<cardinality; j++)
        {
            //wczytanie kolejnego elementu
            cin >> element;
            //cout << element << endl;
            dict[i].insert(element);
        
        }
        
        dict[i].print();
    }
    cout << "*************************************" << endl;
    
    //wczytywanie operacji
    int noOfOperations = 0;
    string operation;
    int setA = 0, setB = 0, setC = 0;
    //string element - zdefiniowany wczesniej
    int key;
    
    scanf("%d", &noOfOperations);
    //cout << noOfOperations << endl;
    
    for(int i=0; i< noOfOperations; i++)
    {
        cin >> operation;
        
        if(operation == "E")
        {
            //operacja na elemencie
            cin >> operation;
            if(operation == "D")
            {
                //dodaj element
                scanf("%d", &setA); //odczytanie numeru zbioru
                //cin >> element; //odczytanie elementu
                scanf("%d", &key); //odczytanie klucza do dodania
                dict[setA].insert_key(key);
                
            } else if(operation == "U")
            {
                //usun element
                scanf("%d", &setA); //odczytanie numeru zbioru
                scanf("%d", &key); //odczytanie klucza do usuniecia
                dict[setA].remove(key);
                
            } else if(operation == "S")
            {
                //sprawdz element
                scanf("%d", &setA); //odczytanie numeru zbioru
                scanf("%d", &key); //odczytanie klucza do sprawdzenia
                if(dict[setA].lookup(key)) {
                    cout << "TAK" << endl;
                } else {
                    cout << "NIE" << endl;
                }
            }
            
        } else if(operation == "Z")
        {
            //operacja na zbiorze
            cin >> operation;
            if(operation ==  "S")
            {
                //suma zbiorów
                 scanf("%d", &setA);
                 scanf("%d", &setB);
                 scanf("%d", &setC);
                
                dict[setC] = dict[setA].sum(dict[setB]);
                
            } else if(operation == "R")
                
            {
                //roznica zbiorów
                scanf("%d", &setA);
                scanf("%d", &setB);
                scanf("%d", &setC);
                
                dict[setC] = dict[setA].XOR(dict[setB]);
                
            } else if(operation == "P")
            {
                //przeciecie zbiorów
                scanf("%d", &setA);
                scanf("%d", &setB);
                scanf("%d", &setC);
                
                dict[setC] = dict[setA].intersection(dict[setB]);
                
            } else if(operation == "D")
            {
                //dopełnienie zbioru
                scanf("%d", &setA);
                scanf("%d", &setB);
                
                dict[setB] = dict[setA].complement();
                
            } else if(operation == "W")
            {
                //wypisz zbiór
                scanf("%d", &setA);
                dict[setA].print();
            
            }
        }
    }
    
    /*
    ADT::Dictionary<string> dict;
    dict.insert("ala");
    dict.insert("kota");
    dict.print();
     */
    
    
    return 0;
}