#include<vector>
#include <fstream> 
#include <string>
#include"redBlackTree.cpp"
#include"heap.cpp"

using namespace std;

// node used to insert data in heap
struct RideNode{
    int rideId;
    int rideCost;
    int rideDuration;
};

// Node used to insert data in rbt
struct RbtRideNode{
    int rideId;
    HeapNode<RideNode> *heapPointer;
};

// comparison function used in rbt
int rbtCompare(RbtRideNode x, RbtRideNode y){
    return x.rideId - y.rideId;
}

// comparison function used in heap
int heapCompare(RideNode x, RideNode y){
    if ((x.rideCost - y.rideCost) != 0 )
        return x.rideCost - y.rideCost;
    else
        return x.rideDuration - y.rideDuration;
}

class GatorTaxi{
    private:
        RedBlackTree<RbtRideNode> *rbTree;
        PairingHeap<RideNode> *heap;
        ofstream out_file;
    
    public:

        GatorTaxi(){
            rbTree = new RedBlackTree<RbtRideNode> (&rbtCompare);
            heap = new PairingHeap<RideNode> (&heapCompare);
            out_file.open("output_file.txt");
        }

        void print(int rideNumber){
            RbtRideNode searchNode;
            searchNode.rideId = rideNumber;
            RbtRideNode *node = rbTree->search(searchNode);
            if (node == nullptr || node->rideId != rideNumber){
                // if the search result is null, output 0,0,0 to signify not found
                out_file << "(0,0,0)\n";
                cout << "(0,0,0)\n";
                return;
            }
            cout << "(" << node->rideId << "," << node->heapPointer->data->rideCost << "," << node->heapPointer->data->rideDuration << ")" << endl;
            out_file << "(" << node->rideId << "," << node->heapPointer->data->rideCost << "," << node->heapPointer->data->rideDuration << ")" << endl;
            
        }

        void print(int rideNumberStart, int rideNumberEnd){
            rbTree->printTree();
            cout<<"heap is: "<<endl;
            heap->printTree();
            RbtRideNode searchStart;
            searchStart.rideId = rideNumberStart;
            RbtRideNode searchEnd;
            searchEnd.rideId = rideNumberEnd;
            vector<RbtRideNode *> searchResult;
            rbTree->searchRange(searchStart, searchEnd, &searchResult);
            cout << "\n Size returned : "<<  searchResult.size() << endl;
            if (searchResult.size() == 0){
                // If the search result size is zero no records were found 
                out_file << "(0,0,0)\n";
                cout << "(0,0,0)\n";
                return;
            }
            for(int i = 0; i < searchResult.size(); i++){
                // Output the returned search results to the file.
                out_file << "(" << searchResult[i]->rideId << ",";
                out_file << searchResult[i]->heapPointer->data->rideCost << ",";
                out_file << searchResult[i]->heapPointer->data->rideDuration << ")";
                if(i != searchResult.size() - 1){
                    out_file << ",";
                }
            }
            out_file << endl;
        }

        bool insert(int rideNumber, int rideCost, int rideDuration){
            // First create the node that is inserted to heap
            RideNode *node = new RideNode;
            node->rideId = rideNumber;
            node->rideCost = rideCost;
            node->rideDuration = rideDuration;
            HeapNode<RideNode> *heapNode = new HeapNode<RideNode>(node);
            
            // Once heap node is created, create rbt node with pointer to heap node
            RbtRideNode *rbtRideNode = new RbtRideNode;
            rbtRideNode->heapPointer = heapNode;
            rbtRideNode->rideId = rideNumber;
            rbTree->printTree();
            if (rbTree->insert(rbtRideNode)){
                cout<<"rbTree inserted"<<endl;
                rbTree->printTree();
                // Insert to heap only if the node is inserted in rbt
                heap->insert(heapNode);
                return true;
            } else{
                out_file << "Duplicate RideNumber\n";
                return false;
            }
            
        }

        void getNextRide(){
            RideNode *node = heap->top();   // Get the top value from heap whic is the min cost ride
            if (node == nullptr){
                out_file << "No active ride requests\n";
                return;
            }
            cout << "(" << node->rideId << "," << node->rideCost << "," << node->rideDuration << ")"<<endl;
            // Output the min node to the output file
            out_file << "(" << node->rideId << "," << node->rideCost << "," << node->rideDuration << ")"<<endl;
            
            heap->deleteMin();  // Remove the min value from heap
            RbtRideNode temp;
            temp.rideId = node->rideId;
            // rbTree->printTree();
            RbtRideNode *rbtNode = rbTree->deleteKey(temp);  // Remove the node from the rbt
            cout<<"After delete"<<endl;
            rbTree->printTree();
            delete node;
            delete rbtNode;
        }

        void cancelRide(int rideNumber){
            RbtRideNode deleteNode;
            deleteNode.rideId = rideNumber;

            RbtRideNode *node = rbTree->search(deleteNode);

            if(node == nullptr || node->rideId != rideNumber){
                // Ride is not found in the rbt tree
                cout << "rideNumber - " << rideNumber << " not found";
                return;
            }

            RideNode *minNode = new RideNode;
            minNode->rideCost = -1;
            minNode->rideDuration = -1;
            minNode->rideId = rideNumber;

            // To delete any node from heap make the node the minimum node and then use remove min
            heap->replaceKey(node->heapPointer, minNode);
            heap->deleteMin();

            // Once the heap node is deleted, delete the node from the red black tree
            RbtRideNode *rbtNode = rbTree->deleteKey(deleteNode);
            delete rbtNode;
            delete minNode;

        }

        void updateTrip(int rideNumber, int newDuration){
            // Update the trip duration. First search the rbt for the node and use it to update the heap
            RbtRideNode *temp = new RbtRideNode;
            temp->rideId = rideNumber;
            RbtRideNode *currentNode = rbTree->search(*temp);

            if(currentNode == nullptr || currentNode->rideId != rideNumber){
                cout << "rideNumber - "<< rideNumber<<" not found";
                return;
            }

            // Created an updated node which can be used to replace the existing node
            RideNode *updateNode = new RideNode;
            updateNode->rideId = rideNumber;
            int currentDuration = currentNode->heapPointer->data->rideDuration;
            int rideCost = currentNode->heapPointer->data->rideCost;
            
            if( currentDuration > newDuration){
                // If the new time is less than the current duration just update the entry
                updateNode->rideDuration = newDuration;
                updateNode->rideCost = rideCost;
                heap->replaceKey(currentNode->heapPointer, updateNode);
            } else if(newDuration > currentDuration && newDuration < 2 * currentDuration){  
                // If new duration is less than twice current add penalty of 10
                updateNode->rideDuration = newDuration;
                updateNode->rideCost = rideCost + 10;
                heap->replaceKey(currentNode->heapPointer, updateNode);
            } else{  
                //  If new duration is greater than twice the current duration cancel the ride
                cancelRide(rideNumber);
            }

        }

};


int main(int argc, char **argv) {

    if (argc < 2){
        cout << "Please give the name of test file as parameter" << endl;
    }
    ifstream ifile(argv[1]);
    string line;
    GatorTaxi gatorTaxi;

    while (!ifile.eof())
    {
        ifile >> line;
        int p_satrt = line.find_first_of('(');
        int p_end = line.find_first_of(')');
        string fn = line.substr(0, p_satrt);
        if (fn.compare("Insert") == 0){
            // Call the insert function with the given ridenumber, ridecost and rideduration
            int p2 = line.find_first_of(',');
            int rideNumber = atoi(line.substr(p_satrt+1, p2).c_str());
            line = line.substr(p2+1, p_end);
            p2 = line.find_first_of(',');
            int rideCost = atoi(line.substr(0, p2).c_str());
            line = line.substr(p2+1, p_end);
            int rideDuration = atoi(line.c_str());
            cout<<"\n***********************"<<endl;
            cout<<endl<<fn<<" : "<<rideNumber<<" : "<<rideCost<<" : "<<rideDuration<<endl;
            bool inserted = gatorTaxi.insert(rideNumber, rideCost, rideDuration);
            if (!inserted){
                break;
            }
        } else if (fn.compare("Print") == 0){
            // Call the print function. based on the number of args two options
            int p2 = line.find_first_of(',');
            if (p2 == string::npos){
                int rideNumber = atoi(line.substr(p_satrt+1, p_end).c_str());
                cout<<"\n***********************"<<endl;
                cout<<endl<<fn<<" : "<<rideNumber<<endl;
                gatorTaxi.print(rideNumber);
                
            } else {
                int rideNumber1 = atoi(line.substr(p_satrt+1, p2).c_str());
                line = line.substr(p2+1, p_end);
                int rideNumber2 = atoi(line.c_str());
                cout<<"\n***********************"<<endl;
                cout<<endl<<fn<<" : "<<rideNumber1<<" : "<<rideNumber2<<endl;
                gatorTaxi.print(rideNumber1, rideNumber2);
                

            }
        } else if (fn.compare("UpdateTrip") == 0){
            // Call the updateTrip function
            int p2 = line.find_first_of(',');
            int rideNumber = atoi(line.substr(p_satrt+1, p2).c_str());
            line = line.substr(p2+1, p_end);
            int rideDuration = atoi(line.c_str());
            cout<<"\n***********************"<<endl;
            cout<<endl<<fn<<" : "<<rideNumber<<" : "<<rideDuration<<endl;
            gatorTaxi.updateTrip(rideNumber, rideDuration);
            

        } else if (fn.compare("CancelRide") == 0){
            // Call the cancel ride function using the ride umber
            int rideNumber = atoi(line.substr(p_satrt+1, p_end).c_str());
            cout<<"\n***********************"<<endl;
            cout<<endl<<fn<<" : "<<rideNumber<<endl;
            gatorTaxi.cancelRide(rideNumber);
        } else if (fn.compare("GetNextRide") == 0){
            // Call the getNextRide function
            cout<<"\n***********************"<<endl;
            cout<<endl<<fn<<endl;
            gatorTaxi.getNextRide();
        }
                
    }
    
}


