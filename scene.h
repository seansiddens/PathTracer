#pragma once

#include "ray.h"
#include "hit.h"
#include "ll.h"

typedef LinkedList HittableList;

typedef struct {
    HittableList *objects;  
} Scene;
