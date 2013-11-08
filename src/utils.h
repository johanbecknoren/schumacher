#ifndef UTILS_H
#define UTILS_H

#define EPSILON float(1e06)
#define PI 3.1415926535897932384626433832795f

// Viewport dimensions
#define WIDTH 400
#define HEIGHT 400


// Refractive indices (en.wikipedia.org/wiki/Refractive_index)
#ifndef REFRACTION_AIR
#define REFRACTION_AIR 1.000293f
#endif
#ifndef REFRACTION_GLASS
#define REFRACTION_GLASS 1.52f
#endif

// Materials enums
enum material_t { STONE, GLASS, MARBLE, LIGHT, MIRROR, CORNELL_CEIL, CORNELL_BACK, CORNELL_RIGHT, CORNELL_LEFT, CORNELL_FLOOR, NUM_MATERIALS/*Leave in last slot*/ };

#endif
