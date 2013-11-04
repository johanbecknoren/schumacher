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

glm::vec3 MonteCarloRayTracer::iterateRay(Ray ray, const Octree &tree, int depth, bool kill) {
	IntersectionPoint ip;
	glm::vec3 color(0.0f);
	glm::vec3 rad(0.0f);
	if(tree.intersect(ray, ip)) {
		
		if (ip.getMaterial().getMaterialType() == LIGHT) {
			addToMeanDepth(depth);
			return ip.getMaterial().getDiffuseColor();
		} else if(ip.getMaterial().getMaterialType() == GLASS) {
		//	return glm::vec3(0.0f,0.0f,1.0f);
		}

		// Do russian roulette to terminate rays.
		float russianRandom = _rgen.nextFloat();
		float killRange = 0.98f;
		if (killRange < russianRandom) kill = true;
		
		if(depth < _maxDepth || !kill) {
			
			rad = ip.getMaterial().getEmission();
			bool isInsideObj = ( glm::dot(ray.getDirection(), ip.getNormal() ) > 0.0f) ? true : false;

			if(isInsideObj && ip.getMaterial().getMaterialType() == GLASS) { // If coming from inside obj going out into air (refraction needed)
//				float n2overn1 = REFRACTION_AIR / ray.getRefractionIndex();
				
				ip.setNormal(-1.0f*ip.getNormal());
				
				float n2overn1 = ray.getRefractionIndex() / REFRACTION_AIR;

				float critical_angle = asin(n2overn1);
				float angle_in = acos(glm::dot(ip.getNormal(), -ray.getDirection()));
				
				if(angle_in > critical_angle) { // Total internal reflection, no refraction
					//std::cout << "Inside, Total internal reflection!\n";
					
					Ray new_ray = calculateReflection(ray, ip);
					
					rad += ip.getMaterial().getDiffuseColor() * iterateRay(new_ray, tree, ++depth, kill);
				} else { // Calc and spawn refracted and reflected rays
					//std::cout << "Inside, Both internal refl and refraction\n";
					
					ip.getMaterial().setRefractionIndex(REFRACTION_AIR);
//					std::cout<<"Inside, before";
//					Ray new_ray = calculateRefraction(ray, ip);
					
					Ray new_ray = Ray(ip.getPoint() + 0.001f*ray.getDirection(), ray.getDirection(), ip.getMaterial().getRefractionIndex());
					
					rad += (1.0f-ip.getMaterial().getOpacity()) *
						ip.getMaterial().getDiffuseColor()*iterateRay(new_ray, tree, ++depth, kill);

//					std::cout<<"Inside, after";
					Ray new_ray2 = calculateReflection(ray, ip);
					return rad;
					rad += ip.getMaterial().getOpacity() *
						ip.getMaterial().getDiffuseColor()*iterateRay(new_ray2, tree, ++depth, kill);


				}

			}
			else { // Check for opacity (-> refraction + reflection), otherwise just reflect
				glm::vec3 origin = ip.getPoint();
				
				glm::vec3 diffuse_dir = glm::vec3(2.f * _rgen.nextFloat() - 1.f,
												  2.f * _rgen.nextFloat() - 1.f,
												  2.f * _rgen.nextFloat() - 1.f);
				glm::normalize(diffuse_dir);
				
				if (glm::dot(diffuse_dir, ip.getNormal()) < 0) {
					diffuse_dir = -diffuse_dir;
				}
				// Not sure if the diffuse dir is in sphere or half-sphere. Seems like
				// sphere.
				// Perfect refl ray
				Ray refl_ray = calculateReflection(ray,ip);
				
				// Interpolate between diffuse and perf refl to get new reflected ray
				float t = ip.getMaterial().getSpecular();
				glm::vec3 dir = glm::normalize(diffuse_dir*(1.0f-t) + refl_ray.getDirection()*t);
				
				refl_ray = Ray(origin + dir * 0.0001f, dir);

				if(ip.getMaterial().getOpacity() < 1.0f-0.0001f) { // Do refraction + reflection
					//float n2overn1 = ip.getMaterial().getRefractionIndex() / REFRACTION_AIR;
					float n2overn1 = ray.getRefractionIndex() / ip.getMaterial().getRefractionIndex();
					float critical_angle = asin(n2overn1);
					float cosIn = glm::dot(ip.getNormal(), -ray.getDirection());
					
					if(acos(cosIn) < critical_angle) { // Refraction + reflection
//						std::cout<<"Outside, refraction+reflection\n";
						
//						std::cout<<"Ray origin before: "<<glm::to_string(ray.getOrigin())<<"Ray dir: "<<glm::to_string(ray.getDirection())<<std::endl;
						Ray refr_ray = calculateRefraction(ray, ip);
//						Ray refr_ray = Ray(ip.getPoint() - 0.001f*ray.getDirection(), ray.getDirection(), ip.getMaterial().getRefractionIndex());

//						std::cout<<"Ray origin after: "<<glm::to_string(refr_ray.getOrigin())<<"Ray dir: "<<glm::to_string(refr_ray.getDirection())<<std::endl;

						rad += (1.0f-ip.getMaterial().getOpacity()) * 
							ip.getMaterial().getDiffuseColor()*iterateRay(refr_ray, tree, ++depth, kill);
						rad += ip.getMaterial().getOpacity() * 
							ip.getMaterial().getDiffuseColor()*iterateRay(refl_ray, tree, ++depth, kill);
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
