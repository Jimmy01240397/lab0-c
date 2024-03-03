#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "strcmp.h"

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

#define q_ascend_and_descend(head, op)                        \
    if (!head)                                                \
        return -1;                                            \
    int len = 0;                                              \
    struct list_head *node, *safe;                            \
    element_t *element;                                       \
    char *check = NULL;                                       \
    for (node = head->prev, safe = node->prev; node != head;  \
         node = safe, safe = node->prev) {                    \
        element = container_of(node, element_t, list);        \
        if (check && newstrcmp(check, element->value) op 0) { \
            list_del(node);                                   \
            e_free(element);                                  \
            continue;                                         \
        }                                                     \
        check = element->value;                               \
        len++;                                                \
    }                                                         \
    return len;

#define max(a, b) ((a) > (b) ? (a) : (b))

#define min(a, b) ((a) < (b) ? (a) : (b))

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
    size += (node != head);
    return size;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;
    struct list_head *fast, *slow;
    for (fast = head->next, slow = head->next;
         fast != head && fast->next != head;
         fast = fast->next->next, slow = slow->next) {
    }
    list_del(slow);
    element_t *element = container_of(slow, element_t, list);
    e_free(element);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head)
        return false;

    struct list_head *node, *safe;
    element_t *element = NULL;
    bool dup = false;

    list_for_each_safe (node, safe, head) {
        element = container_of(node, element_t, list);
        bool nextdup =
            node->next != head &&
            !newstrcmp(element->value,
                       container_of(node->next, element_t, list)->value);
        if (dup || nextdup) {
            list_del(node);
            e_free(element);
        }
        dup = nextdup;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head)
        return;
    struct list_head *nodea, *nodeb, *safea, *safeb;
    for (nodea = head->next, nodeb = nodea->next, safea = nodea->next->next,
        safeb = nodeb->next->next;
         nodea != head && nodeb != head; nodea = safea, nodeb = safeb,
        safea = nodea->next->next, safeb = nodeb->next->next) {
        list_move(nodea, nodeb);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head)
        return;
    for (struct list_head *node = head, *move = head->prev; node != move;
         node = move, move = head->prev) {
        list_move(move, node);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head)
        return;
    struct list_head *first, *last, *firstprev, *lastnext, tmphead;
    if (k < 2)
        return;
    for (first = head->next, last = first; first != head;
         first = lastnext, last = first) {
        for (int i = 0; i < k - 1 && last != head; i++, last = last->next) {
        }
        if (last == head)
            return;
        firstprev = first->prev;
        lastnext = last->next;
        lastnext->prev = firstprev;
        firstprev->next = lastnext;
        tmphead.next = first;
        first->prev = &tmphead;
        tmphead.prev = last;
        last->next = &tmphead;
        q_reverse(&tmphead);
        list_splice(&tmphead, firstprev);
    }
}

#define mergelist(nodea, nodeb, mergeadd, mergesplice, mergehead)              \
    {                                                                          \
        struct list_head *lastnode = nodeb, *safe, **pnode;                    \
        while (nodea && nodeb) {                                               \
            element_t *elementa = container_of(nodea, element_t, list),        \
                      *elementb = container_of(nodeb, element_t, list);        \
            bool cmpresult = ((!descend) * 2 - 1) *                            \
                                 newstrcmp(elementa->value, elementb->value) < \
                             0;                                                \
            pnode = cmpresult ? &nodea : &nodeb;                               \
            safe = (*pnode)->next;                                             \
            lastnode = cmpresult ? nodeb : nodea;                              \
            mergeadd(*pnode, mergehead);                                       \
            *pnode = safe;                                                     \
        }                                                                      \
        mergesplice(lastnode, mergehead);                                      \
    }

void mergeadd(struct list_head *node, struct list_head ***mergehead)
{
    node->prev = NULL;
    node->next = NULL;
    **mergehead = node;
    *mergehead = &((**mergehead)->next);
}

void mergesplice(struct list_head *list, struct list_head ***mergehead)
{
    list->prev = NULL;
    **mergehead = list;
}

void listmergesplice(struct list_head *list, struct list_head *head)
{
    for (struct list_head *node = list, *safe; node; node = safe) {
        safe = node->next;
        list_add_tail(node, head);
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head)
        return;
    int powk = 1, count = 0;
    struct list_head *list = head->next, *pending = NULL;
    if (list == head->prev)
        return;
    head->prev->next = NULL;
    list->prev = NULL;
    while (list) {
        count++;
        if (count != powk) {
            int index = ffs(count) - 1;
            struct list_head **ppending = &pending;
            while (index--) {
                ppending = &((*ppending)->prev);
            }
            struct list_head *nodea = *ppending, *nodeb = nodea->prev,
                             *nextpending = nodeb->prev, **mergehead = ppending;
            mergelist(nodea, nodeb, mergeadd, mergesplice, &mergehead);
            (*ppending)->prev = nextpending;
        }
        powk <<= (count == powk);
        list->prev = pending;
        pending = list;
        list = list->next;
        pending->next = NULL;
    }
    INIT_LIST_HEAD(head);
    while (pending) {
        head->prev->next = NULL;
        struct list_head *nodea = head->next, *nodeb = pending,
                         *nextpending = nodeb->prev;
        INIT_LIST_HEAD(head);
        mergelist(nodea, nodeb, list_add_tail, listmergesplice, head);
        pending = nextpending;
    }
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    q_ascend_and_descend(head, <);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    q_ascend_and_descend(head, >);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
