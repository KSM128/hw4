#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    // faulty implementation: void rotLeft(AVLNode<Key, Value>* node);
    // faulty implementation: void rotRight(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rotLeft(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rotRight(AVLNode<Key, Value>* node);
    // void doBalance(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* doBalance(AVLNode<Key, Value>* node);

};

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotLeft(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* rightChild = node->getRight();
    node->setRight(rightChild->getLeft());
    if(rightChild->getLeft() != nullptr){
        rightChild->getLeft()->setParent(node);
    }
    rightChild->setParent(node->getParent());

    if(node->getParent() == nullptr){
        this->root_ = rightChild;
    } else if(node == node->getParent()->getLeft()){
        node->getParent()->setLeft(rightChild);
    } else {
        node->getParent()->setRight(rightChild);
    }

    rightChild->setLeft(node);
    node->setParent(rightChild);

    return rightChild;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotRight(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* leftChild = node->getLeft();
    node->setLeft(leftChild->getRight());
    if(leftChild->getRight() != nullptr){
        leftChild->getRight()->setParent(node);
    }
    leftChild->setParent(node->getParent());

    if(node->getParent() == nullptr){
        this->root_ = leftChild;
    } else if(node == node->getParent()->getRight()){
        node->getParent()->setRight(leftChild);
    } else {
        node->getParent()->setLeft(leftChild);
    }

    leftChild->setRight(node);
    node->setParent(leftChild);

    return leftChild;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::doBalance(AVLNode<Key, Value>* node){
    if(node == nullptr){
        return nullptr;
    }
    if(node->getBalance() == 2){
        if(node->getLeft() != nullptr && node->getLeft()->getBalance() < 0){
            node->setLeft(rotLeft(node->getLeft()));
        } 
        return rotRight(node);
    } else if(node->getBalance() == -2){
        if(node->getRight() != nullptr && node->getRight()->getBalance() > 0){
            node->setRight(rotRight(node->getRight()));
        } 
        return rotLeft(node);
    }

    return node;
}

// Faulty rotation functions, commented for reference
// template<class Key, class Value>
// void AVLTree<Key, Value>::rotLeft(AVLNode<Key, Value>* node){
//     // get right child and turn its left subtree into this one's right subtree
//     AVLNode<Key, Value>* rightChild = node->getRight();
//     node->setRight(rightChild->getLeft());
//     if(rightChild->getLeft() != nullptr){
//         rightChild->getLeft()->setParent(node);
//     }
//     if(node->getParent() == nullptr){
//         this->root_ = rightChild;
//     } else {
//         // reorganize the parent 
//         rightChild->setParent(node->getParent());
//         if(node == node->getParent()->getLeft()){
//             node->getParent()->setLeft(rightChild);
//         } else {
//             node->getParent()->setRight(rightChild);
//         }
//     }

//     rightChild->setLeft(node);
//     node->setParent(rightChild);
    
//     // set new balance
//     int8_t nodeBalance = node->getBalance();
//     int8_t childBalance = rightChild->getBalance();
//     int8_t zero = 0;
//     node->setBalance(1);
//     rightChild->setBalance(1);
// }

// template<class Key, class Value>
// void AVLTree<Key, Value>::rotRight(AVLNode<Key, Value>* node){
//     // get left child and turn its right subtree into this one's left subtree
//     AVLNode<Key, Value>* leftChild = node->getLeft();
//     node->setLeft(leftChild->getRight());
//     if(leftChild->getRight() != nullptr){
//         leftChild->getRight()->setParent(node);
//     }
//     if(node->getParent() == nullptr){
//         this->root_ = leftChild;
//     } else {
//         // reorganize the parent 
//         leftChild->setParent(node->getParent());
//         if(node == node->getParent()->getRight()){
//             node->getParent()->setRight(leftChild);
//         } else {
//             node->getParent()->setLeft(leftChild);
//         }
//     }
//     leftChild->setRight(node);
//     node->setParent(leftChild);

//     // set new balance
//     int8_t nodeBalance = node->getBalance();
//     int8_t childBalance = leftChild->getBalance();
//     int8_t zero = 0;
//     node->setBalance(-1);
//     leftChild->setBalance(-1);
// }

// template<class Key, class Value>
// void AVLTree<Key, Value>::doBalance(AVLNode<Key, Value>* node){
//     if(node->getBalance() > 1){ // left
//         AVLNode<Key, Value>* leftChild = node->getLeft();
//         if(leftChild->getBalance() >= 0){ // left-left
//             rotRight(node);
//         } else if(leftChild != nullptr) { // left-right
//             rotLeft(leftChild);
//             rotRight(node);
//         }
//     } else if(node->getBalance() < -1){ // right
//         AVLNode<Key, Value>* rightChild = node->getRight();
//         if(rightChild->getBalance() <= 0){
//             rotLeft(node); // right-right
//         } else if(rightChild != nullptr) {
//             rotRight(rightChild); // right-left
//             rotLeft(node);
//         }
//     }
// }

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* newItem = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    // empty tree
    if(this->root_ == nullptr){
        this->root_ = newItem;
        return;
    }

    AVLNode<Key, Value>* parent = nullptr;
    // cast root_ from node to avlnode
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    while(curr != nullptr){
        // replace if same key, otherwise left/right based on key
        parent = curr;
        if(newItem->getKey() < curr->getKey()){
            curr = curr->getLeft();
        } else if(newItem->getKey() > curr->getKey()){
            curr = curr->getRight();
        } else {
            curr->setValue(newItem->getValue());
            delete newItem;
            return;
        }
    }

    newItem->setParent(parent);
    if(newItem->getKey() < parent->getKey()){
        parent->setLeft(newItem);
    } else {
        parent->setRight(newItem);
    }

    // balance the tree
    AVLNode<Key, Value>* thisNode = newItem;
    AVLNode<Key, Value>* par = parent;
    // loop until the root
    while(par != nullptr){
        if(thisNode == par->getLeft()){
            par->updateBalance(1); // left
        } else {
            par->updateBalance(-1); // right
        }

        if(par->getBalance() == 0){
            // balanced
            break;
        }
        if(par->getBalance() >= 2 || par->getBalance() <= -2){
            // unbalanced
            par = doBalance(par);
            break;
        }

        // otherwise go up
        thisNode = par;
        par = par->getParent();
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(node == nullptr){
        return;
    }

    // swap with predecessor if 2 children
    if(node->getLeft() != nullptr && node->getRight() != nullptr){
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(node));
        nodeSwap(pred, node);
    }

    // find (new) parent and child
    AVLNode<Key, Value>* child = nullptr;
    if(node->getLeft() != nullptr){
        child = node->getLeft();
    } else if(node->getRight() != nullptr){
        child = node->getRight();
    } else {
        return;
    }

    AVLNode<Key, Value>* parent = nullptr;
    bool isLeft;
    if(node != this->root_){
        parent = node->getParent();
        if(node == parent->getLeft()){
            parent->setLeft(child);
            isLeft = true;
        } else {
            parent->setRight(child);
            isLeft = false;
        }
    } else {
        this->root_ = child;
    }
    
    child->setParent(parent);
    delete node;

    while(parent != nullptr){
        // update balance 
        if(isLeft){
            parent->updateBalance(1);
        } else {
            parent->updateBalance(-1);
        }
        if(parent->getBalance() == 1 || parent->getBalance() == -1){
            break;
        } else if(parent->getBalance() == 2 || parent->getBalance() == -2){
            AVLNode<Key, Value>* balanceNode = doBalance(parent);
            if(balanceNode->getBalance() != 0){
                break;
            } else {
                parent = balanceNode->getParent();
            }
        } else {
            if(parent->getParent() != nullptr && parent == parent->getParent()->getLeft()){
                isLeft = true;
            } else {
                isLeft = false;
            }
            parent = parent->getParent();
        }
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
