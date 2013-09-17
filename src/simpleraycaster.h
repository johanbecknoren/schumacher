#ifndef SIMPLERAYCAST_H
#define SIMPLERAYCAST_H
#include "octree.h"


class SimpleRaycaster {
public:
	SimpleRaycaster();
	~SimpleRaycaster() {};
	void render(float* pixels, Octree *tree, const int W, const int H);
private:
};

#endif
