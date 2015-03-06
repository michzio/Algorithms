#include<cstdlib>
#include<stdexcept>
#include<string>
#include<cstring>
#include<iostream>

#define DEBBUG 0

/**
 * Implementacja kolejki priorytetowej bazująca 
 * na podstawie kopca
 **/
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
    
    int array_size;
    int heap_counter;
    int *heap_array = NULL;
    
    public:
    
    PriorityQueue(int size);
    
    void Enqueue(int elem);
    int FindMax();
    int DequeueMax();
    void Print(void);
    
    private:
    void compareElementGoingUp(int i);
    void compareElementGoingDown(int i);
    
};


PriorityQueue::PriorityQueue(int size)
{
    heap_counter = 0;
    array_size = size;
    heap_array = (int *) malloc (size * sizeof(int));
}

int PriorityQueue::FindMax()
{
    if(heap_counter < 1)
        throw EmptyHeap("Próba pobrania maksymalnego elementu (szczyt kopca) z pustego kopca");
    
    return heap_array[0];
}

void PriorityQueue::Enqueue(int elem)
{
    
    if(heap_counter + 1 > array_size) {
        //realokowanie tablicy stosu
        printf("Realokowanie tablicy kopca...\n");
        
        int *tmp = NULL;
        tmp = (int *) realloc(heap_array, (++array_size) *sizeof(int));
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

void PriorityQueue::compareElementGoingUp(int currentIdx)
{
    //pobranie elementu nadrzędnego bieżącego
    int parentIdx = (currentIdx-1)/2;
    
    while(currentIdx > 0 && (heap_array[currentIdx] > heap_array[parentIdx]))
    {
        //zamiana elementów swap
        int tmp = heap_array[parentIdx];
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
int PriorityQueue::DequeueMax()
{
    if(heap_counter < 1)
        throw EmptyHeap("Próba zdjęcia elementu z pustej kolejki priorytetowej");
    
    int max = heap_array[0];
    
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
void PriorityQueue::compareElementGoingDown(int currentIdx)
{
    int leftChildIdx = 2*currentIdx + 1;
    
    //wykonuj do momentu gdy nie dojdziesz do liścia kopca
    while(leftChildIdx < heap_counter) {
        
        int rightChildIdx = leftChildIdx + 1;
        //najpierw sprawdzenie dla prawego dziecka
        if(rightChildIdx < heap_counter && heap_array[leftChildIdx] < heap_array[rightChildIdx])
        {
            //element z prawej strony jest większy od elementu z lewej strony
            //jest potencjalnym kandydatem do zamiany
            if(heap_array[currentIdx] < heap_array[rightChildIdx])
            {
                //zamieniamy elementy kopca -> swap
                int tmp = heap_array[rightChildIdx];
                heap_array[rightChildIdx] = heap_array[currentIdx];
                heap_array[currentIdx] = tmp;
                
                //aktualizacja pozycji w której znajduje się bieżacy element
                currentIdx  = rightChildIdx;
                
            } else
                return;
            
            
        } else {
            //element z lewej strony jest większy o elementu z prawej strony
            //jest potencjalnym kandydatem do zamiany
            
            if(heap_array[currentIdx] < heap_array[leftChildIdx])
            {
                //zamieniamy elementy kopca -> swap
                int tmp = heap_array[leftChildIdx];
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
void PriorityQueue::Print(void)
{
    for(int i=0; i < heap_counter; i++)
    {
        printf("%d,", heap_array[i]);
    }
    
    printf("\n");
}


int main(int argc, char **argv)
{
    PriorityQueue *pq = new PriorityQueue(2);
    
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
    std::cout << "Zdjęto element: " << pq->DequeueMax() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueMax() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueMax() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueMax() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueMax() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueMax() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueMax() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueMax() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueMax() << std::endl;
    if(DEBBUG) pq->Print();
    std::cout << "Zdjęto element: " << pq->DequeueMax() << std::endl;
    if(DEBBUG) pq->Print();
    
    return 0;
}