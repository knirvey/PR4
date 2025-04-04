#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int findHeight(Node* root);

bool IsPathEqual(Node* root, int targetheight, int currentheight);

bool equalPaths(Node * root)
{
    // Add your code below
	if(root == nullptr){
		return true;
	}

	int targetheight = findHeight(root);

	return IsPathEqual(root, targetheight, 0);

}

int findHeight(Node* root){
	if(root==nullptr){
		return 0;
	}

	if(root->left == nullptr && root->right == nullptr){
		return 1;
	}

	if(root->right == nullptr){
		return 1+findHeight(root->left);
	}
	if(root->left == nullptr){
		return 1+findHeight(root->right);
	}

	return 1+findHeight(root->left);
}

bool IsPathEqual(Node* root, int targetheight, int currentheight){
	bool checkLeft = true;
	bool checkRight = true;

	if(root==nullptr){
		return true;
	}

	if(root->left == nullptr && root->right == nullptr){
		return currentheight+1 == targetheight;
	}
	
	if(root->left!=nullptr){
		checkLeft = IsPathEqual(root->left, targetheight, currentheight+1);
	}
	if(root->right!=nullptr){
		checkRight = IsPathEqual(root->right, targetheight, currentheight+1);
	}

	if(checkLeft && checkRight){
		return true;
	}else{
		return false;
	}
	
}