/***************************************************
 * Kompilacja:                                     *
 * g++ -std=c++11 -o dijkstra dijkstra.cpp         *
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

#define DEBBUG 1
#define DEBBUG_2 0
#define BUFF_SIZE 128
#define INFINITY std::numeric_limits<int>::max()

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
            std::cout << "odnaleziono " << (*it)->label << std::endl;
    
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

int main(int argc, char **argv)
{
    char *filename;
    FILE *file;
    int start, end;
    
    if(argc != 2)
    {
        printf("Wywołanie programu: ./dijkstra graf.dat\n");
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
    
    while(1) {
        //pobranie wierzchołka początkowego i końcowego
        printf("Podaj wierzchołek początkowy: \n");
        scanf("%d", &start);
        printf("Podaj wierzchołek końcowy: \n");
        scanf("%d", &end);
    
        //sprawdzenie poprawności wprowadzonych wierzchołków
        if(start < 1 || start > G.size())
        {
            printf("Podano niepoprawny wierzchołek początkowy: %d\n", start);
            continue;
        }
    
        if(end < 1 || end > G.size())
        {
            printf("Podano niepoprawny wierzchołek końcowy: %d\n", end);
            continue;
        }
    
        //wyszukiwanie najkrótszej ścieżki - algorytm Dijkstry
        printf("Wyszukiwanie najkrótszej ścieżki pomiędzy wierzchołkami: %d - %d ...\n", start, end);
   
        std::string path;
        int minCost = G.findMinPath(G.getVertex(start), G.getVertex(end), &path);
        printf("Minimalny koszt: %d\n", minCost);
        printf("Trasa: %s\n", path.c_str());
    }
    
    return 0;
}