#include "tree.h"

#include <stack>

#include "sort_meters.h"

class unbalanced_tree {
private:
    struct node {
    public:
        int value;
        node *left;
        node *right;

        explicit node(int value): value(value), left(nullptr), right(nullptr) {

        }
    };

    node *root;
    blab::quantity_meter &assignments, &comparisons;
public:
    explicit unbalanced_tree(blab::quantity_meter &assignments, blab::quantity_meter &comparisons):
        root(nullptr),
        assignments(assignments),
        comparisons(comparisons) {

    }

    void insert(int value) {
        if(root == nullptr) {
            root = new node(value);
            ++assignments;
            return;
        }

        node *current = root;
        while(true) {
            ++comparisons;
            bool pick_right = value > current->value;

            if(pick_right && current->right == nullptr) {
                current->right = new node(value);
                ++assignments;
                break;
            } else if(!pick_right && current->left == nullptr) {
                current->left = new node(value);
                ++assignments;
                break;
            } else if(pick_right) {
                current = current->right;
            } else {
                current = current->left;
            }
        }
    }

    std::vector<int> inorder() {
        std::vector<int> output{};
        std::stack<node*> stack{};

        stack.push(root);

        while(!stack.empty()) {
            node *current = stack.top();

            if(current->left != nullptr) {
                stack.push(current->left);
                current->left = nullptr;
            } else {
                output.push_back(current->value);
                ++assignments;
                stack.pop();
                if(current->right != nullptr) {
                    stack.push(current->right);
                }
            }
        }

        return output;
    }
};

std::vector<int> tree_sort_unbalanced(std::vector<int> list) {
    auto[assignments, comparisons, time] = get_sort_meters();

    time.start();

    unbalanced_tree tree(assignments, comparisons);

    for(int item : list) {
        tree.insert(item);
    }

    list = tree.inorder();

    time.stop();

    return list;
}