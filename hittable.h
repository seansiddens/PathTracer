#pragma once

#include "aabb.h"
#include "hit.h"

enum HittableType { SPHERE, XYRECT, XZRECT, YZRECT};
typedef enum HittableType HittableType;

typedef struct Hittable Hittable;

struct Hittable {
    void *object;
    HittableType type;
};

Hittable *hittable_create(void *object, HittableType type);

bool hittable_intersect(Hittable h, ray r, double t_min, double t_max, HitRecord *rec);

bool hittable_bounding_box(Hittable h, AABB *output_box);

void hittable_delete(Hittable **n);

void hittable_print(Hittable *h);
