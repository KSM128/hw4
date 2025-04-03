#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool equalPathsHelper(Node* node, int thisLength, int& LeafLength){
  if(node == nullptr){
    return true; // base case
  }

  if(node->left == nullptr && node->right == nullptr){ // if the node is a leaf
    if(LeafLength == 0){
      LeafLength = thisLength; // set leaflength to the length of any leaf for comparison
    }
    return (LeafLength == thisLength); // compare this length and the stored value referenced by leaflength 
  }

  // otherwise, recurse to this node's left and right subtree
  bool leftTrue = equalPathsHelper(node->left, thisLength + 1, LeafLength);
  bool rightTrue = equalPathsHelper(node->right, thisLength + 1, LeafLength);
  return (leftTrue && rightTrue);
}

bool equalPaths(Node * root)
{
    // Add your code below
  int LeafLength = 0;
  return equalPathsHelper(root, 0, LeafLength);
}

