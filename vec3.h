#pragma once

#include <stdbool.h>

// Struct definition for a 3 dimensional vector.
typedef struct {
    double x, y, z;
} vec3;

typedef vec3 color; // Use same struct definition for when referring to a color

vec3 v3_init(double, double, double);

double v3_length_squared(vec3);

double v3_length(vec3);

vec3 v3_add(vec3, vec3);

vec3 v3_sub(vec3, vec3);

double v3_dot(vec3, vec3);

vec3 v3_cross(vec3, vec3);

vec3 v3_hadamard(vec3 u, vec3 v);

vec3 v3_scale(vec3, double);

vec3 v3_unit_vector(vec3);

void v3_normalize(vec3 *);

vec3 v3_lerp(vec3, vec3, double);

vec3 v3_random_uniform();

vec3 v3_random_range(double min, double max);

vec3 random_in_unit_sphere();

vec3 random_unit_vector();

vec3 random_in_hemisphere(vec3 normal);

bool v3_near_zero(vec3 v);

vec3 v3_reflect(vec3 v, vec3 n);

void v3_print(vec3);
