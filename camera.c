#include "camera.h"

#include <stdlib.h>

Camera *cam_create(vec3 origin, double aspect_ratio, double focal_length) {
    Camera *cam = (Camera *)malloc(sizeof(Camera));
    cam->aspect_ratio = aspect_ratio;
    cam->viewport_height = 2.0;
    cam->viewport_width = cam->aspect_ratio * cam->viewport_height;
    cam->focal_length = focal_length;

    cam->origin = origin;
    cam->horizontal = v3_init(cam->viewport_width, 0, 0);
    cam->vertical = v3_init(0, cam->viewport_height, 0);
    cam->focal = v3_init(0, 0, cam->focal_length);
    cam->lower_left_corner = v3_sub(
        v3_sub(v3_sub(cam->origin, cam->focal), v3_scale(cam->horizontal, 0.5)),
        v3_scale(cam->vertical, 0.5));

    return cam;
}

void cam_delete(Camera **cam) {
    if (*cam) {
        free(*cam);
        *cam = NULL;
    }
    return;
}

// Given normalized (u, v) coordinates, returns the view ray from the camera
// origin to the viewport.
ray get_view_ray(Camera *cam, double u, double v) {
    // Get direction from camera origin to point on viewport
    vec3 dir =
        v3_add(v3_add(cam->lower_left_corner, v3_scale(cam->horizontal, u)),
               v3_scale(cam->vertical, v));

    // View ray
    ray r = {cam->origin, dir};
    return r;
}
