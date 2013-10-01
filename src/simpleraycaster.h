#ifndef SIMPLERAYCAST_H
#define SIMPLERAYCAST_H
#include "octree.h"

#include "tracer.h"

class SimpleRaycaster : Tracer {
public:
	SimpleRaycaster(const int W, const int H) : Tracer(W, H) {};
	~SimpleRaycaster() {};
	void render(float* pixels, Octree *tree, Camera *cam);
};

#endif
