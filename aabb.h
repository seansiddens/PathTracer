#pragma once

#include "ray.h"

typedef struct AABB AABB;

AABB *aabb_create(vec3 min, vec3 max);

void aabb_delete(AABB **box);

bool aabb_hit(AABB *aabb, ray r, double t_min, double t_max);
