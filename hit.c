#include "hit.h"

// Takes a ray and a surface normal and ensures that the normal recorded in
// the hit_rec is opposite of the incoming ray. Also records whether the
// ray is outside or inside of the object
void set_face_normal(HitRecord *rec, ray r, vec3 outward_normal) {
    if (v3_dot(r.dir, outward_normal) > 0.0) {
        // Ray is inside the sphere
        rec->normal = v3_scale(outward_normal, -1.0);
        rec->front_face = false;
    } else {
        // Ray is outside the sphere
        rec->normal = outward_normal;
        rec->front_face = true;
    }
}
