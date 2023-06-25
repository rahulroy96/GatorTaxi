#include<iostream>
#include <vector>

using namespace std;

enum COLOR { BLACK, RED };

template <typename T>
class RBTNode{
    public:
    T *data;
    RBTNode *parent;
    RBTNode *left;
    RBTNode *right;
    COLOR color;

    
    RBTNode(T *d){
        data = d;
        parent = left = right = nullptr;
        
        color = RED;
    }

    bool isLeftChild(){
        // Checks if the node is the left child of its parent
        if(parent == nullptr){
            return false;
        }

        return this == parent->left;
    }

    bool hasRedChild(){
        // Checks if the node has atleast one red child
        return (left != nullptr && left->color == RED) || (right != nullptr && right->color == RED);
    }

    RBTNode<T> *uncle(){
        // If no parent or grandparent, then no uncle
        if (parent == nullptr or parent->parent == nullptr)
            return nullptr;
    
        // if parent is left child, uncle is right child
        if (parent->isLeftChild())
            return parent->parent->right;
        else
            return parent->parent->left;
    }

    RBTNode<T> *grandparent(){
        // If no parent or grandparent, then no uncle
        if (parent == nullptr)
            return nullptr;
    
        return parent->parent;
    }

    RBTNode<T> *sibling(){
        // If no parent or grandparent, then no uncle
        if (parent == nullptr)
            return nullptr;

        if(this == parent->left){
            return parent->right;
        }
        else
            return parent->left;
    }
};

template <typename T>
class RedBlackTree{
    private:
        RBTNode<T> *root;
        int (*comparator) (T, T);

        // Update the pointers and make the child of u the child of u's parent
        void updatePointers(RBTNode<T> *nodeToRemove, RBTNode<T> *nodeToUpdate) {
            // if parent(u) is null, v is the root of the tree
            if (nodeToRemove->parent == nullptr) {
                root = nodeToUpdate;
            } else if (nodeToRemove->isLeftChild()) {
                nodeToRemove->parent->left = nodeToUpdate;
            } else {
                nodeToRemove->parent->right = nodeToUpdate;
            }
            if (nodeToUpdate != nullptr){
                nodeToUpdate->parent = nodeToRemove->parent;
            }
            
        }

        // left rotate the given node
        void leftRotate(RBTNode<T> *node) {
            // rotated node will be the node which takes node's place ie, the right child of node
            RBTNode<T> *rotatedNode = node->right;

            // connect rotated node's left element as node's right element
            node->right = rotatedNode->left;
            
            // if rotated nodes left is not null, assign node as its parent
            if (rotatedNode->left != nullptr){
                rotatedNode->left->parent = node;
            }
        
            // update root if current node is root
            if (node == root){
                root = rotatedNode;
            }
            // update the rotated node as parents left child if node is parents left child
            else if(node->parent != nullptr && node->isLeftChild()){
                node->parent->left = rotatedNode;
            }
            else if(node->parent != nullptr){
                node->parent->right = rotatedNode;
            }
            // update the parent of the rotated node
            rotatedNode->parent = node->parent;
            
            // make rotated node the new parent of node
            node->parent = rotatedNode;
        
            // connect node with rotated nodes left
            rotatedNode->left = node;
        }

        // right rotate the given node
        void rightRotate(RBTNode<T> *node) {
            // rotated node will be the node which takes node's place ie, the left child of node
            RBTNode<T> *rotatedNode = node->left;

            // connect rotated node's right element as node's left element
            node->left = rotatedNode->right;
            
            // if rotated nodes right is not null, assign node as its parent
            if (rotatedNode->right != nullptr){
                rotatedNode->right->parent = node;
            }
        
            // update root if current node is root
            if (node == root){
                root = rotatedNode;
            }
            // update the rotated node as parents left child if node is parents left child
            else if(node->parent != nullptr && node->isLeftChild()){
                node->parent->left = rotatedNode;
            }
            else if(node->parent != nullptr){
                node->parent->right = rotatedNode;
            }
            // update the parent of the rotated node
            rotatedNode->parent = node->parent;
            
            // make rotated node the new parent of node
            node->parent = rotatedNode;
        
            // connect node with rotated nodes right
            rotatedNode->right = node;
        }

        void swapColors(RBTNode<T> *u, RBTNode<T> *v) {
            COLOR temp;
            temp = u->color;
            u->color = v->color;
            v->color = temp;
        }

        // find the node with the minimum key
        RBTNode<T>* minimum(RBTNode<T>* node) {
            while (node->left != nullptr) {
                node = node->left;
            }
            return node;
        }

        // find the node with the minimum key
        RBTNode<T>* findDeleteReplacement(RBTNode<T>* node) {
            // if single right child its the replacement
            if (node->left == nullptr){
                return node->right;
            }
            // if single left child left is the replacement
            if (node->right == nullptr){
                return node->left;
            }
            // else the minimum of the right subtree is taken
            return minimum(node->right);
        }

        RBTNode<T>* replacement(RBTNode<T> *node) {
            // Both children present
            if (node->left != nullptr && node->right != nullptr) {
                return minimum(node->right);
            }
            // Leaf node
            else if (node->left == nullptr && node->right == nullptr) {
                return nullptr;
            }
            // only left node
            else if (node->left != nullptr) {
                return node->left;
            }
            else{
                return node->right;
            }
        }

        void fixInsert(RBTNode<T>* node){
            // Fix the red black property after insertion operation
            
            if (node == root){
                // If node is root, color it black
                node->color = BLACK;
                return;
            }

            RBTNode<T> *parent = node->parent, *grandparent = node->grandparent(), *uncle = node->uncle();
            
            // If parent is black no double red present
            if (parent->color == BLACK) {
                return;
            }

            // if uncle red, shift red to grand parent by making parent and uncle black
            // Then fix grandparent
            if (uncle != nullptr && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                fixInsert(grandparent);
            } else {
                if (parent->isLeftChild()) {
                    if (node->isLeftChild()) {
                        // LL - perform right rotation
                        swapColors(parent, grandparent);
                    } else {
                        // LR - perform left and then right rotation
                        leftRotate(parent);
                        swapColors(node, grandparent);
                    }
                    rightRotate(grandparent);
                } else {
                    if (node->isLeftChild()) {
                        // RL - perform right and then left
                        rightRotate(parent);
                        swapColors(node, grandparent);
                    } else {
                        // RR - perform left rotation
                        swapColors(parent, grandparent);
                    }
                    // left rotation of RL and RR
                    leftRotate(grandparent);
                }
            }
        }

        void fixDelete(RBTNode<T> *node) {
            // Fix the red black property after deletion operation
            // cout << "Inside fix delete"<<endl;
            while(node != root && node->color == BLACK){
                if (node->sibling() == nullptr){
                    node = node->parent;
                    continue;
                }
                else if (node->sibling()->color == RED){
                    // sibling is red
                    // cout << "Sibling is red"<<endl;
                    node->sibling()->color = BLACK;
                    node->parent->color = RED;
                    if(node->isLeftChild()){
                        // Node is left child, so left rotate
                        // cout << "node is left child"<<endl;
                        leftRotate(node->parent);
                    }
                    else{
                        // Node is right child, so right rotate
                        // cout << "node is right child"<<endl;
                        rightRotate(node->parent);
                    }
                    continue;
                }
                else if (!node->sibling()->hasRedChild()){
                    // The sibling is black and doesnt have a red child
                    // cout << "node sibling has no red child"<<endl;
                    node->sibling()->color = RED;
                    if(node->parent->color == RED){
                        node->parent->color = BLACK;
                        break;
                    }
                    else{
                        node = node->parent;
                    }

                }
                // else if(node->sibling()->left->color == BLACK && node->sibling()->right->color == BLACK){
                //     // sibling is black and both its children are black
                //     node->sibling()->color = RED;
                //     node = node->parent;
                // } 
                else if(node->sibling()->isLeftChild() && node->sibling()->left != nullptr && node->sibling()->left->color == RED) {
                    // sibling is the left child of parent and is black with a red left child
                    // LL
                    node->sibling()->color = node->parent->color;
                    node->parent->color = BLACK;
                    node->sibling()->left->color = BLACK;
                    rightRotate(node->parent);
                    break;
                } else if(node->sibling()->isLeftChild() && node->sibling()->right != nullptr && node->sibling()->right->color == RED){
                    // sibling is the left child of parent and is black with a red right child
                    // LR
                    node->sibling()->right->color = BLACK;
					node->sibling()->color = RED;
					leftRotate(node->sibling());
                    node->sibling()->color = node->parent->color;
					node->parent->color = BLACK;
					node->sibling()->left->color = BLACK;
					rightRotate(node->parent);
					break;
                } else if(!node->sibling()->isLeftChild() && node->sibling()->right != nullptr && node->sibling()->right->color == RED){
                    // sibling is the right child of parent and is black with a red right child
                    // RR
                    node->sibling()->color = node->parent->color;
                    node->parent->color = BLACK;
                    node->sibling()->right->color = BLACK;
                    leftRotate(node->parent);
                    break;
                }  else if(!node->sibling()->isLeftChild() && node->sibling()->left != nullptr && node->sibling()->left->color == RED){
                    // sibling is the right child of parent and is black with a red left child
                    // RL
                    node->sibling()->left->color = BLACK;
					node->sibling()->color = RED;
					rightRotate(node->sibling());
                    node->sibling()->color = node->parent->color;
					node->parent->color = BLACK;
					node->sibling()->right->color = BLACK;
					leftRotate(node->parent);
					break;
                }

            }  

            if(node == root){
                node->color = BLACK;
            }      
            
        }

        RBTNode<T> * searchInternal(T d) {
            // searches for given value
            // if found returns the node (used for delete)
            // else returns the last node while traversing (used in insert)
            RBTNode<T> *temp = root;
            while (temp != nullptr) {
                if (comparator(d, *(temp->data)) < 0) {
                    if (temp->left == nullptr)
                        break;
                    else
                        temp = temp->left;
                } else if (comparator(d, *(temp->data)) == 0) {
                    break;
                } else {
                    if (temp->right == nullptr)
                        break;
                    else
                        temp = temp->right;
                }
            }
        
            return temp;
        }

        void searchRangeRecurse(RBTNode<T> *node, T d1, T d2, vector<T *> *resultArray){
            // Recursive function to get all the values in range d1 and d2
            // resultArray is pointer to the array where the result has to be stored
            if (node == nullptr){
                return;
            }

            if (comparator(*(node->data), d1) > 0) {
                // if current node is greater than the range start recurse into left subtree
                searchRangeRecurse(node->left, d1, d2, resultArray);
            }

            if ((comparator(*(node->data), d1) >= 0) && comparator(*(node->data), d2) <= 0) {
                // if the current node is in range add it to the result array
                resultArray->push_back(node->data);
            }

            if (comparator(*(node->data), d2) < 0) {
                // if current node is less than the range end recurse into right subtree
                searchRangeRecurse(node->right, d1, d2, resultArray);
            }

        }


    public:
        RedBlackTree(int (*c) (T, T)){
            comparator = c;
            root = nullptr;
        }

        T* search(T d){
            RBTNode<T> *node = searchInternal(d);
            if (node == nullptr){
                return nullptr;
            }
            return node->data;
        }

        // Retutn a range of values between the 2 nodes.
        void searchRange(T d1, T d2, vector<T *> *searchResult){
            
            searchRangeRecurse(root, d1, d2, searchResult);
            // return searchResult;
        }


        // inserts the given value to tree
        bool insert(T *data) {
            RBTNode<T> *newNode = new RBTNode<T>(data);
            
            if (root == nullptr) {
                // insert value at root
                // roots color is always black
                newNode->color = BLACK;
                root = newNode;
            } else {
                RBTNode<T> *temp = searchInternal(*(newNode->data));
        
                if (comparator(*(temp->data), *(newNode->data)) == 0) {
                    // return if value already exists
                    return false;
                }
            
                // connect new node to correct node
                newNode->parent = temp;
        
                if (comparator(*(newNode->data), *(temp->data)) < 0)
                    temp->left = newNode;
                else
                    temp->right = newNode;

                fixInsert(newNode);
            }
            return true;
        }

        void deleteNode(RBTNode<T> *nodeToDelete){

            if (nodeToDelete == nullptr) {
                    return;
            }

            RBTNode<T> *replacementNode = findDeleteReplacement(nodeToDelete);

            bool bothBlack = ((replacementNode == nullptr or replacementNode->color == BLACK) && (nodeToDelete->color == BLACK));

            if (replacementNode == nullptr){
                // So the nodeToDelete is a leaf node, i.e, no children
                // cout<<"replacementNode  is null "<<endl;
                
                if (nodeToDelete == root){
                    // cout<<"delete node is the root "<<endl;
                    root = nullptr;
                } else if (nodeToDelete->color == BLACK) {
                    // the nodeToDelete is a leaf and colour is black
                    // cout<<"delete node is black "<<endl;
                    fixDelete(nodeToDelete);
                    // cout<<"Fix delete done "<<endl;
                } else if (nodeToDelete->sibling() != nullptr){
                    // cout<<"delete node is not black "<<endl;
                    nodeToDelete->sibling()->color = RED;
                }

                updatePointers(nodeToDelete, replacementNode);
                
                delete nodeToDelete;
                
            } else if (nodeToDelete->left == nullptr || nodeToDelete->right == nullptr){
                // the nodeToDelete has one child
                
                if (nodeToDelete == root){
                    // the nodeToDelete is root
                    root = replacementNode;
                    replacementNode->color = BLACK;
                } else {
                    // the nodeToDelete is not a root
                    updatePointers(nodeToDelete, replacementNode);
                    if (bothBlack){
                        // cout<<"both black calling fix delete "<<endl;
                        fixDelete(replacementNode);
                    }
                    else{
                        // one out of the nodeToDelete or replacementnode is red so color replacement node as black
                        // cout<<"one node is red coloring replacemnt as black "<<endl;
                        replacementNode->color = BLACK;
                    }

                }
                // delete nodeToDelete;
                
            } else {
                // The delete node has 2 children, replace it with replacement node and then delete replacement node
                // cout << "Delete node has 2 childs, replaceing with "<< replacementNode->data->rideId<<endl;
                T *temp = nodeToDelete->data;
                nodeToDelete->data = replacementNode->data;
                replacementNode->data = temp;
                // printTree();
                deleteNode(replacementNode);
            }

            // printTree();
        }

        T* deleteKey(T data){
            // Search for the node using the given data and delete it from the tree
            // returns the data that is being deleted
            RBTNode<T> *nodeToDelete = searchInternal(data);
            if (nodeToDelete == nullptr) {
                    return nullptr;
            }
            T* deletedData = nodeToDelete->data;

            deleteNode(nodeToDelete);

            return deletedData;

        }

        void printTree() {
            if (root) {
                printHelper(this->root, "", true);
            }
            cout << endl<<endl;
        }

        void printHelper(RBTNode<T> *root, string indent, bool last) {
            if (root != nullptr) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            } else {
                cout << "L----";
                indent += "|  ";
            }

            string sColor = root->color ? "RED" : "BLACK";
            cout << root->data->rideId << "(" << sColor << ")" << endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
            }
        }

};
