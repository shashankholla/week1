#include "linked_list.h"

// Function pointers to (potentially) custom malloc() and
// free() functions.
//
static void *(*malloc_fptr)(size_t size) = NULL;
static void (*free_fptr)(void *addr) = NULL;

bool linked_list_register_malloc(void *(*malloc)(size_t))
{
    malloc_fptr = malloc;
    return true;
}

bool linked_list_register_free(void (*free)(void *))
{
    free_fptr = free;
    return true;
}

struct linked_list *linked_list_create(void)
{
    struct linked_list *ll_list = malloc_fptr(sizeof(struct linked_list));
    NULL_NULL(ll_list)
    ll_list->head = NULL;
    ll_list->size = 0;
    return ll_list;
}

bool linked_list_delete(struct linked_list *ll)
{
    if (ll == NULL)
        return true;

    NULL_FALSE(free_fptr)

    struct node *head = ll->head;

    while (head != NULL)
    {
        struct node *next = head->next;
        free_fptr(head);
        head = next;
    }

    free_fptr(ll);

    return true;
}

size_t linked_list_size(struct linked_list *ll)
{
    if (ll == NULL)
    {
        return SIZE_MAX;
    }
    return ll->size;
}

bool linked_list_insert_end(struct linked_list *ll,
                            unsigned int data)
{
    return linked_list_insert(ll, ll->size, data);
}

bool linked_list_insert_front(struct linked_list *ll,
                              unsigned int data)
{
    return linked_list_insert(ll, 0, data);
}

bool linked_list_insert(struct linked_list *ll,
                        size_t index,
                        unsigned int data)
{

    size_t index_bak = index;
    NULL_FALSE(ll)
    struct node *head = ll->head;
    struct node *prev = ll->head;

    while (index && (head != NULL))
    {
        prev = head;
        head = head->next;
        index--;
    }

    if (index != 0)
        return false;
    if (malloc_fptr == NULL)
        return false;

    struct node *new_node = malloc_fptr(sizeof(struct node));

    if (new_node == NULL)
        return false;

    new_node->data = data;
    new_node->next = head;
    if (prev == NULL)
    {
        ll->head = new_node;
        new_node->next = NULL;
    }
    else
    {
        new_node->next = head;
        if (index_bak == 0)
            ll->head = new_node;
        else
            prev->next = new_node;
    }

    ll->size++;
    return true;
}

size_t linked_list_find(struct linked_list *ll,
                        unsigned int data)
{
    NULL_SIZE_MAX(ll)
    struct node *head = ll->head;
    size_t index = 0;

    NULL_SIZE_MAX(head)

    while (head != NULL && head->data != data)
    {
        head = head->next;
        index++;
    }

    NULL_SIZE_MAX(head)

    if (head->data == data)
        return index;
    return SIZE_MAX;
}

bool linked_list_remove(struct linked_list *ll,
                        size_t index)
{
    // prev head
    // ll->A->B->C

    NULL_FALSE(ll)
    struct node *head = ll->head;
    struct node *prev = NULL;
    size_t index_bak = index;

    NULL_FALSE(head)

    while (index && (head != NULL))
    {
        prev = head;
        head = head->next;
        index--;
    }

    if (index != 0)
        return false;

    if (index_bak == 0)
    {
        ll->head = head->next;
    }
    else
    {
        prev->next = head->next;
    }

    NULL_FALSE(free_fptr)
    NULL_FALSE(head)

    free_fptr(head);
    ll->size--;
    return true;
}

struct iterator *linked_list_create_iterator(struct linked_list *ll,
                                             size_t index)
{
    NULL_NULL(ll)
    NULL_NULL(malloc_fptr)

    struct node *head = ll->head;
    NULL_NULL(head)

    size_t index_bak = index;

    while (index && (head != NULL))
    {
        head = head->next;
        index--;
    }

    if (index != 0)
        return NULL;
    else
    {
        struct iterator *itr = malloc_fptr(sizeof(struct iterator));
        itr->ll = ll;
        itr->current_node = head;
        itr->current_index = index_bak;
        itr->data = head->data;
        return itr;
    }

    return NULL;
}

bool linked_list_delete_iterator(struct iterator *iter)
{
    if (iter == NULL)
        return false;
    free_fptr(iter);
    return true;
}

bool linked_list_iterate(struct iterator *iter)
{
    if (iter == NULL)
        return false;
    if (iter->current_node == NULL)
        return false;
    if (iter->current_node->next == NULL)
        return false;

    iter->current_node = iter->current_node->next;
    iter->current_index++;
    iter->data = iter->current_node->data;
    return true;
}
