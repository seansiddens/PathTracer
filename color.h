#pragma once

#include "vec3.h"

#include <stdint.h>

color linear_to_SRGB(color);

color SRGB_to_linear(color);

color ACES_film(color);

void write_color(uint8_t *image, color pixel_color, uint32_t i,
                 uint32_t samples_per_pixel);
