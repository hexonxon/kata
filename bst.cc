#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <functional>
#include <iostream>
#include <queue>

struct node_t
{
    long val = 0;
    node_t* parent = nullptr;
    node_t* left = nullptr;
    node_t* right = nullptr;
};

node_t* bst_create(long val)
{
    node_t* n = new node_t;
    n->val = val;
    return n;
}

node_t* bst_insert(node_t* root, long val)
{
    node_t* parent = nullptr;
    node_t** pnode = &root;

    while (root) {
        if (root->val == val) {
            return root;
        }
        
        parent = root;
        if (root->val > val) {
            pnode = &root->left;
            root = root->left;
        } else {
            pnode = &root->right;
            root = root->right;
        }
    }

    node_t* new_node = bst_create(val);
    new_node->parent = parent; 

    *pnode = new_node;
    return *pnode;
}

enum traversal_type_t
{
    INORDER = 0,
    PREORDER,
    POSTORDER,
};

void bst_walk_depth(const node_t* root, std::function<void (const node_t*)> cb, traversal_type_t type = INORDER)
{
    assert(root);

    if (type == PREORDER) {
        cb(root);
    }

    if (root->left) {
        bst_walk_depth(root->left, cb, type);
    }

    if (type == INORDER) {
        cb(root);
    }

    if (root->right) {
        bst_walk_depth(root->right, cb, type);
    }

    if (type == POSTORDER) {
        cb(root);
    }
}

void bst_walk_breadth(const node_t* root, std::function<void (const node_t*)> cb)
{
    assert(root);
    
    std::queue<const node_t*> nodes;
    nodes.push(root);

    while (!nodes.empty()) {
        const node_t* node = nodes.front();
        nodes.pop();

        cb(node);
        
        if (node->left) {
            nodes.push(node->left);
        }

        if (node->right) {
            nodes.push(node->right);
        }
    }
}

void bst_delete(node_t* node)
{
    if (!node) {
        return;
    }

    bst_delete(node->left);
    bst_delete(node->right);
    delete node;
}

const node_t* bst_nearest_common_parent(const node_t* root, const node_t* n1, const node_t* n2)
{
    assert (root && n1 && n2);

    if (n1 == n2) {
        return n1;
    }

    // Trace path from root to first node
    const node_t* node = root;
    while (node) {
        if (node->val > n1->val && node->val > n2->val) {
            node = node->left;
        } else if (node->val < n1->val && node->val < n2->val) {
            node = node->right;
        } else {
            if (node->val == n1->val) {
                return n1;
            } else if (node->val == n2->val) {
                return n2;
            }

            return node;
        }
    }

    assert(0);
}

size_t bst_shortest_path_level(const node_t* root)
{
    assert(root);

    std::queue<const node_t*> nodes;
    nodes.push(root);

    size_t level = 1;
    size_t total_nodes = 1;

    while (true) {
        // Collect this level's children
        for (size_t i = 0; i < total_nodes; ++i) {
            const node_t* node = nodes.front();
            nodes.pop();

            if (!node->left && !node->right) {
                return level;
            }

            nodes.push(node->left);
            nodes.push(node->right);
        }

        total_nodes *= 2;
        ++level;
    }
}

size_t bst_shortest_path_depth(const node_t* root)
{
    if (root == nullptr) {
        return 0;
    }

    if (root->left == nullptr) {
        return bst_shortest_path_depth(root->right) + 1;
    } else if (root->right == nullptr) {
        return bst_shortest_path_depth(root->left) + 1;
    } else {
        return std::min(bst_shortest_path_depth(root->left), bst_shortest_path_depth(root->right)) + 1;
    }
}

size_t bst_longest_path_level(const node_t* root)
{
    assert(root);

    std::queue<const node_t*> nodes;
    nodes.push(root);

    size_t level = 1;
    size_t level_nodes = 1;

    while (!nodes.empty()) {
        for (size_t i = 0; i < level_nodes && !nodes.empty(); ++i) {
            const node_t* node = nodes.front();
            nodes.pop();

            if (node->left) {
                nodes.push(node->left);
            }

            if (node->right) {
                nodes.push(node->right);
            }
        }

        ++level;
        level_nodes *= 2;
    }

    return level - 1;
}

size_t bst_longest_path_depth(const node_t* root)
{
    if (!root) {
        return 0;
    }

    if (root->left == nullptr) {
        return bst_longest_path_depth(root->right) + 1;
    } else if (root->right == nullptr) {
        return bst_longest_path_depth(root->left) + 1;
    } else {
        return std::max(
            bst_longest_path_depth(root->left),
            bst_longest_path_depth(root->right)) + 1;
    }
}

////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    node_t* root = bst_insert(nullptr, 13);
    node_t* n10 = bst_insert(root, 10);
    node_t* n15 = bst_insert(root, 15);
    node_t* n3 = bst_insert(root, 3);
    node_t* n11 = bst_insert(root, 11);
    node_t* n17 = bst_insert(root, 17);
    node_t* n0 = bst_insert(root, 0);
    node_t* n16 = bst_insert(root, 16);
    node_t* n13 = bst_insert(root, 13);

    bst_walk_depth(root, [](const node_t* node) {
        std::cout << node->val << std::endl;
    }, PREORDER);

    std::cout << std::endl;

    bst_walk_breadth(root, [](const node_t* node) {
        std::cout << node->val << std::endl;
    });

    assert(n10 == bst_nearest_common_parent(root, n0, n11));
    assert(n10 == bst_nearest_common_parent(root, n10, n0));
    assert(n10 == bst_nearest_common_parent(root, n10, n10));
    assert(n15 == bst_nearest_common_parent(root, n15, n16));
    assert(n15 == bst_nearest_common_parent(root, n17, n15));
    assert(n13 == bst_nearest_common_parent(root, n3, n16));

    assert(1 == bst_shortest_path_level(n0));
    assert(2 == bst_shortest_path_level(n3));
    assert(2 == bst_shortest_path_level(n10));
    assert(3 == bst_shortest_path_level(n13));

    assert(1 == bst_shortest_path_depth(n0));
    assert(2 == bst_shortest_path_depth(n3));
    assert(2 == bst_shortest_path_depth(n10));
    assert(3 == bst_shortest_path_depth(n13));

    assert(1 == bst_longest_path_level(n0));
    assert(2 == bst_longest_path_level(n3));
    assert(3 == bst_longest_path_level(n10));
    assert(4 == bst_longest_path_level(n13));

    assert(1 == bst_longest_path_depth(n0));
    assert(2 == bst_longest_path_depth(n3));
    assert(3 == bst_longest_path_depth(n10));
    assert(4 == bst_longest_path_depth(n13));

    bst_delete(root);
    return 0;
}
