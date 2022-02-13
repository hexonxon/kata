#include <functional>
#include <cstdlib>
#include <cassert>

struct tree_node {
    struct tree_node* left;
    struct tree_node* right;
    int val;
    size_t size; /* Subtree size */
};

static tree_node* make_node(int val)
{
    tree_node* n = (tree_node*) malloc(sizeof(*n));
    n->left = n->right = nullptr;
    n->val = val;
    n->size = 1;
    return n;
}

void insert_node(tree_node* root, int val)
{
    tree_node** pchild = nullptr;        
    if (val < root->val) {
        pchild = &root->left;
    } else {
        pchild = &root->right;
    }

    root->size++;    
    if (*pchild) {
        insert_node(*pchild, val);
    } else {
        *pchild = make_node(val);
    }
}

tree_node* get_random(tree_node* root)
{
    std::function<tree_node*(tree_node*, size_t)> helper =
        [&helper](tree_node* root, size_t idx) -> tree_node* {
            if (!root)
                return nullptr;
            
            size_t left_size = !root->left ? 0 : root->left->size;
            size_t right_size = !root->right ? 0 : root->right->size;

            if (idx < left_size) {
                return helper(root->left, idx);
            } else if (idx == left_size) {
                return root;
            } else {
                return helper(root->right, idx - left_size - 1);
            }
        };

    return helper(root, rand() % root->size);
}

int main()
{
    tree_node* root = make_node(3);
    insert_node(root, 2);
    insert_node(root, 4);
    insert_node(root, 0);
    insert_node(root, 1);

    assert(get_random(root));
    assert(get_random(root));
    assert(get_random(root));
    assert(get_random(root));
    assert(get_random(root));

    return 0;
}
