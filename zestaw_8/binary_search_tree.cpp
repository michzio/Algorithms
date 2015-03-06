#include "binary_search_tree.h"
#include<string>

namespace ADT {
    
/***
 * Metoda size(void) jest publiczna i zwraca rozmiar 
 * bieżącego drzewa BST tj. liczbę wszystkich jego węzłów
 * wywołuje w tym celu metodę prywatną przeciążoną pobierającą
 * wskaźnik na root poddrzewa dla którego ma zwrócić lizbę węzłów
 * umożliwia to spełnienie warunku niezmiennika 
 * size(tree) = size(tree.left) + size(tree.right) + 1
 * gdy bieżące drzewo posiada wskaźnik root ustawiony na NULL
 * to zostanie zwrócona wartość 0
 **/
template<typename K, typename V>
    int BinarySearchTree<K,V>::size(void) const {
        return size(root);
    }

/***
 * Implementacja prywatnej metody size(Node<K,V> *) 
 * wykorzystywana przez publiczna size()
 * do okreslenia liczby elementow w poddrzewie ktorego rootem jest
 * node stosuje jej zmienna skladowa count
 **/
template<typename K, typename V>
    int BinarySearchTree<K,V>::size(Node *node) const
    {
        if(node == NULL)
        {
            return 0;
        } else
        {
            return node->count;
        }
    }

/***
 * Implementacja publicznej metody lookup(K key) wyszukuje 
 * w drzewie przekazanego jako argument klucza i zwraca true lub false 
 * w zaleznosci od wyników wyszukiwania.  Wykorzystuje prywatna metode 
 * lookup(Node *node, K key) przekazujac wskaznik na root biezacego drzewa
 **/
template<typename K, typename V>
    bool BinarySearchTree<K,V>::lookup(const K& key) const {
        return lookup(root, key);
    }

/***
 * Implementacja prywatnej metody lookup(Node *node, K key) 
 * która w poddrzewie binarnym wskazywanym przez node wyszukuje klucza key
 * moze zwracac wartosc false lub true w zaleznosci czy znajdzie poszukiwany
 * klucz.
 **/
template<typename K, typename V>
    bool BinarySearchTree<K,V>::lookup(Node *node, const K& key) const
    {
        //1. node wskazuje na puste drzewo binarne (empty tree)
        if(node == NULL)
            return false;
        //2. sprawdzamy czy szukany klucz znajduje sie w aktualnym wezle
        if(key == node->key)
            return true;
        else if(key < node->key) //klucz mniejszy niz w biezacym wezle
            //szukaj w lewym poddrzewie
            return lookup(node->left, key);
        else //klucz wiekszy niz  w biezacym wezle
            //szukaj w prawym poddrzewie
            return lookup(node->right, key);
    }

/***
 * Implementacja metody publicznej get(K key) która ciała w sposób analogiczny
 * jak metoda lookup(K key) z tą różnicą, że gdy odnajdzie wezel z 
 * pasujacym kluczem zwraca wartosc przypisana do klucza w wezle poprzez
 * referencje. Metoda publiczna wykorzystuje metoe prywatna do 
 * rekurencyjnego przeszukania drzewa poczawszy od wskaznika na root
 **/
template<typename K, typename V>
    V *BinarySearchTree<K,V>::get(const K& key)
    {
        return get(root, key);
    }

/***
 * Implementacja metody prywatnej get(Node *node, K key), która wyszukuje 
 * wezeł z kluczem key w poddrzewie na ktore wskazuje wskaźnik Node *
 * W przypadku gdy dopasuje wezel z kluczem key zwraca przypisana mu wartosc
 * w przeciwnym razie wzraca null.
 **/
template<typename K, typename V>
    V *BinarySearchTree<K,V>::get(Node *node, const K& key)
    {
        //1. node wskazuje na puste drzewo binarne (empty tree)
        if(node == NULL)
            //zwracamy wskaźnik na null
            return NULL;
        //2. sprawdzamy czy szukany klucz znajduje się w aktualnym węźle
        if(key == node->key)
            return &(node->key);
        else if(key < node->key) //klucz mniejszy niz w biezacym wezle
            //szukaj w lewym poddrzewie
            return get(node->left, key);
        else //klucz wiekszy niz w biezacym wezle
            //szukaj w prawym poddrzewie
            return get(node->right, key);
    }

/***
 * Implementacja metody publicznej która wstawia do drzewa binarnego
 * wezeł z nowym kluczem i przypisana mu wartoscia lub gdy istnieje juz 
 * wezeł z podanym kluczem aktualizuje dla dopasowanego wezla wartosc 
 * Metoda korzysta z prywatnej metody insert(Node *, const K&, const V&)
 * podajac jako wskaznik root jako wskaznik drzewa ktorym (key, value) 
 * ma zostac wstawione
 **/
template<typename K, typename V>
    void BinarySearchTree<K, V>::insert(const K& key, const V& value)
    {
        //wstawia (key, value) rekurencyjnie do drzewa o korzeniu root
        //zwraca nowy wskaźnik root który powinien zostać nadpisany
        root = insert(root, key, value);
    }

/***
 * Implementacja metody prywatnej insert(Node *, const K&, const V&) 
 * która wstawia do drzewa wskazywanego przez Node * pare (k,v) 
 * zwraca wskaźnik na drzewo w którym wstawiono element.
 **/
template<typename K, typename V>
   typename BinarySearchTree<K,V>::Node *BinarySearchTree<K, V>::insert(Node *node, const K& key, const V& value)
    {
    //1. Jezeli drzewo na które wskazuje node jest puste to
    //nalezy utworzyc nowy element i nastepnie zwrócic wskaźnik na
    //ten nowy wezel
    if(node == NULL) //puste poddrzewo
        //tworzymy nowy wezel na stosie i zwracamy wskaznik do niego
        return new Node(key, value, 1);
    //2. przeszukujemy drzewo wgłąb szukajac pustego miejsca lub klucza
    if(key == node->key) //znaleziono pasujacy klucz
        //aktualizujemy biezaca wartosc wezla
        node->value = value;
    else if(key < node->key) //klucz mniejszy od klucza w biezacym wezle
        //wstaw do lewego poddrzewa (key, value)  aktualizuj wskaznik lewego
        //poddrzewa np. gdy byłby NULL i dodanoby nowy wezel
        node->left = insert(node->left, key, value);
    else //klucz wiekszy niz w biezacym wezle
        ///wstaw do prawego poddrzew (key,value) aktualizuj wskaznik prawego
        //poddrzewa np. gdyby był NULL i dodnoby nowy wezel
        node->right = insert(node->right, key, value);
    //aktualizujemy licznik liczby wezłów bieżącego poddrzewa
    node->count = size(node->left) + size(node->right) + 1;
    //zwracamy niezmieniony wskaźnik na węzeł
    return node;
    }
    
/***
 * Na podstawie przekazanego wskaźnika na drzewo binarne (Binary Search Tree) 
 * zwraca węzeł z minimalna wartościa klucza dla tego drzewa. Biorąc pod uwagę, że
 * klucze w binary search tree sa uporzadkowane w taki sposob ze minimalna ich 
 * wartosc znajduje sie w najbardziej lewym poddrzewie wystarczy rekurencyjnie 
 * lub w petli dostac sie do tego poddrzewa. Nie ma potrzeby przeszukiwania 
 * całego drzewa
 **/
template<typename K, typename V>
    const typename BinarySearchTree<K,V>::Node *BinarySearchTree<K,V>::min(Node *node) const {
        
        //1. jezeli biezacy wezel nie posiada lewego dziecka tzn ze
        //przechowuje najmniejsza wartosc klucza i zwracamy wskaznik na biezacy
        //node
        if(node->left == NULL) return node;
        //2. jezeli biezacy wezel posiada lewe poddrzewo to rekurencyjie
        //minimalnego klucza wyszukujemyw tym wlasnie lewym poddrzewie
        return min(node->left);
    }

/***
 * Publiczna funkcja min() zwracajaca dla biezacego niepustego drzewa 
 * wartosc klucza przez referencje. Korzysta z prywatnej wersji funkcji min()
 * przekazujac referencje na root (całe drzewo). Jezeli drzewo puste root == NULL
 * to wyrzuć wyjatek
 **/
template<typename K, typename V>
    const K& BinarySearchTree<K, V>::min(void) const
    {
        if(root == NULL)
            throw BinaryTreeException("Binary Search Tree is EMPTY!");
        
        return min(root)->key;
    }
    
/***
 * Analogiczna funkcja jak w przypadku min() tyle, że zwracająca node z 
 * max kluczem (wezeł w najbardziej prawym poddrzewie) 
 * Tym razem zaimpelentowane iteracyjnie a nie rekurencyjnie 
 * tj. przechodzimy trzewo cały czas w prawo while(currentNode->right != NULL)
 **/
template<typename K, typename V>
    const typename BinarySearchTree<K, V>::Node *BinarySearchTree<K, V>::max(Node *node) const
    {
        //tworzymy nowy wskaźnik
        Node *currentNode = node;
        
        //w petli przechodizmy bo najbardzije prawego wezla az do momentu
        //gdy znajdziemy wezel ktory nie ma juz prawego poddrzewa
        while(currentNode->right != NULL) {
            currentNode = currentNode->right;
        }
        //zwracamy otrzymany wezel
        return currentNode;
    }
    
/***
 * analogiczna funkcja jak w przypadku publicznej min() tyle ze zwracajaca 
 * maksymalny klucz w drzewie binarnym (BST) wykorzystujac metode prywatna max()
 * Klucza szukamy w niepustym drzewie i zwracamy do przez referencje do stałe
 * wartosci. Jezeli drzewo jest pusty wyrzucany jest wyjatek! 
 **/
template<typename K, typename V>
    const K& BinarySearchTree<K,V>::max(void) const
    {
        if(root == NULL)
            throw BinaryTreeException("Binary Search Tree is empty!");
        return max(root)->key;
    }

/***
 * Metoda zwraca maksymalna wysokość drzewa binarnego to jest długość
 * najdłuższej drogi w drzewie poczawszy od root aż do najodleglejszego liścia
 **/
template<typename K, typename V>
    int BinarySearchTree<K, V>::maxDepth(void) const
    {
        return maxDepth(root);
    }

/**
 * prywatna wersja funkcji maxDepth() wywoływna rekurencyjnie 
 **/
    
template<typename K, typename V>
int BinarySearchTree<K, V>::maxDepth(Node *node) const
{
    
    //1. Drzewo jest puste najdłuższa droga w drzewie ma długość 0
    if(node == NULL)
        return 0;
    
    //2. Wyznaczamy dlugosc najdluzszej drogi w kazdym z poddrzew (praw i lewe)
    int leftSubTreeMaxDepth = maxDepth(node->left);
    int rightSubTreeMaxDepth = maxDepth(node->right);
    
    //3. Wybieramy wieksza z powyzszych wartosci i zwiekszamy o 1 (aktualny wezel)
    if(leftSubTreeMaxDepth > rightSubTreeMaxDepth)
        return leftSubTreeMaxDepth + 1;
    //w przeciwnym wypadku wybieramy prawe poddrzewo
    return rightSubTreeMaxDepth +1;
}
/***
 * printTree() metoda wypisuje poszczególne klucze drzewa, oraz przypisane im wartosci
 * jako pary (k,v) w kolenosci rosnacych kluczy stosuje metode traverse in-order 
 * Jest to alians metody traverseInOrder()
 **/
template<typename K, typename V>
    void BinarySearchTree<K,V>::printTree(void) const
    {
        traverseInOrder();
    }
    
/***
 * traverseInOrder() przechodzi drzewo wypisujac pary (k,v) w kolejnosci in-order: 
 * 1. Przejdz lewe poddrzewo
 * 2. Odwiedź root 
 * 3. Przejdź prawe poddrzewo 
 **/
template<typename K, typename V>
    void BinarySearchTree<K,V>::traverseInOrder(void) const
    {
        traverseInOrder(root);
        std::cout << std::endl;
    }
/***
 * traverseInOrder(Node *node) przechodzi metoda in-order drzewo na ktore wskazuje node
 **/
template<typename K, typename V>
    void BinarySearchTree<K, V>::traverseInOrder(Node *node) const
    {
        //warunek stopu
        if(node == NULL) return;
        
        //1. przejdz lewe poddrzewo
        traverseInOrder(node->left);
        //2. odwiedz wezel root
        std::cout << (*node) << " ";
        //3. przejdz prawe poddrzewo
        traverseInOrder(node->right);
    }

/***
 * traversePreOrder() przechodzi drzewo wypisujac pary (k,v) w kolejnosci pre-order
 * 1. odwiedź węzeł root
 * 2. przejdź lewe poddrzewo
 * 3. przejdź prawe poddrzewo
 **/
template<typename K, typename V>
    void BinarySearchTree<K, V>::traversePreOrder(void) const
    {
        traversePreOrder(root);
        std::cout << std::endl;
    }
/***
 * traversePreOrder(Node *node) przechodzi metoda pre-order drzewo na ktore wskazuje 
 * node 
 **/
template<typename K, typename V>
    void BinarySearchTree<K, V>::traversePreOrder(Node *node) const
    {
        //warunek stopu
        if(node == NULL) return;
        
        //1. odwiedx wezel root
        std::cout << *node << " ";
        //2. przejdz lewe poddrzewo
        traversePreOrder(node->left);
        //3. przejdz prawe poddrzewo
        traversePreOrder(node->right);
    }
    
/***
 * traversePostOrder() przechodzi drzewo wypisujac pary (k,v) w kolejnosci post-order
 * 1. przejdz lewe poddrzewo
 * 2. przejdz prawe poddrzewo
 * 3. odwiedz wezel root
 **/
template<typename K, typename V>
    void BinarySearchTree<K, V>::traversePostOrder(void) const
    {
        traversePostOrder(root);
        std::cout << std::endl;
    }
    
/***
 * traversePostOrder(Node *node) przechodzi metoda post-order drzewo na ktore wskazuje
 * node 
 **/
template<typename K, typename V>
    void BinarySearchTree<K, V>::traversePostOrder(Node *node) const
    {
        //warunek stopu
        if(node == NULL) return;
        
        //1. przejdz lewe poddrzewo
        traversePostOrder(node->left);
        //2.przejdz prawe poddrzewo
        traversePostOrder(node->right);
        //3. odwiedz wezel root
        std::cout << *node << " ";
    }
    
/***
 * hasPathSum(int x) sprawdza czy w drzewie istnieje droga root-to-leaf dla ktorej suma
 * wartoci w węzłach jest równa x, jezeli tak to zwraca true wpw zwraca false
 **/
template<typename K, typename V>
    bool BinarySearchTree<K, V>::hasPathSum(int x) const
    {
        //wywołuje prywatna metode rekurencyjna o tej samej nazwie przekazujac
        //dodatkoy wskaznik na root biezacego drzewa
        return hasPathSum(root, x);
    }
    
/***
 * hasPathSum(Node *node, int x) sprawdza czy w poddrzewie wskazywanym przez node 
 * istnieje droga od root do liscia na ktorej suma wezlow równa jest x 
 * Sprawdzenia dokunuje sie poprzez rekurencyjne odejmowanie wartosci biezacego wezla 
 * az do momentu gdy dojdziemy do liscia node == NULL wtedy sprawdzamy trzy dla danej 
 * siezki otrzymano wartosc 0 po kolejnym odejmowniu wartosci kluczy w wezlach 
 **/
template<typename K, typename V>
    bool BinarySearchTree<K,V>::hasPathSum(Node *node, int x) const
    {
        //1. gdy doszlismy do konca drogi root-to-leaf sprawdzamy czy odejmujac
        //od poczatkowej wartosci x wartosci kluczy w poszczegolnych wezlach
        //otrzymano wartosc 0
        if(node == NULL) return x == 0;
        //2. wpw odejmujemy wartosc klucza w biezacym wezle
        x -= node->key;
        //rekurencyjnie sprawdzamy istnienie drogi o pomniejszonej wartosci x
        //w którym kolwiek z poddrzew (lewym lub prawym)
        return (hasPathSum(node->right, x) || hasPathSum(node->left, x));
    }
    
/***
 * isTreeSameAs(Node *otherTree) metoda spwawdza czy biezace drzewo jest takie 
 * samo jak drzewo przekazane jako argument otherTree. 
 * Sprawdzanie dokonywne jest zapośrednictwem prywantej metody sameTree(Node *, Node*)
 * przekazujac jako 1 parametr wskaznik root na korzen biezacego drzewa oraz 
 * wskaznik drzewa z ktorym mamy porownac 
 **/
template<typename K, typename V>
    bool BinarySearchTree<K,V>::isTreeSameAs(const BinarySearchTree<K,V> &otherTree) const
    {
        return sameTree(root, otherTree.root);
    }

/***
 * sameTree(Node *tree1, Node *tree2) powrownuje dwa drzewa wskazywane odpowiednio
 * przez wskazniki tree1 oraz tree2. Mozliwe przypadki to 
 * 1. oba drzew sa puste wtedy sa takie same
 * 2. obaw drzewa nie sa puste wtedy sprawdzamy czy zarówno ich prawe poddrzewo jak i
 * ich lewe poddrzewo sa takie same (rekurencyjnie) oraz czy maja ta sama pare (k,v)
 * w biezacym wezle
 * 3. jedno drzewo jest puste drugie nie
 **/
template<typename K, typename V>
    bool BinarySearchTree<K, V>::sameTree(Node *tree1, Node *tree2) const
    {
        //1. oba drzewa puste
        if(tree1 == NULL && tree2 == NULL) return true;
        //2. przypadek oba drzew niepuste
        else if(tree1!= NULL && tree2 != NULL) {
            //porownaj biezacy wezel oraz poszczególne poddrzewa
            bool cmpNode = tree1->key == tree2->key && tree1->value == tree2->value;
            //porownaj czy maja takie same lewe poddrzewa
            bool cmpLeftSubtree = sameTree(tree1->left, tree2->left);
            //porownaj czy maja takie same prawe poddrzewa
            bool cmpRightSubtree = sameTree(tree1->right, tree2->right);
            
            return cmpNode && cmpLeftSubtree && cmpRightSubtree;
        }
        //pozostal przypadek kiedy jedno drzewo jest puste a drugie nie!
        return false;
    }
    
/***
 * printPaths() metoda publiczna wypisujaca wszystkie sciezki od korzeni do lisci
 * korzysta z prywatnej metod o tej samej nazwie pobierajacej wskaźnik na wezel 
 * drzewa poczawszy od ktorego wypisac sciezki (tu root), przekazuje mu również tablice 
 * agregujaca wartosci wezlow na sciezce oraz licznik elementow tej tablicy
 **/
template<typename K, typename V>
    void BinarySearchTree<K,V>::printPaths(void) const{
        
        const Node *pathNodes[MAX_NODES_ON_PATH];
        int n = 0;
        printPaths(root, pathNodes, n);
    }
    
/***
 * printPaths(Node *node, const Node **pathNodes, int counter) to metoda prywatna
 * ktora wypisuje poszczegolne sciezki od korzenia-do-lisci 
 * 1. node wskaźnik na biezacy wezel 
 * 2. pathNodes tablica wezłów poczawszy od root do biezacego wezla node (bez niego)
 * 3. liczba elementów (wezlow) w tablicy)
 * Wezly sa agregowane w tablicy az do momentu dojscia do liscia 
 * wtedy zawartosc tablicy jest wypisywana 
 * sciezki wypisywane sa w kolejnosci od lewej do prawej
 * UWAGA! po wypisaniu najbardziej lewej sciezki wypisywane bedzie sciezka na pierwsza
 * na prawo przy czym dotychczasowe wezly w tablicy beda nadpisywane poczawszy od 
 * wezla w ktorym skrecimy w prawo  itd.
 ***/
template<typename K, typename V>
    void BinarySearchTree<K, V>::printPaths(Node *node, const Node **pathNodes, int counter) const
    {
        if(node == NULL) return;
        
        //1. dodaj biezacy wezel do tablicy
        pathNodes[counter++] = node;
        
        //Węzeł node jest lisciem
        if(node->left == NULL && node->right == NULL) {
            
            //2. wypisz zawartosc tablic z wezlami sciezki pathNodes
            for(int i=0; i< counter; i++)
            {
                std::cout << *pathNodes[i] << ", ";
            }
            
            std::cout << std::endl;
            
            return;
        }
        
        //jezeli wezel nie jest ani NULL ani nie jest lisciem
        //to wyszukaj i wypisz scieze najpierw z lewego potem z prawego poddrzewa
        printPaths(node->left, pathNodes, counter);
        printPaths(node->right, pathNodes, counter);
    }
    
} //ADT namespace

using namespace std;

int main(int argc, char **argv)
{
    cout << "Binary Search Tree Example" << endl;
    cout << "./bst < input.txt > output.txt" << endl;
    
    int inputSize = 0;
    //odczytuje rozmiar pliku wejsciowego
    scanf("%d", &inputSize);
    
    string *value = new string[inputSize];
    //wczytanie wszystkich wartosci przypisanych do kluczy
    for(int i=0; i<inputSize; i++)
    {
        cin >> value[i];
    }
    
    int *keys = new int[inputSize];
    
    //wczytanie wszystkich kluczy typu int
    for(int i=0; i<inputSize; i++)
    {
        scanf("%d", &keys[i]);
    }
    
    ADT::BinarySearchTree<int, std::string> bst;
    
    for(int i=0; i<inputSize; i++) {
        bst.insert(keys[i], value[i]);
    }
    
    cout << "printTree()" << endl;
    bst.printTree();
    
    cout << "traverseInOrder()"<< endl;
    bst.traverseInOrder();
    
    cout << "traversePreOrder()" << endl;
    bst.traversePreOrder();
    
    cout << "traversePostOrder()" << endl;
    bst.traversePostOrder();
    
    cout << "size()" << endl;
    cout << bst.size() << endl;
    
    cout << "lookup(5)" << endl;
    if(bst.lookup(5))
        cout <<  "TRUE" << endl;
    else
        cout << "FALSE" << endl;
    
    cout << "min()" << endl;
    cout << bst.min() << endl;
    
    cout << "max()" << endl;
    cout << bst.max() << endl;
    
    cout << "maxDepth()" << endl;
    cout << bst.maxDepth() << endl;
    
    cout << "hasPathSum(45)" << endl;
    if(bst.hasPathSum(45))
        cout << "TRUE" << endl;
    else
        cout << "FALSE" << endl;
    
    cout << "NEW TREE FROM THE SAME INPUT" << endl;
    
    ADT::BinarySearchTree<int, std::string> bst2;

    for(int i=0; i<inputSize; i++) {
        bst2.insert(keys[i], value[i]);
    }
    
    cout << "isTreeSameAs()" << endl;
    if(bst.isTreeSameAs(bst2))
        cout << "TRUE" << endl;
    else
        cout << "FALSE" << endl;
    
    cout << "printPaths()" << endl;
    bst.printPaths();
    
    return 0;
}