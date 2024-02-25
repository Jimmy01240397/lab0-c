#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Base function for insert an element into queue */
#define q_insert_base(head, s, list_add_method) \
    if (!head || !s)                            \
        return false;                           \
    element_t *element = e_new(s);              \
    if (!element)                               \
        return false;                           \
    list_add_method(&(element->list), head);    \
    return true;

/* Base function for remove an element on queue */
#define q_remove_base(head, sp, bufsize, nextprev)                           \
    if (!head || head->nextprev == head)                                     \
        return NULL; /* When head->next or head->prev == head queue is empty \
                      */                                                     \
    element_t *element = container_of(head->nextprev, element_t, list);      \
    list_del(&(element->list));                                              \
    strncpy(sp, element->value, bufsize - 1);                                \
    sp[bufsize - 1] = '\0';                                                  \
    return element;


/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

element_t *e_new(char *s)
{
    if (!s)
        return NULL;
    element_t *element = malloc(sizeof(element_t));
    if (!element)
        return NULL;
    int len = strlen(s);
    element->value = malloc(len + 1);
    if (!element->value) {
        free(element);
        return NULL;
    }
    strncpy(element->value, s, len);
    element->value[len] = '\0';
    return element;
}

void e_free(element_t *element)
{
    if (!element)
        return;
    free(element->value);
    free(element);
}


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *node = malloc(sizeof(struct list_head));
    if (!node)
        return NULL;
    INIT_LIST_HEAD(node);
    return node;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;
    struct list_head *node, *safe;
    list_for_each_safe (node, safe, l) {
        list_del(node);
        element_t *element = container_of(node, element_t, list);
        e_free(element);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    q_insert_base(head, s, list_add);
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    q_insert_base(head, s, list_add_tail);
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    q_remove_base(head, sp, bufsize, next);
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    q_remove_base(head, sp, bufsize, prev);
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return -1;
    int size = 0;
    struct list_head *node;
    for (node = head->next; node != head && node->next != head;
         node = node->next->next, size += 2) {
    }
    size += (node != head) & 1;
    return size;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
