#ifndef MONTECARLO_H
#define MONTECARLO_H
#include "tracer.h"
#include "octree.h"
#include <mutex>
class MonteCarloRayTracer : Tracer {
public:
	MonteCarloRayTracer(const int W, const int H) : Tracer(W, H),
		_rayCounter(0){};
	void render(float *pixels, Octree *tree, Camera *cam);	
	void test() {
		std::cout << "hej" << "\n";
	};
private:
	void threadRender(int tId, float *pixels, const Octree &tree, 
			const Camera &cam, const int NUM_THREADS);
	
	void addToCount();

	std::mutex _mutex;
	volatile int _rayCounter;
};


#endif
