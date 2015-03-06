/***************************************************
 * Kompilacja:                                     *
 * g++ -std=c++11 -o stree min_spanning_tree.cpp   *
 * Uruchomienie:                                   *
 * ./dijkstra graph01.dat                          *
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
#include<unordered_map>

#define DEBBUG 1
#define DEBBUG_2 0
#define BUFF_SIZE 128
#define INFINITE std::numeric_limits<int>::max()

struct Vertex;

/**
 * krawędzie będą elementami listy sąsiedztwa dla 
 * każdego wierzchołka, pozwoli to na przechowywanie 
 * sąsiadującego wierzchołka wraz z wagą krawędzi prowadzącej 
 * do tego wierzchołka 
 **/

// TRZEBA DODAĆ DO KRAWĘDZI WIERZCHOLEK ŹRÓDŁOWY SOURCE !!!!!!!!
struct Edge
{
private:
    Vertex *source;
    Vertex *destination;
public:
    //waga krawędzi
    int weight;
    //wskaźnik na kolejny element listy sąsiedztwa
    Edge *next;
    //konstruktor krawędzi
    Edge(Vertex *start, int w, Vertex *end) : source(start), weight(w), destination(end), next(0) {}
    
    /**
     * zwraca wierzchołek do którego prowadzi bieżąca krawędź
     **/
    Vertex *getVertex()
    {
        return destination;
    }
    
    /**
     * zwraca wierzchołek źródłowy z którego krawędź wychodzi
     **/
    Vertex *getSourceVertex()
    {
        return source;
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
    void addAdjecentVertex(Vertex *start, int weight, Vertex *end)
    {
        
        Edge *e = new Edge(start, weight, end);
        
        //gdy lista sąsiedztwa jest aktualnie pusta
        if(adjecentVertices == NULL)
        {
            adjecentVertices = e;
            return;
        }
        
        Edge *tmp = NULL;
        
        //ustawiamy wskaźnik tmp na ostatni element na liście sąsiedztwa
        for(tmp = adjecentVertices; tmp->next != NULL; tmp = tmp->next);
        
        //wstawienie nowego elementu
        tmp->next = e;
        
        /*//wstawienie w czasie 0(1)
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

class Dijkstra;

class Kruskal;

class Graph
{
    int noOfVertices = 0;
    
    //lista sąsiedztwa, poszczególne pozycje w tablicy wskazują na bezpośrednich
    //sąsiadów wierzchołka poprzez krawędzie, które prowadzą do nich.
    //będziemy alokować tablice o rozmiarze równym ilości wierzchołków
    Vertex **adjacencyList;
    
    //obiekt z algorytmem Dijkstry
    Dijkstra *dijkstra = 0;
    
    //obiekt z algorytmem Kruskala
    Kruskal *kruskal = 0;
    
public:
    
    Graph(FILE *pFile);
    Graph(int size);
    ~Graph();
    
    void printAdjacencyList(void);
    //ta metoda używa algorytmu Dijkstry
    int findMinPath(Vertex *a, Vertex *b, std::string *path);
    Vertex *getVertex(int label) const;
    int size(void) const;
    Graph *findMinSpanningTree();
    
    void addEdge(Edge *e);
    
private:
    int readLine(char *buffer, size_t n, FILE *pFile);
    void addVerticesToAdjacencyListOf(Vertex *v, char *vertexList);
    
    
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
    adjacencyList = (Vertex **) malloc( sizeof(Vertex *) *noOfVertices);
    
    //dodajemy wszystkie n wierzchołków do tablicy sąsiedztwa
    for(int i=0; i < noOfVertices; i++)
    {
        adjacencyList[i] = new Vertex(i+1);
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
        addVerticesToAdjacencyListOf(adjacencyList[i], buffer);
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
        printf(" %d | ", adjacencyList[i]->label);
        
        for(Edge *e = adjacencyList[i]->adjecentVertices; e !=NULL; e= e->next)
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
        v->addAdjecentVertex(v, weight, adjacencyList[label-1]);
        
        
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
    return adjacencyList[label-1];
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
        distance[i] = INFINITE; //INFINITY
        
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
        if(distance[vIdx] ==  INFINITE) {
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
    
    if(distance[dest->label-1] == INFINITE)
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



/******* MIN SPANNING TREE **********/

/**
 * Klasa struktury zbiorów rozłącznych
 * przechowuje dla zadanego uniwersum jego 
 * podział na rozłączne zbiory tj. grupy zbiorów
 * w których żaden element nie może być w więcej niż jednym 
 * zbiorze.Operacje na DisjointSet to: 
 * - Find(T elem) - znajduje zbiór do którego należy element elem
 *                  przydatne do sprawdzania czy 2 elementy należą 
 *                  do jednego zbioru
 * - Uninon(T elem1, T elem2) - operacja 2 zbiorów do których 
 *                  należą elementy elem1 i elem2 
 **/
template<class T>
class DisjointSet {

public:
    //DisjointSet implementujemy jako las zbiorów rozłącznych
    //gdzie każdy zbiór reprezentujemy jako drzewo skierowane, którego
    //korzeniem jest reprezentant zbioru
    template<class E>
    struct Node {
        //element zbioru opakowany w węzeł drzewa
        const E *element;
        //wkaźnik na rodzica
        Node<E> *parent;
        //ranga drzewa
        int rank;
        //konstrukcja węzła drzewa skierowanego zbioru
        Node(const E *elem) : element(elem), parent(NULL), rank(0) {}
        Node(const E *elem, Node<E> *p) : element(elem), parent(p), rank(0) {}
    };
    
    typedef Node<T> Set;
    
private:
    int numberOfSets;
    
    //mapowanie element typu T węzeł drzewa (reprezentującego zbiór)
    std::unordered_map<const T *, Node<T> *> node;
    
public:
    
    
    
    //konstruktor tworzy DisjointSet dla 0 elementowego uniwersum
    DisjointSet() : numberOfSets(0) { }
    //konstruktor tworzy DisjointSet dla size elementowego uniwersum
    DisjointSet(int size) : numberOfSets(0), node(size) { }
    
    //podstawowe operacje na strukturze:
    Set *Find(const T *elem);
    T *FindRepresentative(const T *elem);
    void Union(const T *elem1, const T *elem2);
    void MakeSet(const T *elem);
    
    int NumberOfSets(void);
    
private:
    Node<T> *Find(Node<T> *node);
    void Union(Set *s1, Set *s2);
};

/**
 * Metoda zwraca liczbę zbiorów rozłącznych
 **/
template<class T>
int DisjointSet<T>::NumberOfSets()
{
    return numberOfSets;
}

/**
 * Metoda tworzy nowy zbiór rozłączny z pojedynczym 
 * elementem uniwersum 'elem' będącym jego reprezentantem 
 * i wstawia go do setRepresentative na pozycji setsNumber
 * a następnie inkrementuje setsNumber++
 * Metoda zakłada, że użytkownik nie próbuje dodać elementu, który 
 * należy już do istniejącego zbioru rozłącznego!
 **/
template<class T>
void DisjointSet<T>::MakeSet(const T *elem)
{
    //1. opakowanie elementu w obiekt Node drzewa
    Node<T> *representative = new Node<T>(elem);
    //2. przypisanie do tablicy ("lasu") nowego zbioru ("drzewa")
    node[elem] = representative;
    //3. zwiększenie liczby zbiorów rozłącznych
    numberOfSets++;
}

/**
 * Metoda odnajduje zbiór do którego należy element 'elem'
 **/
template<class T>
typename DisjointSet<T>::Set *DisjointSet<T>::Find(const T *elem)
{
    //1. pobranie z mapy węzła odpowiadającego temu elementowi
    Node<T> *n = node[elem];
    
    //2. jeżeli bieżacy węzeł jest korzeniem drzewa reprezentującego zbiór
    //   to zwracamy go jako reprezentanta zbioru
    if(n->parent == NULL)
        return n;
    
    //3. wywołanie rekurencyjne przechodzące drzewo do korzenia
    return Find(n);
}

/**
 * Pomocnicza metoda Find do rekurencyjnego przechodzenia drzewa 
 * reprezentującego zbiór rozłączny celem znalezienia reprezentanta zbioru
 * ZASTOSOWANO PathCompression -> spłaszcza strukturę drzewa podczas każdego wyszukiwania
 * stąd kolejne wyszukiwania będą przebiegały szybciej. Polega to na podpinaniu wszystkich
 * napotkanych węzłów bezpośrednio do korzenia.
 **/
template<class T>
DisjointSet<T>::Node<T> *DisjointSet<T>::Find(Node<T> *n)
{
    //1. jeżeli znaleźlśmy reprezentanta zbioru to go zwracamy
    if(n->parent == NULL)
        return n;
    //2. rekurencyjnie przechodzimy do korzenia drzewa ("zbioru")
    //a podpinamy napotkane węzły bezpośrednio do korzenia
    n->parent = Find(n->parent);
    
    //zwracamy wskaźnik na korzeń drzewa
    return n->parent;
}

/**
 * Funkcja wykonująca operację Union dwóch zbiorów set1, set2
 **/
template<class T>
void DisjointSet<T>::Union(const T *elem1, const T *elem2)
{
    Set *s1 = Find(elem1);
    Set *s2 = Find(elem2);
    
    //wywołanie metody pomocniczej operującej na zbiorach
    Union(s1, s2);
}

/**
 * Użycie rangi dla węzłów drzew reprezentujących zbiory
 * Pozwala to na dodwanie zawsze mniejszego drzewa do większego. 
 * Umożlwia uniknięcie powstania drzewa niezbalansowanego -> listy jednokierunkowej
 **/
template<class T>
void DisjointSet<T>::Union(typename DisjointSet<T>::Set *s1,
                           typename DisjointSet<T>::Set *s2)
{
    //suma zbiorów rozłącznych
    if(s1->rank > s2->rank) {
        s2->parent = s1;
        //zmniejszenie liczby zbiorów rozłącznych
        numberOfSets--;
    } else if(s1->rank < s2->rank) {
        s1->parent = s2;
        numberOfSets--;
    } else if(s1 != s2) {
        s2->parent = s1;
        s1->rank++;
        numberOfSets--;
    }
    
}


class Kruskal {
    
    //obiekt z dotychczasowym grafem dla którego szukamy
    //minimalnego drzewa rozpinającego
    const Graph *graph;
    //struktura zbiorów rozłącznych pomocnicza przy algorytmie Kruskala
    DisjointSet<Vertex> *partition;
    
    //obiekt grafu na którym tworzymy minimalne drzewo rozpinające
    Graph *minSpanningTree;
    
    //zbiór z wierzchołkami sortowanymi od min do max wagi
    //zbiór może zawierać elementy o tych samych wagach
    //stąd używamy std::multiset
    
    //1. komparator elementów w zbiorze
    struct minWeightComp {
        bool operator() (const Edge *e1, const Edge *e2) const
        {
            return e1->weight < e2->weight;
        }
    };

    //2. zbiór
    std::multiset<Edge *, minWeightComp> edgeSet;

public:
    Kruskal(const Graph *g);
    void findMinSpanningTree(void);
    Graph *getMinSpanningTree(void);
    
private:
    void init(void);
    bool isSpanningTree(void) const;
    bool willHaveCycle(Edge *e) const;
    void mergeTrees(Edge *e);
};

/**
 * Metoda zwracająca graf z rozwiązaniem 
 * algorytmu Kruskala tj. minimalnym drzewem rozpinającym
 **/
Graph *Kruskal::getMinSpanningTree()
{
    return minSpanningTree;
}

//konstruktor
Kruskal::Kruskal(const Graph *g) : graph(g)
{
    init();
}

/**
 * Inicjalizacja algorytmu kruskala utworzenie lasu wierzchołków
 * (będzie to graf z zaalokowanymi pustymi listami sąsiedztwa)
 **/
void Kruskal::init(void)
{
    //tworzymy pusty graf z size wierzchołami numerowanymi od [1,size]
    minSpanningTree = new Graph(graph->size());
    
    //tworzymy zbiór z wszystkimi krawędziami orginalnego grafu
    //zbiór ten jest sortowany od najmniejszej wagi do największej
    for(int i=0; i<graph->size(); i++)
    {
        //przechodzimy wszystkie wierzchołki grafu
        //a następnie wszystkie krawędzie i wstawiamy
        //je do zbioru krawędzi
        for(Edge *e = graph->getVertex(i+1)->adjecentVertices; e != NULL; e = e->next)
        {
            edgeSet.insert(e);
        }
    }
    
    //Tworzymy strukture zbiorów rozłącznych której uniwersum będą
    //wszystkie wierzchołki grafu oraz inicjalizujemy ją w ten sposób
    //by każdy wierzchołek stanowił niezależny zbiór rozłączny.
    //Otrzymujemy las drzew (grafów) jednowierzchołkowych
    
    //1. alokacja struktury zbioru rozłącznego o odpowiednim rozmiarze
    partition = new DisjointSet<Vertex>();
    for(int i=0; i<graph->size(); i++)
    {
        //2. dodanie zbioru rozłącznego dla i-tego wierzchołka do struktury
        partition->MakeSet(graph->getVertex(i+1));
    }
    
}

/**
 * Główna funkcja wykonująca algorytm kruskala 
 * mając zadany las 'min_spanning_tree' 
 * oraz zbiór z krawędziami uporządkowanymi 'edgeSet'
 **/
void Kruskal::findMinSpanningTree(void)
{
    //algorytm wykonujemy do momentu gdy zbiór krawędzi zawiera
    //jeszcze elementy oraz nowo tworzony graf nie jest jeszcze
    //drzewem rozpinającym
    while(!edgeSet.empty() || !isSpanningTree())
    {
        //pobranie ze zbioru kolejnego wierzchołka o min wadze
        Edge *e = *(edgeSet.begin());
        
        //sprawdzenie czy dodanie do grafu krawędzi e
        //spowoduje połączenie dwóch różnych drzew rozpinających
        //używamy metody wyszukiwania cyklu tj. pytanie czy powstanie
        //cykl
        if(!willHaveCycle(e))
        {
            //zsumowanie zbiorów wierzchołków podgrafów
            //łączonych krawędzią e
            mergeTrees(e);
            //dodanie krawędzi do konstruowanego drzewa rozpinającego
            minSpanningTree->addEdge(e);
            //printf("%d --- %d\n", e->getSourceVertex()->label,
              //     e->getVertex()->label);
        }
        
        
        //usunięcie wierzchołka ze zbioru
        edgeSet.erase(edgeSet.begin());
    }
}

/**
 * Metoda która korzystając z algorytmu Kruskala 
 * (obiekt Kruskal) znajduje minimalne drzewo rozpinające 
 * i zwraca go jako nowy obiekt Graph
 **/
Graph *Graph::findMinSpanningTree()
{
    //utworzenie nowego grafu dla drzew rozpinajacego
    //zmienna zostanie przypisana grafem zwróconym
    //z obiektu Kruskal
    Graph *minSpanningTree = 0;
    
    //wykonanie algorytmu Kruskala
    if(kruskal == NULL) {
        kruskal = new Kruskal(this);
    }
    
   
    kruskal->findMinSpanningTree();
    minSpanningTree = kruskal->getMinSpanningTree();
    
    //zwracamy skonstruowane minimalne drzewo rozpinajace
    return minSpanningTree;
}

/**
 * Tworzy pusty graf o size wierzchołkach numerowanych od 1 do size
 **/
Graph::Graph(int size) : noOfVertices(size)
{
    //alokujemy tablicę wierzchołków z listami sąsiedztwa
    //adjacencyList = (Vertex **) malloc( sizeof(Vertex*) *noOfVertices);
    
    adjacencyList = new Vertex*[noOfVertices];
    
    //alokacja listy sąsiedztwa jako las wierzchołków
    for(int i=0; i<noOfVertices; i++)
    {
        adjacencyList[i] = new Vertex(i+1);
    }
    
    if(DEBBUG)
    {
        printf("Wypisanie lasu wierzchołków: \n");
        
        for(int i=0; i<noOfVertices; i++)
            printf("%d, ", adjacencyList[i]->label);
        printf("\n");
    }
    
}

//dodanie krawędzi
void Graph::addEdge(Edge *edge) {

    //pobieramy wierzchołki pomiędzy którymi mamy wstawić krawędź
    Vertex *srcVertex = adjacencyList[edge->getSourceVertex()->label - 1];
    Vertex *destVertex = adjacencyList[edge->getVertex()->label - 1];
    
    //dodanie nowych krawędzi do grafu reprezentującego min. drzewo rozpinające
    if(DEBBUG) {
        printf("Dodanie krawędzi: \n");
        printf("%d --- %d\n", srcVertex->label,
           destVertex->label);
    }
    srcVertex->addAdjecentVertex(srcVertex, edge->weight, destVertex);
    if(DEBBUG) {
        printf("Dodanie krawędzi: \n");
        printf("%d --- %d\n", destVertex->label,
               srcVertex->label);
    }
    destVertex->addAdjecentVertex(destVertex, edge->weight, srcVertex);
}

/**
 * Sprawdzanie czy po dodaniu krawędzi powstanie w
 * grafie cykl i graf przestanie być drzewem rozpinającym
 **/
bool Kruskal::willHaveCycle(Edge *edge) const
{

    //pobranie zbiorów do których przynależą wierzchołki
    //incydentne poprzez krawędź e
    DisjointSet<Vertex>::Set *s1 = partition->Find(edge->getVertex());
    DisjointSet<Vertex>::Set *s2 = partition->Find(edge->getSourceVertex());
    
    if(s1 == s2)
        return true;
    
    return false;
}

/**
 * Metoda sprawdzająca czy w wyniku wykonywania 
 * algorytmu Kruskala otrzymaliśmy drzewo rozpinające
 **/
bool Kruskal::isSpanningTree(void) const
{
    //Jeżeli liczba podziału uniwersum wierzchołków grafu na
    //zbiory rozłączne wynosi 1 tj. wszystkie wierzchołi należą
    //do jednego zbioru to oznacza, że tworzą one drzewo rozpinające
    if(partition->NumberOfSets() == 1)
    {
        return true;
    }
    
    //w przeciwnym wypadku
    return false;
}

/**
 * Metoda wykonuje operację unii na zbiorach wierzchołków 
 * które reprezentują podgrafy łączone krawędzią "e" (argument) 
 * na obiekcie struktury zbiorów rozłącznych partition
 **/
void Kruskal::mergeTrees(Edge *edge)
{
    partition->Union(edge->getSourceVertex(), edge->getVertex());
}


/************************************/

int main(int argc, char **argv)
{
    char *filename;
    FILE *file;
    int start, end;
    
    if(argc != 2)
    {
        printf("Wywołanie programu: ./kruskal graf.dat\n");
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
    Graph * G = new Graph(file);
    
    //wypsianie listy sąsiedztwa
    G->printAdjacencyList();
    
    printf("### WYSZUKIWANIE MIN. DRZEWA ROZPINAJĄCEGO ###\n\n");
    
    //Znajdowanie minimalnego drzewa rozpinającego
    Graph *minSpanningTree = G->findMinSpanningTree();
    
    printf("\n### MINIMALNE DRZEWO ROZPINAJĄCE ###\n");
    //wypisanie listy sąsiedztwa minimalnego drzewa rozpinającego
    minSpanningTree->printAdjacencyList();
    
    return 0;
}