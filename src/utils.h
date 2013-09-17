#include "material.h"

#define USE_OCTREE true

// Viewport dimensions
#define WIDTH 640
#define HEIGHT 480

// Refractive indices
#define AIR 1.0f
#define GLASS 1.62f

// Materials
Material* stone = new Material(
	glm::vec3(0.7f,0.7f,0.7f),
	0.0f,
	0.7f,
	0.0f,
	1.0f);

Material* glass = new Material(
	glm::vec3(1.0f),
	GLASS,
	0.5f,
	0.9f,
	0.0f);
