#include "linked_list.h"
#include<iostream>
#include<cstdio>

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
 * insert_after() - metoda dodaje węzeł z wartością przekazaną jako drugi parametr val 
 * za węzłem wskazywanym przez pierwszy parametr position (ConstListIterator<T>)
 * oraz zwiększa zmienną count
 **/
template<typename T>
ListIterator<T> LinkedList<T>::insert_after(ConstListIterator<T> position, const T& val)
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
    std::fprintf(stderr, "\n{ ");
    for(ConstListIterator<T> it = begin(); it != end(); ++it) {
        std::fprintf(stderr, "%d, ", *it);
    }
    std::fprintf(stderr,"}\n");
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
    
} // koniec namespace ADT

/* test LinkedList<T>
using namespace std;


int main(int argc, char *argv[])
{
    ADT::LinkedList<int> list;
    
    
    cout << "Testowy program" << endl;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    
    ADT::ConstListIterator<int> itr = list.begin();
    
    printf("Pierwszy element listy to: %d.\n", list.front());
    
    printf("Drugi element (indirection operator): %d.\n", *(++itr));
    
    printf("Trzeci element listy to: %d.\n", list.element_at(++itr) );
    
    list.pop_front();
    list.pop_front();
    list.pop_front();
    //list.pop_front();
    list.front();
    
    return 0;
}
*/


int main(int argc, char *argv[])
{
    //tworzymy liste wskaźnikowa
    ADT::LinkedList<int> L;
    
    int tc;
    scanf("%d", &tc);
    
    for(int t=0; t<tc; t++) {
        char c = ' ';
        while(c!='A' && c!='D' && c!='S')
            c = getchar(); // wczytuj znak po znaku dopoki nie napotkasz znaku dot. polecenia
        if(c=='A') {
            int num;
            scanf("%d", &num);
            L.push_back(num);
        }
        else if(c=='D') {
            if(L.empty()) // lista pusta
                printf("EMPTY\n");
            else {
                int num = L.front();
                L.pop_front();
                printf("%d\n", num);
            }
        }
        else {
            printf("%d\n", L.size());
        }
    }
    
    return 0;
}