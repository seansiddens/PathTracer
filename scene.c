#include "scene.h"
#include "sphere.h"
#include "vec3.h"


// Adds a sphere to the scene
void scene_add_sphere(Scene *scene, double x, double y, double z, double r) {
    ll_insert(scene->objects, sphere_init(v3_init(x, y, z), r), SPHERE);
}
