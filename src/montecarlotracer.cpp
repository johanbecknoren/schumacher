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

	int raysPerPixel = 4; // Preferrably even sqrt number
	
	float sqrtRPP = sqrtf(raysPerPixel);

	float dU = sqrtRPP/float(raysPerPixel);
	float dV = sqrtRPP/float(raysPerPixel);
	
	for (int u = 0; u < _W; ++u) {
		for (int v = 0; v < _H / NUM_THREADS; ++v) {
			glm::vec3 accumDiffColor(0.0f,0.0f,0.0f);
			for (float rpU=-1.0f/(sqrtRPP); rpU<1.0f - 1.0f/(sqrtRPP); rpU += 1.0f/sqrtRPP) {
				for (float rpV=-1.0f/(sqrtRPP); rpV<1.0f-1.0f/(sqrtRPP); rpV += 1.0f/sqrtRPP) {
					float x;
					float y;
					float u2 = u + rpU;
					float v2 = v + rpV;
					calculateXnY(u2, v2 * NUM_THREADS + tId, x, y);
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
// 				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				ProgressBar::printTimedProgBar(_rayCounter, _W * _H * raysPerPixel, "Carlo");
			}
			int id = calculateId(u, v * NUM_THREADS + tId);
			pixels[id + 0] = accumDiffColor.x/float(raysPerPixel);
			pixels[id + 1] = accumDiffColor.y/float(raysPerPixel);
			pixels[id + 2] = accumDiffColor.z/float(raysPerPixel);

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
