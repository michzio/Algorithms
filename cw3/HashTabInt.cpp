#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>

//rozmiar tablicy haszujacej
const int HASHTAB_SIZE = 128;
//maska umozliwiajaca uzycie operatora AND
//na obliczanym indeksie w funkcji haszujacej
//zamiast operacji modulo (%) - szybsze dzialanie
//binarnie jest to liczba np. dla 128 - 1 = 127
//00000000 00000000 00000000 01111111
//po operacji AND z wyznaczna liczba zeruje bity
//na lewo od 8 bitu dajac wartosci z przedzialu [0,127]
//przy zalozeniu rozmiaru tablicy 128 elementowej
const int HASHTAB_MASK = HASHTAB_SIZE - 1;

using namespace std;

//oznaczenie usuniętego elementu
//oraz pustego elementu
const int REMOVED = -2;
const int EMPTY = -1;

class HashTab
{
    //wewnetrzna tablica w ktorej bedziemy przechowywali elementy
    //elemetny sa tym razem integerami
    int _Table[HASHTAB_SIZE];

    //funkcja haszujaca...
    //pobiera element (string) i na jego podstawie
    //wylicza indeks w tablicy _Table[] z zakresu [0, HASHTAB_SIZE-1]
    //indeks jest obliczany w nastepujacy sposób:
    //1. przechodzimy w petli po wszystkich znakach lancucha string
    //2. kazdy znak zamieniamy na odpowiadajaca liczbe (kod ASCII)
    //3. sumujemy kody poszczegolnych znaków lancucha w zmiennej h
    //4. na koncu przemnazamy otrzymana wartosc przez liczbe pierwsza
    //   1049 i stosujemy operacje AND aby otrzymac liczbe z zakresu
    //   [0, HASHTAB_SIZE-1] tj. indeks w tablicy
    unsigned int Hash(int pNr)
    {
        unsigned int h = 0;
        //MODYFIKACJA!!!!
        //modyfikacja funkcji haszujacej dla Stringów
        //przetestowanej wczesniejszym programem testujacym
        //tym razem przypisano pNr wprost do h
        //zamiast wczesniejszego sumowania kodów ASCII znaków
        h = pNr;
        
        //otrzymana liczbe oznaczającą sumę kodów ASCII poszczególnych znaków
        //ciągu string przemnażamy przez liczbę pierwszą 1049 i następnie
        //wykonujemy operacja AND HASHTAB_MASK -1 (zamiast modulo % HASHTAB_SIZE)
        //dajac ten sam efekt tj. zwracajac reszte z dzielenia przez HASHTAB_SIZE
        //w efekcie otrzymujemy indeksy z przedzialu [0, HASHTAB_SIZE]
        return (h * 1049) & HASHTAB_MASK;
    }
    
    //funkcja haszujaca które pozwala zaimplementowac
    //tzw. haszowanie dwukrotne...
    //wyznacza wartość na podstawie ciągu znaków (string)
    //w analogiczny sposób jak Hash() z tą różnicą
    //że otrzymaną wartość sumy kodów ASCII przemnożoną przez liczbę pierwszą
    //1.) podnosimy do kwadratu -> (h*1049) * (h*1049)
    //2.) następnie wykonujemy ponownie operacje modulo % HASHTAB_SIZE (tu stosujac operator AND)
    //    AND jest szybsze od modulo %
    //3.) na końcu wykonujemy operacje bitową OR ( | ) pozwala ona ustawić
    //    ostatni bit (low significant bit) na wartos 1, w efekcie otrzymamy
    //    liczbe nieparzystą i nigdy nie otrzymamy 0 (co jest istotne by nie przesuwać
    //    się o wartość i*0 = 0 w przypadku wystapienia konfliktu!!!)
    unsigned int Hash_Prime(int pNr)
    {
        
        unsigned int h = 0;
        
        //MODYFIKACJA!!!
        //Analogicznie jak w Hash() zastąpienie
        //sumowania kodów ASCII bezpośrednim przypisaniem liczby
        //do h
        h = pNr;
        
        return ((h * 1049)*(h*1049) & HASHTAB_MASK) | 1;
    }
    
    //Funkcja obliczajaca wartosc indeksu przy rehaszowaniu
    //korzysta z dwuktornego haszowania funkcjami H(x) i H'(x)
    //zdefiniowanymi powyżej. Mamy pewność, że Hash_Prime(string)
    //zwraca liczbe która jest względnie pierwsza z rozmiarem tablicy
    //haszujacej i różna od 0.
    //Korzystamy z formuły na dwukrotne haszowanie:
    // (H(x) + i*H'(x)) mod HASHTAB_SIZE
    //operacja modulo została zastąpiona tutaj operacją AND
    unsigned int HashDouble(int pNr, int i)
    {
        return (Hash(pNr) + i*Hash_Prime(pNr)) & HASHTAB_MASK;
    }
    
    
    unsigned int Rehash(unsigned int idx, int pNr);

public:
    HashTab() { Clear(); }
    bool Insert(int pNr);
    bool Find(int pNr);
    bool Remove(int pNr);
    void Clear();

    void Print() {
        for (unsigned int i = 0; i < HASHTAB_SIZE; i++)
            cout << _Table[i] << endl;
    }
};

/**
 * Metoda wstawia nowy element do tablicy haszujacej
 * pobiera element typu string sprawdza czy nie znajduje
 * sie on juz na liscie a nastepnie oblicza dla niego
 * funkcja haszujaca indeks tablicy, gdy dojdzie do kolizji
 * przeprowadza rehaszowanie... moze zaistniec sytuacja
 * ze w tablicy nie ma wolnych miejsc wtedy zwroci false
 */
bool HashTab::Insert(int pNr)
{
    //jezeli w slowniku jest juz przekazany
    //element to nie nalezy go wstawiac kolejny raz
    //zwracamy poprostu FALSE
    if(Find(pNr)) return false;
    
    //W przeciwnym wypadku obliczamy indeks przy uzyciu
    //funkcji haszujacej
    int idx = Hash(pNr);
    
    //sprawdzamy czy dla obliczonego indeksu
    //mamy wolna komórkę w tablicy
    if(_Table[idx] == EMPTY) {
        //jezeli tak to wstawiamy element w wolne miejsce
        //i zwracamy TRUE
        _Table[idx] = pNr;
        return true;
    }
    
    //w przeciwny wypadku mamy problem należy zastosować
    //rehashowanie indeksu i znalesc wolne miejsce
    //jezeli takie istnieje
    idx = Rehash(idx, pNr);
    if(_Table[idx] == EMPTY ||  _Table[idx] == REMOVED) {
        //dodajemy element gdy znaleziono nowe puste miejsce
        //lub puste miejsce po usunieciu wczesniejszego elementu
        _Table[idx] = pNr;
        return true;
    }
    
    //sytuacja gdy w wyniku rehashowania obeszlismy cala tablice
    //do okoła i nie znalezlismy pustego miejsca
    return false;
}

/**
 * Metoda rehaszujaca, która na podstawie aktualnego indeksu
 * oraz przekazanego stringa odnajduje w wyniku rehaszowania liniowego
 * (linear probing)i zwraca wolne miejsce (empty, removed) lub obchodzi 
 * całą tablice i zwraca poczatkowe miejsce 
 **/
unsigned int HashTab::Rehash(unsigned int idx, int pNr)
{
    //jako nowy indeks przyjmujemy na poczatek aktualnie obliczony
    int newIdx = idx;
    
    //pętla przechodząca cyklicznie tablice
    for(int i=0; i< HASHTAB_SIZE; i++) {
        
        //wyznaczamy nowy indeks
       // newIdx = (idx + i) & HASHTAB_MASK;
        newIdx = HashDouble(pNr, i);
        
        //zwróć nowy indeks gdy znaleziono puste miejsce
        if(_Table[newIdx] == EMPTY)
            return newIdx;
        //zwróć nowy indeks gdy wskazuje na komórke z
        //usunietym wczesniejszym elementem
        if(_Table[newIdx] == REMOVED)
            return newIdx;
        //zwróć nowy indeks gdy odnaleziono w tablicy
        //element który probujemy dodać
        if(_Table[newIdx] == pNr)
            return newIdx;
        
    }
    //zwróć indeks elementu po cyklicznym przejsciu calej tablicy
    return newIdx;
}

/**
 * Metoda na podstawie przekazanego elementu string
 * sprawdza czy znajduje sie on w tablicy
 * najpierw oblicza indeks funkcja haszujaca 
 * nastepnie przechodzi cała tablice od tego miejsca 
 * dokonujac rehashowania liniowego. Wychodzi z pętli 
 * w przypadku znalezienia elementu lub wolnego miejsca
 * (empty) pomija miejsca oznaczone tagiem REMOVED
 **/
bool HashTab::Find(int pNr)
{
    int idx = Hash(pNr);
    int newIdx = idx;
    
    //przechodzimy tablice poczawszy od indeksu
    //wyznaczonego przez funkcje haszujaca
    for(int i=0; i < HASHTAB_SIZE; i++) {
        //w kazdej iteracji wyznaczamy nowy indeks
        //stosujac rehaszowanie liniowe
        //newIdx = (idx + i) & HASHTAB_MASK;
        newIdx = HashDouble(pNr, i);
        
        //sprawdzamy czy znaleziono element pName
        //jezeli tak zwracamy TRUE
        if(_Table[newIdx] == pNr)
            return true;
        //jezeli napotkano pustą komórkę w wyniku rehashowania
        //oznacza to ze elementu pName nie ma w tablicy
        //zwracamy FALSE
        if(_Table[newIdx] == EMPTY)
            return false;
        
    }
    //jezeli przeszlismy cala tablice i nie znaleziono elementu
    //to również zwracamy FALSE
    return false;
}

/**
 * Usuwa przekazany element stringowy z tablicy 
 * haszujacej. Najpierw oblicza indeks funkcja 
 * haszujaca a nastepnie sprawdza czy znajduje sie
 * na tej pozycji szukany element jezeli nie 
 * to dokonuje rehashowania az do momentu gdy 
 * znajdzie element do usuniecia natrafi na pusta 
 * komórkę tablicy lub przejdzie całą listę 
 **/
bool HashTab::Remove(int pNr)
{
    
    //najpierw obliczamy indeks pod którym powinien
    //znajdowac sie element za pomoca funkcji haszujacej
    int idx = Hash(pNr);
    
    //sprawdzamy czy znajduje sie pod tym indeksem element
    if(_Table[idx] == pNr) {
        //usuwamy element i zwracamy TRUE
        _Table[idx] = REMOVED;
        return true;
    }
    
    //w przeciwnym wypadku musimy znalesc element
    //stosujac rehashowanie
    int newIdx = idx;
    
    //przechodzimy tablice poczawszy od indeksu
    //wyznaczonego przez funkcje haszujaca
    for(int i=0; i < HASHTAB_SIZE; i++) {
        //w kazdej iteracji wyznaczamy nowy indeks
        //stosujac rehaszowanie liniowe
        //newIdx = (idx + i) & HASHTAB_MASK;
        newIdx = HashDouble(pNr, i);
        
        //sprawdzamy czy znaleziono element pName
        //jezeli tak to usuwamy go ustawiajac komórkę
        //na stałą REMOVED i zwracamy TRUE
        if(_Table[newIdx] == pNr) {
            _Table[newIdx] = REMOVED;
            return true;
        }
        //jezeli napotkano pustą komórkę w wyniku rehashowania
        //oznacza to ze elementu pName nie ma w tablicy
        //zwracamy FALSE
        if(_Table[newIdx] == EMPTY) {
            return false;
        }
        
    }
    
    //sytuacja w której przeszlismy cyklicznie cala tablice
    //nie znalezlismy ani pustego miejsca ani usuwanego elementu
    //(tablica przepełniona)
    return false;
}

void HashTab::Clear()
{
    for (unsigned int i = 0; i < HASHTAB_SIZE; i++) 
        _Table[i] = EMPTY;
}

vector<int> NumberList;
HashTab Tab;

bool ReadFile()
{
    fstream File;
    File.open("numbers.txt");
    if (!File.good()) {
        cout << "B≥πd podczas prÛby odczyty pliku z danymi: names.txt" << endl;
        return false;
    }
    string Line;
    while (getline(File, Line))
        NumberList.push_back(std::stoi(Line));

    File.close();

    return true;
}

bool Test1()
{
    for (unsigned int i = 0; i < NumberList.size(); i++) {
        bool Result = Tab.Insert(NumberList[i]);
        if (i < HASHTAB_SIZE) {
            if (!Result) {
                cout << "Test1 - b≥πd: wstawienie to tablicy nie powiod≥o siÍ!" << endl;
                return false;
            }
        } else
            if (Result) {
                cout << "Test1 - b≥πd: wstawienie to tablicy powiod≥o siÍ, ale tablica powinna byÊ juø pe≥na!" << endl;
                return false;
            }
    }
    cout << "Test1 - ok" << endl;
    return true;
}

bool Test2()
{
    Tab.Clear();
    vector<int> InsList;
    vector<int> RestList;

    for (unsigned int i = 0; i < NumberList.size(); i++) {
        if (rand() % 2 == 0) {
            InsList.push_back(NumberList[i]);
            if (!Tab.Insert(NumberList[i])) {
                cout << "Test2 - b≥πd: wstawienie to tablicy nie powiod≥o siÍ!" << endl;
                return false;
            }
        } else
            RestList.push_back(NumberList[i]);
    }
    for (vector<int>::iterator it = InsList.begin(); it != InsList.end(); it++) {
        if (!Tab.Find(*it)) {
            cout << "Test2 - b≥πd: nie znaleziono elementu, ktÛry zosta≥ wczeúniej wstawiony do tablicy!" << endl;
            return false;
        }
    }
    for (vector<int>::iterator it = RestList.begin(); it != RestList.end(); it++) {
        if (Tab.Find(*it)) {
            cout << "Test2 - b≥πd: znaleziono element, ktÛry nie zosta≥ wczeúniej wstawiony do tablicy!" << endl;
            return false;
        }
    }
    cout << "Test2 - ok" << endl;
    return true;
}

bool Test3()
{
    Tab.Clear();
    vector<int> InsList;
    vector<int> DelList;

    for (unsigned int i = 0; i < HASHTAB_SIZE; i++) {
        if (rand() % 2 == 0) {
            InsList.push_back(NumberList[i]);
            if (!Tab.Insert(NumberList[i])) {
                cout << "Test3 - b≥πd: wstawienie to tablicy nie powiod≥o siÍ!" << endl;
                return false;
            }
        } else {
            DelList.push_back(NumberList[i]);
            if (!Tab.Insert(NumberList[i])) {
                cout << "Test3 - b≥πd: wstawienie to tablicy nie powiod≥o siÍ!" << endl;
                return false;
            }
        }
    }
    for (vector<int>::iterator it = DelList.begin(); it != DelList.end(); it++) {
        if (!Tab.Remove(*it)) {
            cout << "Test3 - b≥πd: nie uda≥o siÍ usunπc elementu, ktÛry zosta≥ wczeúniej wstawiony do tablicy!" << endl;
            return false;
        }
    }
    for (vector<int>::iterator it = DelList.begin(); it != DelList.end(); it++) {
        if (Tab.Find(*it)) {
            cout << "Test3 - b≥πd: znaleziono element, ktÛry zosta≥ wczeúniej usuniÍty z tablicy!" << endl;
            return false;
        }
    }
    for (vector<int>::iterator it = InsList.begin(); it != InsList.end(); it++) {
        if (!Tab.Find(*it)) {
            cout << "Test3 - b≥πd: nie znaleziono element, ktÛry zosta≥ wczeúniej wstawiony do tablicy!" << endl;
            return false;
        }
    }
    for (unsigned int i = HASHTAB_SIZE; i < NumberList.size(); i++) {
        if (!Tab.Insert(NumberList[i])) {
            cout << "Test3 - b≥πd: wstawienie to tablicy nie powiod≥o siÍ!" << endl;
            return false;
        }
    }
    for (unsigned int i = HASHTAB_SIZE; i < NumberList.size(); i++) {
        if (!Tab.Find(NumberList[i])) {
            cout << "Test3 - b≥πd: nie znaleziono element, ktÛry zosta≥ wczeúniej wstawiony do tablicy!" << endl;
            return false;
        }
    }
    cout << "Test3 - ok" << endl;
    return true;
}

int main()
{
    ReadFile();
    Test1();
    Test2();
    Test3();

    return 0;
}