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

glm::vec3 MonteCarloRayTracer::iterateRay(Ray &ray, const Octree &tree, int depth, bool kill) {
	IntersectionPoint ip;
	glm::vec3 color(0.0f);
	int maxDepth = 4;
	glm::vec3 rad(0.0f);
	if(tree.intersect(ray, ip)) {
		float rand = glm::linearRand(0.0f, 1.0f);
		rad = ip.getMaterial().getEmission();
		bool isInsideObj = ( glm::dot(ray.getDirection(), ip.getNormal() ) > 0.0f) ? true : false;
		
		if (ip.getMaterial().getMaterialType() == LIGHT) {
			return ip.getMaterial().getDiffuseColor();
		}

		// Do russian roulette to terminate rays.
		float russianRandom = _rgen.nextFloat();
		float killRange = 0.9f;
		if (killRange > russianRandom) kill = true;
		if(depth < maxDepth || !kill) {
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

					rad += (1.0f-ip.getMaterial().getOpacity())*ip.getMaterial().getDiffuseColor()*iterateRay(new_ray, tree, ++depth, kill);

					new_ray = calculateReflection(ray, ip);
					rad += ip.getMaterial().getOpacity()*ip.getMaterial().getDiffuseColor()*iterateRay(new_ray, tree, ++depth, kill);

				}
			} else { // Check for opacity (-> refraction + reflection), otherwise just reflect
				glm::vec3 origin = ip.getPoint();
				
				// Calc vectors for plane of intersection point
				glm::vec3 a = glm::vec3(1,0,0)*ip.getNormal() - origin;
				glm::vec3 b = glm::cross(a,ip.getNormal());

				// Diffuse refl in random direction
				float phi = glm::linearRand(0.0f,2.0f*PI);
				glm::vec3 diffuse_dir = a*rand*glm::cos(phi) + b*rand*glm::sin(phi) + ip.getNormal()*glm::sqrt(1.0f-rand*rand);
				
				// Not sure if the diffuse dir is in sphere or half-sphere. Seems like
				// sphere.
				if (glm::dot(diffuse_dir, ip.getNormal()) < 0) {
					diffuse_dir = -diffuse_dir;
				}
				// Perfect refl ray
				Ray refl_ray = calculateReflection(ray,ip);
				
				// Interpolate between diffuse and perf refl to get new reflected ray
				float t = ip.getMaterial().getSpecular();
				glm::vec3 dir = glm::normalize(diffuse_dir*(1.0f-t) + refl_ray.getDirection()*t);
				glm::vec3 dir2 = glm::normalize(diffuse_dir);
				refl_ray = Ray(origin, dir2);

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
					rad += ip.getMaterial().getDiffuseColor()*iterateRay(refl_ray, tree, ++depth, kill);
				}
			}
		} else {
			//rad += ip.getMaterial().getDiffuseColor();
		}
	}

	return rad;
}

#define UNIFORM_DIST 0

void MonteCarloRayTracer::threadRender(int tId, float *pixels, const Octree &tree, const Camera &cam, int row, const int NUM_THREADS) {

	int raysPerPixel = 100;
	int *tex = new int[_W * _H * 3];
	for (int u = 0; u < _W / NUM_THREADS; ++u) {

		glm::vec3 accumDiffColor(0.0f,0.0f,0.0f);

		float randU, randV;
		for(int rpp=1; rpp<=raysPerPixel; ++rpp) {

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
				glm::vec3 color = iterateRay(r, tree, 0, false);
				accumDiffColor += color;
			}
			addToCount();
				
			ProgressBar::printTimedProgBar(_rayCounter, _W * _H * raysPerPixel, "Carlo");

		}
			
		int id = calculateId(u * NUM_THREADS + tId, row);

		pixels[id + 0] = accumDiffColor.x/float(raysPerPixel);
		pixels[id + 1] = accumDiffColor.y/float(raysPerPixel);
		pixels[id + 2] = accumDiffColor.z/float(raysPerPixel);

		
	}
	threadDone[tId] = true;
	delete tex;
}

void MonteCarloRayTracer::glRender(float *pixels) {
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawPixels(_W, _H, GL_RGB, GL_FLOAT, pixels);
	glfwSwapBuffers();
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
}
