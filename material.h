#pragma once

enum MaterialType { LAMBERTIAN, METAL };
typedef enum MaterialType MaterialType;

typedef struct {
    MaterialType type;

} Material;
