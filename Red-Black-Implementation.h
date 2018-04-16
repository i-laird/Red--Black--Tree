//
// Created by iancl on 10/16/2017.
//

#ifndef RED_BLACK_RED_BLACK_IMPLEMENTATION_H
#define RED_BLACK_RED_BLACK_IMPLEMENTATION_H

#include "Red-Black.h"
#include <queue>
#include <string>

template <class T>
RBNode<T> * RBNode<T>::singleRight(){
    RBNode<T> * temp = this->left;
    this->left = temp->right;
    temp->right = this;
    swap(this->color, temp->color);
    return temp;
}

template <class T>
RBNode<T> * RBNode<T>::singleLeft(){
    RBNode<T> * temp = this->right;
    this->right = temp->left;
    temp->left = this;
    swap(this->color, temp->color);
    return temp;
}

template <class T>
RBNode<T> * RBNode<T>::leftRight(){
    this->left = this->left->singleLeft();
    return this->singleRight();
}

template <class T>
RBNode<T> * RBNode<T>::rightLeft(){
    this->right = this->right->singleRight();
    return this->singleLeft();
}

template <class T>
void RBNode<T>::insertRecolorFamily(){
    //Reversing the colors of the children and this Node
    //The black is getting pulled down to the children
    this->color = !this->color;
    if(this->left)
        this->left->color = !this->left->color;
    if(this->right)
        this->right->color = !this->right->color;
}

template <class T>
bool RBNode<T>::removeRecolorFamily(){
    if(this->left)
        this->left->color = RED;
    if(this->right)
        this->right->color = RED;
    if(this->color == BLACK)
        return true;
    return false;
}

template <class T>
void RBTree<T>::insert( T const & toEnter){
    //Find place to insert the Node
    RBNode<T> * insertLoc = this->root, * parent = this->root;
    stack<RBNode<T> *> previousLocs;
    bool found= false;
    while(insertLoc && !found){
        parent = insertLoc;
        previousLocs.push(insertLoc);
        if(toEnter < insertLoc->data)
            insertLoc = insertLoc->left;
        else if(insertLoc->data < toEnter)
            insertLoc = insertLoc->right;
        else
            found = true;
    }
    //Each data member may only be present once
    if(!found){
        if(insertLoc == parent) {
            root = new RBNode<T>(toEnter);
            root->color = BLACK;
        }
        else{
            if(toEnter < parent->data) {
                parent->left = new RBNode<T>(toEnter);
                previousLocs.push(parent->left);
            }
            else {
                parent->right = new RBNode<T>(toEnter);
                previousLocs.push(parent->right);
            }
            insertBalanceTree(previousLocs);
        }
    }
}

template <class T>
void RBTree<T>::insertBalanceTree(stack<RBNode<T> * > & nodes){
    RBNode<T> * current = nodes.top(), * parent, * uncle, * grandParent = NULL;
    nodes.pop();
    bool emptyNow = false;
    if(!nodes.empty()) {
        parent = nodes.top();
        nodes.pop();
        if (!nodes.empty()) {
            grandParent = nodes.top();
            nodes.pop();
            uncle = (parent == grandParent->left ? grandParent->right : grandParent->left);
            if (nodes.empty())
                emptyNow = true;

            if (parent->color == RED) {
                //See if a recoloring will fix the problem
                if (RBNode<T>::getColor(uncle) == RED) {
                    grandParent->insertRecolorFamily();
                    nodes.push(grandParent);
                    nodes.push(parent);
                    if(!emptyNow)
                        insertBalanceTree(nodes);
                    this->root->color = BLACK;
                }
                    //If not a rotation is needed
                else {
                    if (parent == grandParent->left && current == parent->left)
                        grandParent = grandParent->singleRight();
                    else if (parent == grandParent->left)
                        grandParent = grandParent->leftRight();
                    else if (current == parent->left)
                        grandParent = grandParent->rightLeft();
                    else
                        grandParent = grandParent->singleLeft();
                    if (emptyNow)
                        this->root = grandParent;
                    else{
                        if(nodes.top()->left==grandParent->left || nodes.top()->left==grandParent->right)
                            nodes.top()->left=grandParent;
                        else
                            nodes.top()->right = grandParent;
                    }
                }
            }
        }
    }
}

template <class T>
void RBTree<T>::remove(T const & item){
    bool found = false, finished = false;
    RBNode<T> * current = this->root, * parent = this->root;
    queue<RBNode<T> *> twoChildren;
    stack<RBNode<T> * > toCheck;

    //Make sure that the BST is not empty
    if(this->root != NULL) {
        bool isLess, isMore;

        //Travel through the BSt until item is found or there are no more places to look
        while (current && !found) {
            toCheck.push(current);
            isLess = item < current->data;
            isMore = current->data < item;
            if (isMore || isLess) {
                parent = current;
                current = (isLess ? current->left : current->right);
            }
            else // if it is not less or greater then it must be the value
                found = true;
        }

        if (found){
            RBNode<T> * leftVal = NULL, *leftValParent = NULL;

            //If the found Node has two children
            if(current->left && current->right){
                //First must find the smallest value of the right subtree
                leftVal = current->right;
                leftValParent = current;
                while(leftVal->left) {
                    twoChildren.push(leftVal); // This will be added to the stack at the end
                    leftValParent = leftVal;
                    leftVal = leftVal->left;
                }

                //The case if the right subtree of the Node to be removed has no Left Nodes
                if(leftValParent != current) {
                    leftValParent->left = leftVal->right;
                    leftVal->right = current->right;
                }

                leftVal->left = current->left;
                current->left = current->right = NULL;

                toCheck.push(leftVal);

                while(!twoChildren.empty()){
                    toCheck.push(twoChildren.front());
                    twoChildren.pop();
                }
            }

                //If the found Node only has one child
            else if((current-> left && !current->right) || (current->right && !current->left)){
                //Set the correct Parent pointer to point to the child of the Node to be removed
                if(current->left) {
                    leftVal = current->left;
                    current->left = NULL;
                }
                else {
                    leftVal = current->right;
                    current->right = NULL;
                }
            }

            //The conditional for both 1 and 2 children set currents pointers to
            //NULL so this conditional deals with the parent pointer and deletion
            //Case if the Node to be removed is a leaf Node
            if(!current->left && !current->right){
                if(current != this->root) {
                    if (current == parent->left)
                        parent->left = leftVal;
                    else
                        parent->right = leftVal;
                }
                    //The case if the BST only has one Node which is necessarily the root
                else
                    this->root = leftVal;
                if(!leftValParent)
                    leftValParent = parent;
                //Mzking sure that the new Tree does not consist of only a single  Node
                if(this->root){
                    if(this->root->right || this->root->left) {
                        //Only swap the colors in the case that the node being removed originally had 2 children
                        if (leftVal)
                            swap(current->color, leftVal->color);
                        if (leftValParent != current)
                            deleteRebalance(toCheck, current, leftValParent);
                            //Special case indicating that the right subtree has no left nodes
                        else{
                            if(leftVal != this->root)
                                deleteRebalance(toCheck, current, parent);
                            if (leftVal->right)
                                leftVal->right->color = BLACK;
                            else if(leftVal->left && !leftVal->right)
                                leftVal->left->color = RED;
                        }

                    }
                    this->root->color = BLACK;
                }
                delete current;
            }

        }
    }
}

template <class T>
void RBTree<T>::deleteRebalance(stack<RBNode<T> *> &nodes, RBNode<T> *&current, RBNode<T> *parent){
    if(current->color!=RED) {
        if (current->data < parent->data && RBNode<T>::getColor(current->left) == RED)
            current->left->color = RED;
        else if (parent->data < current->data && RBNode<T>::getColor(current->right) == RED)
            current->right->color = RED;
        else
            removeDoubleBlack(nodes); //re-balance the tree
    }
}

template <class T>
void RBTree<T>::removeDoubleBlack(stack<RBNode<T> * > & nodes){
    RBNode<T> * current = nodes.top(), * sibling, * parent;
    nodes.pop();
    parent = nodes.top();
    nodes.pop();
    if(current->data < parent->data)
        sibling = parent->right;
    else
        sibling = parent->left;
    //Case if the sibling color is RED
    if(RBNode<T>::getColor(sibling) == RED){
        if(sibling == parent->left){
            parent = parent->singleRight();
            sibling = parent->right->left;
        }
        else{
            parent = parent->singleLeft();
            sibling = parent->left->right;
        }
        deleteRebalance(nodes, current, parent);
    }
    //Case if the sibling of the Node is black with a red child
    else if(sibling && (RBNode<T>::getColor(sibling->right) == RED || RBNode<T>::getColor(sibling->left) == RED)){
        if(sibling == parent->right && sibling->right && sibling->right->color == RED){
            parent = parent->singleLeft();
            parent->right->color = BLACK;
        }
        else if(sibling == parent->right){
            parent = parent->rightLeft();
            parent->color = BLACK;
            parent->right->color = BLACK;
        }
        else if(sibling == parent->left && sibling->left && sibling->left->color == RED){
            parent = parent->singleRight();
            parent->color = BLACK;
        }
        else{
            parent = parent->leftRight();
            parent->color = BLACK;
            parent->left->color = BLACK;
        }
        //See if there is a parent pointer to be reassigned
        if(!nodes.empty()){
            //Need to remove possible left overs in the stack from rotations
            //if(nodes.top()==current)
            //    nodes.pop();
            //if(nodes.top() == parent->left || nodes.top() == parent->right)
              //  nodes.pop();
            if(!nodes.empty()) {
                if (parent->data < nodes.top()->data)
                    nodes.top()->left = parent;
                else
                    nodes.top()->right = parent;
            }
            else
                this->root = parent;
        }
    }
    else if(RBNode<T>::getColor(sibling) == BLACK){
        if(parent->removeRecolorFamily() && parent!=root){
            removeDoubleBlack(nodes);
        }
    }
}


template <class T>
void RBNode<T>::printPreorder(ostream &os, string indent) const{
    os << indent << this->data << "  " << (this->color?(string)"BLACK":"RED");
    os << endl;
    indent = indent + "  "; // every step deeper needs two more spaces indentation
    if(this->left)
        this->left->printPreorder(os, indent);
    else
        os << indent << "NULL\n";
    if(this->right)
        this->right->printPreorder(os, indent);
    else
        os << indent << "NULL\n";
}

#endif //RED_BLACK_RED_BLACK_IMPLEMENTATION_H
