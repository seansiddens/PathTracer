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
//
BVHNode *bvh_create(Scene *s, int64_t start, int64_t end) {
    // Set root of BVH to NULL if scene contains no objects
    if (s->object_count == 0) {
        printf("Scene contains no objects!\n");
        return NULL;
    }

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

// 
// Intersects a ray with our BVH tree. Returns true if a hit occurred, false otherwise.
// Hit information is stored in the HitRecord struct.
// Initial call begins at the root node, where we perform a pre-order traversal
// of the tree. We first intersect the ray with the current node's bounding box. If
// a hit doesn't occur, we know our ray does not intersect with any of this node's 
// children, so we return false. If a hit does occur, we then either:
//      1) If at leaf node, check intersection with the node's hittable.
//      2) If at interior node, recurse down left and right children.
// The function only returns true when a hit has occurred with an actual hittable object.
// By OR'ing the result of the recursive calls, a "true" return value will bubble back
// up the call stack, meaning that we know if a hit has occurred with any object
// in our BVH.
//
bool bvh_hit(BVHNode *node, ray r, double t_min, double t_max, HitRecord *rec) {
    if (node == NULL) {
        return false;
    }
    if (!aabb_hit(node->box, r, t_min, t_max)) {
        return false;
    } else if (node->is_leaf) {
        HitRecord temp_rec;
        if (hittable_intersect(*(node->hittable), r, t_min, t_max, &temp_rec)) {
            if (temp_rec.t < rec->t) {
                *rec = temp_rec;
            }
            return true;
        }
        return false;
    } else {
        bool left_hit = bvh_hit(node->left, r, t_min, t_max, rec);
        bool right_hit = bvh_hit(node->right, r, t_min, t_max, rec);
        return left_hit || right_hit;
    }
}

//
// Debug function for printing contents of BVH
//
void bvh_node_print(BVHNode *node) {
    if (node) {
        if (node->is_leaf) {
            printf("Node is leaf!\n");
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
    } else {
        printf("BVHNode is NULL!\n");
    }
    return;
}
