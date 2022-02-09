#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <vector>

struct tree_node {
    struct tree_node* left;
    struct tree_node* right;
    struct tree_node* parent;
};

static struct tree_node* make_node(struct tree_node* parent)
{
    struct tree_node* n = (struct tree_node*) malloc(sizeof(*n));
    assert(n);
    n->left = n->right = nullptr;
    n->parent = parent;
    return n;
}

const struct tree_node* successor(const struct tree_node* head)
{
    if (!head)
        return nullptr;
        
    /* Leftmost node in right subtree */
    if (head->right) {
        head = head->right;
        while (head->left) {
            head = head->left;
        }
        
        return head;
    }
    
    /* Look for parent up until we return from its leftmost child */
    const struct tree_node* parent = head->parent;
    while (parent) {
        if (parent->left == head)
            break;
        head = parent;
        parent = parent->parent;
    }
    
    return parent;
}

int main()
{
    struct tree_node* head = make_node(nullptr);
    head->left = make_node(head);
    head->left->left = make_node(head->left);
    head->left->right = make_node(head->left);
    head->right = make_node(head);
    
    assert(successor(head->left->left) == head->left);
    assert(successor(head->left) == head->left->right);
    assert(successor(head->left->right) == head);
    
    std::vector<tree_node*> inorder = {head->left->left, head->left, head->left->right, head, head->right};
    for (size_t i = 0; i < inorder.size(); i++) {
        if (i == inorder.size() - 1)
            assert(successor(inorder[i]) == nullptr);
        else
            assert(successor(inorder[i]) == inorder[i + 1]);
    }
    
    return 0;
}
