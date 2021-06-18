#include "bvh.h"
#include "util.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//
// Constructs the BVH for a given scene.
// Construction uses a top down approach, where each recursive call partitions
// the scene objects into two sorted subsets.
// The start and end arguments represent start and end indices of a sub-array of the
// scene objects that we wish to partition and sort.
BVHNode *bvh_create(Scene *s, int64_t start, int64_t end) {
    // Create node
    BVHNode *node = (BVHNode *)calloc(1, sizeof(BVHNode));
    assert(node != NULL);

    // We sort objects by the lower bound of their bounding box, selecting a random axis
    // to sort by each time.
    int axis = random_int(0, 2);
    int64_t span = end + 1 - start; // Size of the sub-array

    if (span == 1) {
        // Sub-array is one element long, so we at a leaf node.
        node->is_leaf = true;
        // AABB's of leaf nodes surround a single hittable object.
        // Get bounding box of object
        AABB box;
        hittable_bounding_box(*(s->objects[start]), &box);
        node->box = box;
        node->hittable = s->objects[start];
    } else if (span == 2) {
        // More than one object in sub-array, so we are at an interior node.
        node->is_leaf = false;
        // Sort sub array
        scene_sort(s, start, end, axis);

        // Partition into two sub-arrays of length 1 and recurse
        node->left = bvh_create(s, start, start);
        node->right = bvh_create(s, end, end);

        // Node bounding box is the union of the children boxes
        node->box = surrounding_box(node->left->box, node->right->box);
    } else {
        // More than one object in sub-array, so we are at an interior node.
        node->is_leaf = false;
        // Sort sub-array
        scene_sort(s, start, end, axis);

        // Partition into two sub-arrays and recurse.
        int64_t mid = start + (int64_t)((span - 1) / 2);
        node->left = bvh_create(s, start, mid);
        node->right = bvh_create(s, mid + 1, end);

        // Node bounding box is the union of the children boxes
        node->box = surrounding_box(node->left->box, node->right->box);
    }

    return node;
}

bool bvh_hit(BVHNode *node, ray r, double t_min, double t_max, HitRecord *rec) {
    if (!aabb_hit(node->box, r, t_min, t_max)) {
        return false;
    } else if (node->is_leaf) {
        return hittable_intersect(*(node->hittable), r, t_min, t_max, rec);
    } else {
        return bvh_hit(node->left, r, t_min, t_max, rec) ||
               bvh_hit(node->right, r, t_min, t_max, rec);
    }
}

void bvh_node_print(BVHNode *node) {
    if (node) {
        if (node->is_leaf) {
            hittable_print(node->hittable);
        } else {
            printf("Interior node\n");
            printf("Left: \n");
            if (node->left->is_leaf) {
                hittable_print(node->hittable);
            } else {
                printf("Interior node\n");
            }
            printf("Right: \n");
            if (node->right->is_leaf) {
                hittable_print(node->hittable);
            } else {
                printf("Interior node\n");
            }
        }
    }
    return;
}
