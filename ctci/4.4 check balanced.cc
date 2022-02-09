#include <cassert>
#include <cstdlib>
#include <algorithm>

struct tree_node {
    struct tree_node* left;
    struct tree_node* right;
};

static struct tree_node* make_node()
{
    struct tree_node* n = (struct tree_node*) malloc(sizeof(*n));
    assert(n);
    n->left = n->right = nullptr;
    return n;
}

bool check_height(struct tree_node* head, size_t& out_height)
{
    if (!head) {
        out_height = 0;
        return true;
    }

    size_t lh, rh;
    if (!check_height(head->left, lh) || !check_height(head->right, rh))
        return false;
    if (std::max(lh, rh) - std::min(lh, rh) > 1)
        return false;
    
    out_height = std::max(lh, rh) + 1;
    return true;
}

bool is_balanced(struct tree_node* head)
{
    size_t height;
    return check_height(head, height);
}

int main()
{
    struct tree_node* head = make_node();
    assert(is_balanced(head));
    
    head->left = make_node();
    head->left->left = make_node();
    assert(!is_balanced(head));
    
    head->right = make_node();
    assert(is_balanced(head));

    return 0;
}
