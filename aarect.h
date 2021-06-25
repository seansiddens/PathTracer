#pragma once

#include "aabb.h"
#include "hit.h"
#include "material.h"

typedef struct {
    double x0, x1, y0, y1, z;
    Material *material;
} XYRect;

XYRect *xy_rect_create(double x0, double x1, double y0, double y1, double z, Material *);

void xy_rect_delete(XYRect **rect);

bool xy_rect_bounding_box(XYRect rect, AABB *output_box);

bool xy_rect_intersect(XYRect rect, ray r, double t_min, double t_max, HitRecord *rec);
