#include<string> 
#include<iostream>
#include<list>

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

namespace ADT {
    class HuffmanTree;
}


class ADT::HuffmanTree
{
    struct Node;
    
    private:
        static const int MAX_CODE_SIZE = 1000;
        static std::list<HuffmanTree*> tree_list;
    
        //korzeń drzewa Huffmana
        Node *root;
    
        //zagnieżdzona struktura węzła drzewa Huffmana
        struct Node
        {
            std::string symbol; //symbol przechowywany w węźle
                                //przechowuje symbol tylko w lisciu
                                //w pozostalych przypadkach pusty
            double probability; //wartosc prawdopodobieństwa
            //krawędzie do lewego i prawego węzła
            //węzły z symbolami beda lisciami i beda mialy
            //ustawione ponizsze wskazniki na NULL
            Node *left, *right;
            //odczytujac kod Huffmana z skonstruowanego drzewa
            //idac od korzenia do symbolu w lisciu
            //gdy idziemy w lewo dopisujemy 0 gdy w prawo 1 do kodu
            //długość kodu równa głębokości liscia w drzewie
            
            Node(double prob, std::string symb) :
                    symbol(symb), probability(prob), left(0), right(0) { }
        };
    public:
        //konstruktor tworzacy puste drzewo
        HuffmanTree() : root(0) { }
        //konstruktor tworzacy drzewo z jednym wezlem
        //na podstawie przekazanego symbolu i prawdopodobienstwa (liscie)
        HuffmanTree(double prob, std::string symb) {
            root = new Node(prob, symb);
        }
        //konstruktor tworzacy drzewo Huffmana z dwóch innych jako lewe
        //i praw poddrzewo w swoim korzeniu umieszcza sume ich
        //prawdopodobienstw
        HuffmanTree(const HuffmanTree& left, const HuffmanTree& right)
        {
            //obliczenie nowego prawdopodobieństwa
            double probability = left.root->probability + right.root->probability;
            if(probability > 1.0)
            std::cout << "Podczas konstruowania drzewa Huffmana uzyskano prawdopodobieństwo > 1.0" << std::endl;
            root = new Node(probability, "");
            root->left = left.root;
            root->right = right.root;
        }
    
    void printInOrder(void) const;
    
    static void add(std::string symbol, double probability);
    static HuffmanTree *buildHuffmanTree(void);
    static bool tree_probability_cmp(HuffmanTree *tree1, HuffmanTree *tree2);
    static void printHuffmanCodes(void);
    
    private:
    void traverseInOrder(Node *node) const; 
    static void printHuffmanCodes(Node *node, int*huffmanCode, int counter);
};

#endif// HUFFMAN_TREE_H