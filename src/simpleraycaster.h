#ifndef SIMPLERAYCAST_H
#define SIMPLERAYCAST_H
#include "octree.h"
#include "camera.h"

class SimpleRaycaster {
public:
	SimpleRaycaster() {};
	~SimpleRaycaster() {};
	void render(float* pixels, Octree *tree, const int W, const int H, Camera *cam);
private:
};

#endif
