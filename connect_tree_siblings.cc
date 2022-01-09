/**
 * Connect all siblings
 *
 * Connect the sibling pointer to the next node in the same level.
 * The last node in each level should point to the first node of the next level in the tree.
 */

#include <queue>
#include <cassert>
#include <cstdlib>

struct tree_head {
    struct tree_head *left;
    struct tree_head *right;
    struct tree_head *next_sibling;
};

void connect_siblings(struct tree_head *head)
{
    std::queue<struct tree_head*> queue;
    struct tree_head* prev = nullptr;

    if (!head)
        return;

    queue.push(head);
    while (!queue.empty()) {
        struct tree_head* node = queue.front();
        queue.pop();

        if (prev)
            prev->next_sibling = node;

        if (node->left)
            queue.push(node->left);

        if (node->right)
            queue.push(node->right);

        prev = node;
    }
}

static struct tree_head* make_node(void)
{
        struct tree_head* h = (struct tree_head*) calloc(sizeof(*h), 1);
        assert(h);
        return h;
}

int main(void)
{
    {
        struct tree_head* h = make_node();
        h->left = make_node();
        /* Right node is empty */

        h->left->left = make_node();
        h->left->right = make_node();

        connect_siblings(h);

        /* expected order of siblings */
        struct tree_head* order[] = {h, h->left, h->left->left, h->left->right, nullptr};
        struct tree_head** sibling = &order[1];
        while (h) {
            assert(h->next_sibling == *sibling);
            h = h->next_sibling;
            ++sibling;
        }
    }

    {
        /* Tree with 1 node */
        struct tree_head* h = make_node();
        connect_siblings(h);
        assert(h->next_sibling == nullptr);
    }

    {
        /* NULL tree */
        connect_siblings(nullptr);
    }
}
