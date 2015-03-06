#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <limits>

using namespace std;

#define DEBBUG 1
#define BUFF_SIZE 128

const unsigned long GRAPH_MASK = ~0u;

struct Vertex;

/**
 * krawędzie będą elementami listy sąsiedztwa
 * dla kążdego wierzchołka, pozwoli to na przechowywanie
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
    //wskaźnik na kolejny element list sąsiedztwa
    Edge *next;
    //konstruktor krawędzi
    Edge(Vertex *v, int w) : destination(v), weight(w) {}
    
    Vertex *getVertex()
    {
        return destination;
    }
    
};

struct Vertex
{
    int label;
    Edge *adjecentVertices = 0;
    
    //konstruktor wierzchołka
    Vertex(int l) : label(l) {}
    
    /**
     * dodawanie wierzchołków do listy sąsiedztwa bieżącego wierzchołka
     * nowy wierzchołek przyległy wstawiamy na początku listy zawsze czas O(1)
     * wstawiania. Domyślna waga to 1, w przypadku gdby graf nie był ważony
     * tak by długość każej krawędzi była równa,
     * wartość przypisana do krawędzi można zmodyfikować metodą setWeight na Edge!
     **/
    void addAdjecentVertex(Vertex *v, int weight = 1 )
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

        
        /* w czasie O(1) - ale potem dodawanie wag i tak O( degree(vertex) )
        e->next = adjecentVertices;
        adjecentVertices = e;
         */
    }
    
    
    /** 
     * metoda która przeszukuje listę sąsiedztwa wierzchołka A w celu 
     * odnalezienia bezpośrednij krawędzi do wierzchołka B 
     * jeżeli znajdzie taką krawędź to zwraca ją 
     **/
     Edge *directEdge(Vertex *B)
     {
         
        //1. Przechodzimy krawędzi poszukując wierzchołka B (prównujemy label'ki)
         for(Edge *e = adjecentVertices; e != NULL; e = e->next)
         {
             //Odnaleźliśmy bezpośrednią krawędź z A do B
             if(e->getVertex()->label == B->label)
                 return e;
         }
         
         //2. zwracamy NULL tj. przeszliśmy całą listę sąsiedztwa i nie znaleźliśmy B
         return NULL;
     }
    
};

class Graph
{
    int noOfVertices = 0;
    //lista sąsiedztwa, poszczególne pozycje w tablicy wskazują na bezpośrenich
    //sąsiadów wierzchołka poprzez krawędzie które prowadzą do nich
    //będziemy alokować tablice o rozmiarze równym ilości wierzchołków
    Vertex *adjacencyList;

public:
    
    Graph(FILE *pFile);
    ~Graph();
    
    
    void printAdjacencyList(void);
    int findMinPath(Vertex *a, Vertex *b, string &path);
    Vertex *getVertex(int label);
    
private:
    int readLine(char *buffer, size_t n, FILE *pFile);
    void addVerticesToAdjacencyListOf(Vertex *v, char *vertexList);
    void addWeightsToGraph(char *weightsList);
    int  findMinPath(Vertex *a, Vertex *b, unsigned long graph, string &str);
    
};

/**
 * konstruktor wczytujący graf z pliku o wskazanym strumieniu
 **/
Graph::Graph(FILE *pFile)
{
    //buffor na odczytywane linijki tekstu
    char *buffer;
    
    
    //zaalokowanie bufforu do którego odczytujemy poszczególne linijki tekstu z pliku
    //w razie gdyby był zamały bufor na pomieszczenie linjki tekstu bedziemy realokowac
    if( !(buffer = (char *) malloc(sizeof(char) * (BUFF_SIZE + 1))) )
    {
        perror("Błąd podczas alokowania buffora pamieci.");
        exit(1);
    }
   
    //1. Wczytujemy liczbe wierzchołków
    if( readLine(buffer, BUFF_SIZE, pFile) < 1)
    {
        printf("Błąd podczas odczytywania liczby wierzchołków grafu.");
        exit(1);
    }
    
    noOfVertices = atoi(buffer);
    
    if(DEBBUG)
        printf("Liczba wierzchołków: %d\n", noOfVertices);
    
    if(DEBBUG)
        printf("### KONSTRUKCJA LISTY SĄSIEDZTWA ###\n");
    
    //alokujemy tablice wierzchołków z listami sąsiedztwa
    adjacencyList = (struct Vertex *)malloc( sizeof(Vertex) * noOfVertices);
    
    //dodajemy wszystkie n wierzchołków do tablicy sąsiedztwa
    for(int i=0; i< noOfVertices; i++)
    {
        adjacencyList[i] = Vertex(i+1);
    }
    
    //wczytujemy kolejne listy sąsiedztwa posczególnych wierzchołków
    for(int i=0; i< noOfVertices; i++)
    {
        if( readLine(buffer, BUFF_SIZE, pFile)  < 1)
        {
            //nie dopuszczamy sytuacji w której wierzchołek nie ma sąsiada
            printf("Wierzchołek %d nie posiada sąsiada!\n", (i+1) );
            exit(1);
        }
        
        if(DEBBUG)
            printf("%s\n", buffer);
        
        //dodajemy odczytane wierzchołki do listy sąsiedztwa używając metody pomocniczej
        addVerticesToAdjacencyListOf(&adjacencyList[i], buffer);
        
    }
    
    //wczytanie wag dla poszczególnych krawędzi
    if(readLine(buffer, BUFF_SIZE, pFile) < 1)
    {
        printf("Błąd podczas wczytywania wag poszczególnych krawędzi.");
        exit(1);
    }
    
    //dodanie wczytanych wag do krawędzi grafu używając metody pomocniczej
    addWeightsToGraph(buffer);
    
    if(DEBBUG)
        printf("### KONIEC KONSTRUKCJI ###\n");
    
    free(buffer);

}

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
        printf("%d | ", adjacencyList[i].label);
        
        for(Edge *e = adjacencyList[i].adjecentVertices; e != NULL; e = e->next)
        {
            printf("%d (%d), ", e->getVertex()->label, e->weight);
        }
        
        printf("\n");
    }
   
    printf("\n");
}

void Graph::addWeightsToGraph(char *weightsList)
{
    int weight;
    char *token;
    int i; //indeks wierzchołka na liście sąsiedztwa
    Edge *e; //wskaźnik na krawędź (wierzchołek) na liście sąsiedztwa
    
    //inicjalizacja wskaźników
    i = 0;
    e = adjacencyList[i].adjecentVertices;
    
    token = strtok(weightsList, ", ");
    
    while(token != NULL)
    {
        if(DEBBUG)
            printf("%s, ", token);
        
        //konwersja wagi na int
        weight = atoi(token);
        
        //gdy doszliśmy do końca listy sąsiedztwa wierzchołka przechodzmi to
        //następnego wierzchołka w tablicy, jeżeli nie ma już więcej wierzchołków
        //to podano za dużo wag!
        if(e == NULL) {
            i++;
            if(i >= noOfVertices)
            {
                printf("Błędna liczba wag....!\n");
                exit(1);
            }
            e = adjacencyList[i].adjecentVertices;
        }
        
        //przypisanie wagi do krawędziw wskazującej na sąsiadujący wierzchołek
        e->weight = weight;
        
        //odczytanie kolejnej wagi
        token = strtok(NULL, ", ");
        //przejście do kolejnej krawędzi na liście sąsiedztwa
        e = e->next;
    }
    
    if(DEBBUG)
        printf("\n");
    
}

/**
 * Metoda pomocnicza ktora pobiera oddzielną ", " listę wierzchołków 
 * i wstawia je jako wierzchołki sąsiednie na liste sąsiedztwa 
 * wierzchołka v pobranego jako pierwszy argument
 **/
void Graph::addVerticesToAdjacencyListOf(Vertex *v, char *vertexList)
{
    int label;
    char *token;
    
    token = strtok(vertexList, ", ");
    
    while(token != NULL)
    {
        if(DEBBUG)
            printf("%s\n", token);
        
        //konwersja na typ int
        label = atoi(token);
        
        //z tablicy wybieamy odpowiedni wierzchołek i wstawiamy
        //na listę sąsiedztwa bieżącego wierzchołka v
        v->addAdjecentVertex( &adjacencyList[label-1] );
        
        
        token = strtok(NULL, ", ");
    }
}

/**
 * metoda pomocnicza umożliwiająca wczytywanie grafu z pliku
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
    while( (ch != '\n') &&  (ch != EOF) ) {
        if(pos == curr_size)
        {
            cout << pos << endl;
            //zapełnilismy rozmiar bufora i nie odczytaliśmy jeszcze całej linii
            curr_size *= 2; //podowjenie bieżącego rozmiaru bufora
            if( (buffer = (char *) realloc(buffer, sizeof(char) * (curr_size + 1))) )
            {
                perror("Błąd podczas realokowania buffora pamieci.");
                exit(1);
            }
            
        }
    
        buffer[pos++] = ch;
        
        ch = getc(pFile);
    }
    
    //znak końca stringu
    buffer[pos] = '\0';
    
    //zwracamy liczbe odczytanych znaków
    return pos;
}

int Graph::findMinPath(Vertex *a, Vertex *b, string &path)
{
    int offset = a->label;
    
    int minCost =  findMinPath(a, b, GRAPH_MASK & ~(1 << offset), path);
    
    //dokonkatowanie początkowego węzła
    path = to_string(a->label) + "," + path;
    
    return minCost;
}

/**
 * Metoda wyszukuje minimalna ścieżkę 
 * zwraca jej długość, a poprzez wskaźnik *path liste krawędzi
 * pobiera 2 parametry wskaźnik na początkowy wierzchołek i wskaźnik 
 * na końcowy wierzchołek
 **/
int Graph::findMinPath(Vertex *a, Vertex *b, unsigned long graph, string &path)
{
    Edge *e;
    
    int minCost = std::numeric_limits<int>::max();
    Edge *nextVertex;
    unsigned long subgraph;
    string minSubPath;
    
    //1. jeżeli wierzchołek (a) nie ma żadnych sąsiadów to
    //   zwracamy pustą ścieżkę path oraz długość 0;
    if(a->adjecentVertices == NULL || a->label == b->label) {
        path = "";
        return 0;
    }
    
    //2. jeżeli wierzchołek (b) jest bezpośrednim sąsiadem
    //   (a) to zwracamy listę składającą się z jednej krawędzi
    //    oraz jej wagę
    
    if( (e = a->directEdge(b)) != NULL )
    {
        path =  to_string(e->getVertex()->label);
        
        return e->weight;
    }
    
    int cost = 0;
    string subPath;
    
    //3. Ani nie jest to wierzchołek nie majacy sąsiada
    //   ani taki dla którego b jest bezpośrednim sąsiadaem
    //   Musimy rekurencyjnie poszukiwać najkrótszej ścieżki
    for( e = a->adjecentVertices; e != NULL; e = e->next)
    {
        //pętla po wrzystkich krawędziach prowadzących do sąsiednich wierzchołków
        
        //indeks sąsiedniego wierzchołka
        int offset = e->getVertex()->label - 1;
        
        //jeżeli sąsiedni węzeł nie został usunięty do tej pory z grafu to możemy
        //wyszukiwać minimalnej ścieżki poczynając od niego
        if(graph & (1 << offset)) {
            //usuwanie n-tego wierzchołka z podgrafu przekazywanego rekurencyjnie
            subgraph = graph & ~(1 << offset);
            
            cost = findMinPath(e->getVertex(), b, subgraph, subPath) + e->weight;
            if(minCost > cost)
            {
                minCost = cost;
                nextVertex = e;
                minSubPath = subPath;
            }
        }
    }

   
    path =  to_string(nextVertex->getVertex()->label) + "," + minSubPath;
    
    return minCost;
    
}

Vertex *Graph::getVertex(int label)
{
    if(label > noOfVertices || label < 1)
    {
        printf("Podano wierzchołek spoza zakresu dostępnych wierzchołków.\n");
    }
    return &adjacencyList[label-1];
}

int main(int argc, char **argv)
{
    //uchwyt do pliku
    FILE *file;
    
    //otwieranie strumienia do pliku z definicją grafu
    if(!(file = fopen("graph3.txt", "r")))
    {
        perror("Błąd podczas otwierania pliku.");
        exit(1);
    }

    //konstrukcja obiektu grafu na stosie
    Graph G(file);
    
    //wypisanie listy sąsiedztwa
    G.printAdjacencyList();
    
    printf("Szukana ścieżka z 1 do 7.\n");
    Vertex *A = G.getVertex(1);
    Vertex *B = G.getVertex(7);
    
    string path;
    
    int minCost = G.findMinPath(A, B, path);
    
    printf("Minimalny koszt: %d\n", minCost);
    printf("Trasa: %s\n", path.c_str());
    
    
    
    printf("Szukana ścieżka z 2 do 5.\n");
    A = G.getVertex(2);
    B = G.getVertex(5);
    
    minCost = G.findMinPath(A, B, path);
    
    printf("Minimalny koszt: %d\n", minCost);
    printf("Trasa: %s\n", path.c_str());
    
    
    printf("Szukana ścieżka z 6 do 3.\n");
    A = G.getVertex(6);
    B = G.getVertex(3);
    
    minCost = G.findMinPath(A, B, path);
    
    printf("Minimalny koszt: %d\n", minCost);
    printf("Trasa: %s\n", path.c_str());
    //zamykanie pliku
    fclose(file);
    return 0;
}

//zadanie domowe
//algorytm najkrotszej sciezki
//rekurencyjny
//dowolna reprezentacja grafu
//lista sasiedztwa
//lub tablica
//wagi beda podawane w takiej samej strukturze jak lista krawedzi
//druga lista ktora jest lista wag
//.... nie wiecej niz 20 wierzchołków