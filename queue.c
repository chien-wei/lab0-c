#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *queue = malloc(sizeof(struct list_head));
    queue->next = queue;
    queue->prev = queue;
    return queue;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (head->next == head) {
        free(head);
        return;
    }

    struct list_head *current = head->next;
    struct list_head *temp;
    do {
        temp = current->next;
        element_t *node = list_entry(current, element_t, list);
        free(node->value);
        free(node);
        current = temp;
    } while (current != head);
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *new_node = malloc(sizeof(element_t));
    if (!new_node) {
        return false;  // Memory allocation failed
    }

    char *copied_string = (char *) malloc(strlen(s) + 1);
    if (!copied_string) {
        free(new_node);
        return false;  // Memory allocation failed
    }

    strncpy(copied_string, s, strlen(s));
    copied_string[strlen(s)] = '\0';

    new_node->value = copied_string;
    if (!new_node->value) {
        free(new_node);
        return false;  // String duplication failed
    }

    new_node->list.next = head->next;
    new_node->list.prev = head;
    head->next->prev = &new_node->list;
    head->next = &new_node->list;
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new_node = malloc(sizeof(element_t));
    if (!new_node) {
        return false;  // Memory allocation failed
    }

    char *copied_string;
    copied_string = (char *) malloc(strlen(s) + 1);
    if (!copied_string) {
        free(new_node);
        return false;  // Memory allocation failed
    }

    strncpy(copied_string, s, strlen(s));
    copied_string[strlen(s)] = '\0';

    new_node->value = copied_string;
    if (!new_node->value) {
        free(new_node);
        return false;  // String duplication failed
    }

    new_node->list.next = head;
    new_node->list.prev = head->prev;
    head->prev->next = &new_node->list;
    head->prev = &new_node->list;
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head) {
        return NULL;
    }

    if (head->next == head) {
        return NULL;
    }

    struct list_head *first = head->next;
    element_t *node = list_entry(first, element_t, list);

    /*sp = (char *) malloc(bufsize + 1);*/
    /*if (sp == NULL) {*/
    /*    return NULL;  // Memory allocation failed*/
    /*}*/
    strncpy(sp, node->value, bufsize);
    sp[bufsize - 1] = '\0';

    head->next = node->list.next;
    head->next->prev = head;
    node->list.next = NULL;
    node->list.prev = NULL;

    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head) {
        return NULL;
    }

    if (head->next == head) {
        return NULL;
    }

    struct list_head *last = head->prev;
    element_t *node = list_entry(last, element_t, list);

    /*sp = (char *) malloc(bufsize + 1);*/
    /*if (sp == NULL) {*/
    /*    return NULL;  // Memory allocation failed*/
    /*}*/
    strncpy(sp, node->value, bufsize);
    sp[bufsize - 1] = '\0';

    head->prev = node->list.prev;
    head->prev->next = head;
    node->list.next = NULL;
    node->list.prev = NULL;

    return node;
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
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/

    if (head->next == head) {
        return false;
    }

    struct list_head *fast = head->next;
    struct list_head *slow = head;

    while (fast != head && fast->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }

    element_t *node = list_entry(slow->next, element_t, list);
    slow->next->next->prev = slow;
    slow->next = slow->next->next;
    free(node->value);
    free(node);

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
void q_reverse(struct list_head *head)
{
    struct list_head *current = head;
    struct list_head *temp;

    do {
        temp = current->next;
        current->next = current->prev;
        current->prev = temp;

        current = temp;
    } while (current != head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    struct list_head *current = head->next;
    struct list_head *temp;
    bool need_sort = true;

    while (need_sort && current != head) {
        need_sort = false;

        temp = current;
        struct list_head *p1 = current;
        struct list_head *p2 = current->next;
        element_t *n1 = list_entry(p1, element_t, list);
        element_t *n2 = list_entry(p2, element_t, list);
        if (descend) {
            if (n1->value < n2->value) {
                // Swap p1, p2
                p1->next = p2->next;
                p1->prev = p2;
                p2->prev = current;
                p2->next = p1;
                temp = p2;
                need_sort = true;
            }
        } else {
            if (n1->value > n2->value) {
                // Swap p1, p2
                p1->next = p2->next;
                p1->prev = p2;
                p2->prev = current;
                p2->next = p1;
                temp = p2;
                need_sort = true;
            }
        }
        current = temp->next;
    }
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere
 * to the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in
 * ascending/descending order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
