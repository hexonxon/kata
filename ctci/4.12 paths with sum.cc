#include <functional>
#include <unordered_map>
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

size_t paths_with_sum(const tree_node* root, int sum)
{
    std::function<size_t(const tree_node*, int, int&, std::unordered_map<int, size_t>&)> helper =
        [&helper](const tree_node* root, int target_sum, int& running_sum, std::unordered_map<int, size_t>& prefix_lookup) -> size_t {
            if (!root)
                return 0;

            running_sum += root->val;
            size_t npaths = prefix_lookup[running_sum - target_sum];
            prefix_lookup[running_sum]++;
            
            npaths += helper(root->left, target_sum, running_sum, prefix_lookup);
            npaths += helper(root->right, target_sum, running_sum, prefix_lookup);
            
            prefix_lookup[running_sum]--;
            running_sum -= root->val;
            
            return npaths;
        };
        
    int running_sum = 0;
    std::unordered_map<int, size_t> prefix_lookup;
    prefix_lookup[0] = 1;
    return helper(root, sum, running_sum, prefix_lookup);
}

int main()
{
    tree_node* root = make_node(3);
    root->left = make_node(1);
    root->left->left = make_node(0);
    root->left->right = make_node(2);
    root->right = make_node(4);
    
    assert(paths_with_sum(root, 3) == 2);
    assert(paths_with_sum(root, 1) == 2);
    assert(paths_with_sum(root, 4) == 3);

    return 0;
}
