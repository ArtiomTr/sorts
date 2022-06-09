#include <iostream>
#include <vector>
#include <stack>

using namespace std;

struct Counters {
    int assignments;
    int comparisons;
    vector<int> list;
};


// Binary search tree ADT. Doesn't have any balancing logic.
class UnbalancedTree {
private:
    // Node of BST.
    struct Node {
    public:
        // Saved value.
        int value;
        // Index of Node (required only for debugging).
        size_t index;
        Node *left;
        Node *right;

        explicit Node(int value, size_t index) :
                value(value),
                index(index),
                left(nullptr),
                right(nullptr) {
        }

        ~Node() {
            delete left;
            delete right;
        }
    };

    // Root Node of the BST.
    Node *root;
    // Debugging counters.
    Counters &counters;
    // The size of BST.
    size_t size;
private:
    // Traverse BST Node, "inorder".
    void traverseNode(Node *target, int depth, vector<int> &output) {
        if (target == nullptr) {
            return;
        }

        // Traversing left subtree, containing all smaller values, than current.
        traverseNode(target->left, depth + 1, output);

        // Pushing Node to the output.
        output.push_back(target->value);
        ++counters.assignments;
        // Debugging information.
        counters.list[target->index] = depth;

        // Traversing right subtree, containing all bigger values, than current.
        traverseNode(target->right, depth + 1, output);
    }

public:
    explicit UnbalancedTree(Counters &counters) :
            root(nullptr),
            counters(counters),
            size(0) {
    }

    ~UnbalancedTree() {
        delete root;
    }

    // Function, which inserts new value into BST.
    void insert(int value) {
        auto &[assignments, comparisons, _] = counters;

        // Index of new Node.
        size_t index = size;
        ++size;

        // Tree doesn't have any nodes - create new root.
        if (root == nullptr) {
            root = new Node(value, index);
            ++assignments;
            return;
        }

        // Searching the parent of the new Node.
        Node *current = root;
        while (true) {
            ++comparisons;
            // Should new value be placed in right subtree?
            bool pickRightSubtree = value > current->value;

            if (pickRightSubtree && current->right == nullptr) { // Found place for the new Node.
                current->right = new Node(value, index);
                ++assignments;
                break;
            } else if (!pickRightSubtree && current->left == nullptr) { // Found place for the new Node.
                current->left = new Node(value, index);
                ++assignments;
                break;
            } else if (pickRightSubtree) { // Continue search.
                current = current->right;
            } else { // Continue search.
                current = current->left;
            }
        }
    }

    // Traverse BST in "inorder"
    vector<int> inorder() {
        auto&[assignments, comparisons, depths] = counters;
        // Resizing list of depths to the size of BST.
        depths.resize(size);

        vector<int> output;
        traverseNode(root, 0, output);
        return output;
    }
};

Counters treeSort(vector<int> &list) {
    vector<int> depths;

    Counters counters{
            .assignments = 0,
            .comparisons = 0,
            .list = depths,
    };

    // Constructing BST.
    UnbalancedTree tree(counters);

    for (int item: list) {
        tree.insert(item);
    }

    // Traversing BST to get sorted list.
    list = tree.inorder();

    return counters;
}

// Function, that merges half-sorted interval from one list into another (auxiliary).
//   begin     - beginning of interval.
//   end       - end of interval.
//   middle    - point, which separates interval into two sorted parts.
//   list      - input list.
//   auxiliary - output list.
//   counters  - debugging counters.
int merge(int begin, int end, int middle, vector<int> &list, vector<int> &auxiliary, Counters &counters) {
    auto&[assignments, comparisons, mergeWidths] = counters;

    // Tiny optimization - if length of sequence is 1, no merge operation required - simply copy element.
    if (end - begin == 1) {
        auxiliary[begin] = list[begin];
        ++assignments;
        return end;
    }

    // Index for iterating first part of interval.
    int firstIndex = begin;
    // Index for iterating second part of interval.
    int secondIndex = middle;

    // Measurements.
    mergeWidths.push_back(end - begin);

    int firstSwappedIndex = end;

    // Loop while not all elements of given interval were merged.
    while (firstIndex < middle || secondIndex < end) {
        // Index of element in output list.
        int currentIndex = firstIndex + (secondIndex - middle);

        if (firstIndex < middle && secondIndex < end) { // Both parts are not merged yet.
            // Measurements.
            ++comparisons;
            // Pick greater element, and put it into auxiliary list. Also, moving corresponding index.
            if (list[firstIndex] < list[secondIndex]) {
                auxiliary[currentIndex] = list[firstIndex];
                ++firstIndex;
            } else {
                auxiliary[currentIndex] = list[secondIndex];
                ++secondIndex;
                firstSwappedIndex = min(firstSwappedIndex, currentIndex);
            }
            ++assignments;
        } else if (firstIndex < middle) { // Only first half is not merged yet.
            // Pick element - nothing to compare.
            auxiliary[currentIndex] = list[firstIndex];
            // Measurements.
            ++assignments;
            // Moving to the next element.
            ++firstIndex;
        } else if (secondIndex < end) { // Only second half is not merged yet.
            // Pick element - nothing to compare.
            auxiliary[currentIndex] = list[secondIndex];
            // Measurements.
            ++assignments;
            // Moving to the next position.
            ++secondIndex;
        }
    }

    return firstSwappedIndex;
}

// Implementation of merge sort. Uses O(N) additional space.
Counters mergeSort(vector<int> &list) {
    // Measurements.
    Counters counters{
            .assignments = 0,
            .comparisons = 0,
            .list{}
    };

    // Preparing temporary list.
    vector<int> auxList{};
    auxList.resize(list.size());

    // Pointers to the lists. The "list" and "auxList" change their meaning after each merge-pass: values from list
    //     are merged into auxList on the first pass, values from auxList are merged into list on second pass and so on.
    //     To simplify array-swapping logic, we use two references, so the task to change "meaning" of lists is trivial
    //     and performant: just call swap(target, auxiliary).
    vector<int> *target = &list,
            *auxiliary = &auxList;

    // The width of sequence, that is ignored during the merge. For example, if we have a list of length 7, on the first pass
    //     the outsiderCount will be set to 1 because mergeWidth is 2, only the last element won't be included. On the
    //     second pass, the outsiderCount will be 3, because mergeWidth is 4, and 3 last elements won't be included.
    //     We track this value to prevent unnecessary copies.
    int outsiderCount = 0;
    // Tracking width of sequences, that are merged on the current pass.
    int mergeWidth = 2;

    while (mergeWidth <= list.size()) {
        // Iterating through all sequences of length "mergeWidth", that should be merged on this pass.
        for (int iteration = 0; iteration < list.size() / mergeWidth; ++iteration) {
            // Merging sequences.
            merge(iteration * mergeWidth,
                  (iteration + 1) * mergeWidth,
                  iteration * mergeWidth + mergeWidth / 2,
                  *target,
                  *auxiliary,
                  counters);
        }

        int listSize = static_cast<int>(list.size());
        // Recalculating outsiderCount.
        int newOutsiderCount = listSize % mergeWidth;
        // Delta of old outsider element count and new.
        int delta = newOutsiderCount - outsiderCount;
        // Checking, if outsider count increased.
        if (delta > 0) {
            // Merging old outsider list with new one.
            int index = merge(listSize - newOutsiderCount,
                              list.size(),
                              list.size() - outsiderCount,
                              *target,
                              *auxiliary,
                              counters);

            // Copying merged outsiders back to the original list.
            for (; index < list.size(); ++index) {
                target->at(index) = auxiliary->at(index);
                ++counters.assignments;
            }
        }

        // Setting outsiderCount.
        outsiderCount = newOutsiderCount;

        // Increasing mergeWidth.
        mergeWidth *= 2;
        // Changing list meanings - target is now auxiliary, and auxiliary is target.
        swap(target, auxiliary);
    }

    // If there are any outsider elements, additional merge-pass required.
    if (outsiderCount > 0) {
        int listSize = static_cast<int>(list.size());
        merge(0,
              listSize,
              listSize - outsiderCount,
              *target,
              *auxiliary,
              counters);
        // Changing list meanings once more.
        swap(target, auxiliary);
    }

    // Target list contains sorted array. However, it is required to put output to the "list" variable. So this is a
    //     check, if target is actually "list" and not "auxList". If not, copying all elements.
    if (target != &list) {
        for (int i = 0; i < list.size(); ++i) {
            list[i] = target->at(i);
            ++counters.assignments;
        }
    }

    return counters;
}

// Fixing max heap, using bottom-up approach.
void siftUp(vector<int> &list, int start, int end, Counters &counters) {
    auto&[assignments, comparisons, _] = counters;

    // Child, that is being checked.
    int child = end;
    // Element, that is in rotation.
    int rotationElement;
    // Flag, indicating if rotation began or not.
    bool isRotating = false;
    // Moving while child is in max heap.
    while (child > start) {
        // Index of parent element.
        int parent = (child - 1) / 2;

        ++comparisons;
        // Comparing parent element to the child, keeping rotation in mind.
        bool move = isRotating ? (list[parent] < rotationElement) : (list[parent] < list[child]);

        if (move) { // Element is not in right place, move it.

            if (!isRotating) { // Begin new rotation.
                rotationElement = list[child];
                ++assignments;
                // Check flag.
                isRotating = true;
            }

            // Push element towards right position.
            list[child] = list[parent];
            ++assignments;

            // Next element to check - child's parent.
            child = parent;
        } else { // Element is on the right place - heap is balanced.
            break;
        }
    }

    // Finish last rotation.
    if (isRotating) {
        list[child] = rotationElement;
        ++assignments;
    }
}

// Function that converts list to max heap.
void heapify(vector<int> &list, Counters &counters) {
    // The last element, that has been added to max heap.
    int end = 1;

    // Adding all elements to max heap.
    while (end < list.size()) {
        // After adding element to the bottom of max heap, we need to sift up all heap.
        siftUp(list, 0, end, counters);
        // Extending max heap, adding one element to the end.
        ++end;
    }
}

// Fixing max heap using top-down approach.
void siftDown(vector<int> &list, int start, int end, Counters &counters) {
    auto&[assignments, comparisons, _] = counters;

    // Root element, that should be fixed.
    int root = start;

    // Child element, that is being checked.
    int child;
    // Item that is in rotation.
    int rotationItem;
    // Flag, indicating, if currently some element is in rotation.
    bool isRotating = false;

    // While left child of root element is in max heap.
    while ((child = 2 * root + 1) <= end) {
        // Saving index, that will be swapped with root element.
        int swap = root;
        // Flag, indicating, if swap variable has changed, or not.
        bool changed = false;

        // Comparing heap root element with left child, keeping rotation in mind.
        if (++comparisons, isRotating ? (rotationItem < list[child]) : (list[swap] < list[child])) {
            changed = true;
            swap = child;
        }

        // Comparing swap element with right child of the root, keeping rotation in mind.
        if (child + 1 <= end &&
            (++comparisons, isRotating && !changed ?
                            (rotationItem < list[child + 1]) :
                            (list[swap] < list[child + 1]))) {
            swap = child + 1;
        }

        // If swap element is root, it means that no additional swap required - heap is already balanced.
        if (swap == root) {
            break;
        } else {
            // If wasn't in rotation, begin it.
            if (!isRotating) {
                // Take element into rotation.
                rotationItem = list[root];
                // Measurements.
                ++assignments;
                // Check flag.
                isRotating = true;
            }

            // Pushing element, towards right position.
            list[root] = list[swap];
            // Measurements.
            ++assignments;

            root = swap;
        }
    }

    // Finish last rotation.
    if (isRotating) {
        list[root] = rotationItem;
        ++assignments;
    }
}

// Implementation of the heap sort algorithm.
Counters heapSort(vector<int> &list) {
    // Measurements.
    Counters counters{
            .assignments = 0,
            .comparisons = 0,
            .list{}
    };

    // Converting array to max heap.
    heapify(list, counters);

    // Tracking the end of max heap. It will reduce because we will remove the top element.
    int end = static_cast<int>(list.size()) - 1;

    // While max heap is not empty.
    while (end > 0) {
        // Moving top of heap to the end.
        swap(list[end], list[0]);
        counters.assignments += 3;

        // Reducing heap size.
        --end;

        // We've removed the top element from the heap. Now, we need to move from the top to the heap bottom.
        siftDown(list, 0, end, counters);
    }

    return counters;
}

// Helper function for printing vectors. Format: each value separated by space.
template<class T>
ostream &operator<<(ostream &out, vector<T> items) {
    for (T item: items) {
        out << item << " ";
    }

    return out;
}

int main() {
    vector<int> items{3, 5, 9, 1, 2, 4, 8, 7, 6};

    {
        vector<int> itemCopy = items;
        auto[assignments, comparisons, medians] = quickSort(itemCopy);

        cout << "Quick sort:\n"
             << "  Assignments: " << assignments << '\n'
             << "  Comparisons: " << comparisons << '\n'
             << "  Medians: " << medians << endl;
    }

    {
        vector<int> itemCopy = items;
        auto[assignments, comparisons, medians] = treeSort(itemCopy);

        cout << "Tree sort:\n"
             << "  Assignments: " << assignments << '\n'
             << "  Comparisons: " << comparisons << '\n'
             << "  Depths: " << medians << endl;
    }

    {
        vector<int> itemCopy = items;
        auto[assignments, comparisons, mergeSizes] = mergeSort(itemCopy);

        cout << "Merge sort:\n"
             << "  Assignments: " << assignments << '\n'
             << "  Comparisons: " << comparisons << '\n'
             << "  Merge sizes: " << mergeSizes << endl;
    }

    {
        vector<int> itemCopy = items;
        auto[assignments, comparisons, _] = heapSort(itemCopy);

        cout << "Heap sort:\n"
             << "  Assignments: " << assignments << '\n'
             << "  Comparisons: " << comparisons << endl;
    }

    return 0;
}
