#pragma once

#include "hit.h"
#include "ll.h"
#include "material.h"
#include "ray.h"

typedef LinkedList HittableList;

typedef struct {
    HittableList *objects;
} Scene;

Scene *scene_create(void);

void scene_delete(Scene **);

void scene_add_sphere(Scene *, double, double, double, double,
                      Material *material);

bool scene_intersect(Scene *, ray, double, double, HitRecord *);

void scene_print(Scene *);
