#include<iostream>

using namespace std;

class BST {

    //Struktura węzła drzewa binarnego
    struct Node {
        int value;
        Node *left, *right;
        Node *parent; //mozna dodać pozwala łatwiej przeszukiwać drzewo
        //konstruktor węzła
        Node(int val, Node *p) : value(val), parent(p), left(NULL), right(NULL) { }
    };
    
    //Korzeń drzewa BST
    Node *root;
 
    /**
     * Pomocnicza prywatna metoda wyszukująca element 'val' w drzewie wskazywanym przez 'node'
     * Wywoływana jest rekurencyjnie przez publiczną
     * metodę o tej samej nazwie. Zaczynamy od przekazania ROOTa
     * i następnie przechodzimy drzewo rekurencyjnie aż do momentu
     * napotkania węzła NULL (brak elementu 'val' w drzewie) lub
     * do znalezienia elementu 'val' w drzewie. Jeżeli wartość 'val' jest mniejsza od
     * wartości w bieżącym węźle (node->value) to przeszukujemy rekurencyjnie lewe poddrzewo
     * w przeciwnym wypadku przeszukujemy prawe poddrzewo!
     * @node - korzeń bieżącego poddrzewa w którym wyszukujemy wartości
     * @val - szukana wartość
     **/
    bool Search(Node *node, int val) const
    {
        //1. drzewo BST jest puste! root == NULL
        if(node == NULL)
            return false;
        
        if(val == node->value)
            return true;
        else if(val < node->value)
            return Search(node->left, val);
        else
            return Search(node->right, val);
    }
    
    /**
     * Prywatna metoda pomocnicza wstawiania nowego węzła do drzewa BST 
     * Wywoływana rekurencyjnie przez metodę publiczną o tej samej nazwie. 
     * Wstawianie rozpoczynamy od ROOTa i rekurencyjnei przechodzimy do odpowiedniego 
     * liscia (leaf) w którym możemy wstawić nowy węzeł tak aby zachować własność 
     * drzewa przeszukiwania binarnego. Ścieżka poszukiwania liścia jest analogiczna jak w przypadku 
     * metody Search() tj. gdy 'val' < node->value to węzeł chcemy wstawić do lewego poddrzewa 
     * w przeciwnym wypadku do prawego poddrzewa
     * @node - korzeń bieżącego poddrzewa do którego wstawiamy nowy węzeł, aby móc wstawić powinno to byc 
     *         drzewo puste czyli liść 
     * @val - wstawiana wartość
     * @parent - węzeł rodzica bierzącego poddrzewa, dla całego drzewa rodzicem korzenia jest NULL
     **/
    Node *Insert(Node *node, int val, Node *parent) {
        
        //doszliśmy do liścia tworzymy nowy węzeł z przekazaną wartością
        if(node == NULL)
            return new Node(val, parent);
        
        if(val == node->value)
            node->value = val; //aktualizacja gdy wartość już istnieje w BST
        else if(val < node->value)
            node->left = Insert(node->left, val, node); //rekurencyjne wstawianie do lewego poddrzewa
        else
            node->right = Insert(node->right, val, node); //rekurencyjne wstawianie do prawego poddrzewa
        //zwracamy bieżący węzeł by można był podczepić go pod rodzica (który wywołał tą metodę rekurencyjnie)
        return node;
    }
    
    /**
     * Prywatna metoda pomocnicza która służy do usuwania elementu 'val' z drzewa BST
     * Wyszukuje elemtn o wartości 'val' w analogiczny sposób jak metoda Insert 
     * przechodząc rekurencyjnie odpowiednio do lewego lub prawego poddrzewa w zaleznosci od 
     * warunku 'val' < node->value. 
     * Następnie usuwa wg jedngo z 3 przypadków:
     * 1) usuwany węzeł W nie ma potomków -> wystarczy tylko usunąć 
     * 2) usuwany węzeł W ma jednego potomka -> usuwamy W i wstawiamy na jego miejsce potomka
     * 3) usuwany węzeł W ma dwóch potomków -> szukamy Max Element w lewym poddrzewie i zastępujamy 
     *                                         węzeł W znalezionym Max Elementem, usuwamy Max Element metoda 2)
     **/
    bool Remove(Node *node, int val, Node *parent)
    {
        //1. przeszliśmy całe drzewo i nie znaleźliśmy węzła zawierającego wartości 'val'
        if(node == NULL)
            return false;
        
        //2. rekurencyjnie poszukujemy wartości 'val' do usunięcia w lewym lub prawy  poddrzewie
        if(val < node->value)
            return Remove(node->left, val, node);
        else if(val > node->value)
            return Remove(node->right, val, node);
        else {
             //val == node->value
             //3. znaleśliśmy węzeł z wartością 'val', należy usunąć więce bieżący węzeł node jednym z powyższych sposobów
            
             //3.1 PUSTE LEWE PODDRZEWO
             if(node->left == NULL) {
                //usuwamny node, w miejsce node wstawiamy jego jedynego potomka node->right
                //     parent                           parent
                //     /   \                            /     \
                //    b     node         ------>       b      child
                //             \
                //              child
                //algorytm działa także dla przypadku gdy child == NULL! -> podpunkt 1) w opisie funkcji
                 
                //SPECJALNY PRZYPADEK dla node == ROOT & parent == NULL
                if(parent == NULL) {
                    //zastępowanie node <-- child
                    node->right->parent = parent; //child->parent = NULL
                    root = node->right;
                    
                    delete node;
                    return true;
                } else {
                    //node != ROOT
                    
                    //zastąpienie node <-- child
                    node->right->parent = parent; //child->parent = parent
                    
                    if(parent->left == node)
                        parent->left = node->right;
                    else
                        parent->right = node->right;
                    
                    delete node;
                    return true;
                }
                 
             }
             //3.2 PUSTE PRAWE PODDRZEWO
             else if(node->right == NULL) {
                 //usuwamy node, w miejsce node wstawiamy jego jedynego potomka node->left
                 //     parent                           parent
                 //     /   \                            /     \
                 //    b     node         ------>       b      child
                 //           /
                 //         child
                 //algorytm działa także dla przypadku gdy child == NULL! -> podpunkt 1) w opisie funkcji
                 
                 //SPECJALNY PRZYPADEK dla node == ROOT & parent == NULL
                 if(parent == NULL) {
                     //zastąpienie node <-- child
                     node->left->parent = parent; //child->parent = NULL
                     root = node->left;
                     
                     delete node;
                     return true;
                 } else {
                        //node != ROOT
                     
                        //zastąpienie node <-- child
                     node->left->parent = parent; //child->parent = parent
                     
                     if(parent->left == node)
                         parent->left = node->left;
                     else
                         parent->right = node->left;
                     
                     delete node;
                     return true;
                     
                 }
                 
             }
             //3.3 WĘZEŁ MA 2 NIEPUSTE PODDRZEWA
             else {
                 //usuwamy node, w jego miejsce wstawiamy Max Element z Lewego Poddrzewa
                 //     parent                           parent
                 //     /   \                            /     \
                 //    b     node         ------>       b      max
                 //           /  \                             /  \
                 //         c     d                           c    d
                 //       /   \                             /   \
                 //      e    max                          e   only (lub NULL)
                 //          /
                 //        only
                 
                 //Znajdujemy max element w lewym poddrzewie node
                 Node *max = MaxInTree(node->left);
                 
                 if(max->parent == node) {
                     //przypadek gdy max elementem jest korzeń lewego poddrzewa usuwanego węzła
                     node->left = max->left; //max == c, node->left = e
                     if(max->left != NULL) max->left->parent = node; //e->parent = node
                     node->value = max->value;
                     
                     delete max;
                     return true;
                 }
                 else {
                     //max element nie jest korzeniem lewego poddrzewa np. jak na rysunku powyżej
                     max->parent->right = max->left; //c->right = only
                     if(max->left != NULL) max->left->parent  = max->parent;  // only->parent = c
                     node->value = max->value;
                     
                     delete max;
                     return true;
                 }
             }
            
        }
        
        //w innych nieuzasadnionych przypadkach zwróć false
        return false;
    }
    
    /**
     * Pomocnicza metoda znajduje maxymalny element w drzewie tj. element najbardziej na prawo
     * w przpadku przekazania drzewa pustego jako argument zwraca pusty wezeł (NULL). 
     * @node - korzeń drzewa w którym szukamy maxymalnego elementu 
     * @return - węzeł maksymalnego elementu
     **/
    Node *MaxInTree(Node *node)
    {
        if(node == NULL)
            return NULL;  //puste drzewo zwracamy NULL
        
        while(node->right != NULL)
            node = node->right;
        
        return node;
    }
    
    void EmptyTree(Node *node)
    {
        //1. doszliśmy do pustego poddrzewa
        if(node == NULL)
            return;
        
        //2. wyczyść lewe i prawe poddrzewo
        EmptyTree(node->left);
        EmptyTree(node->right);
        
        //3. usun bieżące drzewo
        delete node;
        node = NULL;
    }
    
    /***
     * InOrder(Node *node) przechodzi metoda in-order drzewo na ktore wskazuje node
     **/
    
    void InOrder(Node *node) const
    {
        //warunek stopu
        if(node == NULL) return;
        
        //1. przejdz lewe poddrzewo
        InOrder(node->left);
        //2. odwiedz wezel root
        std::cout << node->value << " ";
        //3. przejdz prawe poddrzewo
        InOrder(node->right);
    }
    
public:
    
    //konstruktor domyślny tworzący puste drzewo BST
    BST()  {
        root = NULL;
    }
    
    //konstruktor tworzący drzewo BST na podstawie posortowanej tablicy
    BST(int *tab, int n) {
        
        //1. sortuj tablice
        //Sort(tab, n); - zakładamy, że inputowy tab jest juz posortowany
        
        //2. tworzenie drzewa BST na podstawie posortowanej tablicy
        cout << "Tworzenie Drzewa (kolejność wstawiania elementów tablicy): " << endl;
        root = Create(NULL, tab, 0, n-1);
        cout << endl;
    }
    
    /**
     * Pomocnicza metoda tworząca drzewo BST z posortowanej tablic 'tab' o 'n' elementach
     **/
    Node *Create(Node *parent, int *tab, int start, int end)
    {
        //indeks wartości środkowej tablicy
        int med = (start + end)/2;
        
        cout << med << " ";
        
        //tworzenie korzeń bieżącego drzewa na podstawie wartosci mediany i wskaznika na rodzica
        Node *node = new Node(tab[med], parent);
        
        if(start < med) {
            node->left = Create(node, tab, start, med-1);
        } else
            node->left = NULL;
        
        if(med < end)
            node->right = Create(node, tab, med+1, end);
        else
            node->right = NULL;
        
        return node;
    }
    
    /** 
     * Wstawianie elementu do drzewa binarnego BST
     * Korzysta z pomocniczej funkcji prywatnej wstawiajac rekurencyjnie
     * poczynając od korzenia 
     * @val - wartość którą chcemy wstawić do drzewa BST
     * @return void
     **/
    void Insert(int val)
    {
        //jako ostatni argument przekazujemy rodzica korzenia całego drzewa czyli NULL
        root = Insert(root, val, NULL);
    }
    
    /** 
     * Wyszukiwanie elementu w drzewie binarnym BST 
     * Korzysta z pomocniczej funkcji prywatnej wyszukując 
     * rekurencyjnie poczynając od korzenia 
     * @val - wartość którą wyszukujemy
     * @return zwraca wartość typu bool określająca powodzenie przeszukiwania elementu 'val'
     */
    bool Search(int val) const
    {
        return Search(root, val);
    }
    
    /**
     * Metoda usuwająca węzeł z wartości 'val' z drzewa BST
     * Funkcja przechodzi drzewo BST rekurencyjnie korzystając z 
     * metody prywatnej o tej samej nazwie Remove(Node *root, in val)
     * @val - wartość którą chcemy usunąć
     * @return  zwraca wartość true gdy usunięto lub false gdy nie usunięto element 'val'
     **/
    bool Remove(int val)
    {
        return Remove(root, val, NULL);
    }
    
    //destruktor drzewa
    ~BST()
    {
        //wywołujemy pomocniczą metodę prywatną EmptyTree()
        EmptyTree(root);
    }
    
    /***
     * InOrder()
     * 1. Przejdz lewe poddrzewo
     * 2. Odwiedź root
     * 3. Przejdź prawe poddrzewo
     **/
    void InOrder(void) const
    {
        InOrder(root);
        std::cout << std::endl;
    }
};


int main(int arc, char **argv)
{
    //Tablica posortowanych elementów
    int tab[] = { 1,4,6,8,10,11,20 };
    
    //Utworzenie drzewa BST
    BST *tree = new BST(tab, 7);
    
    cout << "InOrder()" << endl; 
    tree->InOrder();
    
    cout << "Szukanie Elementu 10:" << endl;
    cout << tree->Search(10) << endl;
    cout << "Szukanie Elementu 6:" << endl;
    cout << tree->Search(6) << endl;
    cout << "Usuwanie Elementu 11:" << endl;
    cout << tree->Remove(11) << endl;
    tree->InOrder();
    
    cout << "Szukanie Elementu 11:" << endl;
    cout << tree->Search(11) << endl;
    cout << "Szukanie Elementu 10:" << endl;
    cout << tree->Search(10) << endl;
    cout << "Szukanie Elementu 20:" << endl;
    cout << tree->Search(20) << endl;
    cout << "Usuwanie Elementu 10:" << endl;
    cout << tree->Remove(10) << endl;
    tree->InOrder();
    
    cout << "Szukanie Elementu 20:" << endl;
    cout << tree->Search(20) << endl;
    cout << "Dodanie Elementu 18..." << endl;
    tree->Insert(18);
    tree->InOrder();
    
    cout << "Szukanie Elementu 18:" << endl;
    cout << tree->Search(18) << endl;
    cout << "Usuwanie Elementu 20:" << endl;
    cout << tree->Remove(20) << endl;
    cout << "Szukanie Elementu 18:" << endl;
    cout << tree->Search(18) << endl;
    cout << "Szukanie Elementu 20:" << endl;
    cout << tree->Search(20) << endl;
    
    tree->InOrder();
    return 0;
}


//tab = { 1,4,6,8,9,10,11,20 }
//funkcja BinaryTree(int tab[]) na wejscie tablica
//chce stworzyc sobie zrównoważone drzewo binarne i potrzebuje
//szukania mediany np int v = Mediana(tab)
//mediana jest tylko jedna
//CreateNode(int mediana) tworzenie korzenia
//CreateTree(tab1), CreateTree(tab2)
//wywolania powinny podpinac drzewa w odpowiednich miejscach
//algorytm rekurencyjny jest krótki
//nie musze tworzyc pomocniczych tablic wystarczy operowac
//odpowiednio indeksami tablicy
//najprostrzy sposob wstawiania
//ten algorytm powoduje ze drzewo niezrównowazone
//do tego dodajemy opercje rownowazenia drzewa i juz super

//ZADANIE DOMOWE
//algorytm BST + dodawanie, usuwanie, wyszukiwanie
//elementy drzewa liczby,
//drzewo binarne nie musi byc zrownowazone
//jezeli zrownowazenie to 2pkt :)
//algorytm moze byc iteracyjny lub rekurencyjny
//poprawny nie moze byc tak ze po kasowaniu jest niespojne
//i nie da sie wyszukac elementu
