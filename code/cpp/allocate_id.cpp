/*
Dropbox

Id Allocator / Number / Phone Number

Write a class for an id allocator that can allocate and release ids
*/

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <limits>
#include <stdexcept>
#include <chrono>
#include <mutex>
#include <chrono>
#include <thread>
#include <bitset>
#include <cmath>

#define CURRENT_TIME std::chrono::high_resolution_clock::now()
#define GET_DURATION(a) std::chrono::duration_cast<std::chrono::duration<float>>(a)

// this one is not space efficent but it is the time complexity is o log n i think
// memory complexity is n * 4bytes * 2 because of the 2 containers that have int
class Allocator{
    private:
        int max_value;
        int ids_iterator;
        std::queue<int> released_ids;
        std::unordered_set<int> allocated_ids;
    public:
        explicit Allocator(int max_value) : max_value(max_value){
            ids_iterator = 0;
        }

        int allocateId() {
            // allocate a new id and return it
            int id_to_be_given = -1;

            if (!released_ids.empty()) {
                id_to_be_given = released_ids.front();
                released_ids.pop(); //remove first element
            } else if (ids_iterator <= max_value) {
                id_to_be_given = ids_iterator; // if ids_iterator is 0 then insert 0 and after that incerement the iterator
                ids_iterator++;
            }

            if (id_to_be_given == -1) {
                throw std::overflow_error("No Ids available");
            }

            allocated_ids.insert(id_to_be_given);
            return id_to_be_given;

        }

        void releaseId(int Id) {
            // release allocated id and allow it to be allocated again another time
            if (Id < 0 || Id > max_value || (allocated_ids.find(Id) != allocated_ids.end())) {
                // throw an exception
                throw std::invalid_argument("The Id " + std::to_string(Id) + " cannot be released");
            }
            allocated_ids.erase(allocated_ids.find(Id));
            released_ids.push(Id);
        }
};

class MemoryOrSpaceEfficentAllocator{
    private:
        int max_value;
        std::vector<bool> id_vector;
        std::mutex mutex_;
    public:
        explicit MemoryOrSpaceEfficentAllocator(int max_value) : max_value(max_value){
            id_vector.resize(max_value);
        }

        int allocateId() {
            // search for free ids that are false, if there are none throw error
            std::lock_guard<std::mutex> lock(mutex_);
            int id_to_be_given = -1;
            for (int id = 0; id < max_value; id++) {
                if (id_vector[id] == false) {
                    id_vector[id] = true;
                    id_to_be_given = id;
                    break;
                }
            }
            if (id_to_be_given == -1) {
                throw std::overflow_error("No Ids available");
            }
            return id_to_be_given;
        }

        void releaseId(int Id) {
            // release allocated id and allow it to be allocated again another time
            std::lock_guard<std::mutex> lock(mutex_);
            if (Id < 0 || Id > max_value || id_vector[Id]==false) {
                // throw an exception
                throw std::invalid_argument("The Id " + std::to_string(Id) + " cannot be released");
            }
            id_vector[Id] = false;
        }
};

class BinaryHeapAllocator{
    private:
        int max_value;
        std::vector<bool> id_heap_vector;
        std::mutex mutex_;
        int max_layer;
        int first_index_of_max_layer;
    public:
        explicit BinaryHeapAllocator(int max_val): max_value(max_val){ 
            id_heap_vector.resize(max_value * 2 -1); // talk about why that is
            max_layer = get_tree_layers_count(max_value * 2 -1);
            first_index_of_max_layer = std::pow(2, max_layer-1) -1;
        }

        int get_tree_layers_count(int heap_size) {
            int layer_count=0;
            while(heap_size>0) {
                heap_size/=2;
                layer_count++;
            }
            return layer_count;
        }
        
        int get_id_from_index(int index) {
            if (index >= first_index_of_max_layer) { // check if the index is at the most bottom level of tree
                return index - first_index_of_max_layer;
            }
            return index - (first_index_of_max_layer - max_value);
        }

        int get_index_from_id(int id) {
            if (id + first_index_of_max_layer < max_value * 2 -1) { // check if the index is at the most bottom level of tree
                return id + first_index_of_max_layer;
            }
            return id + (first_index_of_max_layer - max_value);
        }

        int allocateId() {
            // search for free ids that are false, if there are none throw error
            std::lock_guard<std::mutex> lock(mutex_);
            int index = 0;
            if (id_heap_vector[index] == true) { // top of the heap is True so that means that all the elements are also true
                throw std::overflow_error("No Ids available");
            }

            while (index < max_value-1) { // so now we want to walk all the way down the binary tree
                int left_child_index = 2*index + 1;
                int right_child_index = 2*index + 2;

                if (id_heap_vector[left_child_index] == false) {// unallocated Id in the left subtree
                    index = left_child_index;
                } else if (id_heap_vector[right_child_index] == false) { // unallocated Id in the right subtree
                    index = right_child_index;
                } else { //Both subtrees are allocated, this actually means you broke your tree
                    throw std::overflow_error("No Ids available");
                }
            }
            id_heap_vector[index] = true;
            update_tree(index);
            return (get_id_from_index(index));
        }

        void update_tree(int index) { 
            // update the subtrees starting from the leaf node
            int parent_index = index;
            while (parent_index > 0) {
                parent_index = (parent_index - 1) / 2;
                int left_child_index = 2 * parent_index + 1;
                int right_child_index = 2 * parent_index + 2;
                if (id_heap_vector[left_child_index] == true && id_heap_vector[right_child_index] == true) {
                    id_heap_vector[parent_index] = true;
                } else {
                    id_heap_vector[parent_index] = false;
                }
            }
        }

        void releaseId(int Id) {
            // release allocated id and allow it to be allocated again another time
            std::lock_guard<std::mutex> lock(mutex_);
            if (Id < 0 || Id > max_value || id_heap_vector[get_index_from_id(Id)] == false) {
                // throw an exception
                throw std::invalid_argument("The Id " + std::to_string(Id) + " cannot be released");
            }
            id_heap_vector[get_index_from_id(Id)] = false;
            update_tree(get_index_from_id(Id));
        }
};


int main () {
    auto start = CURRENT_TIME;
    Allocator first_allocator(1000);
    for (int i = 0; i < 1000; i++) {
        first_allocator.allocateId();
    }
    auto stop = CURRENT_TIME;
	auto duration = GET_DURATION(stop - start);
    std::cout << std::endl << std::to_string(duration.count()) << std::endl;

    start = CURRENT_TIME;
    MemoryOrSpaceEfficentAllocator second_allocator(1000);
    for (int i = 0; i < 1000; i++) {
        second_allocator.allocateId();
    }
    stop = CURRENT_TIME;
	duration = GET_DURATION(stop - start);
    std::cout << std::endl << std::to_string(duration.count()) << std::endl;

    start = CURRENT_TIME;
    BinaryHeapAllocator third_allocator(1000);
    for (int i = 0; i < 999; i++) {
        // third_allocator.allocateId();
        std::cout<<third_allocator.allocateId()<< " ";
    }
    std::cout<<std::endl;
    third_allocator.releaseId(73);
    std::cout<<third_allocator.allocateId()<< " ";
    std::cout<<third_allocator.allocateId()<< " ";
    third_allocator.releaseId(34);
    std::cout<<third_allocator.allocateId()<< " ";
    stop = CURRENT_TIME;
	duration = GET_DURATION(stop - start);
    std::cout << std::endl << std::to_string(duration.count()) << std::endl;
    return 0;
}

#if 0
class Allocator:

    def __init__(self, max_val):
        self.queue = collections.deque()
        self.first_pass_idx = 0 //your interview might not require this optimization
        self.allocated = set()
        self.max_val = max_val
        
    def allocate(self):
        /*Returns an unallocated id*/
        result = None
        if self.first_pass_idx <= self.max_val:
            self.first_pass_idx += 1
            result = self.first_pass_idx - 1
        elif len(self.queue) > 0:
            result = self.queue.pop()
        if result is not None:
            self.allocated.add(result)
            return result
        else:
            raise CannotAllocateException("No ids available")

    def release(self, id):
        /*Releases the id and allows it to be allocated*/
        if (not 0 <= id < self.max_val) or (id not in self.allocated):
            //You should say that you'd like to throw an exception in case of an error
            raise InvalidIdException(f"The id {id} cannot be released.")
        self.allocated.remove(id)
        self.queue.appendleft(id)
#endif

#if 0
/*
FOLLOW UP:
You might be asked to estimate the amount of memory you need for the above implementation.
You will be asked to make a more space-efficient implementation in which allocate and release might take longer than O(1).
For this, you can use a boolean array (a.k.a. a BitSet in Java, a bit-vector in other languages)
This uses max_id // (8 * 1024 * 1024) MB
*/

class SpaceEfficientAllocator:

    def __init__(self, max_val):
        self.max_val = max_val
        self.bool_array = [False] * max_val

    def allocate(self):
        /*Returns an unallocated id*/
        for id, value in enumerate(self.bool_array):
            if value == False: //The id has not been allocated
                self.bool_array[id] = True
                return id
        raise CannotAllocateException("No ids available")

    def release(self, id):
        /*Releases the id and allows it to be allocated*/
        if (not 0 <= id < self.max_val) or (self.bool_array[id] == True):
            raise Exception(f"The id {id} cannot be released.")
        self.bool_array[id] = False

#endif

#if 0
/*
FOLLOW UP:
This is the part where most people flunk. Come up with a way that is a little faster than O(n) for both allocate and release.
You can use a bit more space.

The hard way is to use an interval tree or a segment tree.
The easy way is to use a binary heap.
*/

class BinaryHeapAllocator:

    def __init__(self, max_val):
        self.max_val = max_val
        self.bool_array = [False] * (2 * max_val)

    def allocate(self):
        /*Returns an unallocated id*/
        index = 0
        if self.bool_array[index] == True:
            raise CannotAllocateException("No ids available")
        while index < max_val:
            left_child_index = 2 * index + 1
            right_child_index = 2 * index + 2
            if self.bool_array[left_child_index] == False: //There's an unallocated id in the subtree
                index = left_child_index
            elif self.bool_array[right_child_index] == False: //... in the right subtree
                index = right_child_index
            else: //Both subtrees are allocated, this actually means you broke your tree
                raise CannotAllocateException("No ids available")
        id = self.get_index_from_id(index)
        self.update_tree(id)
        

    def release(self, id):
        /*Releases the id and allows it to be allocated*/
        if (not 0 <= id < self.max_val) or (self.bool_array[id] == True):
            raise Exception(f"The id {id} cannot be released.")
        self.bool_array[id] = False
        self.update_tree(id)
    
    def get_index_from_id(self, id):
        return id + self.max_val - 1
    
    def get_id_from_index(self, index):
        return index - self.max_val + 1
    
    def update_tree(self, id):
        index = self.get_index_from_id(id)
        while index > 0:
            parent_index = (index - 1) // 2
            both_children_are_true = False
            if index % 2 == 1: //this is a left child
                if self.bool_array[index] == True == self.bool_array[index + 1]:
                    both_children_are_true = True
            else: //this is a right child
                if self.bool_array[index] == True == self.bool_array[index + 1]:
                    both_children_are_true = True
            self.bool_array[parent_index] = both_children_are_true
            index = parent_index
        self.bool_array[0] = self.bool_array[1] and self.bool_array[2]
#endif