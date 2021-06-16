#ifndef __NODE_H__
#define __NODE_H__

#include "hittable.h"
typedef struct Node Node;

// Struct definition for a node of our linked list. Lists will contain
// objects in our scene which can be intersected with, so each node
// contains a void pointer to an object as well as a named constant
// as to what type of object it is.
struct Node {
    void *object;
    HittableType type;
    Node *next;
    Node *prev;
};

Node *node_create(void *, HittableType type);

void node_delete(Node **n);

#endif
