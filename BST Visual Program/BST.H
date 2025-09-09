#ifndef BST_H_
#define BST_H_

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

class BST {

private:
  struct BinaryNode {

    int value;
    int height;
    BinaryNode* left;
    BinaryNode* right;
    int depth;
    int inorder_num;
    int postorder_num;
    int preorder_num;
 
    BinaryNode(int& v) : value(v), left(NULL), right(NULL) {}
    static BinaryNode* insert(int v, BinaryNode* t) {
      if(t == NULL){
	      t = new BinaryNode(v);
      }
      else if (v < t->value)
	      t->left = insert(v, t->left);
      else if (v > t->value)
	      t->right = insert(v, t->right);
      else
	      t->value = v;
      return t;
    }
    
    static BinaryNode* remove(int v, BinaryNode* t) {
      if(t) {
	      if(v < t->value)
	        t->left = remove(v, t->left);
	      else if(t->value < v)
	        t->right = remove(v, t->right);
	      else if(t->left != NULL && t->right != NULL) {
	        t->value = find_min(t->right)->value;
	        t->right = remove(t->value, t->right);
	    } else {
	       BinaryNode* oldNode = t;
	       t = (t->left != NULL) ? t->left : t->right;
	       delete oldNode;
	      }
      }
      return t;
    }
    
    static int calcHeightRecursive(BinaryNode* t){
      if(t == nullptr){
        return -1;
      }
      int leftHeight = calcHeightRecursive(t->left);
      int rightHeight = calcHeightRecursive(t->right);
      t->height = max(leftHeight, rightHeight) + 1;
      return t->height;
    }
    
    static BinaryNode* find_min(BinaryNode* t) {
      if(t == NULL)
	      return NULL;
      if(t->left == NULL)
	      return t;
      return find_min(t->left);
    }

  };

public:
  BST(): root(NULL) {}
  ~BST(){
    deleteTree(root);
  }
  
  bool search(int v) const{
    return search(v, root);
  }
  void insert(int v) { 
    if(search(v)){
      cerr << "Error: key" << v << "already exists in the tree" << endl;
    } else{
       root = BinaryNode::insert(v, root);
    }
  }
  void remove(int v) { 
    if(!search(v)){
      cerr << "Error: key" << v << "not found in the tree" << endl;
    } else{
       root = BinaryNode::remove(v, root);
    }
  }
  
  void calcHeight(){
    if(root != NULL){
       BinaryNode::calcHeightRecursive(root);
    }
  }
  void display() { display(root); }
  void display( BinaryNode* t ) {
    // in-order traversal with indented display.
    static int depth = 0;
    ++depth;
    if ( t == NULL ) return;
    if ( t->right != NULL ) display(t->right);
    for( int i = 0; i != depth; ++i )
      cout << " ";
    cout << t->value << " " << endl;
    if ( t->left != NULL )
      display(t->left);
    --depth;
  }
  
  void creatingDotFile(ofstream& out, const string& mode) {
    out << "digraph G {" << endl;
    
    if(mode == "height") {
      calcHeight();
    }
      
      else if(mode == "preorder") {
        int num = 0;
        preOrder(root, num);
      }
      else if(mode == "inorder") {
        int num = 0;
        inOrder(root, num);
      }
      else if(mode == "postorder") {
        int num = 0;
        postOrder(root, num);
      }
    
    printNodes(root, out, mode);
    printEdges(root, out);
    out << "}" << endl;
    
  }
  
  private:
    BinaryNode* root;
    
       void deleteTree(BinaryNode* node ){
      if(node != nullptr){
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
      }
    }
    
    bool search(int v, BinaryNode* t) const{
      if(t == nullptr){
        return false;
      } 
      if(v < t->value){
        return search(v, t->left);
      }
      if(v > t-> value){
        return search(v, t->right);
      }
      return true;
    }
    void preOrder(BinaryNode* t, int &num) {
      if(!t) {
        return;
      }
      
      t->preorder_num = ++num;
      preOrder(t->left, num);
      preOrder(t->right, num);
      
    }
    
    void inOrder(BinaryNode* t, int &num) {
      if(!t) {
        return;
      }
      
      inOrder(t->left, num);
      t->inorder_num = ++num;
      inOrder(t->right, num);
      
    }
    
    void postOrder(BinaryNode* t, int &num) {
      if(!t) {
        return;
      }
      
      postOrder(t->left, num);
      postOrder(t->right, num);
      t->postorder_num = ++num;
      
    }
    
    void printNodes(BinaryNode* t, ofstream& out, const string &mode) {
      if (t == NULL) {
        return;
      }
      
      out << " key" << t->value << " [";
      
      
      
      if(mode == "height") {
        out << "shape = triangle, color = red, style = filled, label=\"key=" << t->value << ", h=" << t->height << "\"];"<<endl;
      }
      
      else if(mode == "preorder") {
        out << "shape = star, color = lightgreen, style = filled, fontname = \"Comic Sans MS\",fontsize = 12, label=\"key=" << t->value << ", pre=" << t->preorder_num << "\"];"<<endl;

      }
      else if(mode == "inorder") {
       out << " shape = circle, color = pink, style = filled, fontname = \"Comic Sans MS\", fontsize = 12, label=\"key=" << t->value << ", in=" << t->inorder_num << "\"];"<<endl;
      }
      else if(mode == "postorder") {
        out << "shape = diamond, color = orange, style = filled, fontname = \"Comic Sans MS\",fontsize = 12, label=\"key=" << t->value << ", post=" << t->postorder_num << "\"];"<<endl;
      }
      
      
      printNodes(t->left, out, mode);
      printNodes(t->right, out, mode);
      
    }
    
    
    
    void printEdges(BinaryNode* t, ofstream& out) {
      if (t == NULL) {
        return;
      }
      
      if(t->left != NULL) {
        out << "  key" << t->value << " -> key" << t->left->value << ";" << endl;
      }
      
      if(t->right != NULL) {
        out << "  key" << t->value << " -> key" << t->right->value << ";" << endl;
      }
      
      printEdges(t->left, out);
      printEdges(t->right, out);
    }
};

#endif /* BST_H_ */

