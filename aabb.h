#pragma once

#include "ray.h"

typedef struct AABB AABB;

struct AABB {
    vec3 min, max;
};

AABB *aabb_create(vec3 min, vec3 max);

AABB aabb_init(vec3 min, vec3 max);

void aabb_delete(AABB **box);

bool aabb_hit(AABB *aabb, ray r, double t_min, double t_max);

AABB surrounding_box(AABB box0, AABB box1);
