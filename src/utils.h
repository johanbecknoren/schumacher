#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <cstring>
#include <iostream>
#define EPSILON 0.00001f//float(1e06)
#define PI 3.1415926535897932384626433832795f

// Viewport dimensions
#define WIDTH 512
#define HEIGHT 512


// Refractive indices (en.wikipedia.org/wiki/Refractive_index)
#ifndef REFRACTION_AIR
#define REFRACTION_AIR 1.000293f
#endif
#ifndef REFRACTION_GLASS
#define REFRACTION_GLASS 1.52f
#endif

inline char* merge(char *folder, char* filename) {
	char *newArray = new char[std::strlen(folder)+std::strlen(filename)+1];
	std::strcpy(newArray,folder);
    std::strcat(newArray,filename);

	return newArray;
}

// Materials enums
enum material_t { STONE, GLASS, MARBLE, LIGHT, MIRROR, CORNELL_CEIL, CORNELL_BACK, CORNELL_RIGHT, CORNELL_LEFT, CORNELL_FLOOR, NUM_MATERIALS/*Leave in last slot*/ };

#endif
