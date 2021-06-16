#pragma once

enum HittableType { SPHERE };
typedef enum HittableType HittableType;

typedef struct Hittable Hittable;

struct Hittable {
    void *object;
    HittableType type;
};

Hittable *hittable_create(void *object, HittableType type);

void hittable_delete(Hittable **n);

void hittable_print(Hittable *h);
