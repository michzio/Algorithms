#include "huffman_tree.h"
#include <algorithm>

namespace ADT {
    
    std::list<HuffmanTree*> HuffmanTree::tree_list;
    
    bool HuffmanTree::tree_probability_cmp(HuffmanTree *tree1, HuffmanTree *tree2)
    {
        return tree1->root->probability < tree2->root->probability;
        
    }
    
    /**
     *  Metoda dodaje nowy symbol do drzewa Huffmana 
     *  tworzy nowe drzewo jedno wezlowe (Liscia) z 
     *  symbolem i prawdopodobienstwem oraz umieszcza go na liscie
     **/
    void HuffmanTree::add(std::string symbol, double probability)
    {
        //utworzenie nowego drzewa liscia
        HuffmanTree *newTree = new HuffmanTree(probability, symbol);
        //umieszczenie na liscie
        tree_list.push_back(newTree);
    }
    
    /**
     * Metoda na podstawie drzew symboli zagregorwanych 
     * na liscie std::list<HuffmanTree*> buduje drzewo 
     * Huffmana pojedyncze wg: 
     * 1. znajdz dwa drzewa o najmniejszej wartosci 
     *    prawdopodbienstwa w korzeniu 
     * 2. zastap dwa drzewa jednym w którym stana sie one
     *    poddrzewami
     * 3. w korzeniu nowego drzewa umiesc wartosc prawdopodobienstwa
     *    równą sumie prawdopodobienstw z korzeni poddrzew
     * 4. powtarzaj 1-3 az na liscie zostanie jedno drzewo
     * Po skontruowaniu drzewa Huffmana mozemy z niego odczytywac
     * Jezeli drzewo zostalo skonstruowane zwraca go wpw null
     **/
    HuffmanTree *HuffmanTree::buildHuffmanTree(void) {
        
        int noOfTrees = tree_list.size(); //C++11 O(1)
        if(noOfTrees == 0) return NULL;
        
        //else build one tree form many tree leafs
        while(noOfTrees > 1) {
            
            HuffmanTree *minTree1;
            HuffmanTree *minTree2;
            
            //wybierz dwa drzewa o minimalnym prawdopodobienstwie i usun z listy
            auto minItr = std::min_element(tree_list.begin(), tree_list.end(), tree_probability_cmp);
            minTree1 = *minItr;
            tree_list.erase(minItr);
            minItr = std::min_element(tree_list.begin(), tree_list.end(),
                                      tree_probability_cmp);
            minTree2 = *minItr;
            tree_list.erase(minItr);
            
            //utwórz nowe drzewo z dwóch pobranych z listy
            HuffmanTree *newTree = new HuffmanTree(*minTree1, *minTree2);
            tree_list.push_back(newTree);
            
            noOfTrees = tree_list.size(); //C++11 O(1)
        }
        
        return tree_list.front();
        
    }
    
    /***
     * Metoda statyczna wypisuje kody Huffmuna dla wszystkich symboli
     * zdefiniowanych w zbudowanym drzewie Huffmana przechodzi do 
     * lisci z symbolami na zasadzie przechodzenia sciezek w drzewie binarnym
     * Jezeli drzewo Huffmana nie zostalo poprawnie skonstruowane za pomoca
     * methody buildHuffmanTree() tj w root-cie nie ma prawdopodobienstwa 1.0
     * to wypisuje komunikat bledu
     **/
    void HuffmanTree::printHuffmanCodes(void)
    {
        //pobiramy z listy drzewo Huffmana
        HuffmanTree *tree = tree_list.front();
        //sprawdzamy czy jest poprawne
        if(tree->root->probability != 1.0) {
            std::cout << "Niepoprawne drzewo Huffmana nie mozna wypisac kodów!"
                        << std::endl;
            return;
        }
        
        //wywolanie rekurencyjnej metody wypisywania kodów Huffmana
        int n = 0; int huffmanCode[MAX_CODE_SIZE];
        printHuffmanCodes(tree->root, huffmanCode, n);
        
    }
    
    void HuffmanTree::printHuffmanCodes(Node *node, int *huffmanCode, int counter)
    {
        if(node == NULL) return;
        
        if(node->left == NULL && node->right == NULL) {
            
            //Lisc mozemy odczytac symbol + wypisać dla niego kod Huffmana
            std::cout << "(" << node->symbol << ", ";
            //wypisz kod Huffmana
            for(int i=0; i < counter; i++)
            {
                std::cout << huffmanCode[i];
            }
            
            std::cout << ")" << std::endl;
            
            return;
        }
        
        //jezeli wezel nie jest ani null ani nie jest lisciem
        //do idziemy lewa krawedzia dodajac do kodu 0
        //lub prawa krawedzia dodajac do kodu 1
        huffmanCode[counter] = 0;
        printHuffmanCodes(node->left, huffmanCode, counter+1);
        huffmanCode[counter] = 1;
        printHuffmanCodes(node->right, huffmanCode, counter+1);
    }
    
    /***
     * traverseInOrder() przechodzi drzewo wypisujac pary (k,v) w kolejnosci in-order:
     * 1. Przejdz lewe poddrzewo
     * 2. Odwiedź root
     * 3. Przejdź prawe poddrzewo
     **/
    void HuffmanTree::printInOrder(void) const
    {
        traverseInOrder(root);
        std::cout << std::endl;
    }
    /***
     * traverseInOrder(Node *node) przechodzi metoda in-order drzewo na ktore wskazuje node
     **/
    void HuffmanTree::traverseInOrder(Node *node) const
    {
        //warunek stopu
        if(node == NULL) return;
        
        //1. przejdz lewe poddrzewo
        traverseInOrder(node->left);
        //2. odwiedz wezel root
        std::cout << "(" << node->symbol << ", " << node->probability << ")" << ", ";
        //3. przejdz prawe poddrzewo
        traverseInOrder(node->right);
    }

}


int main(int argc, char **argv)
{
    int K = 10; //ilosc słów
    
    //wczytywanie liczby słów
    scanf("%d", &K);
    
    //tablica słów i czestotliwosci wystapien itego slowa
    std::string *S = new std::string[K];
    int *P = new int[K];
    //wczytanie K słów do tablicy S
    for(int i=0; i< K; i++)
        std::cin >> S[i];
    //wczytanie K czestotliwosci do tablicy P
    for(int i=0; i<K; i++)
    scanf("%d", &P[i]);
    
    int sumP = 0;
    for(int i=0; i < K; i++) {
        sumP += P[i];
    }
    
    for(int i=0; i < K; i++) {
        //prawdopodobienstow P[i]/sumP
        ADT::HuffmanTree::add(S[i], (double)P[i]/sumP);
    }
    //budujemy drzewo Huffmana z poddrzew lisci
    std::cout << "PRZEJSCIE INORDER" << std::endl;
    ADT::HuffmanTree *tree = ADT::HuffmanTree::buildHuffmanTree();
    tree->printInOrder();
    //wypisujemy kody Huffmana dla poszczególnych symboli
    std::cout << "KODY HUFFMANA" << std::endl;
    ADT::HuffmanTree::printHuffmanCodes();
    
    return 1;
}