#include <cassert>
#include <climits>
#include <cstdlib>

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

bool is_bst(const struct tree_node* head, int min, int max)
{
    assert(head);
    
    if (head->val < min || head->val > max)
        return false;
    
    if (head->left && !is_bst(head->left, min, head->val))
        return false;
        
    if (head->right && !is_bst(head->right, head->val, max))
        return false;
    
    return true;
}

bool is_bst(const struct tree_node* head)
{
    if (!head)
        return false;
        
    return is_bst(head, INT_MIN, INT_MAX);
}

int main()
{
    struct tree_node* left_subtree =  make_node(3);
    left_subtree->left = make_node(1);
    left_subtree->right = make_node(8);
    assert(is_bst(left_subtree));
    
    struct tree_node* right_subtree = make_node(7);
    assert(is_bst(right_subtree));
    
    struct tree_node* head = make_node(5);
    head->left = left_subtree;
    head->right = right_subtree;
    assert(!is_bst(head));
    
    return 0;
}
