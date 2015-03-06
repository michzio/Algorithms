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
    struct ListIterator;
    
    template<typename T>
    struct ConstListIterator;
    
    template<typename T>
    struct LinkedList;
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
struct ADT::Node {
    T value;
    Node<T> *next;
    
    Node() : next(NULL) { }
    Node(const T& val) : value(val), next(NULL)  { }
};

/**
 * struktura ListIterator - iteratora przechodzącego po liscie LinkedList
 * ListIterator jest parametryzowany odpowiednim typem T zgodnym z typem
 * elementów przechowywanych na liscie LinkedList 
 * Składa się z następujących elementów: 
 * 1) wskaźnika na węzeł parametryzowany typem T  Node<T> *next 
 * 2) przeciążonych operatorów umożliwiających wykonywanie operacji na iteratorze
 *    takich jak np inkrementacja it++, ++it, porównania itp.
 */
template<typename T>
struct ADT::ListIterator {
    private:
        friend class LinkedList<T>;
        friend class ConstListIterator<T>;
        Node<T> *node;
    public:
    //konstruktory iteratora
    ListIterator() : node(NULL) {}
    ListIterator(Node<T> *n) : node(n) { }
    
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
 * struktura ConstListIterator - iterator analogiczny jak powyzej aczkolwiek
 * w wersji const! Iterator w wersji const nie pozwala na zmiane wartosci na którą 
 * wskazuje.
 **/
template<typename T>
struct ADT::ConstListIterator
{
    private:
        friend class LinkedList<T>;
        const Node<T> *node; //tym razem wskaźnik na stały/niemodyfikowalny Node<T>
    public:
        //konstruktory
        ConstListIterator()  : node(NULL) { }
        ConstListIterator(const Node<T> *n) : node(n) { }
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
    bool operator!=(const ListIterator<T>& otherIterator) {
        return node != otherIterator.node;
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

/**
 * Struktura definiująca abstrakcyjny typ danych listy powiązanej 
 * zawiera wskaźnik na czoło listy, oraz licznik ilości przechowywanych 
 * elementów
 */
template<typename T>
struct ADT::LinkedList
{
    private:
        //wskaźnik na czoło listy
        Node<T> head;
        int count;
    public:
        //konstruktor
        LinkedList();
        //destruktor
        ~LinkedList();
    
    //metody:
    ListIterator<T> before_begin();
    ConstListIterator<T> before_begin() const;
    ListIterator<T> begin();
    ConstListIterator<T> begin() const;
    ListIterator<T> end();
    ConstListIterator<T> end() const;
    ConstListIterator<T> cbefore_begin() const;
    ConstListIterator<T> cbegin() const;
    ConstListIterator<T> cend() const;
    
    //3x assign() functions
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);
    void assign(int n, const T& val);
    void assign(std::initializer_list<T> il);
    
    void push_front(const T& val);
    void push_front(T&& val);
    void push_back(const T& val);
    
    void pop_front();
    
    //5x insert_after() functions
    ListIterator<T> insert_after(ConstListIterator<T> position, const T& val);
    ListIterator<T> insert_after(ConstListIterator<T> position, T&& val);
    ListIterator<T> insert_after(ConstListIterator<T> position, int n, const T& val);
    template<class InputIterator>
    ListIterator<T> insert_after(ConstListIterator<T> position, InputIterator first, InputIterator last);
    ListIterator<T> insert_after(ConstListIterator<T> position, std::initializer_list<T> il);
    
    T& front();
    const T& front() const;
    T& element_at(ConstListIterator<T> position);
    
    ListIterator<T> erase_after(ConstListIterator<T> position);
    ListIterator<T> erase_after(ConstListIterator<T> position,
                                ConstListIterator<T> last);
    
    void swap(LinkedList<T>& otherList);
    
    //2x resize() method - wstawia domyślne wartości na nowych pozycjach
    //lub obcina liste to wskazanego rozmiaru
    void resize(int n);
    void resize(int n, const T& val);
    
    void clear();
    
    //metody splice_after()
    void splice_after(ConstListIterator<T> position, LinkedList<T>& otherList);
    void splice_after(ConstListIterator<T> position, LinkedList<T>&& otherList);
    void splice_after(ConstListIterator<T> position, LinkedList<T>& otherList, ConstListIterator<T> i);
    void splice_after(ConstListIterator<T> position, LinkedList<T>&& otherList, ConstListIterator<T> i);
    void splice_after(ConstListIterator<T> position, LinkedList<T>& otherList,
                      ConstListIterator<T> first, ConstListIterator<T> last);
    void splice_after(ConstListIterator<T> position, LinkedList<T>&& otherList,
                       ConstListIterator<T> first, ConstListIterator<T> last);
    
    //proste metody zwracajace rozmiar listy oraz czy jest pusta
    int size() { return count; }
    bool empty() const
    {
        if(head.next == NULL) return true;
                    return false;
    }
    void logLinkedList(void);
    
    //remove()
    void remove(const T& val);
    template<class Predicate>
    void remove_if(Predicate pred);
    
    void unique();
    template<class BinaryPredicate>
    void unique(BinaryPredicate binary_pred);
    
    void reverse();
    
    //sortowanie list
    void sort();
    template<class Compare>
    void sort(Compare comp);
    
    //merge functions
    void merge(LinkedList<T>& otherList);
    void merge(LinkedList<T>&& otherList);
    template<class Compare>
    void merge(LinkedList<T>& otherList, Compare comp);
    template <class Compare>
    void merge(LinkedList<T>&& otherList, Compare comp);
    
};
