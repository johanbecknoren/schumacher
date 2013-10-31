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
	glm::vec3 rad(0.0f);
	if(tree.intersect(ray, ip)) {
		float rand = glm::linearRand(0.0f, 1.0f);
		rad = ip.getMaterial().getEmission();
		bool isInsideObj = ( glm::dot(ray.getDirection(), ip.getNormal() ) > 0.0f) ? true : false;
		
		if(depth < maxDepth) {
			if(isInsideObj) { // If coming from inside obj going out into air (refraction needed)
					//std::cout<<"No!!!\n";
				float n2overn1 = REFRACTION_AIR / ray.getRefractionIndex();
				float critical_angle = asin(n2overn1);
				float cosIn = glm::dot(ip.getNormal(), -ray.getDirection());
				
				if(acos(cosIn) > critical_angle) { // Total internal reflection
					ip.setNormal(-1.0f*ip.getNormal());
					Ray new_ray = calculateReflection(ray, ip);
					rad += ip.getMaterial().getDiffuseColor()*iterateRay(new_ray, tree, ++depth);
				} else { // Calc and spawn refracted and reflected rays
					Material m = ip.getMaterial();
					m.setRefractionIndex(REFRACTION_AIR);
					ip.setMaterial(m);

					Ray new_ray = calculateRefraction(ray, ip);
					rad += 0.5f*ip.getMaterial().getDiffuseColor()*iterateRay(new_ray, tree, ++depth);

					new_ray = calculateReflection(ray, ip);
					rad += 0.5f*ip.getMaterial().getDiffuseColor()*iterateRay(new_ray, tree, ++depth);
				}
			} else { // Check for opacity (-> refraction + reflection), otherwise just reflect
				glm::vec3 origin = ip.getPoint();
				
				// Calc vectors for plane of intersection point
				// glm::vec3 a = glm::vec3(1,0,0)*ip.getNormal() - origin;
				// glm::vec3 b = glm::cross(ip.getNormal(), a);

				// Diffuse refl in random direction
				float phi = glm::linearRand(0.0f,2.0f*PI);
				// glm::vec3 diffuse_dir = a*rand*glm::cos(phi) + b*rand*glm::sin(phi) + ip.getNormal()*glm::sqrt(1.0f-rand*rand);

				float azimut = 2.f * PI * _rgen.nextFloat();
				float uval = 2.f * _rgen.nextFloat() - 1.f;
				float wval = sqrt(1 - uval * uval);
				glm::vec3 diff_dir(wval * cos(azimut), wval * sin(azimut), abs(uval));
				
				// Not sure if the diffuse dir is in sphere or half-sphere. Seems like
				// sphere.
				if (glm::dot(diff_dir, ip.getNormal()) < 0) {
					diff_dir = -diff_dir;
				}
				// Perfect refl ray
				Ray refl_ray = calculateReflection(ray,ip);
				
				// Interpolate between diffuse and perf refl to get new reflected ray
				float t = ip.getMaterial().getSpecular();
				// glm::vec3 dir = glm::normalize(diffuse_dir*(1.0f-t) + refl_ray.getDirection()*t);
				glm::vec3 dir2 = glm::normalize(diff_dir);
				refl_ray = Ray(origin, dir2);

				if(ip.getMaterial().getOpacity() < 1.f) { // Do refraction + reflection
					std::cout<<"Should not happen yet!\n";
					float n2overn1 = ip.getMaterial().getRefractionIndex() / REFRACTION_AIR;
					float critical_angle = asin(n2overn1);
					float cosIn = glm::dot(ip.getNormal(), -ray.getDirection());
					
					if(acos(cosIn) < critical_angle) { // Refraction + reflection
						Ray refr_ray = calculateRefraction(ray, ip);
						
						// TODO: use importance here instead of 50/50
						rad += 0.5f*ip.getMaterial().getDiffuseColor()*iterateRay(refr_ray, tree, ++depth);

						rad += 0.5f*ip.getMaterial().getDiffuseColor()*iterateRay(refl_ray, tree, ++depth);
					}
				} else { // Only reflection
					Ray new_ray = calculateReflection(ray, ip);
					rad += ip.getMaterial().getDiffuseColor()*iterateRay(refl_ray, tree, ++depth);
				}
			}
		} else {
//			rad += ip.getMaterial().getDiffuseColor();
		}
	}

	return rad;
}

#define UNIFORM_DIST 0

void MonteCarloRayTracer::threadRender(int tId, float *pixels, const Octree &tree, const Camera &cam, const int NUM_THREADS) {

#if UNIFORM_DIST
	int raysPerPixel = 16; // Must be even sqrt number (2, 4, 9, 16, 25 etc..)
	float sqrtRPP = sqrtf(raysPerPixel);
#else
	int raysPerPixel = 4;

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

						for (int i = 0; i < 10; ++i)
							accumDiffColor = iterateRay(r, tree, 0);
						/*float intensity = glm::dot(r.getDirection(), - ip.getNormal());
						accumDiffColor.x += intensity*ip.getMaterial().getDiffuseColor().x;
						accumDiffColor.y += intensity*ip.getMaterial().getDiffuseColor().y;
						accumDiffColor.z += intensity*ip.getMaterial().getDiffuseColor().z;*/
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
