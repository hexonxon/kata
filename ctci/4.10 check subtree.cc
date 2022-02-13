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
    int val;
};

static tree_node* make_node(int val)
{
    tree_node* n = (tree_node*) malloc(sizeof(*n));
    n->left = n->right = nullptr;
    n->val = val;
    return n;
}

bool compare_trees(const tree_node* t1, const tree_node* t2)
{
    if (!t1 && !t2)
        return true;
    else if (!t1 || !t2)
        return false;

    if (t1->val != t2->val)
        return false;
        
    if (!compare_trees(t1->left, t2->left))
        return false;
        
    if (!compare_trees(t1->right, t2->right))
        return false;
        
    return true;
}

const tree_node* find_node(const tree_node* haystack, const tree_node* needle)
{
    assert(needle);
    
    if (!haystack)
        return nullptr;
        
    if (haystack->val == needle->val)
        return haystack;
    
    const tree_node* res;    
    res = find_node(haystack->left, needle);
    if (res)
        return res;
        
    res = find_node(haystack->right, needle);
    if (res)
        return res;
        
    return nullptr;
}

bool is_subtree(const tree_node* haystack, const tree_node* needle)
{
    const tree_node* needle_root = find_node(haystack, needle);
    if (!needle_root)
        return false;
    
    return compare_trees(needle_root, needle);
}

int main()
{
    tree_node* t1 = make_node(3);
    t1->left = make_node(1);
    t1->left->left = make_node(0);
    t1->left->right = make_node(2);
    t1->right = make_node(4);
    
    tree_node* t2 = make_node(1);
    t2->left = make_node(0);
    t2->right = make_node(2);
    
    assert(is_subtree(t1, t2));
    assert(!is_subtree(t2, t1));
    
    t1->left->left->left = make_node(-1);
    assert(!is_subtree(t1, t2));
    
    return 0;
}
