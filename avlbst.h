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
		void leftRotation(AVLNode<Key, Value>* node);
		void rightRotation(AVLNode<Key, Value>* node);
		AVLNode<Key, Value>* balance(AVLNode<Key, Value>* node);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

BinarySearchTree<Key, Value>::insert(new_item);
AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(new_item.first));


AVLNode<Key, Value>* parent = node->getParent();
while(parent!=nullptr){

	if(parent->getLeft()==node){
		parent->updateBalance(-1);
	}else{
		parent->updateBalance(1);
	}

	if(parent->getBalance()==0){
		break;
	}

	if(parent->getBalance()==-1 || parent->getBalance()==1){
		node = parent;
		parent = parent->getParent();
		continue;
	}

	if(parent->getBalance()== -2||parent->getBalance()==2){
		balance(parent);
		break;
	}
}
/*	if(this->root_ == nullptr){
		this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
		return;
	}

	AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root);
	AVLNode<Key, Value>* parent = nullptr;

	while(curr!=nullptr){
		parent = curr;

		if(new_item.first == curr->getKey()){
			
			curr->setValue(new_item.second);
			return;
		}
		else if(new_item.first< curr->getKey()){

			curr = curr->getLeft();

		}else{
			curr = curr->getRight();
		}
	}

	AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent)

	if(new_item.first < parent->getKey()){
		parent->setLeft(newNode);
		
		if(parent->getBalance() == 1){
			
			parent->setBalance(0);

		}else if(parent->getBalance()==0){
			
			parent->setBalance(-1);
			fixHelper(parent, -1);
		}else if(parent->getBalance()==-1){

			fixHelper(parent, -1);
		}
	}else{
		parent->setRight(newNode);

		if(parent->getBalance()== -1){

			parent->setBalance(0);

		}else if(parent->getBalance()==0){

			parent->setBalance(1);
			fixHelper(parent, 1);

		}else if(parent->getBalance()==1){

			fixHelper(parent,1);

		}

	} */

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

	if(node==nullptr){
		return;
	}

	AVLNode<Key, Value>* parent = node->getParent();
	bool removed_left = (parent!=nullptr&&parent->getLeft()==node);

	if(node->getLeft() != nullptr && node->getRight()!=nullptr){
		AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
		nodeSwap(node, pred);

		parent = node->getParent();
		removed_left = (parent!=nullptr && parent->getLeft()==node);
	}

	AVLNode<Key, Value>* child;
	if(node->getLeft() != nullptr){
		child = node->getLeft();
	}else{
		child = node->getRight();
	}

	if(parent==nullptr){
		this->root_ = child;
	}else if(removed_left){
		parent->setLeft(child);
	}else{
		parent->setRight(child);
	}

	if(child!=nullptr){
		child->setParent(parent);
	}

	delete node;


	while(parent!=nullptr){

		if(removed_left){
			parent->updateBalance(1);
		}else{
			parent->updateBalance(-1);
		}

		if(parent->getBalance() == -2|| parent->getBalance() == 2){
			
			parent = balance(parent);

			if(parent == nullptr){
				break;
			}
		}else if(parent->getBalance()==-1 || parent->getBalance() == 1){
			break;
		}

		removed_left = (parent->getParent() != nullptr && parent->getParent()->getLeft() == parent);
		parent = parent->getParent();
	}
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::balance(AVLNode<Key, Value>* node){
	if(node->getBalance()== -2){

		if(node->getLeft()->getBalance()==1){
			leftRotation(node->getLeft());
			rightRotation(node);

			node->setBalance(0);
		}else{
			rightRotation(node);

			if(node->getBalance() == -1){
				node->setBalance(0);
				node->getParent()->setBalance(-1);
			}
		}
	}else{
		if(node->getRight()->getBalance()== -1){
			rightRotation(node->getRight());
			leftRotation(node);


			node->setBalance(0);
		}else{
			leftRotation(node);

			if(node->getBalance() == 1){
				node->setBalance(0);
				node->getParent()->setBalance(0);
			}else{
				node->setBalance(-1);
				node->getParent()->setBalance(1);
			}
		}
	}

	return node->getParent();
}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotation(AVLNode<Key, Value>* node){

	AVLNode<Key, Value>* parent = node->getParent();
	AVLNode<Key, Value>* right_child = node->getRight();

	if(parent==nullptr){
		this->root_ = right_child;
	}else if(parent->getLeft() == node){
		parent->setLeft(right_child);
	}else{
		parent->setRight(right_child);
	}

	right_child->setParent(parent);

	node->setRight(right_child->getLeft());

	if(right_child->getLeft() != nullptr){
		right_child->getLeft()->setParent(node);
	}

	right_child->setLeft(node);
	node->setParent(right_child);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotation(AVLNode<Key, Value>* node){
	AVLNode<Key, Value>* parent = node->getParent();
	AVLNode<Key, Value>* left_child = node->getLeft();

	if(parent==nullptr){
		this->root_ = left_child;
	}else if(parent->getLeft() == node){
		parent->setLeft(left_child);
	}else{
		parent->setRight(left_child);
	}

	left_child->setParent(parent);

	node->setLeft(left_child->getRight());

	if(left_child->getRight() != nullptr){
		left_child->getRight()->setParent(node);

		
	}

		left_child->setRight(node);
		node->setParent(left_child);
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
