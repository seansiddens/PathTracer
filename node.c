#include "node.h"
#include "sphere.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//
// Constructor for node - returns pointer to newly created node.
//
Node *node_create(void *object, HittableType type) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (n) {
        n->object = object;
        n->type = type;
        n->next = NULL;
        n->prev = NULL;
    }
    return n;
}

//
// Destructor for a node. DOESN'T free prev or next nodes.
//
void node_delete(Node **n) {
    if (*n) {
        if ((*n)->type == SPHERE) {
            Sphere *s = (Sphere *)(*n)->object;
            sphere_delete(&s);
        }
        free(*n);
        *n = NULL;
    }
    return;
}
