#ifndef WHITTEDTRACER_H
#define WHITTEDTRACER_H

#include "tracer.h"
#include "octree.h"
class WhittedTracer : Tracer {
	public:
		WhittedTracer(const int W, const int H) : Tracer(W, H) {};
		void render(float* pixels, Octree *tree, Camera *cam);
	private:
		Vec3 iterateRay(Ray &ray, Octree *tree, int depth); 
		Vec3 phongShader(Ray &incoming, IntersectionPoint &ip, Octree *tree);
};

#endif
