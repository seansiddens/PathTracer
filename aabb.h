#pragma once

#include "ray.h"

typedef struct AABB AABB;

bool aabb_hit(AABB *aabb, ray r, double t_min, double t_max);
