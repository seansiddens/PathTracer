#pragma once

#include "aabb.h"
#include "hit.h"
#include "material.h"

typedef struct {
    double x0, x1, y0, y1, z;
    Material *material;
} XYRect;

typedef struct {
    double x0, x1, z0, z1, y;
    Material *material;
} XZRect;

typedef struct {
    double y0, y1, z0, z1, x;
    Material *material;
} YZRect;

XYRect *xy_rect_create(double x0, double x1, double y0, double y1, double z, Material *);

XZRect *xz_rect_create(double x0, double x1, double z0, double z1, double y, Material *);

YZRect *yz_rect_create(double y0, double y1, double z0, double z1, double x, Material *);

void xy_rect_delete(XYRect **rect);

void xz_rect_delete(XZRect **rect);

void yz_rect_delete(YZRect **rect);

bool xy_rect_bounding_box(XYRect rect, AABB *output_box);

bool xz_rect_bounding_box(XZRect rect, AABB *output_box);

bool yz_rect_bounding_box(YZRect rect, AABB *output_box);

bool xy_rect_intersect(XYRect rect, ray r, double t_min, double t_max, HitRecord *rec);

bool xz_rect_intersect(XZRect rect, ray r, double t_min, double t_max, HitRecord *rec);

bool yz_rect_intersect(YZRect rect, ray r, double t_min, double t_max, HitRecord *rec);

void xy_rect_print(XYRect *rect);

void xz_rect_print(XZRect *rect);

