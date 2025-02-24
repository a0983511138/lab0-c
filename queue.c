#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "report.h"

void merge_sort(struct list_head *left, struct list_head *right, int leno);

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *l = malloc(sizeof(struct list_head));
    if (!l)
        return NULL;
    l->prev = l;
    l->next = l;
    return l;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;
    struct list_head *tmp = l->next;

    while (tmp != l) {
        element_t *item = list_entry(tmp, element_t, list);
        tmp = tmp->next;
        if (item->value) {
            free(item->value);
        }
        free(item);
    }

    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *node = malloc(sizeof(element_t));
    if (!node)
        return false;

    int length = strlen(s);
    node->value = malloc(sizeof(char) * (length + 1));
    if (!node->value) {
        free(node);
        return false;
    }

    strncpy(node->value, s, length);
    node->value[length] = '\0';

    list_add(&node->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *node = malloc(sizeof(element_t));
    if (!node)
        return false;

    int length = strlen(s);
    node->value = malloc(sizeof(char) * (length + 1));
    if (!node->value) {
        free(node);
        return false;
    }

    strncpy(node->value, s, length);
    node->value[length] = '\0';

    list_add_tail(&node->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;
    if (head->next == head)
        return NULL;

    struct list_head *node = head->next;
    element_t *item = list_entry(node, element_t, list);
    int length = strlen(item->value);

    list_del_init(node);

    strncpy(sp, item->value, length);
    sp[length] = '\0';

    return item;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;
    if (head->prev == head)
        return NULL;

    struct list_head *node = head->prev;
    element_t *item = list_entry(node, element_t, list);
    int length = strlen(item->value);

    list_del_init(node);

    strncpy(sp, item->value, length);
    sp[length] = '\0';

    return item;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
// https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
bool q_delete_mid(struct list_head *head)
{
    if (!head)
        return false;
    if (head == head->next)
        return true;

    float len = (float) q_size(head) / 2;

    while (len > 0) {
        head = head->prev;
        len--;
    }

    element_t *item = list_entry(head, element_t, list);
    list_del(head);
    free(item->value);
    free(item);

    return true;
}

/* Delete all nodes that have duplicate string */
// https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
bool q_delete_dup(struct list_head *head)
{
    return true;
}

/* Swap every two adjacent nodes */
// https://leetcode.com/problems/swap-nodes-in-pairs/
void q_swap(struct list_head *head)
{
    if (!head)
        return;

    int len = q_size(head) / 2;

    while (len > 0) {
        struct list_head *node = head->next;
        list_del(node);
        head = head->next;
        list_add(node, head);
        head = head->next;
        len--;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    struct list_head *start = head, *tmp;

    do {
        tmp = head->next;
        head->next = head->prev;
        head->prev = tmp;
        head = head->next;
    } while (head != start);
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (!head)
        return;

    int i, len = q_size(head);
    struct list_head *node_l = head->next, *node_r = head->next;

    for (i = 0; i < len / 2; i++)
        node_r = node_r->next;

    head->prev->next = NULL;
    head->prev = NULL;
    head->next->prev = NULL;
    head->next = NULL;
    merge_sort(node_l, node_r, len);

    while (node_l->prev)
        node_l = node_l->prev;

    head->next = node_l;
    node_l->prev = head;

    while (node_l->next) {
        printf("%s ", list_entry(node_l, element_t, list)->value);
        node_l = node_l->next;
    }
    printf("\n");
    node_l->next = head;
    head->prev = node_l;
}

void merge_sort(struct list_head *left, struct list_head *right, int len)
{
    printf("-----------------------\nlen: %d\n", len);
    if (!left || !right || len <= 1)
        return;

    struct list_head *node;
    int triger = 0;

    right->prev->next = NULL;
    right->prev = NULL;

    if (len > 2) {
        node = left;
        for (int i = 0; i < len / 4; i++)
            node = node->next;
        merge_sort(left, node, len / 2);

        node = right;
        for (int i = 0; i < (len / 2 + len % 2) / 2; i++)
            node = node->next;
        merge_sort(right, node, len / 2 + len % 2);
    }

    while (left->prev)
        left = left->prev;
    while (right->prev)
        right = right->prev;

    while (left != NULL && right != NULL) {
        element_t *item1 = list_entry(left, element_t, list);
        element_t *item2 = list_entry(right, element_t, list);

        if (strcmp(item1->value, item2->value) > 0) {
            printf("%s > %s\n", item1->value, item2->value);
            node = right;
            if (right->next) {
                right = right->next;
                right->prev = node->prev;
            } else
                triger = 1;
            if (node->prev)
                node->prev->next = right;

            if (left->prev) {
                left->prev->next = node;
                node->prev = left->prev;
            }
            node->next = left;
            left->prev = node;

        } else {
            printf("%s < %s\n", item1->value, item2->value);
            if (left->next)
                left = left->next;
            else {
                triger = 1;
                left->next = right;
                right->prev = left;
            }
        }
        if (triger)
            break;
    }

    printf("end of merge len: %d\n", len);
}
