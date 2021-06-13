#ifndef __NODE_H__
#define __NODE_H__

typedef struct Node Node;

enum ObjectType { SPHERE };
typedef enum ObjectType ObjectType;

// Struct definition for a node of our linked list. Lists will contain
// objects in our scene which can be intersected with, so each node
// contains a void pointer to an object as well as a named constant
// as to what type of object it is.
struct Node {
    void *object;
    ObjectType type;
    Node *next;
    Node *prev;
};

Node *node_create(void *, ObjectType);

void node_delete(Node **n);

#endif
