#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef enum op_type_t {
    OP_ADD,
    OP_SUB,
    OP_NEG,
} op_type_t;

typedef struct exp_node_t {
    bool is_op;
    union {
        op_type_t op;
        int value;
    };

    struct exp_node_t *left;
    struct exp_node_t *right;
} exp_node_t;

static int eval(exp_node_t *root)
{
    assert(root);

    if (!root->is_op) {
        return root->value;
    }

    if (root->op == OP_NEG) {
        /* Unary op */
        assert(root->left);
        return ~eval(root->left);
    }

    /* Binary op */
    assert(root->left && root->right);

    int lval = eval(root->left);
    int rval = eval(root->right);

    switch (root->op) {
        case OP_ADD: return lval + rval;
        case OP_SUB: return lval - rval;
        default: assert(0);
    }
}

static exp_node_t op_node(op_type_t op, exp_node_t *left, exp_node_t *right)
{
    exp_node_t node;
    node.is_op = true;
    node.op = op;
    node.left = left;
    node.right = right;
    return node;
}

static exp_node_t val_node(int val, exp_node_t *left, exp_node_t *right)
{
    exp_node_t node;
    node.is_op = false;
    node.left = left;
    node.right = right;
    node.value = val;
    return node;
}

int main(void)
{
    exp_node_t v5 = val_node(5, NULL, NULL);
    exp_node_t v9 = val_node(9, NULL, NULL);
    exp_node_t add = op_node(OP_ADD, &v5, &v9);
    exp_node_t neg = op_node(OP_NEG, &add, NULL);

    assert(eval(&v5) == 5);
    assert(eval(&v9) == 9);
    assert(eval(&add) == 5 + 9);
    assert(eval(&neg) == ~(5 + 9));
}
