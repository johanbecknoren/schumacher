#ifndef RAYCASTER_H
#define RAYCASTER_H
#include "octree.h"
#include "tracer.h"

class Raytracer : Tracer {
public:
	Raytracer(const int W, const int H) : Tracer(W, H) {}; 
	~Raytracer() {};
	void render(float* pixels, Octree *tree, 
			Camera *cam, int &iterations);
};

#endif
