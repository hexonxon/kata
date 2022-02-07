#include <unordered_set>
#include <vector>
#include <functional>
#include <cassert>
#include <iostream>

struct tree_node {
    struct tree_node* left;
    struct tree_node* right;
    int val;
};

static struct tree_node* make_node(int val)
{
    struct tree_node* n = (struct tree_node*) malloc(sizeof(*n));
    assert(n);
    n->left = n->right = nullptr;
    n->val = val;
    return n;
}

struct tree_node* make_tree(int* data, size_t size)
{
    if (!data || !size)
        return NULL;
        
    size_t root_idx = (size == 1 ? 0 : size >> 1);
    struct tree_node* root = make_node(data[root_idx]);
    root->left = make_tree(data, root_idx);
    root->right = make_tree(data + root_idx + 1, size - root_idx - 1);
    
    return root;
}

static void walk_bst(struct tree_node* root, const std::function<void(struct tree_node*)>& visitor)
{
    if (!root)
        return;
        
    walk_bst(root->left, visitor);
    visitor(root);
    walk_bst(root->right, visitor);
}

int main()
{
    int data[] = {1, 3, 4, 6, 7, 8};
    struct tree_node* root = make_tree(data, 6);
    
    std::vector<int> inorder;
    walk_bst(root, [&](struct tree_node* root) { inorder.push_back(root->val); });
    for (size_t i = 0; i < 6; i++) {
        assert(data[i] == inorder[i]);
    }
    
    return 0;
}
