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
// Returns the length of the linked list.
//
uint32_t ll_length(LinkedList *ll) { return ll->length; }

//
// Searches for node in list containing oldspeak. If node is found, returns
// pointer to the node. If it's not found, returns NULL. If the mtf field is
// true then the found node is moved to front.
//
/*Node *ll_lookup(LinkedList *ll, char *oldspeak) {*/
/*Node *curr_node = ll->head->next;*/
/*while (curr_node != ll->tail) {*/
/*// Check if oldspeak of current node is the one we want*/
/*if (strcmp(curr_node->oldspeak, oldspeak) == 0) {*/
/*// Node found, mtf if enabled and return pointer*/
/*if (ll->mtf) {*/
/*// Disconnect from list*/
/*curr_node->prev->next = curr_node->next;*/
/*curr_node->next->prev = curr_node->prev;*/

/*// Insert back (I don't use insert function so I don't create*/
/*// new nodes)*/
/*curr_node->next = ll->head->next;*/
/*curr_node->prev = ll->head;*/
/*ll->head->next = curr_node;*/
/*curr_node->next->prev = curr_node;*/
/*}*/
/*return curr_node;*/
/*}*/
/*curr_node = curr_node->next; // Go to next node if not found*/
/*links += 1;*/
/*}*/
/*// If it gets to this point, node was not found, so return NULL*/
/*return NULL;*/
/*}*/

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
                printf("Sphere: (%f, %f, %f), r = %f\n", s->center.x, s->center.y, s->center.z, s->radius);
            }

            curr_node = curr_node->next;
        }
    }
    return;
}
