#include "texture.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

vec3 checker_lookup(double u, double v, vec3 p) {
    double sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
    if (sines < 0) {
        return v3_init(0, 0, 0);
    } else {
        return v3_init(1, 1, 1);
    }
}

Texture *create_checker_texture() {
    Texture *tex = (Texture *)malloc(sizeof(Texture));
    assert(tex != NULL);

    tex->lookup = checker_lookup;

    return tex;
}

void delete_texture(Texture **tex) {
    assert(*tex != NULL);
    free(*tex);
    *tex = NULL;
    return;
}
