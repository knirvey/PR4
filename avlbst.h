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
		void removeFix(AVLNode<Key, Value>* node, int8_t diff);
		void insertFix(AVLNode<Key, Value>* node, AVLNode<Key, Value>* parent);
		virtual Node<Key, Value>* createNode(const Key& key, const Value& value, Node<Key, Value>* parent){
			return new AVLNode<Key, Value>(key, value, static_cast<AVLNode<Key, Value>*> (parent));
		}
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
if(this->root_ == NULL){
	this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
	return;
}
AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*> (this->root_);
AVLNode<Key, Value>* parent = NULL;

while(curr != NULL){
	parent = curr;
	if(new_item.first == curr->getKey()){

		curr->setValue(new_item.second);

		return;
	}else if(new_item.first <curr->getKey()){
		curr = curr->getLeft();
	}else{
		curr = curr->getRight();
	}
}

AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);

if(new_item.first < parent->getKey()){
	parent->setLeft(newNode);
}else{
	parent->setRight(newNode);
}

if(parent->getBalance()==-1){
	parent->setBalance(0);
	return;
}else if(parent->getBalance()==1){
	parent->setBalance(0);
	return;
}else{
	if(parent->getLeft()==newNode){
		parent->setBalance(-1);
	}else{
		parent->setBalance(1);
	}
	insertFix(newNode, parent);
}



}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* node, AVLNode<Key, Value>* parent){
	if(parent==nullptr || parent->getParent() == nullptr){
		return;
	}

	AVLNode<Key, Value>* grandparent = parent->getParent();

	if(parent == grandparent->getLeft()){
		grandparent->updateBalance(-1);
		if(grandparent->getBalance()==0){
			return;
		}
		else if(grandparent->getBalance() == -1){
			insertFix(parent, grandparent);
		}else{
			if(node == parent->getLeft()){
				rightRotation(grandparent);
				parent->setBalance(0);
				grandparent->setBalance(0);
			}else{
			AVLNode<Key, Value>* middle = parent->getRight();
			int8_t middleBalance=middle->getBalance();

			leftRotation(parent);
			rightRotation(grandparent);

			if(middleBalance == -1){
				grandparent->setBalance(1);
				parent->setBalance(0);
			}else if(middleBalance==1){
				grandparent->setBalance(0);
				parent->setBalance(-1);
			}else{
				grandparent->setBalance(0);
				parent->setBalance(0);
			}
			middle->setBalance(0);
		}	

		}

	}else {
		grandparent->updateBalance(1);

		if(grandparent->getBalance()==0){
			return;
		}
		else if(grandparent->getBalance()==1){
			insertFix(parent, grandparent);
		}else{
			if(node==parent->getRight()){

				leftRotation(grandparent);
				parent->setBalance(0);
				grandparent->setBalance(0);

			}else{
				AVLNode<Key, Value>* middle = parent->getLeft();
				int8_t middleBalance = middle->getBalance();

				rightRotation(parent);
				leftRotation(grandparent);

				if(middleBalance == 1){
				grandparent->setBalance(-1);
				parent->setBalance(0);

			}else if(middleBalance==-1){
				grandparent->setBalance(0);
				parent->setBalance(1);

			}else{
				grandparent->setBalance(0);
				parent->setBalance(0);
			}
			middle->setBalance(0);
			}
		}
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

	if(node==nullptr){
		return;
	}

	

	if(node->getLeft() != NULL && node->getRight()!=NULL){
		AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
		nodeSwap(node, pred);
	
		
	}

	AVLNode<Key, Value>* parent = node->getParent();
	AVLNode<Key, Value>* child = nullptr;
	
	if(node->getLeft() != nullptr){
		child = node->getLeft();
	}else{
		child = node->getRight();
	}



	int diff =0;

	if(parent == nullptr){
		this->root_ = child;}
	else{
	
		if(node == parent->getLeft()){
			parent->setLeft(child);
			diff = 1;
		}else{
			parent->setRight(child);
			diff = -1;
		}
	}

	if(child!= nullptr){
		child->setParent(parent);
	}
	
	delete node;
	if(parent != nullptr){
		removeFix(parent, diff);
	}
}



template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff){
	if(node == nullptr){
		return;
	}

	AVLNode<Key, Value>* parent = node->getParent();
	int8_t next_diff =0;
	if(parent != NULL){
		if(node == parent->getLeft()){
			next_diff = 1;
		}else{
			next_diff = -1;
		}
	}
	
	int8_t balance = node->getBalance() + diff;
	node->setBalance(balance);

	if(balance == -1 || balance == 1){
		node->setBalance(balance);
		return;
	}

	else if(balance == 0){
		node->setBalance(balance);
		removeFix(parent, next_diff);
	}
	else if(balance == -2){
		AVLNode<Key, Value>* left = node->getLeft();
		int8_t left_balance = left->getBalance();
	

		if(left_balance<=0){
			rightRotation(node);

			if(left_balance	==-1){
				node->setBalance(0);
				left->setBalance(0);
				removeFix(parent, next_diff);
			}
			else{
				node->setBalance(-1);
				left->setBalance(1);
				return;
			}
		}
		else{
				
			AVLNode<Key, Value>* leftRight = left->getRight();
				
				if(leftRight == NULL){
					return;
				}
				int8_t leftRightBalance = leftRight->getBalance();
				leftRotation(left);
				rightRotation(node);
				
				if(leftRightBalance==-1){
					node->setBalance(1);
					left->setBalance(0);
					
				}else if(leftRightBalance==0){
					node->setBalance(0);
					left->setBalance(0);
					
				}else{

					node->setBalance(0);
					left->setBalance(-1);
					
					
				}
				leftRight->setBalance(0);

				
				removeFix(parent, next_diff);


			}
		}
		else if(balance == 2){
			AVLNode<Key, Value>* right = node->getRight();
			if(right == NULL){
				return;
			}
			int8_t right_balance = right->getBalance();

			if(right_balance>=0){
			leftRotation(node);

			if(right_balance	==1){
				node->setBalance(0);
				right->setBalance(0);
				removeFix(parent, next_diff);
			}
			else{
				node->setBalance(1);
				right->setBalance(-1);
				return;
			}
			}else{
				AVLNode<Key, Value>* rightLeft=right->getLeft();
				
				if(rightLeft == NULL){
					return;
				}
				int8_t rightLeftBalance = rightLeft->getBalance();
				rightRotation(right);
				leftRotation(node);

				if(rightLeftBalance ==1){
					node->setBalance(-1);
					right->setBalance(0);
			
				}else if(rightLeftBalance ==0){
					node->setBalance(0);
					right->setBalance(0);
				
				}else{
					node->setBalance(0);
					right->setBalance(1);
				
				}
				rightLeft->setBalance(0);
				removeFix(parent, next_diff);
			}
		
		}
}

		
	
	


template<class Key, class Value>
void AVLTree<Key, Value>::leftRotation(AVLNode<Key, Value>* node){

	if(node == NULL || node->getRight() == NULL){
		return;
	}

	AVLNode<Key, Value>* parent = node->getParent();
	AVLNode<Key, Value>* right_child = node->getRight();

	if(parent==NULL){
		this->root_ = right_child;
	}else if(parent->getLeft() == node){
		parent->setLeft(right_child);
	}else{
		parent->setRight(right_child);
	}

	right_child->setParent(parent);

	node->setRight(right_child->getLeft());

	if(right_child->getLeft() != NULL){
		right_child->getLeft()->setParent(node);
	}

	right_child->setLeft(node);
	node->setParent(right_child);

	
}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotation(AVLNode<Key, Value>* node){

	if(node==NULL || node->getLeft()==NULL){
		return;
	}
	AVLNode<Key, Value>* parent = node->getParent();
	AVLNode<Key, Value>* left_child = node->getLeft();

	if(parent==NULL){
		this->root_ = left_child;
	}else if(parent->getLeft() == node){
		parent->setLeft(left_child);
	}else{
		parent->setRight(left_child);
	}

	left_child->setParent(parent);

	node->setLeft(left_child->getRight());

	if(left_child->getRight() != NULL){
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
