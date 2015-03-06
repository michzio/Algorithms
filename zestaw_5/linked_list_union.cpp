#include "linked_list.h"
#include<iostream>
#include<cstdio>
#include<utility>
#include<functional>

//implementacja LinkedList

namespace ADT {

/**
 * konstruktor domyślny typu LinkedList<T>
 */
template<typename T>
LinkedList<T>::LinkedList() : count(0) { }

/**
 * destruktor 
 */
template<typename T>
LinkedList<T>::~LinkedList()
{
    //usunięcie wszystkich węzłów listy - do zaimplementowania
}
    
/**
 * before_begin() - metoda zwraca iterator listy wskazujący na czoło (head) listy, tuż
 * przed pierwszym elementem listy. Można go użyć np. aby za pomocą metody 
 * insert_after() wstawić coś na pierwszą pozycję listy.
 * Ta wersja funkcji zwraca ListIterator<T> do odczytu/zapisu.
 **/
template<typename T>
ListIterator<T> LinkedList<T>::before_begin()
{
    return ListIterator<T>(&head);
}

/**
 * before_begin() const - metoda zwraca iterator ConstListIterator<T> tylko do odczytu
 * który wskazuje na czoło listy tuż przed pierwszym elementem
 **/
template<typename T>
ConstListIterator<T> LinkedList<T>::before_begin() const
{
    return ConstListIterator<T>(&head);
}
    
/**
 * cbefore_begin() - metoda zwraca iterator wskazujący na czoło (head) listy, tuż przed 
 * pierwszym elementem listy, tzn. że dla wskazywanego przez ten iterator elementu 
 * ->next będzie wskazywać na pierwszy element na liście. Dodatkowo zwracyany jest 
 * stały iterator ConstListIterator<T>
 **/
template<typename T>
ConstListIterator<T> LinkedList<T>::cbefore_begin() const
{
    //do konstruktora stałego iteratora listy przekazujemy adres na czoło listy
    return ConstListIterator<T>( &head );
}

/**
 * begin() - zwraca iterator do odczytu/zapisu, który wskazuje na pierwszy
 * element listy.
 */
template<typename T>
ListIterator<T> LinkedList<T>::begin()
{
    return ListIterator<T>(head.next);
}

/**
 * begin() const - zwraca iterator tylko do odczytu (constant), który wskazuje 
 * na pierwszy element listy 
 */
template<typename T>
ConstListIterator<T> LinkedList<T>::begin() const
{
    return ConstListIterator<T>(head.next);
}

/**
 * end() - zwraca iterator do odczytu/zapisu, który wskazuje na element
 * tuż za ostanim elementem listy.
 */
template<typename T>
ListIterator<T> LinkedList<T>::end()
{
    return ListIterator<T>(0);
}

/**
 * end() const - zwraca iterator tylko do odczytu (constant), który
 * wskazuje na element tuż za ostatnim elementem listy. 
 */
template<typename T>
ConstListIterator<T> LinkedList<T>::end() const
{
    return ConstListIterator<T>(0);
}
    
/**
 * cbegin() - metoda która zwraca stały iterator ConstListIterator<T> (tylko do odczytu)
 * wskazujący na pierwszy element na liście.
 **/
template<typename T>
ConstListIterator<T> LinkedList<T>::cbegin() const
{
    return ConstListIterator<T>(head.next);
}

/**
 * cend() - metoda która zwraca stały iterator ConstListIteratr<T> (tylko do odczytu)
 * wskazujący na element tuż za ostatnim elementem listy
 **/
template<typename T>
ConstListIterator<T> LinkedList<T>::cend() const
{
    return ConstListIterator<T>(0);
}

/**
 * insert_after() - metoda dodaje węzeł z wartością przekazaną jako drugi parametr val
 * za węzłem wskazywanym przez pierwszy parametr position (ConstListIterator<T>)
 * oraz zwiększa zmienną count
 **/
template<typename T>
ListIterator<T> LinkedList<T>::insert_after(ConstListIterator<T> position, const T& val)
{
    Node<T> *currentNode = const_cast<Node<T>*>(position.node);
    if(currentNode) {
        Node<T> *newNode = new Node<T>(std::move(val));
        newNode->next = currentNode->next;
        currentNode->next = newNode;
        count++;
        
        //logLinkedList();
        return ListIterator<T>(currentNode->next);
    }
    
    return NULL;
}
    
/**
 * insert_after() - metoda działająca analogicznie jak powyższa insert_after z tą 
 * różnicą, że przekazywany parametr wartości (val) jest przez przeniesienie std::move()
 **/
template<typename T>
ListIterator<T> LinkedList<T>::insert_after(ConstListIterator<T> position, T&& val)
{
    Node<T> *currentNode = const_cast<Node<T>*>(position.node);
    if(currentNode) {
        Node<T> *newNode = new Node<T>(val);
        newNode->next = currentNode->next;
        currentNode->next = newNode;
        count++;
        
        //logLinkedList();
        return ListIterator<T>(currentNode->next);
    }
    
    return NULL;
}

/**
 * insert_after() - wstawia począwszy od pozycji występującej po tej wskazanej
 * iteratorem position zadaną liczbę (parametr int n) kopi wartości (elementów) 
 * określonych przez 3 parametr
 * Parametry:
 * @position - iterator ConstListIterator<T> wskazujący na pozycje po której wstawiać
 * @n - int określający ilość kopi wstawianej wartości
 * @val - wstawiana wartość jako stała referencja typu const T&
 * return iterator wskazujący na ostatnią wstawioną kopie danych
 **/
template<typename T>
ListIterator<T> LinkedList<T>::insert_after(ConstListIterator<T> position, int n, const T& val)
{
    if(n) { //jezeli liczba wstawianych kopi wieksza od >1
        //tworzymy tymczasowa liste LinkedList<T> za pomoca przeciazonego konstruktora
        //z zadana liczba kopii elementu val (n kopii)
        LinkedList<T> tmpList(n, val);
        //wstawiamy nowa liste po wskazanej pozycji za pomoca metody
        //splice_after(position, std:move(tmpList)) - tj pszenosimy utworzona liste
        return splice_after(position, std::move(tmpList));
        
    } else {
        //jezeli przekazano jako liczbe wstawianych kopi 0 to zwracamy iterator na
        //biezaca pozycje position
        return ListIterator<T>(const_cast<Node<T>*>(position.node));
    }
}
    
/**
 * insert_after() - metoda ma zazadanie przekopiowac elementy poczynając od 
 * wskazywaneych przez piewszy iterator InputIterator first, a kończąc na
 * ostatnim iteratorze InputIterator last  do bieżącej list wstawiając je na liście
 * za pozycją wskazywaną przez iterator ConstListIterator<T> position przekazany jako
 * pierwszy argument do metody.
 * Parametry:
 * @position - wskaźnik na pozycje po której metoda ma wstawiac elementy 
 * @first - InputIterator poczatek listy skąd maja być przekopiowywane dane
 * @last - InputIterator wskazujacy do którego węzła ma postępować przekopiowywanie
 * return - iterator ListIterator<T> wskazujący na ostatni wstawiony element
 **/
template<typename T>
template<class InputIterator>
ListIterator<T> LinkedList<T>::insert_after(ConstListIterator<T> position, InputIterator first,
                             InputIterator last)
{
    //wykorzystujemy przeciążony konstruktor to utworzenia najpierw tymczasowej
    //listy zawierajacej przekopiowane elementy of [first, last)
    LinkedList<T> tmpList(first, last);
    if(!tmpList.empty()) {
        //jezeli lista tymczasowa nie jest pusta to wstawiamy ja metoda splice_after()
        return splice_after(position, std::move(tmpList));
    } else {
        return ListIterator<T>(const_cast<Node<T>*>(position.node));
    }
}
/**
 * insert_after() - wstawia wartości na liste za wskazana przez iterator position
 * pozycje, wartości są pobierane z listy inicjalizującej std::initializer_list<T>
 * Parametry:
 * @position - iterator ConstListIterator<T> wskazujący pozycje po której wstawiać 
 * @il - std::initializer_list<T> lista inicjalizująca z wartościami do wstawienia
 * return - iterator ListIterator<T> na ostatni wstawiony element 
 **/
template<typename T>
ListIterator<T> LinkedList<T>::insert_after(ConstListIterator<T> position,
                                                std::initializer_list<T> il)
{
    if(il.size()) {
        //jezeli rozmiar listy inicjalizujacej wiekszy od zera
        //tworzymy nowa liste LinkedList<T> używając przeciążonego konstruktora
        LinkedList<T> tmpList(il);
        //wstawiamy przenosząc tymczasową listę do bieżącej za pozycje wskazana
        //iteratorem position
        return splice_after(position, std::move(tmpList));

    } else {
        //jeżeli lista inicjalizujaca jest pusta to zwracamy iterator na biezacy element
        return ListIterator<T>(const_cast<Node<T>*>(position.node));
    }
}
    
/**
 * push_front() - wstawia przekazana wartość (referencja tylko do odczytu const T&) 
 * do węzła na początku listy poprzedzającego wszystkie inne węzły na liście
 * Wykorzystuje metodę insert_after() przekazując jako iterator ConstListIterator<T>
 * wskazujący na czoło listy, czyli węzeł poprzedzajacy pierwszy węzeł listy, po którym 
 * zostanie wstawiony nowo tworzony węzeł z wartością value (drugi parametr) w efekcie
 * więc nowo utworzony węzeł stanie się 1 węzłem listy na który będzie teraz wskazywać 
 * składowa LinkedList<T>.head
 * @ val - parametr typu const T& (referencja tylko do odczytu) która jest wartoscia jaka 
 *          zostanie wstawiona w pierwszym węźle listy LinkedList<T>
 **/
template<typename T>
void LinkedList<T>::push_front(const T& val)
{
    insert_after(cbefore_begin(), val);
}
    
/**
 * push_front() - analogicznie jak wyżej tyle, że parametr jest przenoszony,
 * a nie kopiowany
 **/
template<typename T>
void LinkedList<T>::push_front(T&& val)
{
    insert_after(cbefore_begin(), std::move(val));
}
 
/**
 * push_back() place element on the end of linked_list
 **/
template<typename T>
void LinkedList<T>::push_back(const T& val)
{
    ConstListIterator<T> itr = cbefore_begin();
    ConstListIterator<T> befor_itr = itr;
    
    while(itr != end()) {
        befor_itr = itr;
        itr++;
    }
    
    insert_after(befor_itr, val);
}
    
/**
 * front() - metoda odczytuje wartość elementu znajdującego się na pierwszej pozycji listy
 */
template<typename T>
T& LinkedList<T>::front(void)
{
    if(head.next == NULL) printf("'EMPTY'\n'");
    return head.next->value;
}
   
/**
 * const T& front() const - metoda zwracająca stałą referencję na pierwszą wartość na 
 * liście
 **/
template<typename T>
const T& LinkedList<T>::front(void) const
{
    if(head.next == NULL) printf("'EMPTY'\n");
    return head.next->value;
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
void LinkedList<T>::assign(InputIterator first, InputIterator last)
{
    //czyścimy aktualną listę
    clear();
    //przekopiowujemy elementy korzystając z przeciążonej wersji metody insert_after
    //elementy umieszczamy poczynając od wskaźnika na pierwszy element
    //stąd przekazujemy iterator na czoło listy
    insert_after(cbefore_begin(), first, last);
}
    
/**
 * assign(int, const T&) - metoda umieszcza na liście element podany jako 2 argument
 * liczbę razy wskazaną przez pierwszy argument 
 * Parametry:
 * @int n - liczba powtórzeń umieszczanego na liście elmentu
 * @const T& - stała referencja na wartość umieszczanego elementu
 **/
template<typename T>
void LinkedList<T>::assign(int n, const T& val)
{
    //czyścimy aktualną listę
    clear();
    //umieszczamy począwszy od pierwszego elementu n kopi wartości val
    insert_after(cbefore_begin(), n, val);
}
    
/**
 * assign(std::initializer_list<T> il) - metoda umieszcza na liście począwszy od jej 
 * początk elementy przekazane w postaci initializer_list<T> tj. {1, 2, 5}
 **/
template<typename T>
void LinkedList<T>::assign(std::initializer_list<T> il) {
    
    //czyścimy aktualną listę
    clear();
    //korzystamy z odpowiedniego przeciążenia insert_after aby umiescic elementy
    //initializer_list na liscie począwszy od jej pierwszego elementu
    //stąd przekazujemy wskaźnik na czoło elementu
    insert_after(cbefore_begin(), il);
}
    
/**
 * element_at() - metoda zwraca wartość elementu znajdującego się na pozycji wskazywanej przez
 * iterator ConstListIterator<T> position
 * @position - iterator wskazujący na element z którego chcemy odczytać wartość
 **/
template<typename T>
T& LinkedList<T>::element_at(ConstListIterator<T> position)
{
    return const_cast<Node<T>*>(position.node)->value;
}
    
/**
 * logLinkedList() - wypisuje aktualna zawartosc listy 
 **/
template<typename T>
void LinkedList<T>::logLinkedList(void)
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
void LinkedList<T>::pop_front()
{
    if(head.next == NULL) printf("'EMPTY'\n");
    //usunięcie pierwszego węzła listy znajdującego się zaraz za czołem
    erase_after(ConstListIterator<T>(&head));
    
}
  
/**
 * erase_after(position) - metoda usuwa węzeł znajdujący się na pozycji występującej po 
 * węźle na który wskazuje iterator ConstListIterator<T> position
 * @position - iterator wskazujący na pozycję węzła którego nastepnik ma być usunięty
 **/
template<typename T>
ListIterator<T> LinkedList<T>::erase_after(ConstListIterator<T> position)
{
    //zrzutowanie bieżącego węzła, uwolnienie od ograniczenia tylko do odczytu
    Node<T> *currentNode = const_cast<Node<T>*>(position.node);
    
    if(currentNode->next == NULL) return NULL;
    //utworzenie wskaźnika na obiekt węzła usuwanego
    Node<T> *erasingNode = currentNode->next;
    //powiązanie wskaźnika na następny węzełw w bieżącym węźle z następnikiem usuwanego węzła
    currentNode->next = erasingNode->next;
    //zwolnienie pamięci zajmowanej przez obiekt usuwanego węzła
    delete erasingNode;
    count--;
    
    //logLinkedList();
    
    return ListIterator<T>(currentNode->next);
}

    
/**
 * erase_after(position, last) - usuwa węzły począwszy od następnika position aż to 
 * elementu (węzła) wskazywanego przez iterator last 
 * Parametry:
 * position - ConstListIterator<T> pozycja za którą usuwać elementy
 * last - ConstListIterator<T> pozycja do momentu wystąpienia której należy usuwać 
 * return iterator na last
 **/
template<typename T>
ListIterator<T> LinkedList<T>::erase_after(ConstListIterator<T> position,
                                               ConstListIterator<T> last)
{
    //zrzutowanie bieżącego węzła, uwolnienie od ograniczenia tylko do odczytu
    Node<T> *currentNode = const_cast<Node<T>*>(position.node);
    if(currentNode->next == NULL) return NULL;
    
    //utworzenie wskaźnika na obiekt węzła od którego rozpoczynamy proces usuwania
    Node<T> *erasingNode = currentNode->next;
    //wskaźnik na ostatni węzeł w ciągu usuwanych węzłów
    Node<T> *lastNode = const_cast<Node<T>*>(last.node);
    //pętla po usuwanych węzłach aż do ostatniego w ciągu usuwanych węzłów
    while(erasingNode != lastNode)
    {
        //przechowanie aktualnie usuwanego węzła w zmiennej tymczasowej
        Node<T> *tmpNode = erasingNode;
        //postępujemy naprzód z usuwanym węzłem
        erasingNode = erasingNode->next;
        //dealokujemy aktualnie usuwany węzeł wskazywany jeszcze przez tmpNode
        delete tmpNode;
        count--;
    }
    //doszliśmy erasingNode do lastNode, którego nie usuwamy,a który będzie nastepnikiem
    //currentNode
    currentNode->next = lastNode;
    //zwrócenie iteratora listy do lastNode pierwszy za usuniętym ciągiem węzłów
    return ListIterator<T>(lastNode);
}

/**
 * swap() wymienia dane pomiędzy listami w taki sposób że na liście this znajdą się 
 * elementy listy przekazanej jako parametr otherList, natomiast na otherList 
 * zostaną umieszczone elementy z bieżącej list
 **/
template<typename T>
void LinkedList<T>::swap(LinkedList<T>& otherList)
{
    std::swap(this->head.next, otherList.head.next);
}
    
/**
 * resize(n) - funkcja zwiększa/zmniejsza rozmiar listy zwiększając wstawia 
 * jako domyślne wartości 0 
 **/
template<typename T>
void LinkedList<T>::resize(int n)
{
    T emptyObject;
    resize(n, emptyObject);
}
/**
 * resize(n, val) - jak wyżej tylko domyślną wstawianą wartością jest val
 **/
template<typename T>
void LinkedList<T>::resize(int n, const T& val)
{
    ListIterator<T> itr = before_begin();
    
    int length = 0;
    while( itr.next() != end() && length<n) {
        ++itr;
        ++length;
    }
    
    if(length == n)
        erase_after(itr, end());
    else
        insert_after(itr, n-length, val);

}
    
/**
 * clear() - pozostawia liste pusta i o rozmiarze 0 
 **/
template<typename T>
void LinkedList<T>::clear()
{
    erase_after(before_begin(), end());
    count = 0;
}
    
/**
 * splice_after() wstawia za pozycją position elementy z innej listy przenosząc
 **/

template<typename T>
void LinkedList<T>::splice_after(ConstListIterator<T> position, LinkedList<T>& otherList)
{
    splice_after(position, std::move(otherList));
}

template<typename T>
void LinkedList<T>::splice_after(ConstListIterator<T> position, LinkedList<T>&& otherList)
{
    if(otherList.empty()) return;
    
    
    int numberOfMovedElements = otherList.size();
    //tworzymy wskaźnik na bieżący węzeł, uwalniamy od stałości
    Node<T> *currentNode = const_cast<Node<T>*>(position.node);
    //iterator na otherList w wersji przenoszonej rvalue
    ListIterator<T> before = std::move(otherList).before_begin();
    //wskaźnik na węzeł czoła listy
    Node<T> *begin = before.node;
    //wskaźnik na węzeł końca listy
    Node<T> *end = begin;
    while(end && end->next)
        end = end->next;
    //przechowaj wskaźnik na piewrzy węzeł przenoszonej listy
    Node<T> *keep = begin->next;
    if(end) {
        begin->next = end->next;
        end->next  = currentNode->next;
    } else
        begin->next = 0;
    currentNode->next = keep;
    
    count += numberOfMovedElements;
    
}
    
/**
  * splice_after() wstawia za pozycją position dokładnie jeden elementy z innej listy 
  * przenosząc go ze wskazanego miejsca w otherList na wskazane miejsce
  * w bieżącej liście
  **/
template<typename T>
void LinkedList<T>::splice_after(ConstListIterator<T> position, LinkedList<T>& otherList, ConstListIterator<T> i)
{
    splice_after(position, std::move(otherList), i);
}

template<typename T>
void LinkedList<T>::splice_after(ConstListIterator<T> position, LinkedList<T>&& otherList, ConstListIterator<T> i)
{
    ConstListIterator<T> j = i;
    ++j;
    if( position == i || position == j) return;
    splice_after(position, std::move(otherList), i, j);
}

/**
 * splice_after() - jak wyżej tylko przenosi elementy z zakresu (first,last)
 **/
template<typename T>
void LinkedList<T>::splice_after(ConstListIterator<T> position, LinkedList<T>& otherList, ConstListIterator<T> first, ConstListIterator<T> last)
{
    splice_after(position, std::move(otherList), first, last);
}
template<typename T>
void LinkedList<T>::splice_after(ConstListIterator<T> position, LinkedList<T>&& otherList, ConstListIterator<T> first, ConstListIterator<T> last)
{
    int numberOfMovedElements = otherList.size();
    
    //tworzymy wskaźnikna bieżący węzeł, uwalniamy od stałości
    Node<T> *currentNode = const_cast<Node<T>*>(position.node);
    //wskaźniki na węzeł początku i końca przenoszonej listy
    Node<T> *begin = const_cast<Node<T>*>(first.node);
    Node<T> *end = const_cast<Node<T>*>(last.node);
    //przechowaj wskaźnik na piewrzy węzeł przenoszonej listy
    Node<T> *keep = begin->next;
    //because we need to move nodes (first, last) without first and without last
    //we need to find element just before last!
    Node<T> *beforeEnd = begin;
    while (beforeEnd->next != end) { // last is not included: find its predecessor
        beforeEnd = beforeEnd->next;
    }
    if(end) {
        //begin->next = end->next;
        begin->next = beforeEnd->next;
        beforeEnd->next  = currentNode->next;
    } else
        begin->next = 0;
    currentNode->next = keep;
    
    count += numberOfMovedElements;
}

/**
 * remove() usuwa z listy elementy (węzly) zawierające wartość podaną jako 
 * argument
 **/
template<typename T>
void LinkedList<T>::remove(const T& val)
{
    Node<T> *currentNode = &head;
    Node<T> *extraNode = NULL;
    
    while(Node<T> *tmpNode = currentNode->next)
    {
        if(tmpNode->value == val)
        {
            if(&(tmpNode->value) != &val)
            {
                erase_after(ConstListIterator<T>(currentNode));
                continue;
            } else
            {
                extraNode = currentNode;
            }
        }
        currentNode = currentNode->next;
    }
    
    if(extraNode)
        erase_after(ConstListIterator<T>(extraNode));
}

/**
 * remove_if() usuwa element z listy jezeli spełnia on predykat 
 **/
template<typename T>
template<class Predicate>
void LinkedList<T>::remove_if(Predicate pred)
{
    Node<T> *currentNode = &head;
    while(Node<T> *tmpNode = currentNode->next)
    {
        if(pred(tmpNode->value))
        {
            erase_after(ConstListIterator<T>(currentNode));
        } else {
            currentNode = currentNode->next;
        }
    }
}
    
template<typename T>
void LinkedList<T>::unique()
{
    unique(std::equal_to<T>());
}
    
template<typename T>
template<class BinaryPredicate>
void LinkedList<T>::unique(BinaryPredicate binary_pred)
{
    ListIterator<T> first = begin();
    ListIterator<T> last = end();
    if(first == last) return;
    ListIterator<T> next = first;
    while(++next != last)
    {
        if(binary_pred(*first, *next))
        {
            erase_after(first);
        } else {
            first = next;
            next = first;
        }
    }
}
    
template<typename T>
void LinkedList<T>::reverse()
{
    Node<T> *tail = head.next;
    if(!tail) return;
    
    while(Node<T> *tmpNode = tail->next) {
        Node<T> *keep = head.next;
        head.next = tmpNode;
        tail->next = tmpNode->next;
        head.next->next = keep;
    }
}
    
/**
 * sort() - metoda sortuje liste stosując operator funkcyjny std::less<T> 
 **/
template<typename T>
void LinkedList<T>::sort()
{
    //wywołujemy przeciążoną wersję sort() przekazując Comparator std::less<T>()
    sort(std::less<T>());
}
 
/**
 * sort(Compare) - metoda sortująca opierająca sortowanie listy na zadanym 
 * obiekt funkcyjnym
 **/
template<typename T>
template<class Compare>
void LinkedList<T>::sort(Compare comp)
{
    //jeżeli head wskazuje na pustą liste to nie ma listy do sortowania
    Node<T> *listPtr = head.next;
    if(!listPtr)
        return;
    
    unsigned long insize = 1;
    
    while(1)
    {
        Node<T> *pPtr = listPtr;
        listPtr = NULL;
        Node<T> *tailPtr = NULL;
        
        //licznik liczby dokonywanych scaleń listy w bieżącym przebiegu
        unsigned long mergesNumber = 0;
        while(pPtr) {
            mergesNumber++;
            
            Node<T> *qPtr = pPtr;
            
            unsigned long psize = 0;
            for(unsigned long i=0; i< insize; ++i)
            {
                ++psize;
                qPtr = qPtr->next;
                if(!qPtr)
                    break;
            }
            
            //Jezeli qPtr nie przeszedl na koniec listy to mamy dwie listy
            // do scalenia
            unsigned long qsize = insize;
            
            //Mamy teraz 2 listy które należy scalić ze sobą
            while (psize > 0 || (qsize > 0 && qPtr))
            {
                // Należy ustalić czy kolejny węzeł scalany będzie pochodził
                //z list pPtr czy też qPtr
                Node<T> *nextNode;
               if (psize == 0)
                {
                    //pPtr jest puste; nextNode musimy wziąć z listy qPtr
                    nextNode = qPtr;
                    qPtr = qPtr->next;
                    --qsize;
                } else if (qsize == 0 || !qPtr)
                {
                    //qPtr jest puste; nextNode musimy wziąć z listy pPtr
                          nextNode = pPtr;
                          pPtr = pPtr->next;
                          --psize;
                } else if (comp(pPtr->value, qPtr->value))
                {
                    //pierwszy węzeł na liscie pPtr jest mniejszy;
                    //nextNode musimy wziąć z listy pPtr
                     nextNode = pPtr;
                     pPtr = pPtr->next;
                     --psize;
                } else
                {
                    //pierwszy węzeł na liscie qPtr jest mniejszy;
                    //nextNode musimy wziąć z qPtr.
                    nextNode = qPtr;
                    qPtr = qPtr->next;
                    --qsize;
                }
                    //dodajemy następny węzeł do scalanej listy
                if (tailPtr)
                    tailPtr->next = nextNode;
                else
                    listPtr = nextNode;
                    tailPtr = nextNode;
                }
                  // przeszliśmy na liscie pPtr `insize' miejsc i na liscie
                 // q również
                 pPtr = qPtr;
            }
        
        tailPtr->next = NULL;
        
        // Jeżeli zrobiliśmy tylko jedno scalenie to już koniec.
        // Pozwala na mergesNumber == 0, przypadek pustej listy.
        if(mergesNumber <= 1)
        {
            head.next = listPtr;
            return;
        }
        
        // w innym przypadku należy scalić listę o rozmiarze 2 razy większym.
        insize *= 2;
        }
            
    
}

/**
 * merge(otherList) - scala listy sortowane przez sort()
 **/
template<typename T>
void LinkedList<T>::merge(LinkedList<T>& otherList)
{
    merge(std::move(otherList));
}

/**
 * sort(otherList) - jak wyzej tyle, że przenosi zamiast przekopiowywac podliste
 **/
template<typename T>
void LinkedList<T>::merge(LinkedList<T>&& otherList)
{
    merge(std::move(otherList), std::less<T>());
}
/**
 * merge(otherList comp) - dodatkowo jako drugi argument pobiera obiekt funkcyny
 **/
template<typename T>
template<class Compare>
void LinkedList<T>::merge(LinkedList<T>& otherList, Compare comp)
{
    merge(std::move(otherList), comp);
}
/**
 *
 **/
template<typename T>
template<class Compare>
void LinkedList<T>::merge(LinkedList<T>&& otherList, Compare comp)
{
    //przypisujemy do nodePtr wskaźnik na czoło bieżącej listy
    Node<T>* nodePtr = &head;
    
    //dopuki następnik wskaźnika na bieżącą listę
    //oraz wskaźnik na pierwszy węzeł otherList nie są NULL
    //wykonujemy pętlę
    while(nodePtr->next && otherList.head.next)
    {
        if (comp(otherList.head.next->value, nodePtr->next->value)) {
            
             Node<T> *prevNode = &otherList.head;
             Node<T> *nextNode = otherList.head.next;
        
             Node<T> *keepNode = prevNode->next; //!
        
            if(nextNode) {
                prevNode->next = nextNode->next;
                nextNode->next = nodePtr->next;
            } else {
                prevNode->next = NULL;
            }
        
             nodePtr->next = keepNode;
        }
        
             nodePtr = nodePtr->next;
    }
    
    if (otherList.head.next)
    {
            nodePtr->next = otherList.head.next;
            otherList.head.next = NULL;
    }
}



} // koniec namespace ADT

//test LinkedList<T>
using namespace std;

int main() {
    int z;
    scanf("%d", &z);
    while(z--) {
        int size1;
        scanf("%d", &size1);
        ADT::LinkedList<int> L1;
        for(int i=0; i<size1; i++) {
            int element;
            scanf("%d", &element);
            L1.push_back(element);
        }
        int size2;
        scanf("%d", &size2);
        ADT::LinkedList<int> L2;
        for(int i=0; i<size2; i++) {
            int element;
            scanf("%d", &element);
            L2.push_back(element);
        }
        L1.merge(L2);
        for(ADT::ListIterator<int> it = L1.begin(); it != L1.end(); it++)
        printf("%d ", *it);
        printf("\n");
        L1.unique();
        for(ADT::ListIterator<int> it = L1.begin(); it != L1.end(); it++)
        printf("%d ", *it);
        printf("\n");
        
    }
    
}
