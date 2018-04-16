//
// Created by iancl on 10/15/2017.
//

#ifndef RED_BLACK_RED_BLACK_H
#define RED_BLACK_RED_BLACK_H

#include <iostream>
#include <stack>

using namespace std;
const bool BLACK = true, RED = false;

template <class T>
class RBTree;

template <class T>
class RBNode{
        T data;
        RBNode<T> * left, * right;
        bool color;
        friend class RBTree<T>;
    public:
        explicit RBNode(T d, RBNode<T> * l = NULL, RBNode<T> * r = NULL):
            data(d), left(l), right(r), color(RED){}
        ~RBNode(){
            if(left)
                delete left;
            if (right)
                delete right;
        }
        RBNode<T> * singleRight();
        RBNode<T> * singleLeft();
        RBNode<T> * leftRight();
        RBNode<T> * rightLeft();
        void insertRecolorFamily();
        bool removeRecolorFamily();
        static bool getColor(RBNode<T> * node){
            return node ? node->color : BLACK;
        }
        void printPreorder(ostream &os = cout, string indent = "") const;
};

template <class T>
class RBTree{
    RBNode<T> * root;
public:
    explicit RBTree() : root(NULL){}
    ~RBTree(){ if (root) delete root;}
    void insert( T const &);
    void remove(T const &);
    void insertBalanceTree(stack<RBNode<T> * > & nodes);
    void deleteRebalance(stack<RBNode<T> *> &nodes, RBNode<T> *&current, RBNode<T> *parent);
    void removeDoubleBlack(stack<RBNode<T> * > & nodes);
    void printPreorder(ostream & out = cout){if(this->root) this->root->printPreorder();}
};

#endif //RED_BLACK_RED_BLACK_H
