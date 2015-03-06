
#include "doubly_linked_list.h"
#include<iostream>
#include<cstdio>
#include<utility>
#include<functional>
#include<iterator>

//implementacja DoublyLinkedList

namespace ADT {

/**
 * konstruktor domyślny typu DoublyLinkedList<T>
 */
template<typename T>
DoublyLinkedList<T>::DoublyLinkedList() : count(0) {
        //pusta lista wskaźnik na next element wskazuje na siebie samego
        tail.next = &tail;
        //wskaźnik na previud wskazuje na siebie samego
        tail.previous = &tail;
}

/**
 * destruktor
 */
template<typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
    //usunięcie wszystkich węzłów listy - do zaimplementowania
    //clear();
}

/**
 * begin() - zwraca iterator do odczytu/zapisu, który wskazuje na pierwszy
 * element listy.
 */
template<typename T>
ListIterator<T> DoublyLinkedList<T>::begin()
{
    return tail.next;
}

/**
 * begin() const - zwraca iterator tylko do odczytu (constant), który wskazuje 
 * na pierwszy element listy 
 */
template<typename T>
ConstListIterator<T> DoublyLinkedList<T>::begin() const
{
    return tail.next;
}

/**
 * end() - zwraca iterator do odczytu/zapisu, który wskazuje na element
 * tuż za ostanim elementem listy.
 */
template<typename T>
ListIterator<T> DoublyLinkedList<T>::end()
{
    return &tail;
}

/**
 * end() const - zwraca iterator tylko do odczytu (constant), który
 * wskazuje na element tuż za ostatnim elementem listy. 
 */
template<typename T>
ConstListIterator<T> DoublyLinkedList<T>::end() const
{
    return &tail;
}
    
/**
 * rbegin() - zwraca iterator przechodzący wspak po liście elementów
 * poczynając od elementu na który wskazuje tail. Iterując go
 * itr++ przechodzimy w kierunku początku listy. Jest on iteratorem 
 * dwukierunkowym tj. można używać ++itr oraz --itr. 
 * Jest to wersja iteratora do odczytu/zapisu
 **/
template<typename T>
ReverseListIterator<T> DoublyLinkedList<T>::rbegin()
{
    return ReverseListIterator<T>(--end());
}

/**
 * rbegin() const - zwraca iterator przechodzący wspak po liście elementó∑
 * poczynając od elementu na który wskazuje tail. Iterując go 
 * itr++ przechodzimy w kierunku początku listy. Jest to iterator 
 * dwukierunkowy tj. można używać ++itr oraz --itr.
 * Jest to wersja iteratora do odczytu. 
 **/
template<typename T>
ReverseConstListIterator<T> DoublyLinkedList<T>::rbegin() const
{
    return ReverseConstListIterator<T>(--end());
}
    
/**
 * rend() - zwraca iterator przechodzący wspak po liście elementów
 * wskazujący na element teoretyczny poprzedający pierwszy węzeł. 
 * Zwracany iterator jest dwukierunkowy. (++itr, --itr)
 * Iterator umożliwa odczyt/zapis.
 **/
template<typename T>
ReverseListIterator<T> DoublyLinkedList<T>::rend()
{
    return ReverseListIterator<T>(begin());
}
    
/**
 * rend() const - zwraca iterator przechodzący wspak listę elementów
 * wskazujący na element poprzedzający pierwszy węzeł. 
 * Zwracany iterator jest dwukierunkowy (++itr, --itr)
 * Iterator tylko do odczytu (read-only)
 **/
template<typename T>
ReverseConstListIterator<T> DoublyLinkedList<T>::rend() const
{
    return ReverseConstListIterator<T>(begin());
}
    
/**
 * cbegin() - metoda która zwraca stały iterator ConstListIterator<T> (tylko do odczytu)
 * wskazujący na pierwszy element na liście.
 **/
template<typename T>
ConstListIterator<T> DoublyLinkedList<T>::cbegin() const
{
    return ConstListIterator<T>(tail.next);
}

/**
 * cend() - metoda która zwraca stały iterator ConstListIteratr<T> (tylko do odczytu)
 * wskazujący na element tuż za ostatnim elementem listy
 **/
template<typename T>
ConstListIterator<T> DoublyLinkedList<T>::cend() const
{
    return ConstListIterator<T>(&tail);
}
    
/**
 * crbegin() - metoda zwraca stały (tylko do odczytu) iterator iterujący
 * wspak listy elementów, wskazujący na ostatni element list 
 **/
template<typename T>
ReverseConstListIterator<T> DoublyLinkedList<T>::crbegin() const {
    return ReverseConstListIterator<T>(end());
}
    
/**
 * crend() - metoda zwraca stały (tylko do odczytu) iterator iterujący 
 * wspak listy elementów, wskazujący na element poprzedzajacy pierwszy listy
 **/
template<typename T>
ReverseConstListIterator<T> DoublyLinkedList<T>::crend() const {
    return ReverseConstListIterator<T>(begin());
}

/**
 * empty() const - zwraca wartość boolowską okreslającą czy lista jest pusta
 * lista będzie pusta gdy tail.next będzie wskazywał na samego siebie 
 * czyli &tail. Oznacza to że nie mamy pierwszego elementu na liście.
 **/
template<typename T>
bool DoublyLinkedList<T>::empty() const
{
    return tail.next == &tail;
}
    
/**
 * size() const - zwraca liczbę elementów na liście 
 **/
template<typename T>
int DoublyLinkedList<T>::size() const
{
    //return std::distance(begin(), end());
    return count;
}
    
/**
 * insert() - metoda dodaje węzeł z wartością przekazaną jako drugi parametr val
 * przed węzłem wskazanym przez pierwszy parametr position (ConstListIterator<T>)
 * przesuwając listę od tej pozycji o jedną do przodu oraz zwiększa zmienną count
 * wartosc val jest przekopiowywana
 **/
template<typename T>
ListIterator<T> DoublyLinkedList<T>::insert(ConstListIterator<T> position, const T& val)
{
        //tworzymy nowy wezel z wartoscia przekopiowana z val
        DoublyLinkedNode<T> *newNode = new DoublyLinkedNode<T>(val);
        //podpinamy nowo utworzony wezel na pozycie position przesuwajac wezly
        //poczawszy od position o jedna pozycje do przodu
        DoublyLinkedNode<T> *currentNode = position.node;
        newNode->next = currentNode;
        newNode->previous = currentNode->previous;
        currentNode->previous->next = newNode;
        currentNode->previous = newNode;
    
        //zwiekszamy licznik elementow na liscie
        count++;
        //logLinkedList();
    
    return ListIterator<T>(newNode);
}
    
/**
 * insert() - metoda działająca analogicznie jak powyższa insert z tą
 * różnicą, że przekazywany parametr wartości (val) jest przez przeniesienie std::move()
 **/
template<typename T>
ListIterator<T> DoublyLinkedList<T>::insert(ConstListIterator<T> position, T&& val)
{
    //tworzymy nowy wezel z wartoscia przekopiowana z val
    DoublyLinkedNode<T> *newNode = new DoublyLinkedNode<T>(val);
    //podpinamy nowo utworzony wezel na pozycie position przesuwajac wezly
    //poczawszy od position o jedna pozycje do przodu
    DoublyLinkedNode<T> *currentNode = const_cast<DoublyLinkedNode<T> *>(position.node);
    newNode->next = currentNode;
    newNode->previous = currentNode->previous;
    currentNode->previous->next = newNode;
    currentNode->previous = newNode;
    
    //zwiekszamy licznik elementow na liscie
    count++;
    //logLinkedList();
    return ListIterator<T>(newNode);
}

/**
 * insert() - wstawia począwszy od pozycji poprzedzającej tą wskazywaną przez 
 * iterator position zadaną liczbę (parametr int n) kopi wartości (elementów)
 * określonych przez 3 parametr
 * Parametry:
 * @position - iterator ConstListIterator<T> wskazujący na pozycje przed którą wstawiać
 * @n - int określający ilość kopi wstawianej wartości
 * @val - wstawiana wartość jako stała referencja typu const T&
 * return iterator wskazujący na pierwsza wstawioną kopie danych
 **/
template<typename T>
ListIterator<T> DoublyLinkedList<T>::insert(ConstListIterator<T> position, int n, const T& val)
{
   while(n>0)
    {
        insert(position, n);
        --n;
    }
    
    return ListIterator<T>(const_cast<DoublyLinkedNode<T>*>(position.node));
}
    
/**
 * insert() - metoda ma za zadanie przekopiowac elementy poczynając od
 * wskazywaneych przez piewszy iterator InputIterator first, a kończąc na
 * ostatnim iteratorze InputIterator last  do bieżącej list wstawiając je na liście
 * przed węzłem wskazywanym iteratorem ConstListIterator<T> position przekazany jako
 * pierwszy argument do metody.
 * Parametry:
 * @position - wskaźnik na pozycje na której metoda ma wstawiac elementy
 * @first - InputIterator poczatek listy skąd maja być przekopiowywane dane
 * @last - InputIterator wskazujacy do którego węzła ma postępować przekopiowywanie
 * return - iterator ListIterator<T> wskazujący na pierwszy wstawiony element
 **/
template<typename T>
template<class InputIterator>
ListIterator<T> DoublyLinkedList<T>::insert(ConstListIterator<T> position, InputIterator first, InputIterator last)
{
    //wstawiamy po kolei elementy znajdujace sie w zakresie [first,last)
    //na liste poczawszy od pozycji position
    while(first != last)
    {
        insert(position, *first);
        ++first;
    }
    
    return ListIterator<T>(const_cast<DoublyLinkedNode<T>*>(position.node));
}
/**
 * insert() - wstawia wartości na liste przed wskazana przez iterator position
 * pozycje, wartości są pobierane z listy inicjalizującej std::initializer_list<T>
 * Parametry:
 * @position - iterator ConstListIterator<T> wskazujący pozycje przed którą wstawiać
 * @il - std::initializer_list<T> lista inicjalizująca z wartościami do wstawienia
 * return - iterator ListIterator<T> na pierwszy wstawiony element
 **/
template<typename T>
ListIterator<T> DoublyLinkedList<T>::insert(ConstListIterator<T> position,
                                                std::initializer_list<T> il)
{
    for (auto itr = il.begin() ; itr != il.end() ; ++itr)
    {
        insert(position, *itr);
    }
    //jeżeli lista inicjalizujaca jest pusta to zwracamy iterator na biezacy element
    return ListIterator<T>(const_cast<DoublyLinkedNode<T>*>(position.node));
}
    
/**
 * push_front() - wstawia przekazana wartość (referencja tylko do odczytu const T&) 
 * do węzła na początku listy poprzedzającego wszystkie inne węzły na liście
 * Wykorzystuje metodę insert() przekazując jako iterator ConstListIterator<T>
 * wskazujący na pierwszy element listy w którym to miejscu zostanie wstawiona wartość
 * (drugi parametr) przesuwając pozostałe węzły listy do przodu
 * @ val - parametr typu const T& (referencja tylko do odczytu) która jest wartoscia jaka 
 *          zostanie wstawiona w pierwszym węźle listy DoublyLinkedList<T>
 **/
template<typename T>
void DoublyLinkedList<T>::push_front(const T& val)
{
    insert(begin(), val);
}
    
/**
 * push_front() - analogicznie jak wyżej tyle, że parametr jest przenoszony,
 * a nie kopiowany
 **/
template<typename T>
void DoublyLinkedList<T>::push_front(T&& val)
{
    insert(begin(), std::move(val));
}
 
    
/**
 * front() - metoda odczytuje wartość elementu znajdującego się na pierwszej pozycji listy
 */
template<typename T>
T& DoublyLinkedList<T>::front(void)
{
    if(empty()) printf("'EMPTY'\n'");
    return tail.next->value;
}
   
/**
 * const T& front() const - metoda zwracająca stałą referencję na pierwszą wartość na 
 * liście
 **/
template<typename T>
const T& DoublyLinkedList<T>::front(void) const
{
    if(empty()) printf("'EMPTY'\n");
    return tail.next->value;
}
    
/**
 * back() - metoda odczytuje wartość elementu znajdującego się na ostatniej
 * pozycji list w postaci do odczytu/zapisu
 */
template<typename T>
T& DoublyLinkedList<T>::back(void)
{
    if(empty()) printf("'EMPTY'\n'");
    return *(--end());
}
    
/**
 * const T& back() const - metoda zwracająca stałą referencję na ostatnia 
 * wartość na liście w postaci tylko do odczytu
 **/
template<typename T>
const T& DoublyLinkedList<T>::back(void) const
{
    if(empty()) printf("'EMPTY'\n");
    return *(--end());
}
    
/**
 * assign(InputIterator f, InputIterator l) - metoda przypisuje do listy zakres wartości 
 * wskazwywany przez iteratory przekazane jako paramtery. Przypisanie przekopiowywanych 
 * elementów całkowicie zmienia listę, lista będzie miała nowy rozmiar równy zakresowi 
 * przekopiowywanych elementów
 * Parametry:
 * @InputIterator f - iterator wskazujacy na pierwsza wartosc to przekopiowania
 * @InputIterator l - iterator wskazujacy na ostatnia wartosc
 **/
template<typename T>
template<class InputIterator>
void DoublyLinkedList<T>::assign(InputIterator first, InputIterator last)
{
    //pobieramy iteratory na poczatek i na koniec listy
    ListIterator<T> begin = begin();
    ListIterator<T> end = end();
    
    //przechodzimy w petli prze kolejne elementy z zakresu wskazywanego przez iteratory
    //przekazane w argumentach wywołania oraz przez elementy biezacej list
    //w kazdym kroku przekopiowujemy przekazane elementy na kolejne pozycje biezacej list
    while(begin != end && first != last) {
        *begin = *first;
        
        //inkrementacja iteratorów
        ++begin; ++first;
    }
    
    //jezeli pozostaly elementy w przekazanej liscie (zakresie elmentów)
    if(first != last) {
        insert(end(), first, last);
        //element first wskazuje na 1 nie wstawiony jeszcze element
    } else {
        erase(begin, end);
    }
}
    
/**
 * assign(int, const T&) - metoda umieszcza na liście element podany jako 2 argument
 * liczbę razy wskazaną przez pierwszy argument 
 * Parametry:
 * @int n - liczba powtórzeń umieszczanego na liście elmentu
 * @const T& - stała referencja na wartość umieszczanego elementu
 **/
template<typename T>
void DoublyLinkedList<T>::assign(int n, const T& val)
{
    //pobieramy iterator na pierwszy element listy
    ListIterator<T> itr = begin();
    //przechodzimy aktualną listę elementu, dopuki nie dojdziemy do
    //jej konca lub do momentu wyczerpania sie licznika dodawanych elementów
    //w kazdym kroku przesówamy się na kolejny element i jednoczesnie
    //dekrementujemy liczniki liczby dodawanych elementow
    //w kazdym kroku przypisujemy wartość val do kolejnych wezłów
    for(; itr != end() && n > 0; ++itr, --n) {
        *itr = val;
    }
    
    //jezeli licznik liczby elementów val pozozstałych do dodania
    //nie zeszedł do zera to biezaca lista byla zbyt krotka by pomiescic n elementow
    //dodajemy pozostale elementy metoda insert()
    //jezeli okazalo sie ze dodano juz wszystkie elementy to byc moze zostaly stare
    //elementy za tymi które teraz dodawalismy na liscie wiec wywolujemy erase()
    if( n > 0 )
        insert(end(), n, val);
    else
        erase(itr, end());
}
    
/**
 * assign(std::initializer_list<T> il) - metoda umieszcza na liście począwszy od jej 
 * początku elementy przekazane w postaci initializer_list<T> tj. {1, 2, 5}
 **/
template<typename T>
void DoublyLinkedList<T>::assign(std::initializer_list<T> il) {
    
    //czyścimy aktualną listę
    clear();
    //korzystamy z odpowiedniego przeciążenia insert aby umiescic elementy
    //initializer_list na liscie począwszy od jej pierwszego elementu
    //stąd przekazujemy wskaźnik na czoło elementu
    insert(begin(), il);
}
    
/**
 * element_at() - metoda zwraca wartość elementu znajdującego się na pozycji wskazywanej przez
 * iterator ConstListIterator<T> position
 * @position - iterator wskazujący na element z którego chcemy odczytać wartość
 **/
template<typename T>
T& DoublyLinkedList<T>::element_at(ConstListIterator<T> position)
{
    return const_cast<DoublyLinkedNode<T>*>(position.node)->value;
}
    
/**
 * logDoublyLinkedList() - wypisuje aktualna zawartosc listy
 **/
template<typename T>
void DoublyLinkedList<T>::logDoublyLinkedList(void)
{
    std::fprintf(stdout, "\n{ ");
    for(ConstListIterator<T> it = begin(); it != end(); ++it) {
        std::fprintf(stdout, "%d, ", *it);
    }
    
    std::fprintf(stdout,"}\n\n");
}
    
/**
 * pop_front() - metoda zdejmuje/usuwa pierwszy element listy nie zwracajac jego wartosci!
 **/
template<typename T>
void DoublyLinkedList<T>::pop_front()
{
    if(empty()) printf("'EMPTY'\n");
    //usunięcie pierwszego węzła listy wskazywanego przez iterator begin() metoda erase()
    erase(begin());
}

/**
 * push_back() - metoda umieszcza przekazana wartosc val na ostatniej pozycji listy
 **/
template<typename T>
void DoublyLinkedList<T>::push_back(const T& val)
{
    insert(end(), val);
}

/**
 * push_back() - metoda dziala analogicznie jak powyzsza tylko zamiast kopiowac przenosi
 * wartosc val
 **/
template<typename T>
void DoublyLinkedList<T>::push_back(T&& val)
{
    insert(end(), std::move(val));
}

/**
 * pop_back() - metoda usuwa wezel (wartosc) znajdujaca sie na ostatniej pozycji listy
 * jezeli lista jest pusta drukuje empty, NIE zwraca usuwanej wartosc 
 **/
template<typename T>
void DoublyLinkedList<T>::pop_back()
{
    if(empty()) printf("EMPTY\n");
    //usuniecie ostatniego wezla listy wskazywanego przez iterator end() metoda erase()
    erase(--end());
}
  
/**
 * erase(position) - metoda usuwa węzeł znajdujący się na pozycji wskazywanej przez
 * iterator ConstListIterator<T> position i stąd zmiejsza dlugosc listy o 1
 * @position - iterator wskazujący na pozycję węzła który ma być usunięty
 * return - zwraca iterator wskazujacy na kolejny element lub end()
 **/
template<typename T>
ListIterator<T> DoublyLinkedList<T>::erase(ConstListIterator<T> position)
{
    //zrzutowanie bieżącego węzła, uwolnienie od ograniczenia tylko do odczytu
    //wezel currentNode ma zostaś usuniety
    DoublyLinkedNode<T> *currentNode = const_cast<DoublyLinkedNode<T>*>(position.node);
    
    //utworzenie iteratora na nastepny element który to iterator zostanie zwrócony
    ListIterator<T> nextNodeItr(currentNode->next);
    
    //usuwamy currentNode poprzez odpowiednia manipulacje wskaźnikami!
    //currentNode->previous->next ma wskazywac na currentNode->next
    //i analogidznie currentNode->next->previous na currentNode->previous
    DoublyLinkedNode<T> * const nextNode = currentNode->next;
    DoublyLinkedNode<T> * const previousNode = currentNode->previous;
    previousNode->next = nextNode;
    nextNode->previous = previousNode;
    //dealokowanie currentNode
    delete currentNode;
    count--;
    
    //logLinkedList();
    return nextNodeItr;
}

    
/**
 * erase(first, last) - usuwa węzły począwszy od first aż do
 * elementu (węzła) wskazywanego przez iterator last 
 * Parametry:
 * first - ConstListIterator<T> pozycja od której usuwać elementy
 * last - ConstListIterator<T> pozycja do momentu wystąpienia której należy usuwać 
 * return iterator na last
 **/
template<typename T>
ListIterator<T> DoublyLinkedList<T>::erase(ConstListIterator<T> first,
                                               ConstListIterator<T> last)
{
    //przechodzimy w petli wszystkie elementy poczawszy od first az do last
    while(first != last)
    {
        //usuwajac element first funkcja erase zwraca iterator na element nastepny
        //znajdujacy sie za usunietym na liscie w ten sposo mozemy w petli
        //usuwac elementy az do last
        first = erase(first);
    }
    //zwracamy last czyli iterator na pierwszy element znajdujacy sie za wlasnie
    //usunietymi wezlami
    return last;
}

/**
 * swap() wymienia dane pomiędzy listami w taki sposób że na liście this znajdą się 
 * elementy listy przekazanej jako parametr otherList, natomiast na otherList 
 * zostaną umieszczone elementy z bieżącej list
 **/
template<typename T>
void DoublyLinkedList<T>::swap(DoublyLinkedList<T>& otherList)
{
    DoublyLinkedNode<T>& thisTail = this->tail;
    DoublyLinkedNode<T>& otherTail = otherList.tail;
 
    //if ( thisTail.next != &thisTail )
    if(!empty())
    {
        //jezeli lista this nie jest pusta
        
        if(!otherList.empty())
        //if ( otherTail.next != &otherTail ) //1 PRZYPADEK thisList > 0, otherList > 0
        {
            //jezeli lista otherList nie jest pusta
            // obie listy this oraz otherList nie sa puste
            
            //zamieniamy wskaźniki next i previous pomiedzy oboma listami
            //tj thisList bedzie wskazywac na elementy otherList i na odwórt
            std::swap(thisTail.next, otherTail.next);
            std::swap(thisTail.previous, otherTail.previous);
            //std::swap(tail.next, otherList.tail.next);
            //std::swap(tail.previous, otherList.tail.previous);
           
            //tail.next->previous = &tail;
            //tail.previous->next = &tail;
            //otherList.tail.previous->next = &otherList.tail;
            //otherList.tail.previous->next = &otherList.tail;
            //ustawienie wskaźników przeniesionych list thisList -> otherList
            //i otherList -> thisList tak aby ich elementy pierwszy wskaźnikem previous
            //i ostatni wskaźnikiem next wskazywały na poprawny nowy ogon listy
            thisTail.previous->next = &thisTail;
            thisTail.next->previous = &thisTail;
            otherTail.previous->next = &otherTail;
            otherTail.next->previous = &otherTail;
            
        }
        else //2 PRZYPADEK thisList > 0, otherList == 0
        {
            //jezeli otherList jest pusta
            // lista this nie jest pusta i jednoczesnie otherList jest pusta
            
            //do otherList przypisujemy wskaznik na elementy thisList
            otherTail.next = thisTail.next;
            otherTail.previous = thisTail.previous;
            
            //ustawiamy wskaźniki listy this przenisionej do otherList
            //tak aby dla jej pierwszego elemetnu previous wskazywał na &otherTail
            //natomiast dla ostatniego elementu next wskazywał również na &otherTail
            otherTail.next->previous = &otherTail;
            otherTail.previous->next = &otherTail;
            
            //liste this ustawiamy jako pusta thisTail.next = &thisTail
            thisTail.next = &thisTail;
            thisTail.previous = &thisTail;
        }
    }
    else if(!otherList.empty())
    //else if ( otherTail.next != &otherTail) //3 PRZYPADEK thisList == 0, otherList > 0
    {
        // lista this jest pusta oraz lista otherList nie jest pusta
        //ustawienie wskaźników listy this zeby next wskazywalo na pierwszy element
        //otherList natomiast previous na ostatni element otherList
        thisTail.next = otherTail.next;
        thisTail.previous = otherTail.previous;
        
        //dodatkow naleyży usawić przenisione elemtny skrajne z listy otherList
        //tak aby dla pierwszego elementu previous wskazywał na &thisTail
        //natomiast dla ostatniego elementu next wskazywało również na &thisTail
        thisTail.next->previous = &thisTail;
        thisTail.previous->next = &thisTail;
        
        //ustawienie list otherList jako listy pustej dzie ogon.next wskazuje sam
        //na siebie oraz ogon.previous również sam na siebie
        otherTail.next = &otherTail;
        otherTail.previous = &otherTail;
    }
    
    //4 PRZYPADEK thisList == 0, otherList == 0
    //przypadek gdy obie listy sa puste nie wymaga specjalnego zajmowania sie
    //po i tak obie sa puste wiec swapowanie list nie ma sensu
    
}
    
/**
 * resize(n) - funkcja zwiększa/zmniejsza rozmiar listy zwiększając wstawia 
 * jako domyślne wartości 0 
 **/
template<typename T>
void DoublyLinkedList<T>::resize(int n)
{
    T emptyObject;
    resize(n, emptyObject);
}
/**
 * resize(n, val) - jak wyżej tylko domyślną wstawianą wartością jest val
 **/
template<typename T>
void DoublyLinkedList<T>::resize(int n, const T& val)
{
    ListIterator<T> itr = begin();
    
    int length = 0;
    while( itr != end() && length<n) {
        ++itr;
        ++length;
    }
    
    if(length == n)
        erase(itr, end());
    else
        insert(itr, n-length, val);

}
    
/**
 * clear() - pozostawia liste pusta i o rozmiarze 0 
 **/
template<typename T>
void DoublyLinkedList<T>::clear()
{
   
    //wskaźnik na pierwszy węzeł listy
    DoublyLinkedNode<T> *firstNode = tail.next;
    
    //przechodzimy w pętli do momentu gdy pierwszy element listy nie będzie wskazywał
    //na jej ogon
    while(firstNode != &tail)
    {
        //zachowujemy sobie bieżący węzeł
        DoublyLinkedNode<T> *currentNode = firstNode;
        //przechodzimy firstNode do następnego
        firstNode = firstNode->next;
        //dealokujemy firstNode
        delete currentNode;
    }
    
    //ustawienie wskaźników jak dla liast pustaj gdzie ogon wskazuje
    //oboma wskaźnikami sam na siebie
    tail.next = &tail;
    tail.previous = &tail;
    count = 0;
}
    
/**
 * splice() wstawia na pozycji position elementy z innej listy przenosząc je
 **/

template<typename T>
void DoublyLinkedList<T>::splice(ConstListIterator<T> position, DoublyLinkedList<T>& otherList)
{
    splice(position, std::move(otherList));
}

template<typename T>
void DoublyLinkedList<T>::splice(ConstListIterator<T> position, DoublyLinkedList<T>&& otherList)
{
    if(otherList.empty()) return;
    
    //wielkość przenoszonej listy
    int numberOfMovedElements = otherList.size();
    
    //tworzymy wskaźnik na bieżący węzeł wskazywany przez position, uwalniamy od stałości
    DoublyLinkedNode<T> *currentNode = const_cast<DoublyLinkedNode<T>*>(position.node);
    //currentNode <-position
    
    //iterator na pierwszy węzeł otherList oraz na ostatni węzeł otherList
    ListIterator<T> beginItr = otherList.begin();
    ListIterator<T> endItr = otherList.end();
    
    //wskaźnik na pierwszy węzeł listy
    DoublyLinkedNode<T> *beginNode = beginItr.node;  //beginNode <- otherList
    //wskaźnik na końcowy węzeł listy
    DoublyLinkedNode<T> *endNode = endItr.node; //endNode <-otherList
    
    
    if(currentNode != endNode)
    {
        // Remove [first, last) from its old position.
        //dla ostatniego węzeła otherList ustawiamy wskaźnik next na currentNode
        endNode->previous->next  = currentNode;
        //dla węzła poprzedzającego pierwszy na otherList ustawiamy nex na endNode
        //tj usuwamy z otherList [beginNode, endNode)
        beginNode->previous->next = endNode;
        //element poprzedzający węzeł od którego wstawiamy elementy otherList, ustawiamy
        //mu wskaźnik next na początek otherList (beginNode)
        currentNode->previous->next  = beginNode;
        
        //przetrzymujemy w zmiennej wskaźnik na węzeł poprzedzający
        //pozycje od której wstawiamy otherList to thisList
        DoublyLinkedNode<T> * const keepNode = currentNode->previous;
        //nowym wezlem poprzedzajacym bieżacy (position itr) currentNode
        //bedzie wezel koncowy listy otherList czyli endNode->previous
        currentNode->previous = endNode->previous;
        //manipulacja w starej liscie otherList gdzie po usunieciu elementow otherList
        //elementem poprzedzajacym ogon przenoszonej listy bedzie poprzednij poczatku
        //przenoszonej listy
        endNode->previous  = beginNode->previous;
        //na poprzednika poczatkowego wezla nowej listy ustawiamy przetrzymany wezel
        //poprzednika currentNode czyli wezla wskazywanego przez iterator position
        //okrelajacego wezel przed który nalezalo przeniesc liste otherList
        beginNode->previous = keepNode;
    }
    
    //zwiekszenie licznika bieżącej listy o wielkość przenoszonej listy
    count += numberOfMovedElements;
    otherList -= numberOfMovedElements;
    
}
    
/**
  * splice() wstawia na pozycji position dokładnie jeden elementy z innej listy
  * przenosząc go ze wskazanego miejsca w otherList na wskazane miejsce
  * w bieżącej liście przed aktualny wezel wskazywany iteratorem position
  **/
template<typename T>
void DoublyLinkedList<T>::splice(ConstListIterator<T> position, DoublyLinkedList<T>& otherList, ConstListIterator<T> i)
{
    splice(position, std::move(otherList), i);
}

template<typename T>
void DoublyLinkedList<T>::splice(ConstListIterator<T> position, DoublyLinkedList<T>&& otherList, ConstListIterator<T> i)
{
    ConstListIterator<T> j = i;
    ++j;
    if( position == i || position == j) return;
    splice(position, std::move(otherList), i, j);
}

/**
 * splice() - jak wyżej tylko przenosi elementy z zakresu [first,last)
 **/
template<typename T>
void DoublyLinkedList<T>::splice(ConstListIterator<T> position, DoublyLinkedList<T>& otherList, ConstListIterator<T> first, ConstListIterator<T> last)
{
    splice(position, std::move(otherList), first, last);
}
template<typename T>
void DoublyLinkedList<T>::splice(ConstListIterator<T> position, DoublyLinkedList<T>&& otherList, ConstListIterator<T> first, ConstListIterator<T> last)
{
    if(first == last) return;
    
    int numberOfMovedElements = 1; //std::distance(first, last);
    
    //tworzymy wskaźnik na bieżący węzeł wskazywany przez position, uwalniamy od stałości
    DoublyLinkedNode<T> *currentNode = const_cast<DoublyLinkedNode<T>*>(position.node);
    //currentNode <-position
    
    
    //wskaźnik na pierwszy węzeł listy
    DoublyLinkedNode<T> *beginNode = const_cast<DoublyLinkedNode<T> *>(first.node);  //beginNode <- otherList
    //wskaźnik na końcowy węzeł listy
    DoublyLinkedNode<T> *endNode = const_cast<DoublyLinkedNode<T> *>(last.node); //endNode <-otherList
    
    
    if(currentNode != endNode)
    {
        // Remove [first, last) from its old position.
        //dla ostatniego węzeła otherList ustawiamy wskaźnik next na currentNode
        endNode->previous->next  = currentNode;
        //dla węzła poprzedzającego pierwszy na otherList ustawiamy nex na endNode
        //tj usuwamy z otherList [beginNode, endNode)
        beginNode->previous->next = endNode;
        //element poprzedzający węzeł od którego wstawiamy elementy otherList, ustawiamy
        //mu wskaźnik next na początek otherList (beginNode)
        currentNode->previous->next  = beginNode;
        
        //przetrzymujemy w zmiennej wskaźnik na węzeł poprzedzający
        //pozycje od której wstawiamy otherList to thisList
        DoublyLinkedNode<T> * const keepNode = currentNode->previous;
        //nowym wezlem poprzedzajacym bieżacy (position itr) currentNode
        //bedzie wezel koncowy listy otherList czyli endNode->previous
        currentNode->previous = endNode->previous;
        //manipulacja w starej liscie otherList gdzie po usunieciu elementow otherList
        //elementem poprzedzajacym ogon przenoszonej listy bedzie poprzednij poczatku
        //przenoszonej listy
        endNode->previous  = beginNode->previous;
        //na poprzednika poczatkowego wezla nowej listy ustawiamy przetrzymany wezel
        //poprzednika currentNode czyli wezla wskazywanego przez iterator position
        //okrelajacego wezel przed który nalezalo przeniesc liste otherList
        beginNode->previous = keepNode;
    }

    count += numberOfMovedElements;
    otherList.count -= numberOfMovedElements;
}

/**
 * remove() usuwa z listy elementy (węzly) zawierające wartość podaną jako 
 * argument
 **/
template<typename T>
void DoublyLinkedList<T>::remove(const T& val)
{
    //przechodzimy iteratorem itr po calej liscie
    for(ListIterator<T> itr = begin(), nextElementItr = itr; itr != end(); itr = nextElementItr)
    {
        //ustawiamy sobie iterator na nastepny element
        nextElementItr++;
        
        if(*itr == val)
        {
            //jezeli biezacy iterator wskazuje na element zawierajacy wartosc
            //to usuwamy wezel wskazyany tym iteratorem z listy
            erase(itr);
        }
        
    }
}

/**
 * remove_if() usuwa element z listy jezeli spełnia on predykat 
 **/
template<typename T>
template<class Predicate>
void DoublyLinkedList<T>::remove_if(Predicate pred)
{
    //przechodzimy iteratorem itr po calej liscie
    for(ListIterator<T> itr = begin(), nextElementItr = itr; itr != end(); itr = nextElementItr)
    {
        //ustawiamy sobie iterator na nastepny element
        nextElementItr++;
        
        if(pred(*itr))
        {
            //jezeli biezacy iterator wskazuje na element spełniający predykat
            //to usuwamy wezel wskazyany tym iteratorem z listy
            erase(itr);
        }
        
    }
}

/**
 * unique() - metoda usuwa z listy elementy które są sobie równe czyli spelniaja
 * predykat std::equal_to<T>() przy czym aby metoda poprawnie usuneła wszystkie duplikaty
 * lista musi zostać uprzednio posortowana
 **/
template<typename T>
void DoublyLinkedList<T>::unique()
{
    unique(std::equal_to<T>());
}

/**
 * unique() - metoda usuwajaca z posortowanej listy elementy spełniajace predykat
 **/
template<typename T>
template<class BinaryPredicate>
void DoublyLinkedList<T>::unique(BinaryPredicate binary_pred)
{
    //gdy lista pusta
    if(empty()) return;
    
    //przechodzimy liste dwoma iteratorami itr (zaczynajac od 1 elementu) i
    //nextItr (wskazujacym na kolejny element po itr
    //aż do momentu gdy dojdziemy nextItr do konca
    //UWAGA! iteratorowi po przejsciu kazdego korku przypisujemy ostatni nextItr
    // natomiast nextItr inkrementujemy tuż przed sprawdzeniem warunku
    for(ListIterator<T> itr = begin(), nextItr = itr; ++nextItr != end(); nextItr = itr)
    {
        if(binary_pred(*itr, *nextItr))
        {
            //elementy spełniają predykat np sa duplikatami
            //usuwamy element wskazywany nextItr
            erase(nextItr);
            
        } else {
            //tylko gdy nie usuwano elementu przechodzimy iteratorem do nastepnego
            //gdy natomiast usuwano element lista sie skruci i nalezy pozostac itr w
            //w tym samym miejscu
            itr = nextItr;
        }
    }
    
}

/**
 * reverse() - metoda odwraca kolejnosc elementów na liscie  czyli dla kazdego 
 * węzła wskaźnik previous wskazuje na to na co wskazywał next, a next na to na 
 * wskazywał previous
 **/
template<typename T>
void DoublyLinkedList<T>::reverse()
{
    DoublyLinkedNode<T>* currentNode = &tail;
    do
    {
        std::swap(currentNode->next, currentNode->previous);
        currentNode = currentNode->previous;
        // przechodzimy do nastepnego wezla dawne next jest teraz pod previous
    }
    while (currentNode != &tail);
}

/**
 * merge(otherList) scale elementy dwóch list w kolejności rosnącej 
 * przed wstawianiem elementów obie listy poiwnny być posortowane aby 
 * otrzymać odpowiednio scaloną listę
 **/
template<typename T>
void DoublyLinkedList<T>::merge(DoublyLinkedList<T>& otherList)
{
     merge(std::move(otherList));
}
 
/**
 * merge(otherList) - jak wyzej tyle, że przenosi zamiast
 * przekopiowywac podliste
 **/
template<typename T>
void DoublyLinkedList<T>::merge(DoublyLinkedList<T>&& otherList)
{
    merge(std::move(otherList), std::less<T>());
}
  
/**
 * merge(otherList comp) - dodatkowo jako drugi argument pobiera obiekt funkcyny
 **/
template<typename T>
template<class Compare>
void DoublyLinkedList<T>::merge(DoublyLinkedList<T>& otherList, Compare comp)
{
    merge(std::move(otherList), comp);
}

/**
 * merge(otherList, compare) scale elementy dwóch list w kolejności ustalonej comparatorem
 * przed wstawianiem elementów obie listy poiwnny być posortowane comparatorem aby
 * otrzymać odpowiednio scaloną listę
 **/
template<typename T>
template<class Compare>
void DoublyLinkedList<T>::merge(DoublyLinkedList<T>&& otherList, Compare comp)
{
        //otherList wskazuję na tą samą listę co this
        if(this == &otherList) return;
        
        //tworzymy iteratory na początek i koniec listy this oraz otherList
        ListIterator<T> thisFirstItr = begin();
        ListIterator<T> thisLastItr = end();
        ListIterator<T> otherFirstItr = otherList.begin();
        ListIterator<T> otherLastItr = otherList.end();
        //przechodzimy w pętli po wszystkich elementach listy this i other
        //aż przejdziemy ich wszystkie elementy
        while(thisFirstItr != thisLastItr && otherFirstItr != otherLastItr) {
            //w pętli musimy odpowiednio scalić elementy obu list by znajdowaly sie na
            //odpowiadajacych pozycjach
            if(comp(*otherFirstItr,*thisFirstItr))
            {
                //jeżeli kolejny element na liscie other jest mniejszy
                //od elementu na liscie this
                ListIterator<T> nextItr = otherFirstItr;
                transferNode(thisFirstItr, otherFirstItr, ++nextItr);
                //możemy przejść iteratorem listy other do nastpenego elementu
                otherFirstItr = nextItr;
                count++;
                otherList.count -= 1;
            } else {
                ++thisFirstItr;
            }
        }
        
        if(otherFirstItr != otherLastItr)
        transferNode(thisLastItr, otherFirstItr, otherLastItr);
        
}
    
template<typename T>
void DoublyLinkedList<T>::transferNode(ListIterator<T>& thisFirstItr,
                                       ListIterator<T>& otherFirstItr,
                                       ListIterator<T>& otherLastItr) {
        //tworzymy wskaźnik na węzeł bieżacej list wskazujący na miejsce
        //na które należy wstawić mergowny element z other list
        DoublyLinkedNode<T> *thisCurrentNode =
                    const_cast<DoublyLinkedNode<T>*>(thisFirstItr.node);
        //wskaźnik na pierwszy węzeł wstawiany z listy other list
        DoublyLinkedNode<T> *otherBeginNode = otherFirstItr.node;
        //wskaźnik na końcowy węzeł wstawiany z listy other list
        DoublyLinkedNode<T> *otherEndNode = otherLastItr.node;
        
        if(thisCurrentNode != otherEndNode)
        {
            // Remove [first, last) from its old position.
            //dla ostatniego węzeła otherList ustawiamy wskaźnik next na currentNode
            otherEndNode->previous->next  = thisCurrentNode;
            //dla węzła poprzedzającego pierwszy na otherList ustawiamy nex na endNode
            //tj usuwamy z otherList [beginNode, endNode)
            otherBeginNode->previous->next = otherEndNode;
            //element poprzedzający węzeł od którego wstawiamy elementy otherList, ustawiamy
            //mu wskaźnik next na początek otherList (beginNode)
            thisCurrentNode->previous->next  = otherBeginNode;
            
            //przetrzymujemy w zmiennej wskaźnik na węzeł poprzedzający
            //pozycje od której wstawiamy otherList to thisList
            DoublyLinkedNode<T> * const keepNode = thisCurrentNode->previous;
            //nowym wezlem poprzedzajacym bieżacy (position itr) currentNode
            //bedzie wezel koncowy listy otherList czyli endNode->previous
            thisCurrentNode->previous = otherEndNode->previous;
            //manipulacja w starej liscie otherList gdzie po usunieciu elementow otherList
            //elementem poprzedzajacym ogon przenoszonej listy bedzie poprzednij poczatku
            //przenoszonej listy
            otherEndNode->previous  = otherBeginNode->previous;
            //na poprzednika poczatkowego wezla nowej listy ustawiamy przetrzymany wezel
            //poprzednika currentNode czyli wezla wskazywanego przez iterator position
            //okrelajacego wezel przed który nalezalo przeniesc liste otherList
            otherBeginNode->previous = keepNode;
        }
}

/**
 * sort() - metoda sortuje liste stosując operator funkcyjny std::less<T>
 **/
template<typename T>
void DoublyLinkedList<T>::sort()
{
    //wywołujemy przeciążoną wersję sort() przekazując Comparator std::less<T>()
    sort(std::less<T>());
}
    
/**
 * sort(Compare) - metoda sortująca opierająca sortowanie listy na zadanym
 * obiekt funkcyjnym.
 * Metoda sortowania:
 * 1. Tworzymy tablice list DoublyLinkedList (zawiera 64 pozycje).
 * 2. Usuwamy pierwszy element z bieżącej listy do posortowania (this)
 * 3. Scalamy go z listą w pierwszym wiaderku (i=0th), pierwszej pozycji tablicy list
 * 4. Jeżeli przed scaleniem ith wiaderko nie jest puste, scalić ith wiaderko z wiaderkiem i+1th.
 * 5. Powtarzamy krok 3 aż do momentu scalenia z pustym wiaderkiem
 * 6. Powtarzamy kroki 2 i 3 aż do momentu zdjęcia wszystkich elementów z sortowanej listy.
 * 7. Scalamy ze sobą wszystki nie pusta wiaderka znajdujące się w tablicy list zaczynająć
 * od pozycji zero aż do pozycji fill
 * 8. Wymieniamy bieżącą listę (swap()) która jest teraz pusta z scalona posortowana 
 *    list na pozycji (fill-1) tablicy list wykorzystywanej w sortowaniu
 **/
template<typename T>
template<class Compare>
void DoublyLinkedList<T>::sort(Compare comp)
{
  //nie potrzebujemy sortować listy jeżeli zawiera nie wiecej elementów niż 1
   if(tail.next == &tail) return; //lista jest pusta
   if(tail.next->next == &tail) return; //lista zawiera tylko jeden element
    
    DoublyLinkedList<T> carry;
    //Tworzymy tablice list DoublyLinkedList (zawiera 64 pozycje).
    DoublyLinkedList<T> tmp[64];
    DoublyLinkedList<T> *fill = &tmp[0];
    DoublyLinkedList<T> *counter;
    
    //wiemy że bieżąca lista nie jest pusta sprawdzaliśmy do na pocza†ku metody
    //w przeciwnym razie wracaliśmy z pętli
    //pętle następnie wykonujemy aż do momentu gdy bieżąca lista nie stanie się pusta
    int i= 0;
    do
    {
        //wstawia na pozycji carry.begin() dokładnie jedengo elementy z bieżącej listy
        //*this przenosząc go z początku bieżącej listy begin() na wskazane miejsce
        //w liscie carry -> carry.begin()
       carry.splice(carry.begin(), *this, begin());
       //carry.logDoublyLinkedList();
       
       //counter inicjalizujemy wskaźnikiem na listę umieszczoną w pierwszym
       //wiaderku (pozycji tablicy tymczasowej 64 list podwójnie powiązanych)
       for(counter = &tmp[0];
           counter != fill && !counter->empty();
           ++counter)
        //przechodzimy w pętli przez tablicę list wskaźnikiem counter
        //aż do napotkania fill lub gdy lista tablicy będzie pusta
        {
            //scalamy liste na którą wskazuje counter z lista carry
            //używając comparatora comp
            counter->merge(carry, comp);
            //zamieniamy ze sobą listy carry i counter
            carry.swap(*counter);
            
        }
        //czyli aż do momentu scalenia z pustą listą
        carry.swap(*counter);
        
        /*logging
        fprintf(stderr, "Obieg petli: %d\n", ++i);
        fprintf(stderr, "Counter LinkedList: \n");
        counter->logDoublyLinkedList();
        fprintf(stderr, "Carry LinkedList: \n");
        carry.logDoublyLinkedList();
         */
        
        //jezeli counter oraz fill wskazuja na ten sam element tablic list
        if(counter == fill)
            ++fill; //to przesuwamy wskaźnik fill na kolejny element tablicy list
        
    }
    while(!empty());

    for(counter = &tmp[1]; counter != fill; ++counter)
        counter->merge(*(counter - 1), comp);
    
    swap(*(fill - 1));
        
}
    


} // koniec namespace ADT

//test LinkedList<T>
using namespace std;

// a predicate implemented as a function:
bool single_digit (const int& value) { return (value<10); }

int main(int argc, char *argv[])
{
    cout << endl;
    cout << endl;
    cout << "TESTOWY PROGRAM LinkedList<T>" << endl;
    cout << endl;
    cout << "Tworzenie listy..." << endl;
    ADT::DoublyLinkedList<int> list;
    
    cout << "push_front 5, 26, 33" << endl;
    list.push_front(5);
    list.push_front(26);
    list.push_front(33);
    cout << "--------------------------------------" << endl;
    
    cout << "push_back 5, 26, 33" << endl;
    list.push_back(5);
    list.push_back(26);
    list.push_back(33);
    cout << "--------------------------------------" << endl;
    
    ADT::ConstListIterator<int> itr = list.begin();
    ADT::ReverseConstListIterator<int> reverse_itr = list.rbegin();
    cout << endl;
    cout << "front()" << endl;
    cout << "pierwszy element listy: " << list.front() << endl;
    cout << "--------------------------------------" << endl;
    
    cout << "back()" << endl;
    cout << "ostatni element listy: " << list.back() << endl;
    cout << "--------------------------------------" << endl;
    
    cout << endl;
    cout << "*(++itr)" << endl;
    cout << "drugi element (indirection operator): " << *(++itr) << endl;
    cout << "--------------------------------------" << endl;
    
    cout << endl;
    cout << "*(++reverse_itr)" << endl;
    cout << "drugi od konca element (indirection operator): " << *(++reverse_itr) << endl;
    cout << "--------------------------------------" << endl;

    
    cout << endl;
    cout << "list.element_at(++itr)" << endl;
    cout << "trzeci element listy to: " << list.element_at(++itr) << endl;
    cout << "--------------------------------------" << endl;

    
    cout << endl;
    cout << "insert(begin(), 10)" << endl;
    ADT::ConstListIterator<int> const_itr = list.begin();
    list.insert(const_itr, 10);
    list.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "inser(begin(), 20)" << endl;
    const_itr = list.begin();
    list.insert(const_itr, 20);
    list.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "3 x pop_front()" << endl;
    list.pop_front();
    list.pop_front();
    list.pop_front();
    list.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "3 x pop_back()" << endl;
    list.pop_back();
    list.pop_back();
    list.pop_back();
    list.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "front()" << endl;
    cout << "pierwszy element listy: " << list.front() << endl;
    list.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "back()" << endl;
    cout << "ostatni element listy: " << list.back() << endl;
    list.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "empty()" << endl;
    if(list.empty())
        cout << "Lista jest pusta" <<endl;
    else
        cout << "Lista nie jest pusta"<<endl;
    cout << "--------------------------------------" << endl;
    
    cout << "Tworzenie list2...." << endl;
    ADT::DoublyLinkedList<int> list2;
    
    cout << "list2.push_front 11, 21, 31" << endl;
    list2.push_front(11);
    list2.push_front(21);
    list2.push_front(31);
    list2.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "list2.push_back 11, 21, 31" << endl;
    list2.push_back(11);
    list2.push_back(21);
    list2.push_back(31);
    list2.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;

    
    cout << "list.push_front 1, 2, 3" << endl;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    list.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "list.push_back 1, 2, 3" << endl;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "list2.splice_after(begin(), list, list.begin(), itr)" << endl;
    auto it = list.begin();
    it++; it++;
    list2.splice(list2.begin(), list, list.begin(), it);
    list.logDoublyLinkedList();
    list2.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "list2.remove(21)" << endl;
    list2.remove(21);
    list2.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "list2.remove_if(single_digit)" << endl;
    list2.remove_if(single_digit);
    list2.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "list2.push_front 3,3,3,1" << endl;
    list2.push_front(3);
    list2.push_front(3);
    list2.push_front(3);
    list2.push_front(1);
    list2.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "list2.unique()" << endl;
    list2.unique();
    list2.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "list2.reverse()" << endl;
    list2.reverse();
    list2.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    ADT::DoublyLinkedList<int> list3;
    list3.push_front(2);
    list3.push_front(4);
    ADT::DoublyLinkedList<int> list4;
    list4.push_front(3);
    list4.push_front(5);
    
    cout << "list4.swap(list3)" << endl;
    cout << "przed swap()" << endl;
    list3.logDoublyLinkedList();
    list4.logDoublyLinkedList();
    list4.swap(list3);
    cout << "po swap()" << endl;
    list3.logDoublyLinkedList();
    list4.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
  
    cout << "list2.sort()" << endl;
    list2.sort();
    list2.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
   
    cout << "list.sort(std::greater<double>()) & list2.sort(std::greater<double>())" << endl;
    list.sort(std::greater<double>());
    list2.sort(std::greater<double>());
    list.logDoublyLinkedList();
    list2.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    cout << "list.merge(list2, greater<double>())" << endl;
    list.merge(list2, std::greater<double>());
    list.logDoublyLinkedList();
    cout << "--------------------------------------" << endl;
    
    return 0;
}

