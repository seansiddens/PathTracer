#pragma once

#include "ray.h"
#include "vec3.h"

typedef struct {
    double aspect_ratio, vfov, aperture, focus_dist;
    vec3 origin;
    double viewport_height, viewport_width, lens_radius;
    vec3 u, v, w, horizontal, vertical, lower_left_corner;
} Camera;

Camera *cam_create(vec3 vup, vec3 look_from, vec3 look_at, double aspect_ratio,
                   double vfov, double aperture, double focus_dist);

void cam_delete(Camera **cam);

ray get_view_ray(Camera *cam, double u, double v);
