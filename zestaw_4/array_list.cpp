#include "array_list.h"
#include<stdexcept>
#include<cstdio>
#include<cstdlib>

namespace ADT {

/**
 * begin() - zwraca iterator do odczytu/zapisu na pierwszy element listy 
 **/
template<typename T>
ArrayIterator<T> ArrayList<T>::begin()
{
    return ArrayIterator<T>(array);
}

/**
 * end() - zwraca iterator do odczytu/zapisu na element znajdujący się za ostatnim elementem listy (tablicy)
 **/
template<typename T>
ArrayIterator<T> ArrayList<T>::end()
{
    return ArrayIterator<T>(array+size);
}

/**
 * add(position, element) - metoda dodająca element typu T na pozycje wskazana
 * przez iterator ArrayIterator<T> (argument position). Podczas wstawiania 
 * elementu należy przesunąć elementy począwszy od wstawianej pozycji o 
 * jedną pozycję do przodu. UWAGA! należy sprawdzić czy nowy rozmiar tablicy 
 * newSize = currSize + 1; nie będzie wiekszy od capacity! jeżeli newSize > capacity
 * należy realokować tablicę tj. utworzyć nową tablicę o większej pojemnosci do
 * której zostanie przekopiowana wartosci starej tablicy i następnie zwolnić pamięć
 * zajmowaną przez starą tablicę
 * @position - ArrayIterator<T> określający pozycję na którą należy wstawić element
 * @element - element typu const T& do wstawienia, element przekazujemy przez 
 *           referencje tylko do odczytu
 *
 * zwraca ArrayIterator<T> wskazujący na pozycję na którą wstawiono element
 **/
template<typename T>
ArrayIterator<T> ArrayList<T>::add(ArrayIterator<T> position, const T& element)
{
    //sprawdzenie czy nie następuje naruszenie ciągłości listy w tablicy
    if(position.element > (array + size))
        throw std::out_of_range("Próba dodania elementu poza listą. ");
    //sprawdzenie czy nowy zwiększony rozmiar tablicy nie będzie przekraczał
    //pojemności jeżeli tak to wymagane zwiększenie rozmiaru tablicy o 1
    if(size+1>capacity) {
        //tworzenie nowe tablicy o zwiekszonym rozmiarze
        T* newArray = new T[++size];
        //obliczenie pozycji wstawienia elementu
        int idx = (position.element - array);
        //przekopiowanie tablicy zostawiajac wolne miejsce na nowy element
        for(int i=0; i<size; i++) {
            if(i <=  idx) {
                newArray[i] = array[i];
            } else {
                newArray[i] = array[i-1];
            }}
        //dodanie nowego elementu na odpowiedniej pozycji
        newArray[idx] = element;
        //nadpisanie starej tablic nową
        delete [] array;
        array = newArray;
        capacity++;
    } else {
    
        //fprintf(stderr, "Przesuwanie elementów w tablicy...");
    
        //przekopiowanie elementów o jedną pozycję do przodu począwszy od
        //indeksu na który wskazuje position
        //Uwaga! przekopiowywanie realizowane od końca tablicy (element za ostatnim
        //elementem tablicy) schodząc w dół do elementu na który wskazuje
        //position.element
        for(T* ptr= end().element; ptr > position.element; ptr--)
         {
             *ptr = *(ptr-1);
         }
        //przypisanie przekazanego elementu na wskazana pozycję
        *(position.element) = element;
        //zwiększenie rozmiaru tablicy
        size++;
    }
    
    //testowanie poprawności dodania elementu
    //logArray();
    
    
    return position;
}
    
/**
 * enlargeCapacity() metoda zwiększająca o 1 pojemność tablicy 
 * jeżeli pojemność po zwiększeniu przekracza dopuszczalną maksymalną pojemność
 * to wyrzuca wyjątek 
 **/

//stare rozwiązanie z błędem!
template<typename T>
    void ArrayList<T>::enlargeCapacity(void) {
        
        printf("Zwiekszanie pojemności tablicy");
        
        if(capacity + 1 > MAX_CAPACITY)
            throw std::overflow_error("Przekroczenie dopuszczalnej pojemności tablicy");
        
        capacity++;
        
        T* newArray = (T *) realloc(array, sizeof(T)*capacity);
        
        if(newArray !=NULL) {
            array = newArray;
        }
        
        /*
        //zaalokowanie tablicy o większej pojemności w zmiennej newArray
        //oraz preinkrementacja zmiennej capacity
        T* newArray = new T[capacity +1];
       
        //przekopiowanie elementów tablicy array do nowo zalokowanej newArray
        for(int i=0; i< size; i++) {
            newArray[i] = array[i];
        }
        
        //dealokowanie starej tablicy
        delete [] array;
        //przypisanie do zmiennej składowej array nowo zaalokowana tablice z
        //przekopiowanymi elementami
        array = newArray;
        capacity++;
         */
        
        
}

    
/**
 * add_front() - dodaje element z przodu listy na pozycji array[0] tablicy
 * jednocześnie przesuwa wszystkie dotychczasowe elementy o jedna pozycje do przodu
 * Wykorzystuje metode add(position, element) podstawiajac za parametr position
 * iterator na poczatek listy (wywołanie begin()) 
 * @element - element typu const T& który ma byś umieszczony na poczatku listy
 **/
template<typename T>
ArrayIterator<T> ArrayList<T>::add_front(const T& element)
{
    //fprintf(stderr, "Dodawanie elementu %d na początek listy.\n", element);
    return add(begin(), element);
}

/**
 * add_back() - dodaje element na końcu listy na pozycji array[size] tablicy 
 * nie potrzebuje przesuwać elementów bo za ostatnim elementem nie ma już 
 * wiecej bieżących elementów. Wykorzystuje metodę add(position, element) 
 * podając jako argument iteratora position iterator na element za końcem listy
 * (wywołanie end()) 
 * @element - element typu const T& który ma zostać dodany na koniec listy
 **/
template<typename T>
ArrayIterator<T> ArrayList<T>::add_back(const T& element) {
    //fprintf(stderr, "Dodawanie elementu %d na koniec listy.\n", element);
    return add(end(), element);
}
    
/**
 * element_at(ArrayIterator<T> position) - metoda zwracajaca element listy  
 * przez referencje T& dla pozycji wskazanej iteratorem T& 
 * metoda bedzie wykorzystana równeiż przez first(), last(), oraz element_at(int) 
 * @position - argument typu ArrayIterator<T> wskazujący na pozycje z której 
 *              chcemy odczytać element 
 **/
template<typename T>
T& ArrayList<T>::element_at(ArrayIterator<T> position)
{
        return *(position.element);
}

/**
 * element_at(int) - przeciązona wersja metody zwracajaca element w i-tym węźle 
 * (na i-tej pozycji listy) wykorzystuje fakt, że lista jest zaimplementowana 
 * w oparciu o tablicę. Wywołuje metodę element_at(ArrayIterator<T>) tworząc itertor
 * na bazie wskaźnika (array + i) na i-tą pozycje tablicy array 
 * @i - liczba całkowita określająca pozycję z której chcemy odczytać element 
 **/
template<typename T>
T& ArrayList<T>::element_at(int i)
{
        if(i >= size) throw std::out_of_range("Indeks pozycji elementu poza zakresem tablicy implementującej listę.");
        ArrayIterator<T> iterator(array+i);
        return element_at(iterator);
}

    
/** 
 * first() - odczytuje pierwszy element listy 
 * Wykorzystuje metodę element_at(ArrayIterator<T>) przekazując jako argument 
 * iterator wskazujący na pierwszą pozycję na liście za pomocą wywołania metody
 * begin()
 **/
template<typename T>
T& ArrayList<T>::first()
{
        return element_at(begin());
}

/**
 * last() - odczytuje ostatni element listy 
 * wykorzystuje iterator na przedostatni element listy i metode element_at
 **/
template<typename T>
T& ArrayList<T>::last()
{
    return element_at(ArrayIterator<T>(array+(size-1)));
}
    
/**
 * remove() - metoda usuwa element znajdujacy sie na pozycji wskazywanej przez 
 * iterator ArrayIterator<T>  po usunięciu musi przesunąć elementy znajdujące się
 * za tym elementem o jedną pozycję niżej aż do końca listy. Na końcu zmniejsza 
 * bieżący rozmiar listy size--; 
 * @position - parametr typu ArrayIterator<T> wskazujacy na element do usuniecia 
 **/
template<typename T>
ArrayIterator<T> ArrayList<T>::remove(ArrayIterator<T> position)
{
    //jezeli usuwane element znajduje się poza listą
    if(position.element > end().element)
        throw std::out_of_range("Nie można usunąć elementu, iterator wskazuje poza listę.");
    for(T *ptr = position.element; ptr < end().element; ptr++)
    {
        *ptr = *(ptr+1);
    }
    size--;
    
    //testowanie poprawności usunięcia elementu
    //logArray();
    
    return position; //zwracamy iterator wskazujący na tę samą pozycję którą usuwano
}
    
/**
 * remove_first() - metoda usuwa element znajdujący się na pierwszej pozycji list
 * wykorzystuje w tym celu metode remove() oraz iterator ArrayIterator<T> ustawiony
 * na pozycje poczatku listy. (wywołanie begin()) 
 **/
template<typename T>
ArrayIterator<T> ArrayList<T>::remove_first()
{
    return remove(begin());
}
    
/**
 * logArray() - metoda wydrukowuje na standardowe wyjscie bledu aktualna 
 * zawartosc tablicy
 **/
template<typename T>
void ArrayList<T>::logArray(void)
{
    //testowe wypisanie tablic po dodaniu elementu:
    fprintf(stderr, "\n{ ");
    for(int i=0; i<size; i++) {
        fprintf(stderr, "%d, ", array[i]);
    }
    fprintf(stderr, "}\n");
}
    
/**
 * count() - zwraca bieżący rozmiar tablicy 
 **/
template<typename T>
int ArrayList<T>::count(void)
{
        return size;
}

} //namespace ADT end


//test ArrayList<T> - należy odkomentować metodę logArray()
/*
int main(int argc, char *argv[])
{
    ADT::ArrayList<int> list(20);
    
    printf("Aktualny rozmiar list: %d\n", list.count());
    list.add_front(1);
    list.add_front(2);
    list.add_back(3);
    printf("Odczytano z pozycji 2: %d\n",  list.element_at(1));
    printf("Ostatni element: %d\n", list.last());
    printf("Pierwszy element: %d\n", list.first());
    printf("Aktualny rozmiar list: %d\n", list.count());
    list.remove_first();
    printf("Aktualny rozmiar list: %d\n", list.count());
    
    return 0;
}*/


int main(int argc, char *argv[])
{
    int tc;
    scanf("%d", &tc);
    
    //tworzymy liste z tablica o pojemnosci odpowiadajacej liczbie komend
    ADT::ArrayList<int> L(10);
    
    
    for(int t=0; t<tc; t++) {
        char c = ' ';
        while(c!='A' && c!='D' && c!='S')
            c = getchar(); // wczytuj znak po znaku dopoki nie napotkasz znaku dot. polecenia
        if(c=='A') {
            int num;
            scanf("%d", &num);
            L.add_back(num);
        }
        else if(c=='D') {
            if(L.count() == 0) // lista pusta
                printf("EMPTY\n");
            else {
                int num = L.first();
                L.remove_first();
                printf("%d\n", num);
            }
        }
        else {
            printf("%d\n", L.count());
        }
    }

    
    return 0;
}