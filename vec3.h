#pragma once

// Struct definition for a 3 dimensional vector.
typedef struct {
    double x, y, z;
} vec3;

typedef vec3 color; // Use same struct definition for when referring to a color

double v3_length_squared(vec3);

double v3_length(vec3);

vec3 v3_add(vec3, vec3);

vec3 v3_sub(vec3, vec3);

double v3_dot(vec3, vec3);

vec3 v3_scale(vec3, double);

void v3_print(vec3);
