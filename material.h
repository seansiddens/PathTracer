#pragma once

#include "vec3.h"

enum MaterialType { LAMBERTIAN, METAL };
typedef enum MaterialType MaterialType;

typedef struct {
    MaterialType type;
    void *material;
} Material;

typedef struct {
    color albedo;
} Lambertian;

Material *create_lambertian(color albedo);

void mat_delete(Material **mat);
