#pragma once

#include "vec3.h"

typedef struct {
    vec3 (*lookup)(double u, double v, vec3 p);
} Texture;

Texture *create_checker_texture();

void delete_texture(Texture **tex);
