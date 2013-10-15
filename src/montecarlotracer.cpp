#include "progressbar.h"
#include "montecarlotracer.h"
#include "timer.h"
#include <thread>
#include <vector>
#include <chrono>
void MonteCarloRayTracer::addToCount() {
	_mutex.lock();
	++_rayCounter;
	_mutex.unlock();
}

#define UNIFORM_DIST 1

void MonteCarloRayTracer::threadRender(int tId, float *pixels, 
		const Octree &tree, const Camera &cam, const int NUM_THREADS) {
#if UNIFORM_DIST
	int raysPerPixel = 16; // Must be even sqrt number (2, 4, 9, 16, 25 etc..)
#else
	int raysPerPixel = 30;
#endif
	int maxDepth = 4;
	float sqrtRPP = sqrtf(raysPerPixel);

	float dU = sqrtRPP/float(raysPerPixel);
	float dV = sqrtRPP/float(raysPerPixel);
	
	for (int u = 0; u < _W / NUM_THREADS; ++u) {
		for (int v = 0; v < _H; ++v) {
			glm::vec3 accumDiffColor(0.0f,0.0f,0.0f);
#if UNIFORM_DIST
			// Distributes rays uniformly within the pixel (u,v)
			for (float rpU=-1.0f/(sqrtRPP); rpU<1.0f - 1.0f/(sqrtRPP); rpU += 1.0f/sqrtRPP) {
				for (float rpV=-1.0f/(sqrtRPP); rpV<1.0f-1.0f/(sqrtRPP); rpV += 1.0f/sqrtRPP) {
					float x;
					float y;
					float u2 = u + rpU;
					float v2 = v + rpV;
					calculateXnY(u2 * NUM_THREADS + tId, v2, x, y);
					Ray r = cam.createRay(x, y);
					IntersectionPoint ip;
	
					if (tree.intersect(r, ip)) {
						float intensity = glm::dot(r.getDirection(), - ip.getNormal());
						accumDiffColor.x += intensity*ip.getMaterial().getDiffuseColor().x;
						accumDiffColor.y += intensity*ip.getMaterial().getDiffuseColor().y;
						accumDiffColor.z += intensity*ip.getMaterial().getDiffuseColor().z;
						
					}
				addToCount();
				}
				ProgressBar::printTimedProgBar(_rayCounter, _W * _H * raysPerPixel, "Carlo");
			}
			int id = calculateId(u * NUM_THREADS + tId, v);
			pixels[id + 0] = accumDiffColor.x/float(raysPerPixel);
			pixels[id + 1] = accumDiffColor.y/float(raysPerPixel);
			pixels[id + 2] = accumDiffColor.z/float(raysPerPixel);
#else
			float randU, randV;
			
			for(int rpp=1; rpp<=raysPerPixel; ++rpp) {
				randU = glm::linearRand(-0.5f,0.5f);
				randV = glm::linearRand(-0.5f,0.5f);
				float x;
				float y;
				float u2 = u + randU;
				float v2 = v + randV;
				calculateXnY(u2 * NUM_THREADS + tId, v2, x, y);
				Ray r = cam.createRay(x, y);
				IntersectionPoint ip;
	
				if (tree.intersect(r, ip)) {
					float intensity = glm::dot(r.getDirection(), - ip.getNormal());
						accumDiffColor.x += intensity*ip.getMaterial().getDiffuseColor().x;
						accumDiffColor.y += intensity*ip.getMaterial().getDiffuseColor().y;
						accumDiffColor.z += intensity*ip.getMaterial().getDiffuseColor().z;
					
					}
			
			addToCount();
		}
		ProgressBar::printTimedProgBar(_rayCounter, _W * _H * raysPerPixel, "Carlo");
	
	int id = calculateId(u * NUM_THREADS + tId, v);
	pixels[id + 0] = accumDiffColor.x/float(raysPerPixel);
	pixels[id + 1] = accumDiffColor.y/float(raysPerPixel);
	pixels[id + 2] = accumDiffColor.z/float(raysPerPixel);
#endif
		}
	}
}

void MonteCarloRayTracer::render(float *pixels, Octree *tree, Camera *cam) {
	const int NUM_THREADS = std::thread::hardware_concurrency();
	std::cout << "Starting carlo tracer with " << NUM_THREADS << " threads.\n";
	std::vector<std::thread> threads;
	// Start threads
	Timer::getInstance()->start("Carlo");
	for (int i = 0; i < NUM_THREADS; ++i) {
		threads.push_back(std::thread(&MonteCarloRayTracer::threadRender, this,
					i, pixels, *tree, *cam, NUM_THREADS));
	}

	// Join threads
	for (auto &thread : threads) {
		thread.join();
	}
	std::cout << std::endl;
	Timer::getInstance()->stop("Carlo");
	Timer::getInstance()->printRealTime("Carlo");
}
