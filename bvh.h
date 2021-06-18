#pragma once

#include "aabb.h"
#include "hit.h"
#include "scene.h"
#include "hittable.h"
#include "ray.h"

typedef struct BVHNode BVHNode;

struct BVHNode {
    bool is_leaf;
    AABB box;
    BVHNode *left;
    BVHNode *right;
    Hittable *hittable;
};

BVHNode *bvh_create(Scene *s, int64_t start, int64_t end);

bool bvh_hit(BVHNode *node, ray r, double t_min, double t_max, HitRecord *rec);

void bvh_node_print(BVHNode *node);
