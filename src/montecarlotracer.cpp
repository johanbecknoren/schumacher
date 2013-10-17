#include "progressbar.h"
#include "montecarlotracer.h"
#include "timer.h"
#include <thread>
#include <vector>
#include <chrono>
#include "SFMT.h"
#include <glm/gtx/random.hpp>

void MonteCarloRayTracer::addToCount() {
	_mutex.lock();
	++_rayCounter;
	_mutex.unlock();
}

glm::vec3 MonteCarloRayTracer::iterateRay(Ray &ray, const Octree &tree, int depth) {
	IntersectionPoint ip;
	glm::vec3 color(0.0f);
	int maxDepth = 4;
	
	if(tree.intersect(ray, ip)) {
		if(depth < maxDepth) {
		}
	}

	return glm::vec3();
}

#define UNIFORM_DIST 0

void MonteCarloRayTracer::threadRender(int tId, float *pixels, const Octree &tree, const Camera &cam, const int NUM_THREADS) {

#if UNIFORM_DIST
	int raysPerPixel = 1; // Must be even sqrt number (2, 4, 9, 16, 25 etc..)
	float sqrtRPP = sqrtf(raysPerPixel);
#else
	int raysPerPixel = 16;

#endif
	
	
	for (int u = 0; u < _W / NUM_THREADS; ++u) {
		for (int v = 0; v < _H; ++v) {
			glm::vec3 accumDiffColor(0.0f,0.0f,0.0f);
#if UNIFORM_DIST
			// Distributes rays uniformly within the pixel (u,v)
			for (float rpU=-1.0f/(sqrtRPP); rpU<1.0f - 1.0f/(sqrtRPP); rpU += 1.0f/sqrtRPP) {
				for (float rpV=-1.0f/(sqrtRPP); rpV<1.0f-1.0f/(sqrtRPP); rpV += 1.0f/sqrtRPP) {

					// Reason for this code? ^

			// for (float rpU = 0.f; rpU < 1.0f; rpU += 1.0f / sqrtRPP) {
			// 	for (float rpV = 0.f; rpV < 1.f; rpV += 1.0f / sqrtRPP) {
					float u2 = u * NUM_THREADS + tId + rpU;
					float v2 = v + rpV;
#else
			float randU, randV;
			for(int rpp=1; rpp<=raysPerPixel; ++rpp) {
				{
					randU = _rgen.nextFloat() / 1.f;
					randV = _rgen.nextFloat() / 1.f;
					
					float u2 = u * NUM_THREADS + tId + randU;
					float v2 = v + randV;
#endif					
					float x;
					float y;
					calculateXnY(u2, v2, x, y);
					Ray r = cam.createRay(x, y);
					IntersectionPoint ip;
					
					if (tree.intersect(r, ip)) {
						accumDiffColor = iterateRay(r, tree, 0);
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
		}
	}
}

void MonteCarloRayTracer::testTimers(){
	Timer::getInstance()->start("GLMR");
	int s = 1000000;
	for ( int i = 0; i < s; ++i) {
		
		glm::linearRand(-0.5f, 0.5f);
		glm::linearRand(-0.5f, 0.5f);
	}
	Timer::getInstance()->stop("GLMR");
	Timer::getInstance()->start("RNG");	
	for (int i = 0; i < s; ++i) {
		(void)(_rgen.nextFloat() - 0.5f);
		(void)(_rgen.nextFloat() - 0.5f);
	}
	Timer::getInstance()->stop("RNG");

	Timer::getInstance()->start("SFMT");
	for (int i = 0; i < s; ++i) {
		(void)(sfmt_genrand_uint32(&_randomGenerator) - 0.5f);
		(void)(sfmt_genrand_uint32(&_randomGenerator) - 0.5f);
	}
	Timer::getInstance()->stop("SFMT");
	Timer::getInstance()->printRealTime("SFMT");
	Timer::getInstance()->printRealTime("GLMR");
	Timer::getInstance()->printRealTime("RNG");
}

void MonteCarloRayTracer::render(float *pixels, Octree *tree, Camera *cam) {
	const int NUM_THREADS = std::thread::hardware_concurrency();
	sfmt_init_gen_rand(&_randomGenerator, 1234);
//	int i = sfmt_genrand_uint32(&_randomGenerator);

	_rgen = Rng();

	testTimers();

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
