
template <typename T>
class HeapNode{
    public:
        T *data;
        HeapNode *lChild;
        HeapNode *nSibling;  // The next sibling node
        HeapNode *pSibling;  // The previous sibling node in this level
        HeapNode *parent;

        HeapNode(T *d){
            data = d;
            parent = lChild = nSibling = pSibling = nullptr;
        }

        void addChild(HeapNode *node) {
            // Add the new node as the left most child of this node
            node->pSibling = nullptr;
            node->parent = this;  // The parent of the new node will always be the current node
            if(lChild == nullptr) {  // If the node currently doesn't have a lChild add the new node as its only lChild
                lChild = node;
            }
            else {
                // If there is already an left child. Make the current left child as the next sibling for the
                // new node and new node as the previous sibling of current left child.
                // Now make the new node as the new left child. 
                lChild->pSibling = node;  // Curent left childs previous sibling becomes the new node
                lChild->parent = nullptr;  // Current left childs parent pointer is removed 
                node->nSibling = lChild;  // Cureent left child is made the new nodes next sibling
                node->pSibling = nullptr;  // The new nodes previous sibling will be null
                lChild = node;  // Now the node is made the left most child 
            }
        }

        void removeSelf(){
            // Adjust the pointers so that the node no longer belongs in the heap
            if(this->parent != nullptr){
                this->parent->lChild = this->nSibling;  // Assign the next sibling as parents left most child
            }
            if (this->nSibling != nullptr){
                this->nSibling->pSibling = this->pSibling;  // Make next Siblings previous as current nodes previous sibling
            }
            if (this->pSibling != nullptr){
                this->pSibling->nSibling = this->nSibling;  // Make prev Siblings next as curent nodes next
            }

            this->nSibling = nullptr;
            this->pSibling = nullptr;
            this->parent = nullptr;
        }


};


template <typename T>
class PairingHeap{
    private:
        HeapNode<T> *root;
        int (*comparator) (T, T);

        HeapNode<T> * meld(HeapNode<T> *node1, HeapNode<T> *node2){
            // If root is null add the new node to merge as root
            // cout << "meld" <<endl;
            if (node1 == nullptr){
                // cout << "rootNode is null" <<endl;
                return node2;
            } else if(node2 == nullptr){
                // If the new node is null, dont do anything
                // cout << "newNode is null" <<endl;
                return node1;
            } else if (node1->data != nullptr && node2->data != nullptr && comparator(*(node2->data), *(node1->data)) < 0) {
                // If current root is smaller, add the new node as child of root
                // cout << "comparator less" <<endl;
                node2->addChild(node1);
                return node2;
            } else {
                // If new node is smaller, make root the child of new node and then new node as root
                // cout << "comparator more" <<endl;
                node1->addChild(node2);
                return node1;
            }
        }

        // Perform the TwoPass merge used when root node is deleted
        // new Node is the left most child from where to start the merge
        HeapNode<T> * twoPassMerge(HeapNode<T> *leftMostNode){
            if (leftMostNode == nullptr || leftMostNode->nSibling == nullptr){
                // cout << "leftMostNode - " <<", leftMostNode->nSibling" << endl;
                return leftMostNode;
            } else {
                // cout << "left pointer not null" << endl;
                HeapNode<T> *node1, *node2, *nodeNextIter;
                node1 = leftMostNode;
                node2 = leftMostNode->nSibling;
                nodeNextIter = leftMostNode->nSibling->nSibling;
                node1->nSibling = nullptr;
                node2->nSibling = nullptr;
                return meld(meld(node1, node2), twoPassMerge(nodeNextIter));
            }
        }

    public:
        PairingHeap(int (*c) (T, T)){
            comparator = c;
            root = nullptr;
        }

        // The key for the node is replaced by the new data value
        void replaceKey(HeapNode<T> *node, T *data){
            if (node == nullptr){
                return;
            }
            if(node == root){
                // If the node is the root node, remove it and reisert for increasing else just replace the data
                if (comparator(*(node->data), *data)>0){
                    // The  new value is less than the current root so just update the value
                    node->data = data;
                    return;
                } else {
                    // If the root nodes value is being increased, remove and reinsert
                    HeapNode<T> *leftMostChild = node->lChild;
                    
                    node->lChild = nullptr;
                    node->nSibling = nullptr;
                    node->pSibling = nullptr;
                    
                    root = twoPassMerge(leftMostChild);
                    node->data = data;
                    insert(node);
                }
            }
            else{
                // node is not the root, remove the node and insert it with new value
                node->removeSelf();
                // T * existingData = node->data;
                node->data = data;
                insert(node);
            }
           
        }

        // Get the top value from the heap
        T* top(){
            if (root != nullptr)
                return root->data;
            else
                return nullptr;
        }
        
        // Delete the top value from the heap
        void deleteMin(){
            if (root == nullptr){
                // If the root is null no delete
                return;
            }
            HeapNode<T> *minNode = root;  // Assign the root as the min node
            HeapNode<T> *leftChild = root->lChild;  // Get the left child of the root
            
            root = twoPassMerge(leftChild);  // Two pass merge starting from the left child

            cout << "Min deleted" << endl;
            printTree();

            delete minNode;  // Delete the min node
        }

        // Insert a new node to the heap. The node is added by melding the new node with the current tree
        void insert(HeapNode<T> *newNode){
            this->root = meld(newNode, root);  
            cout << "Inserted" << endl;
            printTree();
        }

        void printTree() {
            cout<<"In print tree heap" << endl;
            if (root != nullptr) {
                printHelper(this->root, "", true, 0, 10);
            } else {
                cout<<"Root is null" <<  endl;
            }
            cout << endl << endl;
        }

        void printHelper(HeapNode<T> *node, string indent, bool last, int count, int max) {
            if (node != nullptr && count < max) {
                cout << indent;
                if (last) {
                    cout << "R----";
                    // indent += "   ";
                } else {
                    cout << " ---- ";
                    // indent += "";
                }

                cout << "root - ("<< node->data->rideId  <<"," << node->data->rideCost<<","<< node->data->rideDuration   << ")" << endl;
                
                printHelper(node->nSibling, indent, false, ++count, max);
                printHelper(node->lChild, indent, true, ++count, max);
                
            }
        }

};