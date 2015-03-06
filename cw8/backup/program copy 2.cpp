/**
 * Dane wejściowe do programu: 
 * <ilosc lini>
 * <linia_1> 
 * <nazwa_przystanku>, <czas_przejazdu>, <nazwa_przystanku>, <czas_przejazdu>, ... <nazwa_przystanku>
 * ....
 * <linia_n>
 * <nazwa_przystanku>, <czas_przejazdu>, <nazwa_przystanku>, <czas_przejazdu>, ... <nazwa_przystanku>
 **/

#include<cstdio> 
#include<cstring>
#include<cstdlib>
#include<string> 
#include<unordered_map>
#include<set>
#include<algorithm>
#include<iostream>
#include<stack>


#define DEBBUG 1 
#define DEBBUG_2 0
//domyślny rozmiar buforu
#define BUFF_SIZE 128

#define INFINITE std::numeric_limits<int>::max()

struct Vertex;

/**
 * krawędzie będą elementami list sąsiedztwa przyporządkowanym
 * poszczególnym wierzchołkom. pozwala to na przechowywanie 
 * sąsiadującego wierzchołka wraz z wagą krawędzi prowadzącej do tego wierzchołka
 **/
struct Edge
{
private:
    Vertex *startStop;
    Vertex *endStop;
public:
    //numer linii komunikacji miejskiej
    int lineNo;
    //czas przejazdu
    int time;
    //wskaźnik na kolejny element listy sąsiedztwa
    Edge *next = 0;
    //konstruktor krawędzi
    Edge(int lno, Vertex *start, int t, Vertex *end)
            : lineNo(lno), startStop(start), time(t), endStop(end) { }
    /**
     * zwraca wierzchołek do którego prowadzi bieżąca krawędź
     **/
    Vertex *getEndVertex()
    {
        return endStop;
    }
    
    Vertex *getStartVertex()
    {
        return startStop;
    }
};

struct Vertex
{
    //indeks wierzchołka (liczbowy)
    int index;
    //etykieta wierzchołka (nazwa przystanku)
    std::string label;
    //lista sąsiedztwa wierzchołka
    Edge *adjacentVertices = 0;
   
    //konstruktor wierzchołka
    Vertex(int idx, std::string lbl) : index(idx), label(lbl) { }
    
    /**
     * dodawanie wierzchołka sąsiadującego (w postaci krawędzi łączącej wierzchołki)
     * do listy sąsiedztwa bieżącego wierzchołka. 
     * nowy wierzchołek przyległy wstawiamy zawsze na początku listy -> czas O(n)
     * można też wstawiać na początek listy nowe wierzchołki (krawędzie) to O(1)
     **/
    void addAdjacentVertex(Edge *edgeToVertex) {
     
        //gdy lista sąsiedztwa jest aktualnie pusta
        if(adjacentVertices == NULL)
        {
            adjacentVertices = edgeToVertex;
            return;
        }
        
        Edge *tmp;
        //ustawiamy wskaźnik tmp na ostatni element na liście sąsiedztwa
        for(tmp = adjacentVertices; tmp->next != NULL; tmp = tmp->next);
        
        //wstawienie nowego elementu (krawędzi prowadzącej do sąsiedniego wierzchołka)
        tmp->next = edgeToVertex;
        
    }
    
    void addAdjacentVertex(int lineNo, Vertex *start, int time, Vertex *end)
    {
        Edge *e = new Edge(lineNo, start, time, end);
        
        addAdjacentVertex(e);
    }
    
    /**
     * metoda która przeszukuje listę sąsiedztwa wierzchołka A w celu 
     * odnalezienia bezpośredniej krawędzi do wierzchołka B
     **/
    Edge *directEdge(Vertex *B)
    {
        //1. przechodzimy krawędzi poszukując wierzchołka B
        for(Edge *e = adjacentVertices; e!= NULL; e = e->next)
        {
            //odnaleźliśmy bezpośrednią krawędź z A do B
            if(e->getEndVertex()->index == B->index)
                return e;
        }
        
        //2. zwracamy NULL tj. przeszliśmy całą listę sąsiedztwa i nie
        //   znaleźliśmy B
        return NULL;
    }
    
    //przeciążenie operatora <
    bool operator< (Vertex const &q) const { return index < q.index; }
};

class Dijkstra;

class Graph
{
protected:
    //liczba wierzchołków grafu (przystanków)
    int noOfVertices = 0;
    
    //lista sąsiedztwa grafu - tablica unikalnych wierzchołków
    //z których każdy posiada listę krawędzi do bezpośrednio sąsiadujących wierzchołków
    
    //tablicę reprezentująca Listę Sąsiedztwa Grafu
    //początkowo alokowana jako pusta tablica
    //będzie realokowana metodą resize() podczas próby dodanie nowego
    //wierzchołka
    Vertex **adjacencyList = 0;
    
    //obiekt implementujący algorytm Dijkstry
    //przydatny przy znajdowaniu najkrótszej ścieżki pomiędzy
    //dwoma wskazanymi wierzchołkami (przystankami) -> rozwiazanie podpunktu 1) zadania
    Dijkstra *dijkstra = 0;
    
public:
    
    void printAdjacencyList(void);
    //metoda używająca algorytmu Dijkstry znajduja najkrótsze połączenie
    int findMinPath(Vertex *a, Vertex *b, std::string *path);
    //obieranie wierzchołka grafu na podstawie numeru
    Vertex *getVertex(int i) const;
    int size(void) const;
    
protected:
    //metody pomocnicze w konstrukcji grafu na podstawie pliku tekstowego
    int readLine(char * &buffer, size_t n, FILE *pFile);
    virtual bool loadGraphFromFile(FILE *pFile) = 0;
};

int Graph::size(void) const
{
    return noOfVertices; 
}

/**
 * Implementacja kolejki priorytetowej na bazie std::set<> z C++ STL
 * wykorzystywana w algorytmie Dijkstry
 **/
class PriorityQueue
{
    //tablica priorytetów - mapowanie:
    //Vertex->index  ---> index tablicy ----> priorytet wierzchołka
    int size;
    static int *priorities;
    
    //funktor - komparator elementów w zbiorze (std:set<>)
    //ustawia elementy zgodnie z określonym priorytetem w kolejce priorytetowej
    
    //komparator porządkujący elementy od tego o najmniejszym priorytecie do
    //tego z największym priorytetem
    struct minPriorityComp {
        bool operator() (const Vertex *v1, const Vertex *v2) const
        {
            return priorities[v1->index-1] < priorities[v2->index-1];
        }
    };
    
    //funktor zwracający true gdy napotka w kolejce priorytetowej
    //wierzchołek grafu o danym indeksie
    struct vertexIndexEqual {
        vertexIndexEqual(int idx) : index(idx) {}
        bool operator() (const Vertex *vert) {
            return vert->index == index;
        }
        private:
        int index;
    };
    
    //analogiczny funktor zwracający true gdy napotka w kolejce priorytetowej
    //wierzchołek grafu o danej etykiecie (label)
    struct vertexLabelEqual {
        vertexLabelEqual(std::string l) : label(l) {}
        bool operator() (const Vertex *vert) {
            return vert->label == label;
        }
        private:
        std::string label;
    };
    
    //STRUKTURA DANYCH W KTÓREJ IMPLEMENTUJEMY KOLEJKE PRIORYTETOWĄ
    //wstawianie do kolejki -> O(logn)
    //zdejmowanie pierwszego elementu kolejki - O(1)
    //aktualizacja priorytetu elementu w kolejce - O(n) //uzycie find_if()
    std::multiset<const Vertex *, minPriorityComp> queue;
    
public:
    //konstruktor kolejki priorytetowej
    PriorityQueue(int s) : size(s) {
        priorities = (int *) malloc(sizeof(int) *size);
    }
    ~PriorityQueue() {}
    
    //metoda wstawiająca wierzchołek do kolejki
    void push(const Vertex *v, int priority);
    //metoda zdejmująca pierwszy element z kolejki
    const Vertex *pop(void);
    //metoda zmieniająca priorytet elementu (wierzchołka) w kolejce
    void changePriority(const Vertex *v, int priority);
    //metoda zmniejszajaca prioryte elementu (wierzchołka) w kolejce
    void decreasePriority(const Vertex *v, int decrement = 1);
    //metoda sprawdzająca czy kolejka jest pusta
    bool empty(void);
    //wypisanie zawartości kolejki priorytetowej
    void print(void);
};

//inicjalizacja statycznej zmiennej składowej - tablica priorytetów
int *PriorityQueue::priorities = 0;

/**
 * Metoda wstawia element do kolejki. Złożoność O(log(n))
 **/
void PriorityQueue::push(const Vertex *v, int priority)
{
    //zapisanie priorytetu w tablicy & dodanie wierzchołka do kolejki
    priorities[v->index -1] = priority;
    queue.insert(v);
}

/**
 * Metoda pobiera pierwszy element z kolejki o najmniejszym priorytecie
 * Złożoność 0(1)
 **/
const Vertex *PriorityQueue::pop(void)
{
    auto iterator = queue.begin();
    const Vertex *vertex = *iterator;
    queue.erase(iterator);
    
    return vertex;
}

/**
 * Metoda aktualizująca priorytet elementu w kolejce. Złożoność O(n)
 **/
void PriorityQueue::changePriority(const Vertex *v, int priority)
{
    //usunięcie elementu z kolejki jezeli istnieje
    //auto it = queue.find(v); //O(log(n))
    auto it = std::find_if(queue.begin(), queue.end(), vertexIndexEqual(v->index));  //O(n)
    if(it != queue.cend()) {
        if(DEBBUG_2)
            std::cout << "odnaleziono " << (*it)->label << std::endl;
        
        //usunięcie starego elementu
        queue.erase(it); //O(1)
        if(DEBBUG_2) {
            std::cout << "po usunięciu " << v->label << " mamy "; print();
        }
    }
    
    //wstawienie elementu z nowym priorytetem
    push(v, priority);
    
    if(DEBBUG_2) {
        std::cout << "po dodaniu " << v->label << " mamy "; print();
    }
}

/** 
 * Metoda dekrementująca priorytet kolejki o 1  lub podana wartosc
 * jako drugi argument... podajac wartość ujemna staje sie metoda inkrementujaca
 **/
void PriorityQueue::decreasePriority(const Vertex *v, int decrement)
{
    //pobieramy aktualny priorytet wskazanego wierzchołka v
    int currentPriority = priorities[v->index-1];
    //zmieniamy priorytet tego elementu w kolejce aktualicjąc o decrement
    changePriority(v, currentPriority - decrement);
}

/**
 * Metoda określajaca czy kolejka priorytetowa jest pusta
 **/
bool PriorityQueue::empty()
{
    return queue.size() == 0;
}

/**
 * klasa implementująca algorytm Dijkstry 
 * wyszukuje najkrótsze ścieżki między dwoma wierzchołkami
 **/
class Dijkstra
{
    //graf na którym algorytm Dijkstry wyszukuje minimalnych ścieżek
    const Graph *graph;
    
    //tablica z czasami przejazdów od przystanku początkowego (wierzchołek źródłowy)
    //do wszystkich innych i-tych wierzchołków grafu idąc najkrótszą ścieżką
    //odległości obliczane są jako sumy czasów przejazdu odcinków ( krawędzi )
    //pomiędzy wierzchołkami znajdującymi się na najkrótszej ścieżce z SOURCE do
    //i-tego wierzchołka
    //alokowane w zależności od wielkości grafu i inicjalizowane czasem niskończonym
    //oznaczającym brak połączenia pomiędzy przystankami
    int *travelTime;
    
    //tablica bezpośrednich przedków i-tych przystanków końcowych grafu
    //na jej podstawie można odtworzyć ciąg wierzchołków dla najkrótszej
    //ścieżki z wierzchołka startowego do i-tego wierzchołka docelowego
    const Vertex **predecessor;
    
    //tablica bezpośrednich krawędzi poprzedzających przystanek końcowy
    //najkrótszych ścieżek pomiędzy przystankiem poczatkowym a
    //i-tym przystankiem
    const Edge **predecessorEdge;
    
    //bieżący wierzchołek źródłowy (przystanek początkowy) - stosowany
    //do wyszukiwania ścieżek, jest ustawiany przez funkcje minPathsFrom(Vertex *src)
    const Vertex *source = 0;
    
public:
    //konstruktor
    Dijkstra(const Graph *g) : graph(g) {
        //alokowanie tablic pomocniczych  dla algorytmu Dijkstry
        travelTime = (int *) malloc(sizeof(int) * (graph->size()));
        predecessor = (const Vertex **) malloc(sizeof(const Vertex *) * (graph->size()) );
        predecessorEdge = (const Edge **) malloc(sizeof(const Edge *) * (graph->size()) );
    }
    //destruktor
    ~Dijkstra();
    
    //pobranie minimalnej ścieżki na podstawie ostatnich obliczeń między (src, dest)
    int minPath(const Vertex *src, const Vertex *dest, std::string *path);
private:
    //metoda wyszukuje najkrótszą ścieżkę na podstawie obliczeń pomiędzy (src, dest)
    int findMinPathsFrom(const Vertex *src);
    //metoda inicjalizujaca tablice wykorzystywane przez algorytm Dijkstry
    void init(void);
};

/**
 * Metoda wypisuje aktualny graf reprezentowany w postaci listy sąsiedztwa
 **/
void Graph::printAdjacencyList(void)
{
    printf("\n Wypisanie listy sąsiedztwa: \n");
    
    for(int i=0; i<noOfVertices; i++)
    {
        printf(" %d:%s | ", adjacencyList[i]->index,
                           adjacencyList[i]->label.c_str());
        
        for(Edge *e = adjacencyList[i]->adjacentVertices; e != NULL; e = e->next)
        {
            printf("%d:%s (%d), ", e->getEndVertex()->index,
                                e->getEndVertex()->label.c_str(),
                                e->time);
            
        }
        
        printf("\n");
    }
    
    printf("\n");
}

/**
 * Metoda pomocnicza umożliwiająca wczytywanie grafu z pliku
 **/
int Graph::readLine(char * &buffer, size_t n, FILE *pFile)
{
    
    //inicjalizacja pozycji w buforze na początek bufora
    int pos = 0;
    int curr_size = n;
    //zmienna do której odczytujemy poszczególne znaki z pliku
    int ch;
    
    ch = getc(pFile);
    
    //czytamy każdą linijkę aż napotkamy znak specjalny nowej linii '\n'
    while( (ch != '\n') && (ch != EOF) ) {
        if(pos == curr_size)
        {
            if(DEBBUG)
                printf("Zapełniono buffor podczas wczytywania linii tekstu, pozycja: %d. Realokacja...\n", pos);
            curr_size *= 2; //podwojenie bieżącego rozmiaru buffora

            char *tmp = (char *) realloc(buffer, sizeof(char) * (curr_size + 1));
            
            if( tmp == NULL )
            {
                perror("Błąd podczas realokowania pamięci buffora.");
                exit(1);
            } else {
                buffer = tmp;
            }
        }
        
        buffer[pos++] = ch;
        ch = getc(pFile);
    }
    
    //dodanie znaku końca stringu
    buffer[pos] = '\0';
    
    //zwracamy liczbę odczytanych znaków -> pusty ciąg to 0
    return pos;
}

Vertex *Graph::getVertex(int index) const
{
    if( index > noOfVertices || index < 1)
    {
        printf("Podano wierzchołek spoza zakresu dostępnych wierzchołków. \n");
        return NULL;
    }
    return adjacencyList[index-1];
}

/**
 * Klasa reprezentujaca graf sieci komunikacji miejskiej 
 * z wierzchołkami reprezentowanymi jako przystanki.
 **/
class StopsGraph : public Graph
{
    std::unordered_map<std::string, int> labelToIndexMap;

public:
    StopsGraph(FILE *pFile);
    ~StopsGraph();
private:
    //implementacja wczytująca graf dostarczony jako siatkę linii autobusowych
    bool loadGraphFromFile(FILE *pFile);
    //metoda przetwarzająca wczytaną listę przystanków i czasów przejazdów
    bool processLineDetails(int lineNo, char *stopsList);
    //metoda pomocnicza dodająca połączenie między przystankami do grafu
    bool addConnection(int lineNo, std::string startStop, int time, std::string endStop);
    //metoda  realokująca tablicę listy sąsiedztwa
    void resizeGraph();
};

/**
 * konstruktor klasy dziedziczącej po Graph specyficznej dla sieci linii autobusowych
 **/
StopsGraph::StopsGraph(FILE *pFile)
{
    if(DEBBUG)
        printf("Konstruktor StopsGraph\n");
    
    //wywołanie metody wczytującej graf z pliku specyficznej dla sieci linii autobusowej
    loadGraphFromFile(pFile);
}

/** 
 * Destruktor klasy StopsGraph
 **/
StopsGraph::~StopsGraph()
{
    if(DEBBUG)
        printf("Destrukcja obiektu klasy StopsGraph\n");
}

/**
 * Metoda wczytuje siatke lini autobusowych dostarczona w nastepujacej postaci:
 * <ilosc lini>
 * <linia_1>
 * <nazwa_przystanku>, <czas_przejazdu>, <nazwa_przystanku>, <czas_przejazdu>, ... <nazwa_przystanku>
 * ....
 * <linia_n>
 * <nazwa_przystanku>, <czas_przejazdu>, <nazwa_przystanku>, <czas_przejazdu>, ... <nazwa_przystanku>
 **/
bool StopsGraph::loadGraphFromFile(FILE *pFile)
{
    //buffor do którego odczytujemy poszczególne linijki tekstu
    char *buffer;
    
    //alokowanie buffora, gdyby był za mały na wczytanie linijki tekstu to realokowany
    if( !(buffer = (char *) malloc(sizeof(char) * (BUFF_SIZE +1))) )
    {
        perror("Błąd podczas alokowania buffora pamięci.");
        return false;
    }
    
    if(DEBBUG)
        printf("Wczytywanie siatki linii autobusowych z pliku...\n");
    
    
    //1. Wczytywanie liczby linii komunikacji miejskiej
    int noOfLines = 0;
    
    if(readLine(buffer, BUFF_SIZE, pFile) < 1)
    {
        printf("Błąd podczas odczytywania liczby linii komunikacji miejskiej.\n");
        return false;
    }
    
    noOfLines = atoi(buffer);
    
    if(DEBBUG)
        printf("Liczba linii komunikacji miejskiej: %d\n", noOfLines);
    
    //wczytywanie danych dotyczących poszczególnych linii komunikacyjnych
    for(int i = 0; i<noOfLines; i++) {
        
        //wczytujemy numer linii
        if(readLine(buffer, BUFF_SIZE, pFile) < 1) {
            printf("Błąd podczas wczytywania %d-tego numeru linii...\n", i);
            return false;
        }
        
        int lineNo = atoi(buffer);
        
        //wczytanie listy przystanków oraz czasów przejazdów pomiędzy liniami
        if(readLine(buffer, BUFF_SIZE, pFile) < 1) {
            printf("Błąd podczas wczytywania listy przystanków linii: %d\n", lineNo);
            return false;
        }
        
        if(DEBBUG)
            printf("Lista przystanków: %s\n", buffer);
        
        //przetwarzanie danych dotyczących poszczególnych linii komunikacji miejskiej
        if(!processLineDetails(lineNo, buffer))
        {
            printf("Błąd podczas przetwarzania listy przystanków linii: %d\n", lineNo);
            return false;
        }
    }
    
    
    return true;
}

/**
 * Metoda pomocnicza która przetwarza listę przystanków i czasów przejazdów
 * @stopsList wczytaną dla linii komunikacji miejskiej o numerze @lineNo 
 * Zwraca true lub false w zależności od powodzenia procesu wczytywania
 **/
bool StopsGraph::processLineDetails(int lineNo, char *stopsList)
{
    //pomocnicze zmienne do których odczytywane są kolejne przystanki i czasy przejazdu
    std::string startStop;
    int time;
    std::string endStop;
    
    //zminna pomocnicza przy przetwarzaniu listy przystanków
    char *token;
    
    //1. wczytanie początkowego przystanku
    token = std::strtok(stopsList, ", ");
    
    //2. wczytywanie kolejnych połączeń (czas, przystanek końcowy) pomiędzy przystankami
    while(token != NULL ) {
        
        //początkowy przystanek <- dotychczasowy końcowy
        startStop = token;
        
        //czas przejazdu
        token = std::strtok(NULL, ", ");
        if(token == NULL) //nie ma wiecej przystanków kończymy pętle
            break;
        
        time = atoi(token);
        
        //końcowy przystanek
        token = std::strtok(NULL, ", ");
        
        if(token == NULL) {
            printf("Wykryto swobodną krawędź (nie ma wierzchołka końcowego)!");
            return false;
        }
        endStop = token;
        
        //dodanie połączenia pomiędzy przystankami do grafu
        if(!addConnection(lineNo, startStop, time, endStop)) {
            printf("Dodawanie krawędzi połączenia między przystankami zakończone niepowodzeniem!");
            return false;
        }
    }
    
    return true;
}

/**
 * Metoda pomocnicza która dodaje nowe połączenie do grafu 
 * Krawedź pomiędzy przystankiem startStop, endStop o czasie przejazdu time lini o numerze lineNo
 **/
bool StopsGraph::addConnection(int lineNo, std::string startStop, int time, std::string endStop)
{
    printf("Dodawanie połączenia: %d (%s)-%d-(%s)\n", lineNo, startStop.c_str(), time, endStop.c_str());
    
    //1. sprawdzenie czy przystanki startStop i endStop istnieją już w grafie:
    //   - przypisanie nazwy przystanku do numeru wierzchołka
    //   - dodanie nowego wierzchołka do grafu (do tablicy z lista sasiedztwa)
    if(labelToIndexMap.find(startStop) == labelToIndexMap.end()) {
        
        //pobranie numeru indeksu dla nowego wierzchołka grafu
        int startStopIdx = ++noOfVertices;
        
        //brak mapowania nazwy przystanku na numer wierzchołka -> dodanie mapowania
        labelToIndexMap[startStop] = startStopIdx;
        
        //realokowanie rozmiaru tablicy przechowującej Listę Sąsiedztwa Grafu
        resizeGraph();
        
        //dodanie nowego wierzchołka do tablicy z Listą Sąsiedztwa Grafu
        adjacencyList[startStopIdx-1] = new Vertex(startStopIdx, startStop);
    }
    if(labelToIndexMap.find(endStop) == labelToIndexMap.end()) {
        //pobranie numeru indeksu dla nowego wierzchołka grafu
        int endStopIdx = ++noOfVertices;
        
        //brak mapowania nazwy przystanku do numeru wierzchołka -> dodanie mapowania
        labelToIndexMap[endStop] = endStopIdx;
        
        //realokowanie rozmiaru tablicy przechowującej Listę Sąsiedztwa Grafu
        resizeGraph();
        
        //dodanie nowego wierzchołka do tablicy z Listą Sąsiedztwa Grafu
        adjacencyList[endStopIdx-1] = new Vertex(endStopIdx, endStop);
    }
    
    //2. wstawienie do listy sąsiedztwa dwóch połączeń (Edge)
    //   startStop -> krawędź do endStop
    //   endStop -> krawędź do startStop
    adjacencyList[labelToIndexMap[startStop] -1]
        ->addAdjacentVertex( //utworzenie krawędzi do wierzchołka sąsiada
                lineNo,
                         adjacencyList[labelToIndexMap[startStop]-1],
                         time,
                         adjacencyList[labelToIndexMap[endStop]-1]
                         );
    
    
    adjacencyList[labelToIndexMap[endStop] -1]
        ->addAdjacentVertex( //utworzenie krawędzi do wiezchołka sąsiada
                            lineNo,
                            adjacencyList[labelToIndexMap[endStop]-1],
                            time,
                            adjacencyList[labelToIndexMap[startStop]-1]
                          );
    return true;
}

/**
 * Metoda zwiększająca, realokująca rozmiar tablicy w której przechowywana jest 
 * reprezentacja grafu w postaci listy sąsiedztwa
 **/
void StopsGraph::resizeGraph()
{
    if(DEBBUG)
        printf("Realokacja tablicy z reprezentacją grafu w postaci listy sąsiedztwa...\n");
    
    Vertex **tmp = (Vertex **) realloc(adjacencyList, noOfVertices * sizeof(Vertex *));
    
    if(tmp != NULL) {
        adjacencyList = tmp;
    } else {
        printf("Błąd podczas realokowania tablicy...\n");
        return;
    }
}

/**
 * Metoda pozwala na zainicjalizowanie tablic pomocniczych algorytmu Dijkstry
 **/
void Dijkstra::init(void)
{
    for(int i =0; i < graph->size(); i++) {
        //wypełnienie tablic najkrótszych czasów przejazdów z wierzchołka
        //źródłowego do i-tego wierzchołka grafu wartościami INFINITE
        //oznaczającymi czas nieskończony
        //podczas wykonywania algorytmu wartości tablic będą porównywane i
        //nadpisywane tymi odpowiadającymi krótszym drogą z SOURCE do i-tego
        //wierzchołka
        travelTime[i] = INFINITE;
        
        //analogicznie ustawiamy na NULL wszystkie wierzchołki poprzedników
        predecessor[i] = 0;
        //oraz wszystkie krawędzie poprzedzające
        predecessorEdge[i] = 0;
    }
    
    source = NULL;
}

/**
 * Metoda wykonująca główny algorytm Dijkstry wyszukujący najkrótsze ścieżki
 * z wierzchołka źródłowego SOURCE do pozostałych wierzchołków
 **/
int Dijkstra::findMinPathsFrom(const Vertex *src)
{
    if(source != NULL && src->index == source->index) {
        //jeżeli metoda została już wykonana dla bieżącego
        //wierzchołka źródłowego SOURCE to nie ma potrzeby
        //wykonywać jej ponownie
        return 1;
    }
    
    //1. inicjalizowanie tablic pomocniczych
    //   travelTime[i] <- INFINITY
    //   predecessor[i] <- NULL
    //   predecessorEdge[i] <- NULL
    //   source = NULL
    init();
    
    //2. przypisanie wierzchołka źródłowego
    source = src;
    
    //3. inicjalizacja czasu przejazdu z src do src = 0
    travelTime[source->index -1] = 0;
    
    //4. utworzenie zbioru (kolejki priorytetowej) wszystkich
    //   wierzchołków (przystanków) grafu
    PriorityQueue queue(graph->size());
    //umieszczenie pierwszego startowego wierzchołka (początkowego przystanku)
    //w kolejce priorytetowej z waga 0 oraz pozostałych z wagą infinity
    for(int i=0; i<graph->size(); i++)
    {
        queue.push(graph->getVertex(i+1), travelTime[i]);
    }
    
    //petla wykonywana dopuki beda elementy w kolejce priorytetowej
    //do kolejki dodawane sa za kazdym razem wierzchołki sąsiadujące z
    //z odpowiednim priorytetem
    while(!queue.empty()) {
        if(DEBBUG_2)
            queue.print();
        
        //pobranie wierzchołka z kolejki priorytetowej
        //o najmniejszym czasie przejazdu
        const Vertex *v = queue.pop();
        int vIdx = v->index -1;
        if(travelTime[vIdx] == INFINITY) {
              //wierzchołek niedostępny
              if(DEBBUG)
                  printf("INFINITY znalezione...\n");
              break;
        }
        
        //5. Pętla po wszystkich sąsiadach wierzchołka v
        for(Edge *e = v->adjacentVertices; e != NULL; e = e->next)
        {
            int wIdx = e->getEndVertex()->index -1;
            
            if(DEBBUG)
                printf("Relaksacja krawędzi (%d:%s, %d:%s)\n",
                       v->index, v->label.c_str(),
                       e->getEndVertex()->index, e->getEndVertex()->label.c_str());
            
            //6. Relaksacja krawędzi (v, w)
            if(travelTime[wIdx] > travelTime[vIdx] + e->time)
            {
                //nadpisanie czasu przejazdu dla najkrótszej ścieżki
                //oraz poprzednika wierzchołka wIdx i krawędzi poprzedzającej wIdx
                travelTime[wIdx] = travelTime[wIdx] + e->time;
                predecessor[wIdx] = v;
                predecessorEdge[wIdx] = e;
                
                //aktualizacja priorytetu wierzchołka sąsiada (w)
                queue.decreasePriority(e->getEndVertex(), travelTime[wIdx]);
                
                if(DEBBUG) {
                    printf("Nowa waga: %d\n", travelTime[wIdx]);
                }
            }
        }
        
    }
    
    //zwraca 1 w przypadku sukcesu
    return 1;
}

int Dijkstra::minPath(const Vertex *src, const Vertex *dest, std::string *path)
{
    if(! (source != NULL && source->index == src->index) )
    {
        findMinPathsFrom(src);
    }
    
    if(travelTime[dest->index-1] == INFINITE)
    {
        *path = "Brak trasy między wskazanymi wierzchołkami.";
        return -1;
    }
    
    //rekonstrukcja ścieżki
    std::stack<int> pathStack;
    pathStack.push(dest->index);
    
    int lastVertexIndex = -1;
    
    for(const Vertex *prev = predecessor[dest->index-1];
        prev != NULL; prev = predecessor[prev->index-1])
    {
        if(prev->index == dest->index || prev->index == lastVertexIndex) break;
        pathStack.push(prev->index);
        lastVertexIndex = prev->index;
    }
    
    //do puki na stosie jest wierzchołek to ściągamy go ze stosu
    while(!pathStack.empty())
    {
        *path += std::to_string(pathStack.top()) + "-";
        
        pathStack.pop();
    }
    
    *path = path->substr(0, path->length()-1);
    
    //zwróć najkrótszą ścieżkę do wierzchołka dest
    return travelTime[dest->index-1];
}

int Graph::findMinPath(Vertex *a, Vertex *b, std::string *path)
{
    if(dijkstra == NULL)
    {
        //tworzenie obiektu implementującego algorytm Dijkstry
        //dla bieżącego grafu
        dijkstra = new Dijkstra(this);
    }
    
    int travelTime = dijkstra->minPath(a, b, path);
    
    return travelTime;
}

/**
 * Funkcja wejściowa do programu
 **/
int main(int argc, char **argv)
{
    char *filename;
    FILE *file;
    
    if(argc != 2)
    {
        printf("Wywołanie programu: ./program rozklad.txt\n");
        exit(1);
    }
    
    //pobranie nazwy pliku z argumentu wywołania programu
    filename = argv[1];
    if( !(file = fopen(filename, "r")))
    {
        perror("Błąd podczas otwierania pliku.");
        exit(1);
    }
    
    //konstrukcja grafu
    StopsGraph G(file);
    
    //wypisanie listy sąsiedztwa
    G.printAdjacencyList();
    
    int start, end;
    
    while(1) {
        //pobranie przystanku początkowego i końcowego
        printf("Podaj przystanek początkowy: \n");
        scanf("%d", &start);
        printf("Podaj przystanek końcowy: \n");
        scanf("%d", &end);
        
        //sprawdzenie poprawności wprowadzonych wierzchołków
        if(start < 1 || start > G.size())
        {
            printf("Podano niepoprawny przystanek początkowy: %d\n", start);
            continue;
        }
        
        if(end < 1 || end > G.size())
        {
            printf("Podano niepoprawny przystanek końcowy: %d\n", end);
            continue;
        }
        
        //wyszukiwanie najkrótszej ścieżki - algorytm Dijkstry
        printf("Wyszukiwanie najkrótszej ścieżki pomiędzy wierzchołkami: %d - %d ...\n", start, end);
        
        std::string path;
        int minCost = G.findMinPath(G.getVertex(start), G.getVertex(end), &path);
        printf("Minimalny koszt: %d\n", minCost);
        printf("Trasa: %s\n", path.c_str());
    }


}
