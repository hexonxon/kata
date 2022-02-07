#include <unordered_set>
#include <vector>
#include <list>
#include <queue>
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

std::vector<std::list<struct tree_node*>> list_depths(struct tree_node* head)
{
    if (!head)
        return {};
        
    std::vector<std::list<struct tree_node*>> res;
    std::queue<struct tree_node*> nodes;
    nodes.push(head);
    
    res.push_back({head});
    std::list<tree_node*>* cur_level = &res[0];
    
    while (true) {
        std::list<tree_node*> next_level;
        for (tree_node* n : *cur_level) {
            if (n->left)
                next_level.push_back(n->left);
            if (n->right)
                next_level.push_back(n->right);
        }
        
        if (next_level.empty())
            break;

        res.emplace_back(next_level);
        cur_level = &res.back();
    }

    return res;    
}

int main()
{
    struct tree_node* root = make_node(5);
    root->left = make_node(3);
    root->left->left = make_node(2);
    root->left->right = make_node(4);
    root->right = make_node(7);
    root->right->right = make_node(8);
    
    std::vector<std::list<tree_node*>> lists = list_depths(root);
    assert(lists.size() == 3);
    
    auto p = lists[0].begin();
    assert((*p)->val == 5); ++p;
    assert(p == lists[0].end());

    p = lists[1].begin();
    assert((*p)->val == 3); ++p;
    assert((*p)->val == 7); ++p;
    assert(p == lists[1].end());
    
    p = lists[2].begin();
    assert((*p)->val == 2); ++p;
    assert((*p)->val == 4); ++p;
    assert((*p)->val == 8); ++p;
    assert(p == lists[2].end());
    
    return 0;
}
