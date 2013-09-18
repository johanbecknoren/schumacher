#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "octree.h"
#include "camera.h"

class Raytracer {
public:
	Raytracer() {};
	~Raytracer() {};
	void render(float* pixels, Octree *tree, const int W, const int H, Camera *cam, const unsigned int iterations);
};

#endif
