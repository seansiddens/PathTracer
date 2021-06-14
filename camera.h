#pragma once

#include "ray.h"
#include "vec3.h"

typedef struct {
    double aspect_ratio, viewport_height, viewport_width, focal_length;
    vec3 origin;
    vec3 horizontal, vertical, focal, lower_left_corner;
} Camera;

Camera *cam_create(vec3 origin, double aspect_ratio, double focal_length);

void cam_delete(Camera **cam);

ray get_view_ray(Camera *cam, double u, double v);
