#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iostream>

namespace ADT {
    template<typename K, typename V>
    class BinarySearchTree;
}


template<typename K, typename V>
class ADT::BinarySearchTree
{
    struct Node;
    
    const int MAX_NODES_ON_PATH = 500;
    
    Node *root;
    
    //zagnieżdżona struktura węzła drzewa binarnego
    struct Node
    {
        K key;   //klucz węzła, typ K powinien implementować operator<
        V value; //wartość przypiasana do danego klucza
        Node *left, *right; //lewa i prawa gałąź bieżącego węzła
        int count; //liczba węzłów w poddrzewie dla którego ten węzeł to root
        
        Node(const K& k, const V& v, int n) :
                            key(k), value(v), count(n), left(0), right(0) { }
        
        //przeciazony operator << dla cout poszczegolnych wezłówk jako (k,v)
        friend std::ostream&
        operator<<( std::ostream& stream, Node const& n )
        {
            stream << "(" << n.key << "," << n.value << ") ";
            return stream;
        }
    };
    
    int size(Node *node) const;
    bool lookup(Node *node, const K& key) const;
    V *get(Node *node, const K& key);
    Node *insert(Node *node, const K& key, const V& value);
    const Node *min(Node *node) const;
    const Node *max(Node *node) const;
    void traverseInOrder(Node *node) const;
    void traversePreOrder(Node *node) const;
    void traversePostOrder(Node *node) const;
    int maxDepth(Node *node) const;
    bool hasPathSum(Node *node, int x) const;
    bool sameTree(Node *tree1, Node *tree2) const;
    void printPaths(Node *node, const Node **pathNodes, int counter) const;
    
public:
    int size(void) const;
    bool lookup(const K& key) const;
    V *get(const K&);
    void insert(const K& key, const V& value);
    const K& min(void) const;
    const K& max(void) const;
    int maxDepth(void) const;
    void printTree(void) const;
    void traverseInOrder(void) const;
    void traversePreOrder(void) const;
    void traversePostOrder(void) const;
    bool hasPathSum(int x) const;
    bool isTreeSameAs(const BinarySearchTree<K,V> &otherTree) const;
    void printPaths(void) const;
    
private:
    struct BinaryTreeException : public std::exception
    {
        std::string s;
        BinaryTreeException(std::string ss) : s(ss) {}
        ~BinaryTreeException() throw () {} // Updated
        const char* what() const throw() { return s.c_str(); }
    };
};




#endif //BINARY_SEARCH_TREE_H