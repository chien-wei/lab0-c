#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

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
    while (!queue) {
        queue = malloc(sizeof(struct list_head));
    }
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
    struct list_head *p1 = head->next;
    struct list_head *p2 = head->next->next;
    while (p1 != head && p2 != head) {
        bool is_dup = false;
        element_t *n1 = list_entry(p1, element_t, list);
        element_t *n2 = list_entry(p2, element_t, list);
        while (strcmp(n2->value, n1->value) == 0) {
            is_dup = true;
            // delete n2
            p2->next->prev = p1;
            p1->next = p2->next;
            free(n2->value);
            free(n2);
            p2 = p1->next;
            if (p2 == head) {
                break;
            }
            n2 = list_entry(p2, element_t, list);
        }

        if (is_dup) {
            // delete n1
            p1->prev->next = p2;
            p2->prev = p1->prev;
            free(n1->value);
            free(n1);
        }
        if (p2 == head) {
            break;
        }
        p1 = p2;
        p2 = p1->next;
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    struct list_head *p1 = head->next;
    struct list_head *p2 = head->next->next;

    while (p1 != head && p2 != head) {
        p2->next->prev = p1;
        p1->prev->next = p2;
        p1->next = p2->next;
        p2->prev = p1->prev;
        p2->next = p1;
        p1->prev = p2;

        p1 = p1->next;
        p2 = p1->next;
    }
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

    int count = k;

    struct list_head *left = head;
    struct list_head *current = head;
    struct list_head *right;
    struct list_head *end;
    do {
        count--;
        current = current->next;
        if (count == 0) {
            /*element_t *node = list_entry(current, element_t, list);*/
            /*printf(" %s \n", node->value);*/
            right = current->next;
            right->prev = left;
            end = left->next;
            left->next = right;
            // insert one by one in reverse order until the last one is reached
            do {
                struct list_head *temp = current->prev;

                left->next = current;
                current->prev = left;
                current->next = right;
                right->prev = current;

                left = current;
                current = temp;
            } while (current != end);

            left->next = end;
            end->prev = left;
            end->next = right;
            right->prev = end;

            left = end;
            count = k;
        }
        /*element_t *node = list_entry(current, element_t, list);*/
        /*printf("%s %i\n", node->value, count);*/
    } while (current->next != head);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    struct list_head *current;
    struct list_head *p1;
    struct list_head *p2;
    bool need_sort = true;

    while (need_sort) {
        current = head;
        need_sort = false;
        do {
            p1 = current->next;
            p2 = current->next->next;
            if (p1 == head || p2 == head) {
                break;
            }
            element_t *n1 = list_entry(p1, element_t, list);
            element_t *n2 = list_entry(p2, element_t, list);
            if (descend) {
                if (strcmp(n1->value, n2->value) < 0) {
                    // Swap p1, p2
                    p2->prev = p1->prev;
                    p1->prev->next = p2;
                    p1->next = p2->next;
                    p2->next->prev = p1;
                    p2->next = p1;
                    p1->prev = p2;
                    need_sort = true;
                }
            } else {
                if (strcmp(n1->value, n2->value) > 0) {
                    // Swap p1, p2
                    p2->prev = p1->prev;
                    p1->prev->next = p2;
                    p1->next = p2->next;
                    p2->next->prev = p1;
                    p2->next = p1;
                    p1->prev = p2;
                    need_sort = true;
                }
            }
            current = current->next;
        } while (current->next != head && current->next->next != head);
    }
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    q_reverse(head);
    struct list_head *p1 = head->next;
    struct list_head *p2 = head->next->next;
    while (p1 != head && p2 != head) {
        element_t *n1 = list_entry(p1, element_t, list);
        element_t *n2 = list_entry(p2, element_t, list);
        while (strcmp(n2->value, n1->value) > 0) {
            // delete n2
            p2->next->prev = p1;
            p1->next = p2->next;
            free(n2->value);
            free(n2);
            p2 = p1->next;
            if (p2 == head) {
                break;
            }
            n2 = list_entry(p2, element_t, list);
        }
        p1 = p2;
        p2 = p1->next;
    }
    q_reverse(head);

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere
 * to the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    q_reverse(head);
    struct list_head *p1 = head->next;
    struct list_head *p2 = head->next->next;
    while (p1 != head && p2 != head) {
        element_t *n1 = list_entry(p1, element_t, list);
        element_t *n2 = list_entry(p2, element_t, list);
        while (strcmp(n2->value, n1->value) < 0) {
            // delete n2
            p2->next->prev = p1;
            p1->next = p2->next;
            free(n2->value);
            free(n2);
            p2 = p1->next;
            if (p2 == head) {
                break;
            }
            n2 = list_entry(p2, element_t, list);
        }
        p1 = p2;
        p2 = p1->next;
    }
    q_reverse(head);

    return q_size(head);
}

void merge_two_list(struct list_head *a, struct list_head *b, bool descend)
{
    struct list_head *b_current = b->next;
    struct list_head *a_1 = a;
    struct list_head *a_2 = a->next;

    do {  // loop for b
        element_t *na_2 = list_entry(a_2, element_t, list);
        element_t *nb_current = list_entry(b_current, element_t, list);
        /*printf("%s %s\n", na_2->value, nb_current->value);*/
        if (descend) {
            // loop for a
            while (a_2 != a && strcmp(na_2->value, nb_current->value) > 0) {
                a_1 = a_2;
                a_2 = a_2->next;
                na_2 = list_entry(a_2, element_t, list);
            }
            // remove b_current
            b_current->next->prev = b_current->prev;
            b_current->prev->next = b_current->next;

            // insert b_current between a_1, a_2
            b_current->next = a_2;
            b_current->prev = a_1;
            a_1->next = b_current;
            a_2->prev = b_current;

            // move pointer
            a_1 = b_current;
            b_current = b->next;
        } else {
            // loop for a
            while (a_2 != a && strcmp(na_2->value, nb_current->value) < 0) {
                /*printf("%s %s\n", na_2->value, nb_current->value);*/
                a_1 = a_2;
                a_2 = a_2->next;
                na_2 = list_entry(a_2, element_t, list);
            }
            // remove b_current
            b_current->next->prev = b_current->prev;
            b_current->prev->next = b_current->next;

            // insert b_current between a_1, a_2
            b_current->next = a_2;
            b_current->prev = a_1;
            a_1->next = b_current;
            a_2->prev = b_current;

            // move pointer
            a_1 = b_current;
            b_current = b->next;
        }
    } while (b_current != b);
}

/* Merge all the queues into one sorted queue, which is in
 * ascending/descending order */
int q_merge(struct list_head *head, bool descend)
{
    // Allocation is disallowed in this function.
    // 	q_remove_head with insertion is not practical here.
    // TODO: how to use pq then?

    // Have to read the Declaration to know the type to sort was queue_contex_t
    // But maybe struct list_head imply this function can be used on type other
    // than queue_contex_t?

    /*typedef struct {*/
    /*    struct list_head *q;*/
    /*    struct list_head chain;*/
    /*    int size;*/
    /*    int id;*/
    /*} queue_contex_t;*/

    // merge second to last queue into the first
    queue_contex_t *first = NULL;
    queue_contex_t *node;

    // got this error when I use list_for_each_entry
    // list_for_each_entry (node, head, chain) {
    /*Running static analysis...*/
    /*queue.c:371:5: style: Label 'int' is not used. [unusedLabel]*/
    /*    list_for_each_entry (node, head, chain) {*/
    /*    ^*/
    /**/
    /*Fail to pass static analysis.*/

    struct list_head *li;
    list_for_each (li, head) {
        node = list_entry(li, queue_contex_t, chain);
        if (node->id == 0) {
            first = node;
            continue;
        }

        // merge node to first
        merge_two_list(first->q, node->q, descend);

        // debug
        /*element_t *n;*/
        /*list_for_each_entry (n, first->q, list) {*/
        /*    printf("%s ", n->value);*/
        /*}*/
        /*printf("\n");*/

        // There is no need to free the 'queue_contex_t' and its member 'q'
        // since they will be released externally. However, q_merge() is
        // responsible for making the queues to be NULL-queue, except the first
        // one.
        /*node->q = NULL;*/
    }

    return q_size(first->q);
}
