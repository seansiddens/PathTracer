#pragma once

#include "aabb.h"
#include "hit.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"

#include <stdint.h>

typedef struct {
    Hittable **objects;
    uint32_t object_count;
    uint32_t max_count;
} Scene;

Scene *scene_create(void);

Scene *random_scene(void);

void scene_delete(Scene **);

void scene_add_sphere(Scene *, double x, double y, double z, double r,
                      Material *material);

bool scene_intersect(Scene *, ray r, double t_min, double t_max, HitRecord *rec);

void scene_print(Scene *);

void scene_sort(Scene *, int64_t start, int64_t end, uint8_t axis);

void scene_insert_hittable(Scene *, Hittable *);
