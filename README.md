# GatorTaxi
A mock taxi app which uses a min heap and red black tree to keep track of rides. A pairing heap is used as the min heap to keep track of the ride with minimum cost. The min heap is ordered based on the rideCost and if 2 rides have the same cost, the ride duration has been used. The red-black tree is ordered based on the ride number. Below are the main functionalities implemented.

### Print(rideNumber)
It makes use of the redBlack tree to make the search by ride number in O(log n). The space requirement for print is O(1).

###(rideNumber1, rideNumber2)
Uses the binary search tree property of the redblack tree to implement a recursive function that will add the rides with ride numbers between the 2 ride numbers into a list. The function runs in O(log(n) + m) where m is the number of matching nodes. The function recursively iterates the left sub-tree if the current node is greater than range start given by ride number 1. Similarly, it iterates the right subtree only if the current node is less than the range end given by rideNumber2. This makes sure that only the nodes which are in the range will be visited, except for at most 2 nodes - one node below the range and one node above the range. The space complexity of this operation is O(m) used to store the rides that match this range criterion.

### Insert(rideNumber, rideCost, rideDuration) 
Insert first tries to insert the node into the red-black tree. If the insertion to the red-black tree is successful, the node is inserted into the min heap. The red-black tree node also maintains a pointer to the equivalent node in the heap. If the insert fails there is a duplicate node. Inserting a new node to Red-Black Tree takes O(log n) time to search for the proper node to insert and then O(1) to add the node into the tree. Once inserted the rebalancing takes O(log n) time worst-case complexity.
### GetNextRide() 
Uses the min heap to retrieve the ride with min cost. Accessing the min value from min heap can be done in O(1) complexity. After this, the element has to be deleted, the ammortized time complexity for deleteMin operation in pairing heap is O(log n). I have used the two pass merge strategy to merge the children after the delete operation. Once, the node is
deleted from heap, we also have to delete the node from the red-black tree. This deletion can be done in O(log n). So in total GetNextRide works in O(log n) complexity.

### CancelRide() 
We use the red-black tree to search for the ride and then use the heap pointer to find the ride in the min heap. The red black tree search and delete is done in O(log n) time. Once we get the node in heap from the red black tree - we can remove the sub tree rooted at node from the heap and reinsert it into the heap with minimum value making sure that the node to be deleted is the min node. Now we can remove the node using delete-min operation. The overall complexity of this is O(log n).
### UpdateTrip() 
There are three cases for update trip - 
- If the new duration is less than the existing duration, we update the ride with the
new trip duration. Once the ride duration is updated, it might affect the nodes location in heap tree. So we use the replace key method to change the nodes value. This is done in O(1) complexity. If the node whose value is being updated is the root node just update the value by assigning the new value. If not, remove the sub tree rooted at that node, assign the new value, then reinsert the node which can also be done in O(1) complexity. No change is required wit respect to red black tree.
- If the new duration is greater than the current duration but less than twice the current duration, we update the ride duration with new ride duration and also add a penalty of 10 to the ride cost. Again we follow the same steps as case 1. The only difference is that since we are increasing the duration and cost, the root node may change and thus this operation may involve a delete-min operation which take O(log n) complexity. Thus this operation runs in O(log n) complexity.
- If the new duration is greater than twice the current duration, we cancel the ride. This is done in O(log N) complexity using the cancel ride operation.
