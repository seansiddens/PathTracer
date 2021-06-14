#include "ll.h"
#include "sphere.h"

#include <stdio.h>
#include <stdlib.h>

//
// Constructor for a linked list. Initializes length and mtf fields and
// creates and links the head and tail sentinel nodes
//
LinkedList *ll_create(bool mtf) {
    LinkedList *ll = (LinkedList *)malloc(sizeof(LinkedList));
    if (ll) {
        ll->length = 0; // Sentinel nodes aren't included in size of list
        ll->mtf = mtf;

        // Create sentinel nodes
        ll->head = node_create(NULL, 0);
        ll->tail = node_create(NULL, 0);

        // Link them
        ll->head->next = ll->tail;
        ll->tail->prev = ll->head;
    }
    return ll;
}

//
// Destructor for a linked list. Deletes every node in list and then free's
// the linked list.
void ll_delete(LinkedList **ll) {
    if (*ll) {
        // Delete every node in the list
        Node *next;               // What's the next node?
        Node *curr = (*ll)->head; // What's the current node?
        // Stop when current node is NULL
        while (curr) {
            next = curr->next;
            node_delete(&curr); // Delete current node
            curr = next;        // Go to next node
        }
        free(*ll);
        *ll = NULL;
    }
    return;
}

//
// Inserts node at head of linked list. A lookup is performed before inserting
// to check whether there is already a matching node containing the same
// oldspeak string. If there is a duplicate, node is not inserted.
//
void ll_insert(LinkedList *ll, void *object, ObjectType type) {
    // Only insert if node not already present
    Node *n = node_create(object, type);
    n->next = ll->head->next;
    n->prev = ll->head;
    ll->head->next = n;
    n->next->prev = n;

    ll->length += 1;
    return;
}

void ll_print(LinkedList *ll) {
    if (ll) {
        Node *curr_node = ll->head->next;
        while (curr_node != ll->tail) {
            if (curr_node->type == SPHERE) {
                Sphere *s = curr_node->object;
                printf("Sphere: (%f, %f, %f), r = %f\n", s->center.x,
                       s->center.y, s->center.z, s->radius);
            }

            curr_node = curr_node->next;
        }
    }
    return;
}
