// Online C++ compiler to run C++ program online
#include <vector>
#include <list>
#include <iostream>
#include <functional>
#include <cstdlib>
#include <cassert>

struct tree_node {
    struct tree_node* left;
    struct tree_node* right;
};

static tree_node* make_node()
{
    tree_node* n = (tree_node*) malloc(sizeof(*n));
    n->left = n->right = nullptr;
    return n;
}

const tree_node* nearest_common_ancestor(const tree_node* root, const tree_node* p, const tree_node* q)
{
    std::function<bool(const tree_node*, const tree_node*, const tree_node*, const tree_node*&)> helper = 
        [&helper](const tree_node* root, const tree_node* p, const tree_node* q, const tree_node*& found) -> bool {
            if (!root)
                return false;
                
            if (root == p && root == q) {
                found = root;
                return true;
            }
    
            bool lres = helper(root->left, p, q, found);
            if (found)
                return true;
        
            bool rres = helper(root->right, p, q, found);
            if (found)
                return true;
    
            if (lres && rres) {
                found = root;
                return true;
            } else if (root == p || root == q) {
                if (lres || rres) {
                    found = root;
                }
                return true;
            }
        
            return lres || rres;    
        };
    
    const tree_node* found = nullptr;
    helper(root, p, q, found);
    return found;
}

int main()
{
    tree_node* root = make_node();
    root->left = make_node();
    root->left->left = make_node();
    root->left->right = make_node();
    root->right = make_node();
    
    assert(nearest_common_ancestor(root, root->left, root->left) == root->left);
    assert(nearest_common_ancestor(root, root->left->left, root->left->right) == root->left);
    assert(nearest_common_ancestor(root, root->left, root->left->right) == root->left);
    assert(nearest_common_ancestor(root, root->left->left, root->right) == root);
    
    return 0;
}
