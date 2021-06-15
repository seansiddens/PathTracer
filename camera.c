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
                   double vfov, double aperture, double focus_dist) {
    Camera *cam = (Camera *)malloc(sizeof(Camera));
    assert(cam != NULL);

    cam->vfov = vfov;
    double theta = degrees_to_radians(vfov);
    double h = tan(theta / 2);

    cam->aspect_ratio = aspect_ratio;
    cam->viewport_height = 2.0 * h;
    cam->viewport_width = cam->aspect_ratio * cam->viewport_height;

    cam->w = v3_unit_vector(v3_sub(look_from, look_at));
    cam->u = v3_unit_vector(v3_cross(vup, cam->w));
    cam->v = v3_cross(cam->w, cam->u);

    cam->focus_dist = focus_dist;
    cam->aperture = aperture;
    cam->origin = look_from;
    cam->horizontal = v3_scale(cam->u, cam->viewport_width * cam->focus_dist);
    cam->vertical = v3_scale(cam->v, cam->viewport_height * cam->focus_dist);
    cam->lower_left_corner =
        v3_sub(v3_sub(v3_sub(cam->origin, v3_scale(cam->horizontal, 0.5)),
                      v3_scale(cam->vertical, 0.5)),
               v3_scale(cam->w, cam->focus_dist));
    cam->lens_radius = aperture / 2;

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
// Given normalized (s, t) coordinates, returns the view ray from the camera
// origin to the viewport.
//
ray get_view_ray(Camera *cam, double s, double t) {
    vec3 rd = v3_scale(random_in_unit_disk(), cam->lens_radius);
    vec3 offset = v3_add(v3_scale(cam->u, rd.x), v3_scale(cam->v, rd.y));

    // Get direction from camera origin to point on viewport
    vec3 dir = v3_add(cam->lower_left_corner, v3_scale(cam->horizontal, s));
    dir = v3_add(dir, v3_scale(cam->vertical, t));
    dir = v3_sub(dir, cam->origin);
    dir = v3_sub(dir, offset);

    // View ray
    ray r = {v3_add(cam->origin, offset), dir};
    return r;
}
