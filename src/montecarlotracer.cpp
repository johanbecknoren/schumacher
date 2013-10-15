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

void MonteCarloRayTracer::threadRender(int tId, float *pixels, 
		const Octree &tree, const Camera &cam, const int NUM_THREADS) {
	int raysPerPixel = 16; // Preferrably even sqrt number
	
	float sqrtRPP = sqrtf(raysPerPixel);

	float dU = sqrtRPP/float(raysPerPixel);
	float dV = sqrtRPP/float(raysPerPixel);
	
	for (int u = 0; u < _W / NUM_THREADS; ++u) {
		for (int v = 0; v < _H; ++v) {
			for (int rpU=1; rpU<=int(sqrtRPP); ++rpU) {
				for (int rpV=1; rpV<=int(sqrtRPP); ++rpV) {
					float x;
					float y;
					float u2 = u + rpU*dU;
					float v2 = v + rpV*dV;
					//calculateXnY(u * NUM_THREADS + tId, v, x, y);
					calculateXnY(u2 * NUM_THREADS + tId, v2, x, y);
					Ray r = cam.createRay(x, y);
					IntersectionPoint ip;
	
					if (tree.intersect(r, ip)) {
						float intensity = glm::dot(r.getDirection(), - ip.getNormal());
						int id = calculateId(u * NUM_THREADS + tId, v);
						pixels[id + 0] = intensity*ip.getMaterial().getDiffuseColor().x;
						pixels[id + 1] = intensity*ip.getMaterial().getDiffuseColor().y;
						pixels[id + 2] = intensity*ip.getMaterial().getDiffuseColor().z;
					}
				addToCount();
				}
// 				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				ProgressBar::printTimedProgBar(_rayCounter, _W * _H * raysPerPixel, "Carlo");
			}
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
