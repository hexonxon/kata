#include <assert.h>

struct slist_node_t
{
    slist_node_t* next;
};

static slist_node_t* reverse(slist_node_t* head)
{
    slist_node_t* prev = nullptr;
    while (head) {
        slist_node_t* next = head->next;
        head->next = prev;

        prev = head;
        head = next;
    }

    return prev;
}

static void slist_test(void)
{
    slist_node_t n1, n2, n3;
    n1.next = &n2;
    n2.next = &n3;
    n3.next = nullptr;

    slist_node_t* new_head = reverse(&n1);

    assert(new_head == &n3);
    assert(n3.next == &n2);
    assert(n2.next == &n1);
    assert(n1.next == nullptr);

    assert(nullptr == reverse((slist_node_t*)nullptr));

    // Single node list
    assert(&n1 == reverse(&n1));

}

////////////////////////////////////////////////////////////////////////////////

struct list_node_t
{
    list_node_t* prev;
    list_node_t* next;
};

static list_node_t* reverse(list_node_t* head)
{
    list_node_t* prev = nullptr;
    while (head) {
        list_node_t* next = head->next;
        head->next = head->prev;
        head->prev = next;

        prev = head;
        head = next;
    }

    return prev;
}

static void list_test(void)
{
    list_node_t n1, n2, n3;
    n1.prev = nullptr;
    n1.next = &n2;
    n2.prev = &n1;
    n2.next = &n3;
    n3.prev = &n2;
    n3.next = nullptr;

    list_node_t* new_head = reverse(&n1);

    assert(new_head == &n3);
    assert(n3.prev == nullptr && n3.next == &n2);
    assert(n2.prev == &n3 && n2.next == &n1);
    assert(n1.prev == &n2 && n1.next == nullptr);

    assert(nullptr == reverse((list_node_t*)nullptr));

    // Single node list
    assert(&n1 == reverse(&n1));
}

int main(void)
{
    slist_test();
    list_test();

    return 0;
}
