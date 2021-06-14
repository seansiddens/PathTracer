#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include "node.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct LinkedList LinkedList;

//
// Struct definition for a linked list. length is size of list, head and tail
// are pointers to the head and sentinel nodes, and mtf is a flag for whether or
// not the move-to-front rule is on.
//
struct LinkedList {
    uint32_t length;
    Node *head; // Head sentinel node
    Node *tail; // Tail sentinel node
    bool mtf;
};

LinkedList *ll_create(bool mtf);

void ll_delete(LinkedList **ll);

void ll_insert(LinkedList *ll, void *, ObjectType);

void ll_print(LinkedList *ll);

#endif
