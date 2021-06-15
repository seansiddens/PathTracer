#include "camera.h"
#include "util.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

//
// Creates a camera with a specified aspect ratio and vertical field-of-view in
// degrees.
//
Camera *cam_create(vec3 vup, vec3 look_from, vec3 look_at, double aspect_ratio,
                   double vfov) {
    Camera *cam = (Camera *)malloc(sizeof(Camera));
    assert(cam != NULL);

    cam->vfov = vfov;
    double theta = degrees_to_radians(vfov);
    double h = tan(theta / 2);

    cam->aspect_ratio = aspect_ratio;
    cam->viewport_height = 2.0 * h;
    cam->viewport_width = cam->aspect_ratio * cam->viewport_height;

    vec3 w = v3_unit_vector(v3_sub(look_from, look_at));
    vec3 u = v3_unit_vector(v3_cross(vup, w));
    vec3 v = v3_cross(w, u);

    cam->origin = look_from;
    cam->horizontal = v3_scale(u, cam->viewport_width);
    cam->vertical = v3_scale(v, cam->viewport_height);
    cam->lower_left_corner =
        v3_sub(v3_sub(v3_sub(cam->origin, v3_scale(cam->horizontal, 0.5)),
                      v3_scale(cam->vertical, 0.5)),
               w);
    return cam;
}

//
// Deallocates camera memory.
//
void cam_delete(Camera **cam) {
    if (*cam) {
        free(*cam);
        *cam = NULL;
    }
    return;
}

//
// Given normalized (u, v) coordinates, returns the view ray from the camera
// origin to the viewport.
//
ray get_view_ray(Camera *cam, double u, double v) {
    // Get direction from camera origin to point on viewport
    vec3 dir = v3_add(v3_add(cam->lower_left_corner, v3_scale(cam->horizontal, u)),
                      v3_scale(cam->vertical, v));

    // View ray
    ray r = {cam->origin, dir};
    return r;
}
