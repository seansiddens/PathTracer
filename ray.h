#pragma once

#include "vec3.h"

typedef struct {
    vec3 orig;
    vec3 dir;
} ray;

double ray_at(ray, double);
