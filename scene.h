#pragma once

#include "ray.h"
#include "hit.h"
#include "ll.h"

typedef LinkedList HittableList;

typedef struct {
    HittableList *objects;  
} Scene;

Scene *scene_create(void);

void scene_delete(Scene **);

void scene_add_sphere(Scene *, double, double, double, double);

bool scene_intersect(Scene*, ray, double, double, HitRecord);

void scene_print(Scene *);
