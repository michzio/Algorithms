#include<cstdlib>
#include<stdexcept>
#include<string>
#include<cstring>
#include<iostream>

#define DEBBUG 0

/**
 * Przykładowe komparatory określający kolejność elementów
 * w kolejce priorytetowej. Jest to obiekt funkcyjny (funktor) 
 * który jest używany do porównywania elementów.
 **/
struct LessInt {
  bool operator()(const int& a, const int& b) const
    {
        return a < b;
    }
} lessInt;

struct GreaterInt {
    bool operator()(const int& a, const int& b) const
    {
        return a > b;
    }
} greaterInt;

struct LessString {
    bool operator()(const std::string& a, const std::string& b) const
    {
        return a < b;
    }
} lessString;

struct GreaterString {
    bool operator()(const std::string& a, const std::string& b) const
    {
        return a > b;
    }
} greaterString;

/**
 * Implementacja kolejki priorytetowej bazująca 
 * na podstawie kopca
 **/
template<class T, class Compare>
class PriorityQueue
{
    class EmptyHeap : public std::out_of_range
    {
        public:
        //konstruktor wyjątku z C-style stringiem
        explicit EmptyHeap(const char* msg) : std::out_of_range(msg) { }
        //konstruktor wyjątku z C++ stringiem
        explicit EmptyHeap(const std::string& msg) : std::out_of_range(msg) { }
        //destruktor - virtual aby pozwolić tworzenie podklas
        virtual ~EmptyHeap() throw() { }
        
    };
    
    Compare compare;
    
    int array_size;
    int heap_counter;
    T *heap_array;
    
    public:
    
    PriorityQueue(int size, Compare comp);
    
    void Enqueue(T elem);
    T Top();
    T DequeueTop();
    void Print(void);
    
    private:
    void compareElementGoingUp(int i);
    void compareElementGoingDown(int i);
    
};

/**
 * Konstruktor kolejki priorytetowej
 **/
template<class T, class Compare>
PriorityQueue<T, Compare>::PriorityQueue(int size, Compare comp)
{
    //ustawienie komparatora
    compare = comp;
    
    heap_counter = 0;
    array_size = size;
    heap_array = (T *) malloc( sizeof(T) *size);
}

/**
 * Funkcja pobiera maksymalny element z kolejki priorytetowej
 **/
template<class T, class Compare>
T PriorityQueue<T, Compare>::Top()
{
    if(heap_counter < 1)
        throw EmptyHeap("Próba pobrania maksymalnego elementu (szczyt kopca) z pustego kopca");
    
    return heap_array[0];
}

/**
 * Umieszczanie elementu w kolejce priorytetowej 
 * na odpowiedniej pozycji. Najpierw wstawia na koniec 
 * tablicy reprezentujacej kolejke a potem przesiewanie w górę.
 **/
template<class T, class Compare>
void PriorityQueue<T, Compare>::Enqueue(T elem)
{
    if(heap_counter + 1 > array_size) {
        //realokowanie tablicy stosu
        if(DEBBUG) printf("Realokowanie tablicy kopca...\n");
        
        T *tmp = NULL;
        tmp = (T *) realloc(heap_array, (++array_size) *sizeof(T));
        if(tmp == NULL) {
            printf("Błąd podczas realokowania tablicy...\n");
            return;
        } else {
            heap_array = tmp;
        }

    }

    //dodajemy element na koniec tablicy
    heap_array[heap_counter++] = elem;
    //uporządowanie ponowne stosu przechodząc stos binarny w górę
    //począwszys od wstawionego elementu
    compareElementGoingUp(heap_counter-1);
    
}

/**
 * Funkcja realizująca przesiewanie w góre tj. porównywanie 
 * bieżącego elementu z kolejnymi rodzicami, aż dojdziemy do 
 * szczytu stosu lub natrafimy na element z którym bieżący 
 * element będzie w odpowiedniej relacji
 **/
template<class T, class Compare>
void PriorityQueue<T, Compare>::compareElementGoingUp(int currentIdx)
{
    //pobranie elementu nadrzędnego bieżącego
    int parentIdx = (currentIdx-1)/2;
    
    while(currentIdx > 0 && compare(heap_array[currentIdx], heap_array[parentIdx]))
    {
        //zamiana elementów swap
        T tmp = heap_array[parentIdx];
        heap_array[parentIdx] = heap_array[currentIdx];
        heap_array[currentIdx] = tmp;
        
        currentIdx = parentIdx;
        parentIdx = (currentIdx -1)/2;
        
    }
}

/**
 * Funkcja zdejmuje element maksymalny z kolejki priorytetowej
 * i usuwa go. Po usunieciu elementu szczytowego umieszcza na szczycie
 * kopca ostatni element tablicy i nastepnie wykonuje przesiewanie w dół
 **/
template<class T, class Compare>
T PriorityQueue<T, Compare>::DequeueTop()
{
    if(heap_counter < 1)
        throw EmptyHeap("Próba zdjęcia elementu z pustej kolejki priorytetowej");
    
    T max = heap_array[0];
    
    //jako pierwszy element przypisujemy ostatni element w tablicy
    heap_array[0] = heap_array[--heap_counter];
    //uporządkowanie elementu przeniesionego na szczyt kopca w idąc w dół
    compareElementGoingDown(0);
    
    return max;
}

/**
 * Metoda przesiewania kopca w dół. Polaga na tym, że dla elementu 
 * o wskazanym indeksie currentIdx przechodzimy kopiec w dół porównując
 * bieżacy element z kolejnymi elementami potomnymi celem umieszczenia 
 * go na odpowiedniej pozycji w kopcu. 
 * Stosowane podczas usuwania elementu ze szczytu kopca, kiedy wymagana
 * jest rearanżacja kopca tak by spełniał on ustaloną relację
 * Proces przesiewania w dół jest kontynuowany aż do momentu gdy nie 
 * dojdziemy do liścia lub nie znajdziemy dziecka którego wartość mniejsza 
 * lub równa wartości bieżacego przesiewanego elementu.
 **/
template<class T, class Compare>
void PriorityQueue<T, Compare>::compareElementGoingDown(int currentIdx)
{
    int leftChildIdx = 2*currentIdx + 1;
    
    //wykonuj do momentu gdy nie dojdziesz do liścia kopca
    while(leftChildIdx < heap_counter) {
        
        int rightChildIdx = leftChildIdx + 1;
        //najpierw sprawdzenie dla prawego dziecka
        if(rightChildIdx < heap_counter && compare(heap_array[rightChildIdx], heap_array[leftChildIdx]))
        {
            //element z prawej strony jest większy od elementu z lewej strony
            //jest potencjalnym kandydatem do zamiany
            if(compare(heap_array[rightChildIdx], heap_array[currentIdx]))
            {
                //zamieniamy elementy kopca -> swap
                T tmp = heap_array[rightChildIdx];
                heap_array[rightChildIdx] = heap_array[currentIdx];
                heap_array[currentIdx] = tmp;
                
                //aktualizacja pozycji w której znajduje się bieżacy element
                currentIdx  = rightChildIdx;
                
            } else
                return;
            
            
        } else {
            //element z lewej strony jest większy o elementu z prawej strony
            //jest potencjalnym kandydatem do zamiany
            
            if(compare(heap_array[leftChildIdx], heap_array[currentIdx]))
            {
                //zamieniamy elementy kopca -> swap
                T tmp = heap_array[leftChildIdx];
                heap_array[leftChildIdx] = heap_array[currentIdx];
                heap_array[currentIdx] = tmp;
                
                //aktualizacja pozycji w której znajduje się bieżący element
                currentIdx = leftChildIdx;
                
            } else
                return;
        }
        
        //aktualicacja licznika indeksu wskazujacego na nowe lewe dziecko
        //bieżącego nowego węzła
        leftChildIdx = 2*currentIdx + 1;
    }
}

/**
 * Pomocnicza metoda która wypisuje zawartość kolejki priorytetowej 
 * w celach testowych
 **/
template<class T, class Compare>
void PriorityQueue<T, Compare>::Print(void)
{
    for(int i=0; i < heap_counter; i++)
    {
        std::cout << heap_array[i] << ",";
    }
    
    std::cout << std::endl;
}


int main(int argc, char **argv)
{
    PriorityQueue<int, LessInt> *pq = new PriorityQueue<int, LessInt>(2, lessInt);
    
    std::cout << "Umieszczanie elementów w kolejce..." << std::endl;
    
    pq->Enqueue(2);
    pq->Enqueue(5);
    pq->Enqueue(1);
    pq->Enqueue(10);
    pq->Enqueue(-5);
    pq->Enqueue(-1);
    pq->Enqueue(-20);
    pq->Enqueue(30);
    pq->Enqueue(15);
    pq->Enqueue(10);
    pq->Print();
    
    std::cout<< "Zdejmowanie elementów z kolejki..." << std::endl;
    std::cout << "Zdjęto element: " << pq->DequeueTop() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueTop() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueTop() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueTop() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueTop() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueTop() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueTop() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueTop() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueTop() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueTop() << std::endl;
    if(DEBBUG) pq->Print();
    
    
    PriorityQueue<std::string, LessString> *spq = new PriorityQueue<std::string, LessString>(10, lessString);
    
    spq->Enqueue("lol");
    spq->Enqueue("pies");
    spq->Enqueue("kot");
    spq->Enqueue("banan");
    spq->Enqueue("dom");
    spq->Enqueue("kwiatek");
    spq->Enqueue("baran");
    spq->Enqueue("kot");
    spq->Enqueue("krowa");
    spq->Enqueue("nie");
    spq->Print();
    
    std::cout<< "Zdejmowanie elementów z kolejki..." << std::endl;
    std::cout << "Zdjęto element: " << spq->DequeueTop() << std::endl;
    if(DEBBUG) spq->Print();
    std::cout << "Zdjęto element: " << spq->DequeueTop() << std::endl;
    if(DEBBUG) spq->Print();
    std::cout << "Zdjęto element: " << spq->DequeueTop() << std::endl;
    if(DEBBUG) spq->Print();
    std::cout << "Zdjęto element: " << spq->DequeueTop() << std::endl;
    if(DEBBUG) spq->Print();
    std::cout << "Zdjęto element: " << spq->DequeueTop() << std::endl;
    if(DEBBUG) spq->Print();
    std::cout << "Zdjęto element: " << spq->DequeueTop() << std::endl;
    if(DEBBUG) spq->Print();
    std::cout << "Zdjęto element: " << spq->DequeueTop() << std::endl;
    if(DEBBUG) spq->Print();
    std::cout << "Zdjęto element: " << spq->DequeueTop() << std::endl;
    if(DEBBUG) spq->Print();
    std::cout << "Zdjęto element: " << spq->DequeueTop() << std::endl;
    if(DEBBUG) spq->Print();
    std::cout << "Zdjęto element: " << spq->DequeueTop() << std::endl;
    if(DEBBUG) spq->Print();
    
    
    
    
    
    PriorityQueue<int, GreaterInt> *gpq = new PriorityQueue<int, GreaterInt>(2, greaterInt);
    
    std::cout << "Umieszczanie elementów w kolejce..." << std::endl;
    
    gpq->Enqueue(2);
    gpq->Enqueue(5);
    gpq->Enqueue(1);
    gpq->Enqueue(10);
    gpq->Enqueue(-5);
    gpq->Enqueue(-1);
    gpq->Enqueue(-20);
    gpq->Enqueue(30);
    gpq->Enqueue(15);
    gpq->Enqueue(10);
    gpq->Print();
    
    std::cout<< "Zdejmowanie elementów z kolejki..." << std::endl;
    std::cout << "Zdjęto element: " << gpq->DequeueTop() << std::endl;
    if(DEBBUG) gpq->Print();
    std::cout << "Zdjęto element: " << gpq->DequeueTop() << std::endl;
    if(DEBBUG) gpq->Print();
    std::cout << "Zdjęto element: " << gpq->DequeueTop() << std::endl;
    if(DEBBUG) gpq->Print();
    std::cout << "Zdjęto element: " << gpq->DequeueTop() << std::endl;
    if(DEBBUG) gpq->Print();
    std::cout << "Zdjęto element: " << gpq->DequeueTop() << std::endl;
    if(DEBBUG) gpq->Print();
    std::cout << "Zdjęto element: " << gpq->DequeueTop() << std::endl;
    if(DEBBUG) gpq->Print();
    std::cout << "Zdjęto element: " << gpq->DequeueTop() << std::endl;
    if(DEBBUG) gpq->Print();
    std::cout << "Zdjęto element: " << gpq->DequeueTop() << std::endl;
    if(DEBBUG) gpq->Print();
    std::cout << "Zdjęto element: " << gpq->DequeueTop() << std::endl;
    if(DEBBUG) gpq->Print();
    std::cout << "Zdjęto element: " << gpq->DequeueTop() << std::endl;
    if(DEBBUG) gpq->Print();
    
    PriorityQueue<std::string, GreaterString> *gspq = new PriorityQueue<std::string, GreaterString>(10, greaterString);
    
    gspq->Enqueue("lol");
    gspq->Enqueue("pies");
    gspq->Enqueue("kot");
    gspq->Enqueue("banan");
    gspq->Enqueue("dom");
    gspq->Enqueue("kwiatek");
    gspq->Enqueue("baran");
    gspq->Enqueue("kot");
    gspq->Enqueue("krowa");
    gspq->Enqueue("nie");
    gspq->Print();
    
    std::cout<< "Zdejmowanie elementów z kolejki..." << std::endl;
    std::cout << "Zdjęto element: " << gspq->DequeueTop() << std::endl;
    if(DEBBUG) gspq->Print();
    std::cout << "Zdjęto element: " << gspq->DequeueTop() << std::endl;
    if(DEBBUG) gspq->Print();
    std::cout << "Zdjęto element: " << gspq->DequeueTop() << std::endl;
    if(DEBBUG) gspq->Print();
    std::cout << "Zdjęto element: " << gspq->DequeueTop() << std::endl;
    if(DEBBUG) gspq->Print();
    std::cout << "Zdjęto element: " << gspq->DequeueTop() << std::endl;
    if(DEBBUG) gspq->Print();
    std::cout << "Zdjęto element: " << gspq->DequeueTop() << std::endl;
    if(DEBBUG) gspq->Print();
    std::cout << "Zdjęto element: " << gspq->DequeueTop() << std::endl;
    if(DEBBUG) gspq->Print();
    std::cout << "Zdjęto element: " << gspq->DequeueTop() << std::endl;
    if(DEBBUG) gspq->Print();
    std::cout << "Zdjęto element: " << gspq->DequeueTop() << std::endl;
    if(DEBBUG) gspq->Print();
    std::cout << "Zdjęto element: " << gspq->DequeueTop() << std::endl;
    if(DEBBUG) gspq->Print();
    
    return 0;
}