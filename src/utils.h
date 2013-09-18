#ifndef UTILS_H
#define UTILS_H

#include "material.h"

// Viewport dimensions
#define WIDTH 1280
#define HEIGHT 720

// Refractive indices (en.wikipedia.org/wiki/Refractive_index)
#define REFRACTION_AIR 1.000293f
#define REFRACTION_GLASS 1.52f

// Materials enums
enum material_t { STONE, GLASS, MARBLE };

#endif
