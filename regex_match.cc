/**
 * Regular expression matching
 *
 * Given a text and a pattern, determine if the pattern matches with the text completely
 * or not at all by using regular expression matching.
 * 
 * For simplicity, assume that the pattern may contain only two operators: '.' and '*':
 * - Operator '*' in the pattern means that the character preceding '*' may not appear
 *   or may appear any number of times in the text.
 * - Operator '.' matches with any character in the text exactly once.
 */

#include <map>
#include <set>
#include <vector>
#include <stack>
#include <utility>
#include <cassert>

/* TODO: i don't like this, maybe have some sort of NFA builder enclosure */
static int g_global_states;

enum {
    NFA_INPUT_ANY = 0x80, /* MSB of an ASCII character is never set */
};

struct nfa_transition {
    char input; /* For input-driven (not epsilon) transitions */
    bool is_epsilon;
    int target_state;
};

struct nfa {
    int start_state = 0;
    int accept_state = 0;
    std::set<int> current_states;
    std::map<int, std::vector<nfa_transition>> states;
};

static void make_input_transition(nfa& nfa, char input, int from_state, int to_state)
{
    nfa.states[from_state].emplace_back(nfa_transition {
        .input = input,
        .is_epsilon = false,
        .target_state = to_state,
    });
}

static void make_epsilon_transition(nfa& nfa, int from_state, int to_state)
{
    nfa.states[from_state].emplace_back(nfa_transition {
        .is_epsilon = true,
        .target_state = to_state,
    });
}

static int new_state()
{
    return g_global_states++; /* TODO: atomic inc? */
}

static int new_accept_state(nfa& nfa)
{
    int s = new_state();
    nfa.accept_state = s;
    return s;
}

static int new_start_state(nfa& nfa)
{
    int s = new_state();
    nfa.start_state = s;
    return s;
}

static nfa nfa_atom(char input)
{
    nfa nfa;
    make_input_transition(nfa, input, new_start_state(nfa), new_accept_state(nfa));
    return nfa;
}

static nfa nfa_atom_any()
{
    return nfa_atom(NFA_INPUT_ANY);
}

static nfa nfa_concatenation(const nfa& nfa1, const nfa& nfa2)
{
    nfa nfa;

    nfa.start_state = nfa1.start_state;
    nfa.accept_state = nfa2.accept_state;

    nfa.states.insert(nfa1.states.begin(), nfa1.states.end());
    nfa.states.insert(nfa2.states.begin(), nfa2.states.end());

    /* [s1] -> [a1] -e-> [s2] -> [a2] */
    make_epsilon_transition(nfa, nfa1.accept_state, nfa2.start_state);
    return nfa;
}

static nfa nfa_closure(const nfa& arg)
{
    nfa nfa;

    new_start_state(nfa);
    new_accept_state(nfa);
    nfa.states.insert(arg.states.begin(), arg.states.end());

    /*
     * Original:
     *  [s] -> [a]
     * Closure:
     *  [s'] -e-> [s] -> [a] -> [a']
     *   |         ^      |      ^
     *   |         +--e---+      |
     *   +------------e-----------
     */
    make_epsilon_transition(nfa, nfa.start_state, nfa.accept_state);
    make_epsilon_transition(nfa, nfa.start_state, arg.accept_state);
    make_epsilon_transition(nfa, arg.accept_state, arg.start_state);
    make_epsilon_transition(nfa, arg.accept_state, nfa.accept_state);

    return nfa;
}

static bool nfa_match(nfa& nfa, const char* str)
{
    /* Move to start state */
    nfa.current_states = {nfa.start_state};

    /* Consume input transitioning NFA into new set of current states */
    while (*str != '\0') {
        std::set<int> new_states;
        for(int s : nfa.current_states) {
            for (const nfa_transition& t : nfa.states[s]) {
                if (t.is_epsilon || (t.input == *str || t.input == NFA_INPUT_ANY)) {
                    new_states.insert(t.target_state);
                } else {
                    return false;
                }
            }
        }

        nfa.current_states = new_states;
    }

    /* After we exhaust all input, check if any of current states is an accept state */
    for (int s : nfa.current_states) {
        if (s == nfa.accept_state) {
            return true;
        }
    }

    return false;
}

static bool is_atom(char c)
{
    return (c != '*');
}

static unsigned precedence(char op)
{
    switch(op) {
    case '+': return 0;
    case '*': return 1;
    default: assert(0);
    }
}

static std::stack<char> to_postfix(const char* re)
{
    std::stack<char> ops;
    std::stack<char> postfix;
    for (size_t i = 0; re[i] != '\0'; ++i) {
        char op = 0;
        if (re[i] == '*') {
            op = re[i];
        } else if (i != 0) {
            op = '+';
        } 

        if (op) {
            while (!ops.empty() && precedence(op) < precedence(ops.top())) {
                postfix.push(ops.top());
                ops.pop();
            }
            ops.push(op);
        }

        if (re[i] != '*') {
            postfix.push(re[i]);
        }
    }

    while (!ops.empty()) {
        postfix.push(ops.top());
        ops.pop();
    }

    return postfix;
}

/* Op is on top */
static nfa parse_postfix_op(std::stack<char>& postfix)
{
    char op = postfix.top();
    postfix.pop();

    if (op == '*') {
        return nfa_closure(parse_postfix_op(postfix));
    } else if (op == '+') {
        return nfa_concatenation(parse_postfix_op(postfix), parse_postfix_op(postfix));
    } else {
        return nfa_atom(op);
    }
}

nfa re_compile(const char* re)
{
    std::stack<char> postfix = to_postfix(re);
    return parse_postfix_op(postfix);
}

bool re_match(const char* re, const char* str)
{
    /* Build the NFA and match
     * TODO: build can be done once per regex */
    nfa nfa = re_compile(re);
    return nfa_match(nfa, str);
}
