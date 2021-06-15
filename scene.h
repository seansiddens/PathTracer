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

Scene *random_scene(void);

void scene_delete(Scene **);

void scene_add_sphere(Scene *, double x, double y, double z, double r,
                      Material *material);

bool scene_intersect(Scene *, ray r, double t_min, double t_max, HitRecord *rec);

void scene_print(Scene *);
