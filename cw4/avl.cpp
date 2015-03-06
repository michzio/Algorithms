#include<iostream>

/**
 * AVL - BRAKUJE IMPLEMENTACJI RÓWNOWAŻENIA (ROTACJI) PO USUNIĘCIU WĘZŁÓW W FUNKCJI REMOVE
 **/

using namespace std;

enum Subtree { LEFT_SUBTREE, RIGHT_SUBTREE };

class AVL {

    //Struktura węzła drzewa binarnego
    struct Node {
        int value;
        int balance; //balance = h_right(node) - h_left(node) -> drzewo AVL (zrównoważone) gdy [-1,0,1]
        Node *left, *right;
        Node *parent; //mozna dodać pozwala łatwiej przeszukiwać drzewo
        //konstruktor węzła
        Node(int val, Node *p) : value(val), parent(p), left(NULL), right(NULL) { }
    };
    
    //Korzeń drzewa BST
    Node *root;
    
    //4 RODZAJE ROTACJI
    
    /**
     * Rotacja w lewo (Left-Left) 
     * Gdy 3 węzły łaczą się lewymi krawędziami.
     * Węzeł b(A) = 2, b(B) = 1, (A, B kolejne węzły połaczone prawymi krawędziami)
     *        Parent                                      Parent
     *          |                                           |
     *          A (-2)                                      B (0)
     *        /   \                                       /   \
     *      B (-1) Right(A)          ----->           Left(B)  A (0)
     *     /  \      (h)                               (h+1)  /    \
     * Left(B) Right(B)                                  Right(B)  Right(A)
     * (h+1)     (h)                                       (h)      (h)
     **/
    Node *RotateLeft(Node *A)
    {
        Node *B = A->left;
        
        A->left = B->right; //A->left = Right(B)
        if(A->left != NULL) A->left->parent = A;//Right(B)->parent = A
        //A->right = Right(A)
        
        B->right = A;
        //B->left = Left(B)
        
        B->parent = A->parent;
        A->parent = B;
        if(B->parent != NULL) {
            if(B->parent->left == A)
                B->parent->left = B;
            else
                B->parent->right = B;
        } else {
            root = B;
        }
        
        //wszpółrzynniki zrównoważenia węzłów (A,B): (-2,-1) -> (0,0) lub (-2,0) -> (-1, 1)
        if(B->balance) {
            A->balance = 0;
            B->balance = 0;
        } else
        {
            A->balance = -1;
            B->balance = 1;
        }
        
        
        return B;
        
    }
    
    /**
     * Rotacja w prawo (right-right)
     * Gdy 3 węzły łączą się prawymi krawędziami.
     * Węzeł b(A) = 2, b(B) = 1, (A, B kolejne węzły połaczone prawymi krawędziami)
     *        Parent                                      Parent
     *          |                                           |
     *          A (2)                                       B (0)
     *        /   \                                       /   \
     *   Left(A)   B (1)           ----->            (0) A    Right(B)
     *    (h)     /  \                                 /   \     (h+1)
     *       Left(B)  Right(B)                    Left(A)   Left(B)
     *         (h)        (h + 1)                  (h)        (h)
     **/
    Node *RotateRight(Node *A)
    {
        Node *B = A->right;
       // Node *Parent = A->parent;
        
        A->right = B->left; //A->right = Left(B)
        if(A->right != NULL) A->right->parent = A; //Left(B)->parent = A
        //A->left = Left(A)
        
        B->left = A;
        //B->right = Right(B)
        
        B->parent = A->parent;
        A->parent = B;
        if(B->parent != NULL) {
            if(B->parent->left == A)
                B->parent->left = B;
            else
                B->parent->right = B;
        } else {
            root = B; //jezeli węzeł A był korzeniem całego drzewa AVL
                      //to zmieniamy aby root wskazywał teraz na B
        }
        
        //wszpółrzynniki zrównoważenia węzłów (A,B): (2,1) -> (0,0) lub (2,0) -> (1, -1)
        if(B->balance == 1)
        {
            A->balance = 0;
            B->balance = 0;
        } else {
            A->balance = 1;
            B->balance = -1;
        }
        
        return B;
    }
    
    /**
     * Rotacja podwójna prawo-lewo (right-left)
     * Gdy 3 węzły łączą się odpowiednio prawą i lewą krawędzią 
     **/
    Node *RotateRightLeft(Node *A)
    {
        //1. rotacja w lewo
        RotateLeft(A->right);
        
        //2. rotacja w prawo
        Node *C = RotateRight(A);
        
        //dostosowanie współczynników równoważności
        C->left->balance = (C->balance ==  1) ?  -1 : 0;
        C->right->balance = (C->balance ==  -1) ?  1 : 0;
        C->balance = 0;
        
        return C;
    }
    
    /**
     * Rotacja podwójna lewo-prawo (left-right)
     * Gdy 3 węzły łączą się odpowiednio lewą i prawą krawędzią 
     **/
    Node *RotateLeftRight(Node *A)
    {
        //1. rotacja w prawo
        RotateRight(A->left);
        
        //2. rotacja w lewo
        Node *C = RotateLeft(A);
        
        //dostosowanie współczynników równoważności
        C->left->balance = (C->balance == -1) ?  1 : 0;
        C->right->balance = (C->balance ==  1) ? -1 : 0;
        C->balance = 0;
        
        return C;
    }
 
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
    Node *Insert(Node *node, int val, Node *parent, bool *rotated) {
        
        //doszliśmy do liścia tworzymy nowy węzeł z przekazaną wartością
        if(node == NULL) {
            Node *newNode = new Node(val, parent);
            newNode->balance = 0; //ustawiamy balance liścia
            //ustawienie balancu rodzica!
            return newNode;
        }
        
        if(val == node->value)
            node->value = val; //aktualizacja gdy wartość już istnieje w BST
        else if(val < node->value) {
            node->left = Insert(node->left, val, node, rotated); //rekurencyjne wstawianie do lewego poddrzewa
            
            if(node->left->value == val) {
                //node to bezpośredni rodzic wstawionego węzła!
                node->balance = (node->balance == 0) ? -1 : 0;
                if(node->balance == 0) {
                    *rotated = true; //na tym etapie można już zakończyć równoważenie
                }
            } else {
            //sprawdzamy kolejne węzły na ścieżce dodawania nowego węzła
            //idąc w kierunku roota, przekazujemy flage oznaczajaca ze dodano
            //wezel w lewym poddrzewie!
                if(! *rotated)
                    node = checkAVLBalance(node, LEFT_SUBTREE, val, rotated);
            }
            
        } else {
            node->right = Insert(node->right, val, node, rotated); //rekurencyjne wstawianie do prawego poddrzewa
            
            if(node->right->value == val) {
                //node to bezpośredni rodzic wstawionego węzła!
                node->balance = (node->balance == 0) ? 1 : 0;
                if(node->balance == 0) {
                    *rotated = true; //na tym etapie można już zakończyć równoważenie
                }
            } else {
            //sprawdzamy kolejne węzły na ścieżce dodawania nowego węzła
            //idąc w kierunku roota, przekazujemy flage oznaczajaca ze dodano
            //wezel w prawym poddrzewie!
                if(! *rotated)
                    node = checkAVLBalance(node, RIGHT_SUBTREE, val, rotated);
            }
        }
    
        
        //zwracamy bieżący węzeł by można był podczepić go pod rodzica (który wywołał tą metodę rekurencyjnie)
        return node;
    }
    
    Node *checkAVLBalance(Node *node, Subtree sub, int val, bool *rotated)
    {
        //metoda sprawdza równowage węzła w drzewie AVL
        
        
        //aktualizacja współczynnika zrównoważenia węzła gdy miał wartość 0
        if(node->balance == 0) {
            //gdy nowy węzeł dodan w lewym poddrzewie
           if(sub == LEFT_SUBTREE)
               node->balance = -1;
           else
               node->balance = 1;
        } else if(node->balance == 1) {
            //gdy węzeł miał wartość 1! dodanie nowego wezła mogło doprowadzić do utraty równowagi drzewa
            if(sub == LEFT_SUBTREE) {
                node->balance = 0;
                *rotated = true; //!!!
            } else
                //POTRZEBNA ROTACJA!
                if(node->right->balance == -1) {
                    cout << "DLA: " << val << " ROTACJA RightLeft" << endl;
                     *rotated = true;
                    return RotateRightLeft(node);
                    
                } else {
                    cout << "DLA: " <<val << " ROTACJA RightOnce" << endl;
                    *rotated = true;
                    return RotateRight(node);
                    //InOrder();
             }
            
        } else { //node->balance == -1
            //gdy węzeł miał wartość -1! dodanie nowego wezła mogło doprowadzić do utraty równowagi drzewa
            if(sub == RIGHT_SUBTREE) {
                node->balance = 0;
                *rotated = true; //!!!
            } else
                //POTRZEBNA ROTACJA
                if(node->left->balance == 1) {
                    cout << "DLA: " << val << " ROTACJA LeftRight" << endl;
                    *rotated = true;
                    return RotateLeftRight(node);
                    
                } else {
                    cout << "DLA: " << val << " ROTACJA LeftOnce" << endl;
                    *rotated = true;
                    return RotateLeft(node);
                }
        }
        
        return node;
        
    }
    
    /**
     * Prywatna metoda pomocnicza która służy do usuwania elementu 'val' z drzewa BST
     * Wyszukuje element o wartości 'val' w analogiczny sposób jak metoda Insert
     * przechodząc rekurencyjnie odpowiednio do lewego lub prawego poddrzewa w zaleznosci od 
     * warunku 'val' < node->value. 
     * Następnie usuwa wg jedngo z 3 przypadków:
     * 1) usuwany węzeł W nie ma potomków -> wystarczy tylko usunąć 
     * 2) usuwany węzeł W ma jednego potomka -> usuwamy W i wstawiamy na jego miejsce potomka
     * 3) usuwany węzeł W ma dwóch potomków -> szukamy Max Element w lewym poddrzewie i zastępujamy 
     *                                         węzeł W znalezionym Max Elementem, usuwamy Max Element metoda 2)
     **/
    bool Remove(Node *node, int val, Node *parent, bool *balanced)
    {
        
        //1. przeszliśmy całe drzewo i nie znaleźliśmy węzła zawierającego wartości 'val'
        if(node == NULL)
            return false;
        
        //2. rekurencyjnie poszukujemy wartości 'val' do usunięcia w lewym lub prawy  poddrzewie
        if(val < node->value) {
            bool removedFlag =  Remove(node->left, val, node, balanced);
        
            //gdy usunięto węzeł w lewym poddrzewie node to należy sprawdzić zrównoważenie
            //bieżącego węzła node
            if(removedFlag && ! *balanced)
                checkAVLBalanceAfterRemoving(node, LEFT_SUBTREE, val, balanced);
            return removedFlag;
        } else if(val > node->value) {
            bool removedFlag = Remove(node->right, val, node, balanced);
            
            //gdy usunięto węzeł w prawym poddrzewie node to należy sprawdzić zrównoważenie
            //bieżącego węzła node
            if(removedFlag && ! *balanced)
                checkAVLBalanceAfterRemoving(node, RIGHT_SUBTREE, val, balanced);
            return removedFlag;
        } else {
            
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
                    if(node->right != NULL)
                        node->right->parent = parent; //child->parent = parent
                    
                    if(parent->left == node) {
                        parent->left = node->right;
                    } else {
                        
                        parent->right = node->right;
                    }
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
                     if(node->left !=NULL)
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
                     
                     //sprawdzamy zrównoważenie rodzica usuwanego węzłą max tutaj usuwany węzeł node
                     //max jest usuwany z lewego węzła node ->dostosowanie zrównoważenie node konieczne
                     checkAVLBalanceAfterRemoving(node, LEFT_SUBTREE, val, balanced);
                     
                     return true;
                 }
                 else {
                     //max element nie jest korzeniem lewego poddrzewa np. jak na rysunku powyżej
                     max->parent->right = max->left; //c->right = only
                     if(max->left != NULL) max->left->parent  = max->parent;  // only->parent = c
                     node->value = max->value;
                     
                     Node *maxParent = max->parent;
                     delete max;
                     
                     //musimy przejść rekurencyjnie całe lewe poddrzewo aż do węzła node
                     //tak by po usunięciu węzła max (najbardziej prawego w lewym poddrzewie)
                     //zrównoważyć poszczególne węzł włacznie z węzłem node
                     //dla wszystki przypadków mamy do czynienia ze zmniejszeniem wysokości prawego
                     //poddrzewa oprócz przypadku gdy maxParent == node -> wtedy zmniejszenie wysokosci
                     //lewego poddrzewa
                     checkAVLBalanceInLeftSubtree(maxParent, node, val);
                     
                     return true;
                 }
             }
            
        }
        
        //w innych nieuzasadnionych przypadkach zwróć false
        return false;
    }
    
    /**
     * funkcja pomocnicza wywoływana podczas gdy usuwamy node który 
     * posiada lewe i prawe poddrzewo, w takim przypadku 
     * node było zastepnowane max elementem w lewym poddrzewie
     * max element był usuwany i należy sprawdzić zrównoważenie 
     * tego lewego poddrzewa node 
     * Parametry 
     * parent -> maxParent - rodzic elementu max który usunięto z lewego poddrzewa
     * node - właściwy usuwany węzeł przechodzimy aż do tego węzła sprawdzając zrównoważenie
     *        później już zrównowazenie będzie sprawdzane rekurencyjnie w funkji remove! 
     * val - wartość usuwana TYLKO w celach testowych do wypisania komunikatów
     **/
    void checkAVLBalanceInLeftSubtree(Node *parent, Node *node, int val)
    {
        bool balanced = false;
        
        do {
        //zrownowaz parent dla którego usunieto wezel w prawym poddrzewie!
        parent = checkAVLBalanceAfterRemoving(parent, RIGHT_SUBTREE, val,  &balanced);
        
        //gdy w wyniku procesu równoażenie udało sie poprawnie zrównoważyć i dalsze
        //równoważenie nie jest już konieczne!
        if(balanced)
            return;
            
        //po dokonaniu zrównoważenia przejdź w górę
        parent = parent->parent;
        
        } while(parent != node); //postępuj tak aż dojdziesz do węzła node
        
        //doszliśmy do węzła node (właśiwy węzeł usuwany przez funkcję Remove())
        //w tym przypadku zmniejszeniu uległa wysokośc lewego poddrzewa
        checkAVLBalanceAfterRemoving(node, LEFT_SUBTREE, val,  &balanced);
    }
    
    Node *checkAVLBalanceAfterRemoving(Node *node, Subtree sub, int val, bool *balanced)
    {
        
        //sytuacja gdy skróceie poddrzewa node nie powoduje
        //skrócenia wysokości całego drzewa o korzeniu node
        if(node->balance == 0)
        {
            if(sub == LEFT_SUBTREE)
                //zmniejszeniu uległo lewe poddrzewo
                node->balance = 1;
            else if(sub == RIGHT_SUBTREE)
                //zmniejszeniu uległo prawe poddrzewo
                node->balance = -1;
            *balanced = true;
            return node;
        } else if( (node->balance == 1 && sub == RIGHT_SUBTREE)
                || (node->balance == -1 && sub == LEFT_SUBTREE)) {
            //skrócone zostało cięższe poddrzewo
            node->balance = 0;
            //tym razem drzewo nie jest jeszcze zrównoważone i nie ustawiamy flagi balanced na 'true'
            //całe drzewo z korzeniem jako node ma teraz wysokość mniejsza o 1 niezrównoważenie
            //mogło się przenieść na wyższe węzły w kierunku root'a całego drzewa AVL
            return node;
        } else if( (node->balance == -1 && sub == RIGHT_SUBTREE)
                || (node->balance = 1 && sub == LEFT_SUBTREE) ){
           //skrócone zostało lżejsze poddrzewo
            
           //pobieramy wskaźnik na cięższe poddrzwo
            Node *heavySubtree = (sub == RIGHT_SUBTREE) ? node->left : node->right;
            
           //należy rozważyć 3 przypadki w cięższym poddrzewie
           //A. Współczynnik zrównoważenia (balance) w korzeniu cięższego poddrzewa
           //   jest równy 0 to należy wykonać rotację pojedynczą RightOnce lub LeftOnce
           //   w zależności od wartości współczynnika balance w bieżącym węźłe node
            if(heavySubtree->balance == 0)
            {
                //w wyniku rotacji wysokość drzewa z korzeniem node nie ulegnie zmianie?
                *balanced = true;
                
                if(node->balance == 1) //cięższe prawe poddrzewo
                {
                    cout << "DLA: " << val << " ROTACJA RightOnce" << endl;
                    return RotateRight(node);
                }
                else //node->balance == -1, cięższe lewe poddrzewo
                {
                    cout << "DLA: " << val << " ROTACJA LeftOnce" << endl;
                    return RotateLeft(node);
                }
                
            }
            
            //B. Współczynnik zrównoważenia węzła node i korzenia cięższego poddrzewa
            //   mają taką samą wartość.
            else if(heavySubtree->balance == node->balance) {
                
                //dla przypadku gdy cięższe jest prawe poddrzewo
                //-> rotacja pojedyncza RightOnce
                if(node->balance == 1)
                {
                    cout << "DLA: " << val << " ROTACJA RightOnce" << endl;
                    
                    return RotateRight(node);
                    //drzewo z korzeniem node zmieniło swoją wysokość
                    //w wyniku rotacji, niezrównoważenie może przeniśc się na wyższe
                    //węzły, nie mamy jeszcze zabalansowanego całego drzewa AVL
                } else {
                    //dla przypadku gdy cięższe jest lewe poddrzewo
                    //-> toracja pojedyncza LeftOnce
                    cout << "DLA: " << val << " ROTACJA LeftOnce" << endl;
                    
                    return RotateLeft(node);
                    //drzewo z korzeniem node zmienio swoją wysokość
                    //w wuniku rotacji, niezrównoważenie może przenieśc się na wyższe
                    //węzły, nie mamy jeszcze balanswoanego całego drzewa AVL
                }
            }
            //C. Współcznynnik zrównoważenia węzła node i korzenia cięższego poddrzewa
            //   mają przeciwna wartość 1 <-> -1 lub -1 <->  1
             else {
            
                 //gdy cięższe prawe poddrzewo -> podwójna rotacja
                 if(node->balance == 1)
                 {
                     cout << "DLA: " << val << " ROTACJA RightLeft" << endl;
                     
                     return RotateRightLeft(node);
                 } else {
                     cout << "DLA: " << val << " ROTACJA LeftRight" << endl;
                     
                     return RotateLeftRight(node);
                 }
                 //w wyniku rotacji uległo zmniejszenie wysokości drzewa
                 //o korzeniu w węźle node, stąd niezrównoważenie może
                 //propagować na węzły leżące wyżej na ścieżce w kierunku root'a
                 //należy kontynuować sprawdzanie zrównoważenia węzłów drzewa AVL
             }
        }
        
        return node;
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
        std::cout << node->value << "(" << node->balance << ")" << " ";
        //3. przejdz prawe poddrzewo
        InOrder(node->right);
    }
    
    
    /**
     * prywatna wersja funkcji Height() wywoływna rekurencyjnie
     **/
    
    int Height(Node *node) const
    {
        
        //1. Drzewo jest puste najdłuższa droga w drzewie ma długość 0
        if(node == NULL)
            return 0;
        
        //2. Wyznaczamy dlugosc najdluzszej drogi w kazdym z poddrzew (praw i lewe)
        int leftSubTreeHeight = Height(node->left);
        int rightSubTreeHeight = Height(node->right);
        
        //3. Wybieramy wieksza z powyzszych wartosci i zwiekszamy o 1 (aktualny wezel)
        if(leftSubTreeHeight > rightSubTreeHeight)
            return leftSubTreeHeight + 1;
        //w przeciwnym wypadku wybieramy prawe poddrzewo
        return rightSubTreeHeight +1;
    }

public:
    
    //konstruktor domyślny tworzący puste drzewo BST
    AVL()  {
        root = NULL;
    }
    
    //konstruktor tworzący drzewo BST na podstawie posortowanej tablicy
    AVL(int *tab, int n) {
        
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
        bool rotated = false;
        //jako ostatni argument przekazujemy rodzica korzenia całego drzewa czyli NULL
        root = Insert(root, val, NULL, &rotated);
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
        bool balanced = false;
        return Remove(root, val, NULL, &balanced);
    }
    
    //destruktor drzewa
    ~AVL()
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
    
   
    int Height(void) const
    {
        //prywatna wersja funkcji Height zwraca liczbę węzłów w najdłuższej ścieżce
        //musimy odjąć jeden węzeł by uzyskać liczbę krawędzi na tej najdłuższej ścieżce
        return Height(root) -1;
    }
};


int main(int arc, char **argv)
{
    //Tablica posortowanych elementów
    //int tab[] = { 1,4,6,8,10,11,20 };
    
    //Utworzenie drzewa BST
    AVL *tree = new AVL();
    
    /*tree->Insert(1);  tree->InOrder();
    tree->Insert(4);  tree->InOrder();
    tree->Insert(6);  tree->InOrder();
    tree->Insert(8);  tree->InOrder();
    tree->Insert(10);  tree->InOrder();
    tree->Insert(11);  tree->InOrder();
    tree->Insert(20);  tree->InOrder();
     */
    //przykład z wykładów ostatni slajd z rotacja podwójną 
    tree->Insert(4); tree->InOrder();
    tree->Insert(3); tree->InOrder();
    tree->Insert(9); tree->InOrder();
    tree->Insert(2); tree->InOrder();
    tree->Insert(7); tree->InOrder();
    tree->Insert(12); tree->InOrder();
    tree->Insert(5); tree->InOrder();
    tree->Insert(8); tree->InOrder();
    tree->Insert(11); tree->InOrder();
    tree->Insert(6); tree->InOrder();
    
    
    cout << "InOrder()" << endl; 
    tree->InOrder();
    
    cout << "WYSOKOSC: " << tree->Height() << endl;
    
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
    
    cout << "WYSOKOSC: " << tree->Height() << endl;

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
