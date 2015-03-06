#include<iostream>
#include<memory>
#include<initializer_list>

/**
 * Zdefiniowanie typu LinkedList w przestrzeni nazw ADT w celu enkapsulacji 
 * i uniknięcia ewentualnych konfliktów nazw ze względu
 */

namespace ADT {
    template<typename T>
    struct Node;
    
    template<typename T>
    struct DoublyLinkedNode;
    
    template<typename T>
    struct ListIterator;
    
    template<typename T>
    struct ReverseListIterator;
    
    template<typename T>
    struct ConstListIterator;
    
    template<typename T>
    struct ReverseConstListIterator;
    
    template<typename T>
    struct LinkedList;
    
    template<typename T>
    struct DoublyLinkedList;
}

/**
 * struktura węzła (Node) listy powiązanej LinkedList
 *  Node jest parametryzowane typem T, w ten sposób może przechowywać
 *  w publicznym (domyślnie w struct) polu wartość (value) dowolnego typu
 *  Struktura Node posiada:
 *  1) wartość  T value
 *  2) wskaźnik na kolejny element listy Node *next
 */

template<typename T>
struct ADT::DoublyLinkedNode {
    T value;
    DoublyLinkedNode<T> *next;
    DoublyLinkedNode<T> *previous;
    
    DoublyLinkedNode() : value(0), next(NULL), previous(NULL) { }
    DoublyLinkedNode(const T& val) : value(val), next(NULL), previous(NULL) {}
    
};

/**
 * struktura ListIterator - iteratora przechodzącego po liscie LinkedList
 * ListIterator jest parametryzowany odpowiednim typem T zgodnym z typem
 * elementów przechowywanych na liscie LinkedList 
 * Składa się z następujących elementów: 
 * 1) wskaźnika na węzeł parametryzowany typem T  Node<T> *node
 * 2) przeciążonych operatorów umożliwiających wykonywanie operacji na iteratorze
 *    takich jak np inkrementacja it++, ++it, porównania itp.
 */
template<typename T>
struct ADT::ListIterator {
    private:
        friend class DoublyLinkedList<T>;
        friend class ConstListIterator<T>;
        friend class ReverseListIterator<T>;
        DoublyLinkedNode<T> *node;
    public:
    //konstruktory iteratora
    ListIterator() : node(NULL) {}
    ListIterator(DoublyLinkedNode<T> *n) : node(n) { }
    
    //przeciążone operatory
    
    //1) indirection operator - zwraca referencje do value typu T znajdującą
    //   się w bieżącym węźle Node
    T& operator*() const
    {
        return this->node->value;
    }
    
    //2) structure dereference operator - gdyby T było typu posiadajacego
    // składowe np struct, class możliwe użycie itr->field
    T* operator->() const
    {
        return &(this->node->value);
    }
    
    //3) operator ++ preinkrementacji iteratora ++itr
    ListIterator<T>& operator++()
    {
        node = node->next;
        return *this;
    }
    
    //4) operator ++ postinkrementaji iteratora itr++
    ListIterator<T> operator++(int)
    {
        ListIterator<T> tmp(*this);
        node = node->next;
        return tmp;
    }
    
    //5) operator porównania ==
    bool operator==(const ListIterator<T> &otherIterator) {
        return node == otherIterator.node;
    }
    
    //6) operator nierówności !=
    bool operator!=(const ListIterator<T> &otherIterator) {
        return node != otherIterator.node;
    }
    
    //7) operator -- predekrementacji iteratora --itr
    ListIterator<T>& operator--()
    {
        node = node->previous;
        return *this;
    }
    
    //8) operator -- postdekrementacji iteratora itr--
    ListIterator<T>& operator--(int)
    {
        ListIterator<T>& tmp(*this);
        node = node->previous;
        return tmp;
    }
    
    //Iterator na następny element w liscie
    ListIterator<T> next() const
    {
        if(node)
            return ListIterator<T>(node->next);
         else
            return ListIterator<T>(0);
    }
};

/**
 * struktura ReverseListIterator - przechodzi po liście DoublyLinkedList 
 * wspak. ReverseListIterator jest parametryzowany odpowiednim typem T 
 * zgodnym z typem elementów przechowywanych na liscie DoublyLinkedList
 * Składa się z następujących elementów:
 * 1) wskaźnika na węzeł parametryzowany typem T  DoublyLinkedNode<T> *node
 * 2) przeciążonych operatorów umożliwiających wykonywanie operacji 
 * na iteratorze takich jak np inkrementacja itr++, ++itr, porównania itp.
 **/
template<typename T>
struct ADT::ReverseListIterator {
    
    private:
        friend class DoublyLinkedList<T>;
        friend class ReverseConstListIterator<T>;
        DoublyLinkedNode<T> *node;
    
    public:
        //konstruktory iteratora
        ReverseListIterator() : node(NULL) {}
        ReverseListIterator(DoublyLinkedNode<T> *n) : node(n) { }
        ReverseListIterator(ListIterator<T> itr) {
            node = itr.node;
        }
    
    //przeciazone operatory
    
    //1) indirection operator - zwraca referencje do value typu T znajdującą
    //   się w bieżącym węźle Node
    T& operator*() const
    {
        return this->node->value;
    }
    
    //2) structure dereference operator - gdyby T było typu posiadajacego
    // składowe np struct, class możliwe użycie itr->field
    T* operator->() const
    {
        return &(this->node->value);
    }
    
    //3) operator ++ preinkrementacji iteratora ++itr
    ReverseListIterator<T>& operator++()
    {
        node = node->previous;
        return *this;
    }
    
    //4) operator ++ postinkrementaji iteratora itr++
    ReverseListIterator<T> operator++(int)
    {
        ReverseListIterator<T> tmp(*this);
        node = node->previous;
        return tmp;
    }
    
    //5) operator porównania ==
    bool operator==(const ReverseListIterator<T> &otherIterator) {
        return node == otherIterator.node;
    }
    
    //6) operator nierówności !=
    bool operator!=(const ReverseListIterator<T> &otherIterator) {
        return node != otherIterator.node;
    }
    
    //Iterator na następny element w liscie
    ReverseListIterator<T> next() const
    {
        if(node)
        return ReverseListIterator<T>(node->previous);
        else
        return ReverseListIterator<T>(0);
    }

};

/**
 * struktura ConstListIterator - iterator analogiczny jak powyzej aczkolwiek
 * w wersji const! Iterator w wersji const nie pozwala na zmiane wartosci na którą 
 * wskazuje.
 **/
template<typename T>
struct ADT::ConstListIterator
{
    private:
        friend class DoublyLinkedList<T>;
    friend class ReverseListIterator<T>;
    friend class ReverseConstListIterator<T>;
        const DoublyLinkedNode<T> *node; //tym razem wskaźnik na stały/niemodyfikowalny Node<T>
    public:
        //konstruktory
        ConstListIterator()  : node(NULL) { }
        ConstListIterator(const DoublyLinkedNode<T> *n) : node(n) { }
        ConstListIterator(const ListIterator<T>& itr) : node(itr.node) { }
    
        //przeciążone operatory
    
    //1) indirection operator - zwraca referencje na niemodyfikowalny(stały) typ
    // T dla wartości bieżącego węzła
    const T& operator*() const
    {
        return this->node->value;
    }
    
    //2) structure dereference operator
    const T* operator->() const
    {
        return &(this->node->value);
    }
    
    //3) operator ++ preinkrementacji iteratora ++itr
    ConstListIterator<T>& operator++()
    {
        node = node->next;
        return *this;
    }
    
    //4) operator ++ postinkrementaji iteratora itr++
    ConstListIterator<T> operator++(int)
    {
        ConstListIterator<T> tmp(*this);
        node = node->next;
        return tmp;
    }
    
    //5) operator porównania ==
    bool operator==(const ConstListIterator<T>& otherIterator) {
        return node == otherIterator.node;
    }
    
    //6) operator nierówności !=
    bool operator!=(const ConstListIterator<T>& otherIterator) {
        return node != otherIterator.node;
    }
    
    //7) operator -- predekrementacji iteratora --itr
    ConstListIterator<T>& operator--()
    {
        node = node->previous;
        return *this;
    }
    
    //8) operator -- postdekrementacji iteratora itr--
    ConstListIterator<T>& operator--(int)
    {
        ConstListIterator<T>& tmp(*this);
        node = node->previous;
        return tmp;
    }
    
    //Iterator na następny element w liscie
    ConstListIterator<T> next() const
    {
        if(node)
            return ConstListIterator<T>(node->next);
        else
            return ConstListIterator<T>(0);
    }


};

/**
 * struktura ReverseConstListIterator - iterator analogiczny jak powyzej
 * aczkolwiek w wersji const! Iterator w wersji const nie pozwala na zmiane
 * wartosci na którą wskazuje.
 **/
template<typename T>
struct ADT::ReverseConstListIterator
{
    private:
        friend class DoublyLinkedList<T>;
        const DoublyLinkedNode<T> *node;
        //tym razem wskaźnik na stały/niemodyfikowalny Node<T>
    public:
    //konstruktory
    ReverseConstListIterator()  : node(NULL) { }
    ReverseConstListIterator(const DoublyLinkedNode<T> *n) : node(n) { }
    ReverseConstListIterator(const ReverseListIterator<T>& itr) : node(itr.node) { }
    
    //przeciążone operatory
    
    //1) indirection operator - zwraca referencje na niemodyfikowalny(stały) typ
    // T dla wartości bieżącego węzła
    const T& operator*() const
    {
        return this->node->value;
    }
    
    //2) structure dereference operator
    const T* operator->() const
    {
        return &(this->node->value);
    }
    
    //3) operator ++ preinkrementacji iteratora ++itr
    ReverseConstListIterator<T>& operator++()
    {
        node = node->previous;
        return *this;
    }
    
    //4) operator ++ postinkrementaji iteratora itr++
    ReverseConstListIterator<T> operator++(int)
    {
        ReverseConstListIterator<T> tmp(*this);
        node = node->previous;
        return tmp;
    }
    
    //5) operator porównania ==
    bool operator==(const ReverseConstListIterator<T>& otherIterator) {
        return node == otherIterator.node;
    }
    
    //6) operator nierówności !=
    bool operator!=(const ReverseConstListIterator<T>& otherIterator) {
        return node != otherIterator.node;
    }
    
    //Iterator na następny element w liscie
    ReverseConstListIterator<T> next() const
    {
        if(node)
        return ReverseConstListIterator<T>(node->previous);
        else
        return ReverseConstListIterator<T>(0);
    }
    
    
};

//operator porównania dla ListIterator, ConstListIterator
template<typename T>
inline bool operator==(const ADT::ListIterator<T>& itr1,
                       const ADT::ConstListIterator<T>& itr2) {
    return itr1.node == itr2.node;
}
//operator nierówności dla ListIterator, ConstListIterator
template<typename T>
inline bool operator!=(const ADT::ListIterator<T>& itr1,
                       const ADT::ConstListIterator<T>& itr2) {
    return itr1.node != itr2.node;
}

//operator porównania dla ReverseListIterator, ReverseConstListIterator
template<typename T>
inline bool operator==(const ADT::ReverseListIterator<T>& itr1,
                       const ADT::ReverseConstListIterator<T>& itr2) {
    return itr1.node == itr2.node;
}
//operator nierówności dla ReverseListIterator, ReverseConstListIterator
template<typename T>
inline bool operator!=(const ADT::ReverseListIterator<T>& itr1,
                       const ADT::ReverseConstListIterator<T>& itr2) {
    return itr1.node != itr2.node;
}

/**
 * Struktura definiująca abstrakcyjny typ danych DoublyLinkedList 
 * listy powiązanej dwukierunkowej gdzie węzły list zawierają wskaźniki
 * na węzeł poprzedni oraz węzeł następny, przy czym dla pierwszego 
 * węzła wskaźnik previous = NULL, dla ostatniego węzła next = NULL
 * Lista podwójnie wiązana DoublyLinkedList w bieżącej implementacji
 * posiada wskaźnik na czoło listy (head) oraz na ogon listy (tail)
 * posiada również licznik ilości przechowywanych elementów
 * DoublyLinkedList dziedzyczy po zwykłej LinkedList (liscie jednokierunkowej)
 * implementując dodatkowy wskaźnik na ostatni element (tail) 
 * własne konstruktory, oraz dodatkowe metody niezaimplementowadne w LinkedList
 * takie jak metody zwracające ReverseListIterator, ReverseConstListIterator,
 * metody pozwalające na manipulacje wskazanym elementem w czasie O(1) 
 * jak np. insert_after() -> insert(), erease_after() -> erease() 
 * może również przeciążać metody z LinkedList które tam wymagały czasu O(n) 
 * a tu możliwa jest realizacja w czasie O(1) ze względu na dostęp do wskaźnika
 * na wcześniejszy element
 */
template<typename T>
struct ADT::DoublyLinkedList
{
    private:
        //zawiera wskaźnik na ostatni element listy
        //tail->next - to bedzie wskazywac na pierwszy element listy
        //double linked list jest w tym przypadku cykliczna tj.
        //ostatni element listy wskazuje .next na pierwszy element
        //pierwszy element listy wskazuje .previous na ostatni element
        //jezeli mamy listę pustą to tail.next wskazuje na tail
        DoublyLinkedNode<T> tail;
        int count;
    public:
        //konstruktor pusta list
        DoublyLinkedList();
        //destruktor
        ~DoublyLinkedList();
        ListIterator<T> begin();
        ConstListIterator<T> begin() const;
        ListIterator<T> end();
        ConstListIterator<T> end() const;
        ReverseListIterator<T> rbegin();
        ReverseConstListIterator<T> rbegin() const;
        ReverseListIterator<T> rend();
        ReverseConstListIterator<T> rend() const;
        ConstListIterator<T> cbegin() const;
        ConstListIterator<T> cend() const;
        ReverseConstListIterator<T> crbegin() const;
        ReverseConstListIterator<T> crend() const;
        bool empty() const;
        int size() const;
        void resize (int n);
        void resize (int n, const T& val);
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;
        //3x assing() functions
        template<class InputIterator>
        void assign(InputIterator first, InputIterator last);
        void assign(int n, const T& val);
        void assign(std::initializer_list<T> il);
        void push_front(const T& val);
        void push_front(T&& val);
        void pop_front();
        void push_back(const T& val);
        void push_back(T&& val);
        void pop_back();
        //5x insert()
        //single element
        ListIterator<T> insert(ConstListIterator<T> position, const T& val);
        //fill
        ListIterator<T>
            insert(ConstListIterator<T> position, int n, const T& val);
        //range
        template <class InputIterator>
        ListIterator<T> insert(ConstListIterator<T> position,
                               InputIterator first, InputIterator last);
        //move
        ListIterator<T> insert(ConstListIterator<T> position, T&& val);
        //initializer list
        ListIterator<T> insert(ConstListIterator<T> position,
                               std::initializer_list<T> il);
        //2x erase()
        ListIterator<T> erase(ConstListIterator<T> position);
        ListIterator<T> erase(ConstListIterator<T> first, ConstListIterator<T> last);
        void swap(DoublyLinkedList& otherList);
        void clear();
        //6xsplice()
        //entire list
        void splice(ConstListIterator<T> position, DoublyLinkedList<T>& otherList);
        void splice(ConstListIterator<T> position, DoublyLinkedList<T>&& otherList);
        //single element
        void splice(ConstListIterator<T> position, DoublyLinkedList<T>& otherList, ConstListIterator<T> i);
        void splice(ConstListIterator<T> position, DoublyLinkedList<T>&& otherList, ConstListIterator<T> i);
        //element range
        void splice(ConstListIterator<T> position, DoublyLinkedList<T>& otherList, ConstListIterator<T> first, ConstListIterator<T> last);
        void splice(ConstListIterator<T> position, DoublyLinkedList<T>&& otherList, ConstListIterator<T> first, ConstListIterator<T> last);
        void remove(const T& val);
        template <class Predicate>
        void remove_if(Predicate pred);
        void unique();
        template <class BinaryPredicate>
        void unique(BinaryPredicate binary_pred);
        void reverse();
        T& element_at(ConstListIterator<T> position);
        void logDoublyLinkedList(void);
        //4x merge() function
        void merge(DoublyLinkedList<T>& otherList);
        void merge(DoublyLinkedList<T>&& otherList);
        template <class Compare>
        void merge(DoublyLinkedList<T>& otherList, Compare comp);
        template <class Compare>
        void merge(DoublyLinkedList<T>&& otherList, Compare comp);
        //2x sort() function
        void sort(void);
        template<class Compare>
        void sort(Compare comp);
    
    //operator porównania dla DoublyLinkedList list1, DoublyLinkedList list2
    bool operator==(const DoublyLinkedList<T>& list2) {
        //tworzymy sobie iteratory na koniec list list1 i list2
        ConstListIterator<T> list1EndItr = this->end();
        ConstListIterator<T> list2EndItr = list2.end();
        
        //iteratory którymi będziemy przesuwać się w pętli
        //zaczynające od początku odpowiednio listy list1 i list2
        ConstListIterator<T> list1Itr = this->begin();
        ConstListIterator<T> list2Itr = list2.begin();
        
        //przechodzimy w petli obie listy od ich poczatku az do ich konca
        //porownujac kolejno elementy list list1 i list2 az do momentu
        //gdy dojdziemy do końców obu list lub napotkamy na elementy nie pasujace
        //do siebie na dwóch listach
        while(list1Itr != list1EndItr && list1Itr != list2EndItr &&
              *list1Itr == *list2Itr) //aż do momentu gdy odpowidające sobie elementy do siebie pasują !
        {
            ++list1Itr; ++list2Itr;
        }
        
        //jezeli obie listy udalo sie nam przejsc do konca to sa one zgodne
        if(list1Itr == list1EndItr && list2Itr == list2EndItr)
        return 1;
        //wpw list nie sa takie same
        return 0;
    }
    
    //operator nierówności dwóch list
    bool operator!=(const DoublyLinkedList<T>& list2) {
        
        return !(*this == list2);
    }
    
    private:
        //przenosi węzły pomiędzy [otherFirstItr, otherLastItr)
        //na pozycje thisFirsItr
        void transferNode(ListIterator<T>& thisFirstItr,
                          ListIterator<T>& otherFirstItr,
                          ListIterator<T>& otherLastItr);
    
};

//operator porównania dla DoublyLinkedList list1, DoublyLinkedList list2
template<typename T>
inline bool operator==(const ADT::DoublyLinkedList<T>& list1,
                       const ADT::DoublyLinkedList<T>& list2) {
    //tworzymy sobie iteratory na koniec list list1 i list2
    ADT::ConstListIterator<T> list1EndItr = list1.end();
    ADT::ConstListIterator<T> list2EndItr = list2.end();
    
    //iteratory którymi będziemy przesuwać się w pętli
    //zaczynające od początku odpowiednio listy list1 i list2
    ADT::ConstListIterator<T> list1Itr = list1.begin();
    ADT::ConstListIterator<T> list2Itr = list2.begin();
    
    //przechodzimy w petli obie listy od ich poczatku az do ich konca
    //porownujac kolejno elementy list list1 i list2 az do momentu
    //gdy dojdziemy do końców obu list lub napotkamy na elementy nie pasujace
    //do siebie na dwóch listach
    while(list1Itr != list1EndItr && list1Itr != list2EndItr &&
          *list1Itr == *list2Itr) //aż do momentu gdy odpowidające sobie elementy do siebie pasują !
    {
        ++list1Itr; ++list2Itr;
    }
    
    //jezeli obie listy udalo sie nam przejsc do konca to sa one zgodne
    if(list1Itr == list1EndItr && list2Itr == list2EndItr)
        return 1;
    //wpw list nie sa takie same
    return 0;
}

//operator nierówności dwóch list
template<typename T>
inline bool operator!=(const ADT::DoublyLinkedList<T>& list1,
                       const ADT::DoublyLinkedList<T>& list2) {
    
    return !(list1 == list2);
}
