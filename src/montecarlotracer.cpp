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

void MonteCarloRayTracer::addToMeanDepth(int d) {
	_depthMutex.lock();
	_meanRayDepth += d;
	_depthMutex.unlock();
}

glm::vec3 MonteCarloRayTracer::iterateRay(Ray &ray, const Octree &tree, int depth, bool kill) {
	IntersectionPoint ip;
	glm::vec3 color(0.0f);
	glm::vec3 rad(0.0f);
	if(tree.intersect(ray, ip)) {
		
		if (ip.getMaterial().getMaterialType() == LIGHT) {
			return ip.getMaterial().getDiffuseColor();
		}

		// Do russian roulette to terminate rays.
		float russianRandom = _rgen.nextFloat();
		float killRange = 0.98f;
		if (killRange < russianRandom) kill = true;
		
		if(depth < _maxDepth || !kill) {
			
			rad = ip.getMaterial().getEmission();
			bool isInsideObj = ( glm::dot(ray.getDirection(), ip.getNormal() ) > 0.0f) ? true : false;

			if(isInsideObj) { // If coming from inside obj going out into air (refraction needed)
				//std::cout<<"No!!!\n";
				float n2overn1 = REFRACTION_AIR / ray.getRefractionIndex();
				float critical_angle = asin(n2overn1);
				float cosIn = glm::dot(ip.getNormal(), -ray.getDirection());
				
				if(acos(cosIn) > critical_angle) { // Total internal reflection
					ip.setNormal(-1.0f*ip.getNormal());
					Ray new_ray = calculateReflection(ray, ip);
					rad += ip.getMaterial().getDiffuseColor()*iterateRay(new_ray, tree, ++depth, kill);
				} else { // Calc and spawn refracted and reflected rays
					Material m = ip.getMaterial();
					m.setRefractionIndex(REFRACTION_AIR);
					ip.setMaterial(m);

					Ray new_ray = calculateRefraction(ray, ip);

					rad += (1.0f-ip.getMaterial().getOpacity()) *
						ip.getMaterial().getDiffuseColor()*iterateRay(new_ray, tree, ++depth, kill);

					new_ray = calculateReflection(ray, ip);
					rad += ip.getMaterial().getOpacity() *
						ip.getMaterial().getDiffuseColor()*iterateRay(new_ray, tree, ++depth, kill);

				}
			}
			else { // Check for opacity (-> refraction + reflection), otherwise just reflect
				glm::vec3 origin = ip.getPoint();
				
				// // Calc vectors for plane of intersection point
				// glm::vec3 a = glm::vec3(1,0,0)*ip.getNormal() - origin;
				// glm::vec3 b = glm::cross(a,ip.getNormal());

				// // Diffuse refl in random direction
				// float phi = glm::linearRand(0.0f,2.0f*PI);
				// float rand = _rgen.nextFloat();
				// glm::vec3 diffuse_dir = a*rand*glm::cos(phi) + b*rand*glm::sin(phi) + ip.getNormal()*glm::sqrt(1.0f-rand*rand);

				// float x1 = _rgen.nextFloat(), x2 = , x3 = _rgen.nextFloat();
				glm::vec3 diffuse_dir = glm::vec3(2.f * _rgen.nextFloat() - 1.f,
												  2.f * _rgen.nextFloat() - 1.f,
												  2.f * _rgen.nextFloat() - 1.f);
				glm::normalize(diffuse_dir);
				// float x1, x2, S;
				// do {
				// 	x1 = 2.0f * _rgen.nextFloat() - 1.0f;
				// 	x2 = 2.0f * _rgen.nextFloat() - 1.0f;
				// 	S = x1 * x1 + x2 * x2;
				// } while (S > 1.f);
				// glm::vec3 diffuse_dir = glm::vec3(2.0f * x1 * sqrt(1.0f - S),
				// 								  2.0f * x2 * sqrt(1.0f - S),
				// 								  abs(1.0f - 2.0f * S));
				if (glm::dot(diffuse_dir, ip.getNormal()) < 0) {
					diffuse_dir = -diffuse_dir;
				}
				// Not sure if the diffuse dir is in sphere or half-sphere. Seems like
				// sphere.
				// Perfect refl ray
				Ray refl_ray = calculateReflection(ray,ip);
				
				// Interpolate between diffuse and perf refl to get new reflected ray
				float t = 0.f; // ip.getMaterial().getSpecular();
				glm::vec3 dir = glm::normalize(diffuse_dir*(1.0f-t) + refl_ray.getDirection()*t);
				
				refl_ray = Ray(origin + dir * 0.0001f, dir);

				if(ip.getMaterial().getOpacity() < 1.f) { // Do refraction + reflection
					//std::cout<<"Should not happen yet!\n";
					float n2overn1 = ip.getMaterial().getRefractionIndex() / REFRACTION_AIR;
					float critical_angle = asin(n2overn1);
					float cosIn = glm::dot(ip.getNormal(), -ray.getDirection());
					
					if(acos(cosIn) < critical_angle) { // Refraction + reflection
						Ray refr_ray = calculateRefraction(ray, ip);
						

						rad += /*(1.0f-ip.getMaterial().getOpacity())*/0.5f*ip.getMaterial().getDiffuseColor()*iterateRay(refr_ray, tree, ++depth, kill);

						rad += /*ip.getMaterial().getOpacity()*/0.5f*ip.getMaterial().getDiffuseColor()*iterateRay(refl_ray, tree, ++depth, kill);

					}
				} else { // Only reflection
					Ray new_ray = calculateReflection(ray, ip);
					rad +=ip.getMaterial().getDiffuseColor() * iterateRay(refl_ray, tree, ++depth, kill);
				}
			}
		}
		else {
			addToMeanDepth(depth);
			//rad += ip.getMaterial().getDiffuseColor();
		}
	}

	return rad;
}

#define UNIFORM_DIST 0

void MonteCarloRayTracer::threadRender(int tId, float *pixels, const Octree &tree, const Camera &cam, int row, const int NUM_THREADS) {

	int *tex = new int[_W * _H * 3];
	for (int u = 0; u < _W / NUM_THREADS; ++u) {

		glm::vec3 accumDiffColor(0.0f,0.0f,0.0f);

		float randU, randV;
		for(int rpp=1; rpp<=_raysPerPixel; ++rpp) {

			randU = _rgen.nextFloat() / 1.f;
			randV = _rgen.nextFloat() / 1.f;
				
			float u2 = u * NUM_THREADS + tId + randU;
			float v2 = row + randV;
				
			float x;
			float y;
			calculateXnY(u2, v2, x, y);
			Ray r = cam.createRay(x, y);
			IntersectionPoint ip;
				
			if (tree.intersect(r, ip)) {
				Ray ray(ip.getPoint() + r.getDirection() * 0.00001f, r.getDirection());
				glm::vec3 color = iterateRay(ray, tree, 0, false);
				accumDiffColor += color;
			}
			addToCount();
				
			ProgressBar::printTimedProgBar(_rayCounter, _W * _H * _raysPerPixel, "Carlo");

		}
			
		int id = calculateId(u * NUM_THREADS + tId, row);

		pixels[id + 0] = accumDiffColor.x/float(_raysPerPixel);
		pixels[id + 1] = accumDiffColor.y/float(_raysPerPixel);
		pixels[id + 2] = accumDiffColor.z/float(_raysPerPixel);

		
	}
	threadDone[tId] = true;
	delete tex;
}

void MonteCarloRayTracer::glRender(float *pixels) {
#ifdef USE_OPENGL
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawPixels(_W, _H, GL_RGB, GL_FLOAT, pixels);
	glfwSwapBuffers();
#endif
}
 
void MonteCarloRayTracer::render(float *pixels, Octree *tree, Camera *cam) {

	const int NUM_THREADS = std::thread::hardware_concurrency();

	_rgen = Rng();

	std::cout << "Starting carlo tracer with " << NUM_THREADS << " threads.\n";
	std::vector<std::thread> threads;
	// Start threads
	Timer::getInstance()->start("Carlo");
	for (int row = 0; row < _H; ++row) {
		for (int i = 0; i < NUM_THREADS; ++i) {
			threads.push_back(std::thread(&MonteCarloRayTracer::threadRender, this,
										  i, pixels, *tree, *cam, row, NUM_THREADS));
			threadDone.push_back(false);
		}
		for (auto &thread : threads) {
			thread.join();
		}
		threads.clear();
		threadDone.clear();
		glRender(pixels);
	}
	
	// Join threads
	
	std::cout << std::endl;
	Timer::getInstance()->stop("Carlo");
	Timer::getInstance()->printRealTime("Carlo");
	std::cout << "Mean depth: " << float(_meanRayDepth) / float(_W * _H * _raysPerPixel) << std::endl;
}
