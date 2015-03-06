/***************************************************
 * @Autor: Michał Ziobro, Data: May 6th 2014       *
 * PROGRAM JEST MODYFIKACJĄ WCZEŚNIEJSZEGO PROGRAMU*
 * Z ALGORYTMEM DIJKSTRY                           *
 * Kompilacja:                                     *
 * g++ -std=c++11 -o komiwojazer komiwojazer.cpp   *
 * Uruchomienie:                                   *
 * ./komiwojazer graph01.dat                       *
 ***************************************************/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include<iostream>
#include <limits>
#include<set>
#include<stack>
#include<string>
#include<algorithm>

#define DEBBUG 0
#define DEBBUG_2 0
#define DEBBUG_3 1
#define DEBBUG_4 1
#define BUFF_SIZE 128
#define INFINITY std::numeric_limits<int>::max()

/**
 * Funkcja pobiera wskaźnik na pierwszy element permutowanej tablcy (begin) i na element tuż za ostatnim (end) tj. begin + length
 * na podstawie funkcji z STL std::next_permutation()
 **/
bool generateNextPermutation(int *begin, int *end)
{
    //jeżeli początkowy i końcowy element tablicy się pokrywają
    if (begin == end) return false;
    int *i = end -1;
    if (begin == i) return false;
    
    //pętla nieskończona
    while (1) {
        //deklatacja 2 wskaźników na elementy tablicy (permutowanego ciągu)
        int *p1, *p2;
        
        //w pierwszym wykonaniu pętli "i" wskazuje na ostatni element ciągu, w natępnych przejściach
        //pętli na kolejne elementy idąc z prawej w lewą stronę ciągu
        p1 = i;
        
        //element poprzedzający w ciągu mniejszy od występującego po nim, (leftElem < rightElem)
        //np w ciągu 5 3 4 2 1 przejdziemy pętlą, aż do napotkania pary elementów (3,4) p1 -> 4, i będzie wskazywać na i -> 3
        if (*(--i) < *p1) {
            //p2 ustawiamy na element tuż za ostatnim w ciągu
            p2 = end;
            
            //pętla wykonywana do momentu gdy elementy idąc od końca nie są większe od elementu na który wskazuje "i"
            // w przykładzie jak wyżej zaczynamy od --p2 -> 1
            // i będziemy szukać p2 wskazującego na element który będzie większy od elementu na który wskazuje "i"
            // dojdziemy wten sposób p2 -> 4
            while (!(*i < *(--p2)))
                ;
            //zamieniamy element na który wskazuje "i" z elementem na który wskazuje p2  (zawsze: *i < *p2)
            int tmp = *i;
            *i = *p2;
            *p2 = tmp;
            //w przykładzie będzie 5 4 3 2 1 !
            
            //odwracamy kolejność elementów w końcówce ciągu począwszy od elementu będącego na pozycji za elementem wskazywanym przez "i"
            //teraz jest to element p1 -> 3  czyli odwracamy (3,2,1) -> (1,2,3)
            std::reverse(p1, end);
            //otrzymamy w przykładzie 5 4 1 2 3
            
            return true;
        }
        
        //przeiterowaliśmy wskaźnikiem "i" od ostatniego elementu, aż do początkowego
        //tj. gdy ciąg był uporządkowany od największego -> najmniejszego elementu np. 5 4 3 2 1 !
        //w takiej sytuacji odwracamy kolejność ciągu na -> 1 2 3 4 5
        //i zwracamy false oznaczające, że nie można już wygenerować wiecej permutacji
        if (i == begin) {
            std::reverse(begin, end);
            return false;
        }
       
    }
}

struct Vertex;

/**
 * krawędzie będą elementami listy sąsiedztwa dla 
 * każdego wierzchołka, pozwoli to na przechowywanie 
 * sąsiadującego wierzchołka wraz z wagą krawędzi prowadzącej 
 * do tego wierzchołka 
 **/
struct Edge
{
private:
    Vertex *destination;
public:
    //waga krawędzi
    int weight;
    //wskaźnik na kolejny element listy sąsiedztwa
    Edge *next;
    //konstruktor krawędzi
    Edge(Vertex *v, int w) : destination(v), weight(w) {}
    
    /**
     * zwraca wierzchołek do którego prowadzi bieżąca krawędź
     **/
    Vertex *getVertex()
    {
        return destination;
    }

};

struct Vertex
{
    int label;
    //lista sąsiedztwa wierzchołka
    Edge *adjecentVertices = 0;
    
    //konstruktor wierzchołka
    Vertex(int l) : label(l) {}
    
    /**
     * dodawanie wierzchołków do listy sąsiedztwa bieżącego wierzchołka
     * nowy wierzchołek przyległy wstawiamy na początku listy -> zawsze czas O(1)
     * wstawiania. Domyślna waga to 1, w przypadku gdyby nie był ważony graf
     * (tak by długość każdej krawędzi była równa). 
     * wartość przypisaną do krawędzi można zmodyfikować metodą setWeight() na Edge!
     **/
    void addAdjecentVertex(Vertex *v, int weight = 1)
    {
        Edge *e = new Edge(v, weight);
        
        //gdy lista sąsiedztwa jest aktualnie pusta
        if(adjecentVertices == NULL)
        {
            adjecentVertices = e;
            return;
        }
        
        Edge *tmp;
        //ustawiamy wskaźnik tmp na ostatni element na liście sąsiedztwa
        for(tmp = adjecentVertices; tmp->next !=NULL; tmp = tmp->next);
        //wstawienie nowego elementu
        tmp->next = e;
        
        /*wstawienie w czasie 0(1)
        e->next = adjecentVertices;
        adjecentVertices = e;
         */
    }
    
    /**
     * metoda która przeszukuje listę sąsiedztwa wierzchołka A w celu 
     * odnalezienia bezpośredniej krawędzi do wierzchołka B
     **/
    Edge *directEdge(Vertex *B)
    {
            //1. Przechodzimy krawędzi poszukując wierzchołka B
        for(Edge *e = adjecentVertices; e!= NULL; e = e->next)
        {
            //Odnaleźliśmy bezpośrednią krawędź z A do B
            if(e->getVertex()->label == B->label)
                return e;
        }
        
        //2. zwracamy NULL tj. przeszlismy cala liste sasiedztwa i nie znalezlismy B
        return NULL;
    }
    
     bool operator< ( Vertex const &q) const {return label < q.label;}
};

std::ostream& operator<<(std::ostream &strm, const Vertex &v) {
    return strm << v.label;
}

class Dijkstra;

class Graph
{
    int noOfVertices = 0;
    
    //lista sąsiedztwa, poszczególne pozycje w tablicy wskazują na bezpośrednich
    //sąsiadów wierzchołka poprzez krawędzie, które prowadzą do nich.
    //będziemy alokować tablice o rozmiarze równym ilości wierzchołków
    Vertex *adjacencyList;
    
    //obiekt z algorytmem Dijkstry
    Dijkstra *dijkstra = 0;
    
public:
    
    Graph(FILE *pFile);
    ~Graph();
    
    void printAdjacencyList(void);
    //ta metoda używa algorytmu Dijkstry
    int findMinPath(Vertex *a, Vertex *b, std::string *path);
    Vertex *getVertex(int label) const;
    int size(void) const;
    
    /*************** PROBLEM KOMIWOJAŻERA ******************************/
    int minHamiltonianCycle(Vertex **cycle , int &weight);
    static void displayCycle(Vertex **cycle, int length);
    /*************** PROBLEM KOMIWOJAŻERA - END ************************/
    
private:
    int readLine(char *buffer, size_t n, FILE *pFile);
    void addVerticesToAdjacencyListOf(Vertex *v, char *vertexList);
    /************** PROBLEM KOMIWOJAŻERA *******************************/
     int isHamiltonianCycle(int *permutation, int n, int weight);
     int getEdgeWeightBetween(int v1, int v2);
    /************** PROBLEM KOMIWOJAŻERA - END *************************/
    
};


//implementacja Kolejki priorytetów używając std::set<>
class PriorityQueue
{
    //tablica priorytetów - mapowanie Vertex->label --> idx tablicy
    int size;
    static int *priorities;
    
    //komparator elementów w zbiorze (kolejce) bazujący na ich priorytetach
    struct minPriorityComp {
        bool operator() (const Vertex *v1, const Vertex *v2) const
        {
            return priorities[v1->label-1] < priorities[v2->label-1];
        }
    };
    
    struct labelEqual {
        labelEqual(int l) : label(l) {}
        bool operator()(const Vertex * vert) {
            return vert->label == label;
        }
        private:
        int label;
    };
    
    std::multiset<const Vertex *, minPriorityComp> queue;
    
public:
    PriorityQueue(int s) : size(s) {
        priorities = (int *) malloc(sizeof(int) * size);
    }
    ~PriorityQueue() {}
    
    void push(const Vertex *v, int priority);
    const Vertex *pop(void);
    
    void decreasePriority(const Vertex *, int priority);
    bool empty(void);
    
    
    void print(void)
    {
        for(auto it = queue.cbegin(); it != queue.cend(); it++)
        {
            std::cout << (*it)->label << ", ";
        }
        
        std::cout << std::endl;
    }
    
};

//inicjalizacja statycznej zmiennej składowej
int *PriorityQueue::priorities = 0;

/**
 * Metoda wstawia element do kolejki. Złożoność O(log(n))
 **/
void PriorityQueue::push(const Vertex *v, int priority)
{
    //zapisanie priorytetu w tablicy & dodanie wierzchołka do kolejki
    priorities[v->label -1] = priority;
    queue.insert(v);
}



/**
 * Metoda pobiera pierwszy element z kolejki o najmniejszym priorytecie.
 * Złożoność 0(1)
 **/
const Vertex *PriorityQueue::pop(void)
{   auto iterator = queue.begin();
    queue.erase(iterator);
    return *iterator;
}

/**
 * Metoda aktualizujaca priorytet elementu w kolejce. Złożoność O(n)
 **/
void PriorityQueue::decreasePriority(const Vertex *v, int priority)
{
    
    //auto it = queue.find(v); //O(log(n))
     auto it = std::find_if(queue.begin(), queue.end(), labelEqual(v->label));  //O(n)
    
    if(it != queue.cend()) {
    
        if(DEBBUG_2)
            std::cout << "odnajdziono " << (*it)->label << std::endl;
    
        //usuniecie starego elementu
        queue.erase(it); //O(1)
        if(DEBBUG_2) {
            std::cout << "po usunięciu " << v->label << " mamy "; print();
    }
        
    }
    //ustawienie nowego priorytetu dla wierzchołka
    priorities[v->label-1] = priority;
    //dodanie nowego elementu
    queue.insert(v); //O(log(n))
    
    if(DEBBUG_2) {
        std::cout << "po dodaniu " << v->label << " mamy "; print();
    }
}

/**
 * Metoda określa czy kolejka priorytetowa jest pusta
 **/
bool PriorityQueue::empty()
{
    return queue.size() == 0;
}

class Dijkstra
{
    //graf na którym algorytm Dijkstry wyszukuje minimalnych ścieżek
    const Graph *graph;
    
    //tablica z odległościami (kosztami) dojścia od wierzchołka początkowego
    //src do i-tego wierzchołka (i-ty element tablicy) idąc najkrótszą ścieżką
    //Odległości obliczane są jako suma wag krawędzi znajdujących się na najkrótszej
    //ścieżce ze źródła do i-tego wierzchołka. Początkowo inicjalizowane (funkcja
    //init() wartościami nieskończoności. Alokowane w zależności od wielkości grafu
    int *distance;
    
    //tablica przodków danego wierzchołka wracając po najkrótszej ścieżce
    //pozwala nam rekonstruować najkrótszą ścieżkę
    const Vertex **predecessor;
    
    //bieżący wierzchołek źródłowy - do wyszukiwania ścieżek
    //ustawiany przez funkcje search MinPathsFrom(Vertex *src);
    const Vertex *source = 0;
    
    
public:
    //konstruktor
    Dijkstra(const Graph *g) : graph(g) {
        //alokowanie tablic pomocniczych do algorytmu Dijkstry
        distance = (int *) malloc(sizeof(int) * (graph->size()));
        predecessor = (const Vertex **) malloc(sizeof(const Vertex*) * (graph->size()) );
    }
    //destruktor
    ~Dijkstra();
    
    //pobranie minimalnej ścieżki na podstawie ostatnich obliczeń między (src, dest)
    int minPath(const Vertex *src, const Vertex *dest, std::string *path);
  
private:
    //metoda wyszukuje minimalnej długości ścieżki z wierzchołka źródłowego
    int searchMinPathsFrom(const Vertex *src);
    //metoda resetuje i inicjalizuje pomocnicze tablice wykorzystywane
    //przez algorytm Dijkstry
    void init(void);

};

/**
 * konstruktor wczytujacy graf z pliku o wskazanym strumieniu
 **/
Graph::Graph(FILE *pFile)
{
    //buffor na odczytywanie linijki tekstu
    char *buffer;
    
    //zaalokowanie bufforu do którego odczytujemy poszczególne linijki tekstu
    //z pliku. w razie gdyby był za mały na umieszenie linii tekstu bedziemy go
    //realokowac
    if( !(buffer = (char *) malloc(sizeof(char) * (BUFF_SIZE + 1))) )
    {
        perror("Bład podczas alokowania buffora pamięci.");
        exit(1);
    }
    
    //1. Wczytujemy liczbę wierzchołków
    if(readLine(buffer, BUFF_SIZE, pFile) < 1)
    {
        printf("Błąd podczas odczytywania liczby wierzchołków grafu.\n");
        exit(1);
    }
    
    noOfVertices = atoi(buffer);
    
    if(DEBBUG)
        printf("Liczba wierzchołków: %d\n", noOfVertices);
    
    if(DEBBUG)
        printf("### KONSTRUKCJA LISTY SĄSIEDZTWA ###\n");
    
    //alokujemy tablicę wierzchołków z listami sąsiedztwa
    adjacencyList = (struct Vertex *) malloc( sizeof(Vertex) *noOfVertices);
    
    //dodajemy wszystkie n wierzchołków do tablicy sąsiedztwa
    for(int i=0; i < noOfVertices; i++)
    {
        adjacencyList[i] = Vertex(i+1);
    }
    
    //wczytujemy kolejne listy sąsiedztwa poszczególnych wierzchołków
    for(int i=0; i < noOfVertices; i++)
    {
        if( readLine(buffer, BUFF_SIZE, pFile) < 1)
        {
            //nie dopuszczamy sytuacji w której wierzchołek nie ma sąsiadów
            printf("Wierzchołek %d nie posiada sąsiada!\n", (i+1));
            //exit(1);
        }
        
        if(DEBBUG)
            printf("%s\n", buffer);
        
        //dodajemy odczytane wierzchołki do listy sąsiedztwa używając metody pomocniczej
        addVerticesToAdjacencyListOf(&adjacencyList[i], buffer);
    }
    
    if(DEBBUG)
        printf("### KONIEC KONSTRUKCJI ###");
    
    
    free(buffer);
}

/**
 * metoda zwraca rozmiar graphu tj. liczbę jego wierzchołków
 **/
int Graph::size(void) const
{
    return noOfVertices;
}

/**
 * destruktor 
 **/
Graph::~Graph()
{
    free(adjacencyList);
}

/**
 * Metoda wypisuje aktualny graf w postaci listy sąsiedztwa
 **/
void Graph::printAdjacencyList(void)
{
    printf("\nWypisanie listy sąsiedztwa: \n");
    for(int i=0; i<noOfVertices; i++)
    {
        printf(" %d | ", adjacencyList[i].label);
        
        for(Edge *e = adjacencyList[i].adjecentVertices; e !=NULL; e= e->next)
        {
            printf("%d (%d), ", e->getVertex()->label, e->weight);
        }
        
        printf("\n");
    }
    printf("\n");
}

/**
 * Metoda pomocnicza, która pobiera oddzielona ", " listę wierzchołków
 * wraz z wagami odpowiadającymi poszczególnym krawędzią prowadzącym do 
 * danego wierzchołka. Metoda wstawia odczytane wierzchołki jako wierzchołki
 * sąsiadujące na liste sąsiedztwa wierzchołka v pobranego jako pierwszy 
 * argument. 
 * ex. 1 6, 2 3, 3 6
 * na powyższej przykładowej liscie pierwsza liczba oznacza nr wierzcholka
 * druga oznacza wagę wierzchołka
 **/
void Graph::addVerticesToAdjacencyListOf(Vertex *v, char *vertexList)
{
    int label;
    int weight;
    char *token;
    
    token = strtok(vertexList, " ");
    while(token != NULL)
    {
        //odczytanie numeru węzła
        if(DEBBUG)
            printf("%s ", token);
        
        label = atoi(token);
        
        //odczytanie wagi
        token = strtok(NULL, ",");
        if(token == NULL) {
            printf("Błąd w pliku z inputem grafu. Braku wagi dla krawędzi %d -> %d.\n", v->label, label);
            exit(1);
        }
        
        if(DEBBUG)
            printf("%s\n", token);
        
        weight = atoi(token);
        //z tablicy wybieramy odpowiedni wierzchołek i wstawiamy
        //na listę sąsiedztwa bieżącego wierzchołka v podając jako
        //2 parametr wage krawędzi poprowadzonej pomiędzy tymi wierzchołkami
        v->addAdjecentVertex( &adjacencyList[label-1], weight);
        
        
        token = strtok(NULL, ", ");
    }
    
}

/**
 * Metoda pomocnicza umożliwiająca wczytanie grafu z pliku
 **/
int Graph::readLine(char *buffer, size_t n, FILE *pFile)
{
    //inicjalizacja pozycji w buforze na poczatek bufora
    int pos = 0;
    int curr_size = n;
    //zmienna do odczytywania poszczególnych znaków z pliku
    int ch;
    
    ch = getc(pFile);
    
    //czytamy kazda linijke az napotkamy znak specjalny nowej linii
    while( (ch != '\n') && (ch != EOF) ) {
        if(pos == curr_size)
        {
            std::cout << pos << std::endl;
            //zapełniliśmy rozmiar buffora i nie odczytaliśmy jeszcze całej linii
            curr_size *= 2; //podwojenie bieżącego rozmiaru buffora
            if( (buffer = (char *) realloc(buffer, sizeof(char) * (curr_size + 1))) )
            {
                perror("Błąd podczas realokowania pamięci buffora.");
                exit(1);
            }
        }
        
        buffer[pos++] = ch;
        ch = getc(pFile);
    }
          
    //znak końca stringu
    buffer[pos] = '\0';
    
    //zwracamy liczbę odczytanych znaków
    return pos;
}
          
int Graph::findMinPath(Vertex *a, Vertex *b, std::string *path)
{
    if(dijkstra == NULL)
    {
        //tworzenie obiektu implementującego algorytm Dijkstry
        //dla bieżącego grafu
        dijkstra = new Dijkstra(this);
    }
    
    int distance = dijkstra->minPath(a, b, path);
    
    return distance;
}

Vertex *Graph::getVertex(int label) const
{
    if( label > noOfVertices || label < 1)
    {
        printf("Podano wierzchołek spoza zakresu dostępnych wierzchołków. \n");
        return NULL;
    }
    return &adjacencyList[label-1];
}

/**
 * Metoda pozwala na zaincjalizowanie tablic pomocniczyc algorytmu Dijkstry.
 **/
void Dijkstra::init(void)
{
    for(int i = 0; i < graph->size(); i++) {
        //wypełnienie tablic odległości (kosztów) dla najkrótszych ścieżek
        //do i-tego wierzchołka elementami określającymi INFINITY
        //przy wykonywaniu algorytmu wartości w tablicy będą porównywane i
        //nadpisywane tymi odpowiadającymi krótszym drogą z src do i-tego wierzchołka
        distance[i] = INFINITY; //INFINITY
        
        //analogiczne wypełnienie poprzedników elementami NULL
        predecessor[i] = 0;
    }
    
    source = NULL;
}

/**
 * Metoda wykonująca główny algorytm Dijkstry wyszukujący najkrótsze
 * ścieżki z wierzchołka źródłowego src do pozostałych.
 **/
int Dijkstra::searchMinPathsFrom(const Vertex *src)
{
    if(source != NULL && src->label == source->label)  {
        //jeżeli metoda została wykonana już dla bieżacego
        //wierzchołka źródłowego src to nie ma potrzeby wykonywać
        //jej ponownie
        return 1;
    }
    
    //1. zainicjalizowanie tablic pomocniczych
    //   destination[i] <- INFINITY
    //   predecessor[i] <- NULL
    //   source = NULL
    init();
    //2. przypisanie wierzchołka źródłowego
    source = src;
    
    //3. inicjalizacja odległości z src do src = 0
    distance[source->label - 1]  = 0;

    //4. utworzenie zbioru (kolejki priorytetowej) wszystkich wierzchołków
    PriorityQueue queue(graph->size());
    for(int i=0; i<graph->size(); i++)
    {
        //std::cout << "po usunięciu " << v->label << " mamy "; print();
        queue.push(graph->getVertex(i+1), distance[i]);
    }
    
    while(!queue.empty()) {
        
        if(DEBBUG_2)
            queue.print();
        
        //pobranie wierzchołka z kolejki o najmniejszej odległości
        const Vertex *v = queue.pop();
        int vIdx = v->label -1;
        if(distance[vIdx] ==  INFINITY) {
            //wierzchołek niedostępne
            if(DEBBUG)
                printf("INFINITY znalezione...\n");
            break;
        }
        
        
        //5. Pętla po wszystkich sąsiadach wierzchołka v
        for(Edge *e = v->adjecentVertices; e != NULL; e = e->next)
        {
            
            int wIdx = e->getVertex()->label-1;
            
            if(DEBBUG)
                printf("Relakasacja krawędzi (%d, %d)\n",
                       v->label, e->getVertex()->label);
            
             //6. Relaksacja krawędzi (v, w)
             if(distance[wIdx] == 0 || distance[wIdx] > distance[vIdx] + e->weight)
             {
                 
                 //nadpisanie najkrótszej drogi i poprzednika
                 distance[wIdx] = distance[vIdx] + e->weight;
                 predecessor[wIdx] = v;
                 
                 //aktualizacja priorytetu wierzchołka sąsiada (w)
                 queue.decreasePriority(e->getVertex(), distance[wIdx]);
                
                 if(DEBBUG) {
                     printf("Nowa Waga: %d\n",distance[wIdx]);
                 }
             }
        }
    }
    
    //zwraca 1 w przypadku sukcesu
    return 1;
}

int Dijkstra::minPath(const Vertex *src, const Vertex *dest, std::string *path)
{
    if(! (source != NULL && source->label == src->label) )
    {
        searchMinPathsFrom(src);
    }
    
    if(distance[dest->label-1] == INFINITY)
    {
        *path = "Brak trasy między wskazanymi wierzchołkami.";
        return -1;
    }
    
    //rekonstrukcja ścieżki
    std::stack<int> pathStack;
    pathStack.push(dest->label);
    
    int lastLabel = -1;
    
    for(const Vertex *prev = predecessor[dest->label-1]; prev != NULL;
        prev = predecessor[prev->label-1]) {
        
        if(prev->label == dest->label || prev->label == lastLabel) break;
        pathStack.push(prev->label);
        lastLabel = prev->label;
    }

    while(!pathStack.empty())
    {
        *path += std::to_string(pathStack.top()) + "-";
        
        pathStack.pop();
    }
    
    *path = path->substr(0, path->length()-1);
    
    //zwróć najkrótszą ścieżkę do wierzchołka dest
    return distance[dest->label-1];
}


/*************** PROBLEM KOMIWOJAŻERA ******************************/
int Graph::minHamiltonianCycle(Vertex **cycle, int &weight) {
    
    //przypisanie nieskończonej długości cyklu Hamiltona - brak cyklu oznacza
    weight = INFINITY;
    
    //jeżeli graf jest pusty
    if(noOfVertices < 1)
        return 0;
    
    //wygenerowanie rosnącego ciągu wszystkich wierzchołków grafu 1,2,..., noOfVertices, który będzie permutowany
    int *permutation = new int[noOfVertices];
    
    for(int i=0; i< noOfVertices; i++)
        permutation[i] = i+1;
    
    //minimalny cykl Hamiltona reprezentowany jako permutacja wierzchołków w postaci tablicy int *
    int newWeight = INFINITY;
    
    do {
        
        //wypisanie testowe wygenerowanej permutacji
        if(DEBBUG_3)
        {
            for(int i=0; i< noOfVertices; i++)
                std::cout << permutation[i] << ",";
            
            std::cout << std::endl;
        }
        
        //dla każdej permutacji wierzchołków sprawdzamy czy tworzy ona cyklem Hamiltona w Grafie
        //jeżeli tak to wyliczamy jej wagę i porównujemy za wagą cyklu znalezionego do tej pory (początkowo cykl = NULL, waga = nieskończoność)
        //w tym celu używamy funkcji pomocniczej isHamiltonianCycle(int *permutation, int n, int weight)
        newWeight = isHamiltonianCycle(permutation, noOfVertices, weight);
        //jeżeli nowo znaleziony cykl Hamiltona jest krótszy od dotychczasowego to aktualizujemy
        //jeżeli nie znaleziono cyklu Hamiltona to newWeight = INFINITY,
        //jeżeli wartość newWeight większa od dotychczasowej weight to pomijamy przypadek
        if(newWeight < weight) {
            
            if(DEBBUG_4)
                std::cout << "Nowa waga: " << newWeight << std::endl;
            
            //przekopiowanie wierzchołków do tablicy
            for(int i=0; i<noOfVertices; i++)
            {
                 if(DEBBUG_4)
                    std::cout << adjacencyList[ (permutation[i] - 1) ] << " ";
                
                cycle[i] = &adjacencyList[ (permutation[i] - 1) ];
            }
            
            if(DEBBUG_4)
                std::cout << std::endl;
            
            weight = newWeight;
            
        }
        
        
        //generowanie kolejnych permutacji na podstawie ciągu w talicy permutation przy użyciu funkcji z STL
        //} while(std::next_permutation(permutation, permutation + noOfVertices));
      } while(generateNextPermutation(permutation, permutation + noOfVertices));
    
    //usunięcie pomocniczej tablicy reprezentującej permutcje ciągów wierzchołków grafu
    delete [] permutation;
    
    if(weight == INFINITY)
        return 0;
    
    return noOfVertices;
}

/**
 * Funkcja sprawdza istnienie krawędzi pomiędzy parą wierzchołków v1 i v2
 * jeżeli krawędź istnieje to zwraca wage tej krawędzi 
 * jeżeli krawędź nie istnieje to zwraca INFINITY 
 **/
int Graph::getEdgeWeightBetween(int v1, int v2)
{
    
    //przeglądamy liste sąsiedztwa dla wierzchołka v1
    for(Edge *e = adjacencyList[v1-1].adjecentVertices; e != NULL; e = e->next)
    {
        if(e->getVertex()->label == v2)
        {
            //jeżeli znaleźliśmy krawędź łącząca wierzchołki(v1, v2)
            return e->weight;
        }
        
        //else szukamy dalej
    }
    
    return INFINITY;
    
}

/**
 * Funkcja displayCycle  jest funkcją pomocniczą i wypisuje cykl o długość length
 **/
void Graph::displayCycle(Vertex **cycle, int length)
{
    for(int i=0; i < length; i++)
        std::cout << *cycle[i] << "-";
    
    std::cout << *cycle[0] << std::endl;
}

/** 
 * Funkcja pomocnicza dokonująca sprawdzenia czy dla danej permutacji (int *permutation) istnieje cykl Hamiltona
 * oraz wyznaczająca jego wagę. Funkcja zwraca INFINITY (brak cyklu) bez dalszego sprawdzania jęzeli w trakcie 
 * sprawdzania kolejnych krawędzi sumaryczna waga okaże się być większa od wagi przekazanej jako 3 parametr (weight)
 * ma to zapobiec nadmiarowemu sprawdzaniu permutacji dla których wiadomo już w trakcie, że nawet jeżeli będzie dawała 
 * ona cykl Hamiltona to nie będzie to cykl minimalny w tym grafie
 **/
int Graph::isHamiltonianCycle(int *permutation, int n, int weight)
{
    //zmienna w której sumujemy wagi poszczególnych krawędzi cyklu
    int cycleWeight = 0;
    int edgeWeight = 0;
    
    //sprawdzamy istnienie krawędzi pomiędzy kolejnymi wierzchołkami wynikającymi z permutacji
    //aby permutacja była cyklem muszą istnieć krawędzie pomiędzy każdą kolejną parą uporządkowaną wierzchołków
    //oraz parą uporządkowaną (last, first)
    //dla każdej takiej pary wywołujemy funkcje getEdgeWeightBetween(int v1, int v2), która zwraca wagę krawędzi
    //lub jeżeli taka krawędź nie istnieje zwraca INFINITY -> kończymy wtedy przeglądanie permutacji i zwracamy INFINITY
    //jeżeli suma wag dla dotychczasowych par cycleWeight > weight (3 argument) to również zwracamy INFINITY
    //gdy przeszliśmy całą permutacje to zwracamy sumę wag cycleWeight
    for(int i=0; i<n-1; i++)
    {
        edgeWeight = getEdgeWeightBetween(permutation[i], permutation[i+1]);
        
        if(edgeWeight == INFINITY)
            return INFINITY;
        
        cycleWeight += edgeWeight;
        
        if(cycleWeight >= weight)
            return INFINITY;
    }
    
    edgeWeight = getEdgeWeightBetween(permutation[n-1], permutation[0]);
   
    if(edgeWeight == INFINITY)
        return INFINITY;
    
    cycleWeight += edgeWeight;
    
    if(cycleWeight >= weight)
        return INFINITY;
    
    return cycleWeight;
}

/*************** PROBLEM KOMIWOJAŻERA - END ************************/

int main(int argc, char **argv)
{
    char *filename;
    FILE *file;
    int start, end;
    
    if(argc != 2)
    {
        printf("Wywołanie programu: ./komiwojazer graf.dat\n");
        exit(1);
    }
    
    //wczytanie grafu jako listy sąsiedztwa
    filename = argv[1];
    if( !(file = fopen(filename, "r")))
    {
        perror("Błąd podczas otwierania pliku.");
        exit(1);
    }
    
    //konstrukcja obiektu grafu na stosie
    Graph G(file);
    
    //wypsianie listy sąsiedztwa
    G.printAdjacencyList();
    
    //problem komiwojazera (TSP - travelling salesman problem) - znajdowanie najkrotszego cyklu Hamiltona (jeżeli istnieje)
    //cykl Hamiltona to cykl przechodzący przez każdy z wierzchołków grafu dokładnie raz i wracający do wierzchołka początkowego
    //problem komiwojazera znajduje najktórszy (pod względem sumarycznej wagi) cykl Hamiltona
    Vertex **hamiltonianCycle =  (Vertex **) malloc( sizeof(Vertex *) * G.size());
    int length, weight;
    
    if(DEBBUG_4)
        std::cout << "WYSZUKIWANIE CYKLU HAMILTONA" << std::endl;
    
    //znajdowanie min. cyklu Hamiltona, zwraca długość cyklu lub 0 gdy nie znajdzie żadnego cykl Hamiltona w grafie
    if(!(length = G.minHamiltonianCycle(hamiltonianCycle, weight))) {
        printf("Wskazany graf nie jest hamiltonowski (nie zawiera cyklu Hamiltona)!\n");
    } else {
        if(DEBBUG_4)
        std::cout << "KONIEC WYSZUKIWANIA" << std::endl << std::endl;
        //wypisanie cyklu hamiltona
        
        Graph::displayCycle(hamiltonianCycle, length);
        std::cout << "Waga cyklu Hamiltona: " << weight << std::endl << std::endl;
    }

    return 0;
}