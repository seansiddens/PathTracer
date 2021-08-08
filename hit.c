#include "hit.h"

// Takes a ray and a surface normal and ensures that the normal recorded in
// the hit_rec is opposite of the incoming ray. Also records whether the
// ray is outside or inside of the object
void set_face_normal(HitRecord *rec, ray r, vec3 outward_normal) {
    if (v3_dot(r.dir, outward_normal) > 0.0) {
        // If the dot product between the incoming ray and the surface normal
        // is positive, this means that the two vectors are going in about the same
        // direction. We flip the normal such that it goes opposite to the incoming ray.
        rec->normal = v3_scale(outward_normal, -1.0);
        rec->front_face = false;
    } else {
        // Dot product is negative, so the surface normal is oriented correctly against
        // the incoming ray.
        rec->normal = outward_normal;
        rec->front_face = true;
    }
}
