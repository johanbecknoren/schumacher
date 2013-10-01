#ifndef WHITTEDTRACER_H
#define WHITTEDTRACER_H

#include "tracer.h"
#include "octree.h"
class WhittedTracer : Tracer {
	public:
		WhittedTracer(const int W, const int H) : Tracer(W, H) {};
		void render(float* pixels, Octree *tree, Camera *cam);
	private:
		glm::vec3 iterateRay(Ray &ray, Octree *tree, int depth); 
		glm::vec3 phongShader(Ray &incoming, IntersectionPoint &ip, Octree *tree);
};

#endif
