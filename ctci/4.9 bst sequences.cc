// Online C++ compiler to run C++ program online
#include <vector>
#include <list>
#include <iostream>
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

void generate_interleavings(const std::vector<int>& a, size_t a_pos, const std::vector<int>& b, size_t b_pos, std::vector<int>& prefix, std::vector<std::vector<int>>& reы)
{
    if (a_pos == a.size() || b_pos == b.size()) {
        res.emplace_back(prefix);
        std::vector<int>& seq = res.back();
        std::copy(a.begin() + a_pos, a.end(), std::back_inserter(seq));
        std::copy(b.begin() + b_pos, b.end(), std::back_inserter(seq));
        return;
    }
    
    prefix.push_back(a[a_pos]);
    generate_interleavings(a, a_pos + 1, b, b_pos, prefix, res);
    prefix.pop_back();
    
    prefix.push_back(b[b_pos]);
    generate_interleavings(a, a_pos, b, b_pos + 1, prefix, res);
    prefix.pop_back();
}

std::vector<std::vector<int>> bst_sequences(tree_node* root)
{
    if (!root)
        return {{}};

    std::vector<std::vector<int>> left_seqs = bst_sequences(root->left);
    std::vector<std::vector<int>> right_seqs = bst_sequences(root->right);
    std::vector<std::vector<int>> melded;
    std::vector<int> prefix = {root->val};
    
    for (const std::vector<int>& a : left_seqs) {
        for (const std::vector<int>& b : right_seqs) {
            generate_interleavings(a, 0, b, 0, prefix, melded);
        }
    }
    
    return melded;
}

int main()
{
    tree_node* root = make_node(3);
    root->left = make_node(1);
    root->left->left = make_node(0);
    root->left->right = make_node(2);
    root->right = make_node(4);
    
    std::vector<std::vector<int>> seqs = bst_sequences(root);
    for (const std::vector<int>& s : seqs) {
        for (int v : s) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}
