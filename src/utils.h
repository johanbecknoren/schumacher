#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <cstring>
#include <iostream>
#include "glm/glm.hpp"
#define EPSILON real(0.00001)//float(1e06)
#define PI real(3.1415926535897932384626433832795) // M_PI istället

// Viewport dimensions
#define WIDTH 100
#define HEIGHT 100

typedef double real;
typedef glm::dvec3 Vec3;


// Refractive indices (en.wikipedia.org/wiki/Refractive_index)
#ifndef REFRACTION_AIR
#define REFRACTION_AIR real(1.000293)
#endif
#ifndef REFRACTION_GLASS
#define REFRACTION_GLASS real(1.52)
#endif

inline char* merge(char *folder, char* filename) {
	char *newArray = new char[std::strlen(folder)+std::strlen(filename)+1];
	std::strcpy(newArray,folder);
    std::strcat(newArray,filename);

	return newArray;
}

// Materials enums
enum material_t { STONE, GLASS, MARBLE, LIGHT, MIRROR, CORNELL_CEIL, CORNELL_BACK, CORNELL_RIGHT, CORNELL_LEFT, CORNELL_FLOOR, CORNELL_TALL_BOX, NUM_MATERIALS/*Leave in last slot*/ };

#endif
