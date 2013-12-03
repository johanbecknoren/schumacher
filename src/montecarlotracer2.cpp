#include "progressbar.h"
#include "montecarlotracer2.h"
#include "timer.h"
#include <thread>
#include <vector>
#include <chrono>
#include "SFMT.h"
#include <glm/gtx/random.hpp>
#include "glmprint.h"
//#include "sphere.h"
//#include "scenebuilder.h"

void MonteCarloRayTracer2::addToCount() {
	_mutex.lock();
	++_rayCounter;
	_mutex.unlock();
}

void MonteCarloRayTracer2::addToMeanDepth(int d) {
	_depthMutex.lock();
	_meanRayDepth += d;
	_depthMutex.unlock();
}

glm::vec3 MonteCarloRayTracer2::iterateRay(Ray &ray, const Octree &tree, int depth, bool kill) {
	IntersectionPoint ip;

	glm::vec3 Lrd(0.0f), Ls(0.0f), Ldl(0.0f);

	glm::vec3 rad(0.0f);

	int countd = 1, counts = 2, countl = 20;

	if(tree.intersect(ray, ip)) {
		
		if (ip.getMaterial()->getMaterialType() == LIGHT) {
			addToMeanDepth(depth);
			return ip.getMaterial()->getDiffuseColor();
		}

		// Do russian roulette to terminate rays.
		float russianRandom = _rgen.nextFloat();

		float killRange = 0.9f;
		if (killRange < russianRandom) kill = true;
		
		if( (depth < _minDepth || !kill) && depth < _maxDepth) {
			
			rad = ip.getMaterial()->getEmission();
			//bool isInsideObj = ( glm::dot(ray.getDirection(), ip.getNormal() ) >= 0.0f) ? true : false;

			glm::vec3 diffuse_dir;
			IntersectionPoint ip_temp;

			// diffuse indirect light
			for(int i=0; i<countd; ++i) {
				diffuse_dir = glm::vec3(2.f * _rgen.nextFloat() - 1.f,
												  2.f * _rgen.nextFloat() - 1.f,
												  2.f * _rgen.nextFloat() - 1.f);
				diffuse_dir = glm::normalize(diffuse_dir);
				if (glm::dot(diffuse_dir, ip.getNormal()) < 0) {
					diffuse_dir = -diffuse_dir;
				}

				Ray diffuse_ray(ip.getPoint() + 0.0001f*diffuse_dir, diffuse_dir);
				float cosA = glm::dot( glm::normalize(ip.getNormal()), diffuse_dir);

				glm::vec3 brdf = ip.getMaterial()->getDiffuseColor() * (1.f-ip.getMaterial()->getSpecular()) / PI;
				float pdf = cosA / PI;
				if(tree.intersect(diffuse_ray, ip_temp)) {
					if(ip_temp.getMaterial()->getMaterialType() != LIGHT) {
						Lrd += iterateRay(diffuse_ray, tree, depth+1, kill) * brdf * cosA / pdf;
						//Lrd += (2.f * ip.getMaterial()->getDiffuseColor()
						//	* (1.f-ip.getMaterial()->getSpecular()) * iterateRay(diffuse_ray, tree, depth+1, kill)) * cosA;
	
					}
				}
			}
			Lrd /= float(countd);

			// perfect speuclar reflections
			/*for(int i=0; i<counts; ++i) {
				Ray refl_ray = calculateReflection(ray, ip);
				float cosA = glm::dot(ip.getNormal(), refl_ray.getDirection());
				
				if(tree.intersect(refl_ray, ip_temp)) {
					if(ip_temp.getMaterial()->getMaterialType() != LIGHT) { // Indirect
						Ls += ip.getMaterial()->getDiffuseColor() 
							* ip.getMaterial()->getSpecular() * iterateRay(refl_ray, tree, depth+1, kill) * cosA/1.f;
					} else { // Direct
						Ls += ip.getMaterial()->getDiffuseColor() * ip.getMaterial()->getSpecular() * cosA/1.f;
					}
				}
			}
			Ls /= float(counts);*/

			// direct diffuse light (shadow rays)
			for(int i=0; i<countl; ++i) {
				// Slumpad rikning mot ljuskälla
				// Kolla så att strålen verkligen når ljuskällan och inte är skuggad.
				glm::vec3 randLightPoint = _lightQuad.getRandomPoint(_rgen.nextFloat(),_rgen.nextFloat(),_rgen.nextFloat());
				//std::cout<<glm::to_string(randLightPoint);
				//randLightPoint = glm::normalize(randLightPoint);
				glm::vec3 d = randLightPoint - ip.getPoint();
				float lightDist = glm::length(d);
				glm::vec3 randLightDir = glm::normalize(ip.getPoint() - randLightPoint); // From light source to ip
				Ray lightSourceRay(randLightPoint+0.001f*randLightDir, randLightDir);
				Ray reverseLightSourceRay(ip.getPoint() - 0.001f*randLightDir, -randLightDir);

				if(tree.intersect(reverseLightSourceRay, ip_temp)) {
					//if(ip_temp.getMaterial()->getMaterialType() == LIGHT) {
						float visibility = (ip_temp.getMaterial()->getMaterialType()==LIGHT) ? 1 : 0;
						float cosThetaPrim = glm::dot(-ip_temp.getNormal(), d);
						float cosTheta = glm::dot(ip.getNormal(), d);
						cosThetaPrim = glm::max(0.0f, cosThetaPrim);
						cosTheta = glm::max(0.0f, cosTheta);

						Ldl += (ip.getMaterial()->getDiffuseColor() * (1.f-ip.getMaterial()->getSpecular())
							* _lightQuad.getArea() * ip_temp.getMaterial()->getEmission()
							* cosTheta * cosThetaPrim) / (lightDist*lightDist*lightDist*lightDist);
					//} 
					//else {
					//	std::cout<<"MissedLightSås!";
					//}
				}
				
			}

			Ldl /= float(countl);
			
			return Lrd+Ls+Ldl;

		}
		else {
			addToMeanDepth(depth);
		}
	}

	return rad;
}

void MonteCarloRayTracer2::threadRender(float *pixels, const Octree &tree, const Camera &cam, MonteCarloRayTracer2::ThreadData thd) {
	for (int u = 0; u < _W / thd.NUM_THREADS; ++u) {
		glm::vec3 accumDiffColor(0.0f,0.0f,0.0f);

		float randU, randV;
		for(int rpp=1; rpp<=_raysPerPixel; ++rpp) {
			float x;
			float y;
			{
				randU = _rgen.nextFloat() / 1.f;
				randV = _rgen.nextFloat() / 1.f;
				
				float u2 = u * thd.NUM_THREADS + thd.tId + randU;
				float v2 = thd.row + randV;
				
				calculateXnY(u2, v2, x, y);
			}
			Ray r = cam.createRay(x, y);
			IntersectionPoint ip;
				
			if (tree.intersect(r, ip)) {

				r.setOrigin(ip.getPoint() + r.getDirection() * 0.00001f);

				glm::vec3 color = iterateRay(r, tree, 0, false);
				accumDiffColor += color;
			}
			addToCount();
				
			ProgressBar::printTimedProgBar(_rayCounter, _W * _H * _raysPerPixel, "Carlo");
		}
		int id = calculateId(u * thd.NUM_THREADS + thd.tId, thd.row);

		pixels[id + 0] = glm::min(1.f, accumDiffColor.x/float(_raysPerPixel));
		pixels[id + 1] = glm::min(1.f, accumDiffColor.y/float(_raysPerPixel));
		pixels[id + 2] = glm::min(1.f, accumDiffColor.z/float(_raysPerPixel));

		
	}
}

void MonteCarloRayTracer2::glRender(float *pixels) {
#ifdef USE_OPENGL
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawPixels(_W, _H, GL_RGB, GL_FLOAT, pixels);
	glfwSwapBuffers();
#endif
}
 
void MonteCarloRayTracer2::render(float *pixels, Octree *tree, Camera *cam, bool singleThread, bool renderDuring) {
	Timer::getInstance()->start("Carlo");
	int NUM_THREADS = 1;//std::thread::hardware_concurrency();
	std::cout << "Starting carlo tracer with ";
	if (singleThread) {
		NUM_THREADS = 1;
		std::cout << NUM_THREADS << " thread\n";
	}
	else {
		std::cout << NUM_THREADS << " threads\n";
	}

	_rgen = Rng();
	std::vector<std::thread> threads;
	// Start threads
	
	for (int row = _H - 1; row >= 0 ; --row) {
		for (int i = 0; i < NUM_THREADS; ++i) {
			ThreadData thd(i, row, NUM_THREADS);
			threads.push_back(std::thread(&MonteCarloRayTracer2::threadRender, this,
										  pixels, *tree, *cam, thd));
		}
		for (auto &thread : threads) {
			thread.join();
		}
		threads.clear();
#ifdef USE_OPENGL
		if (renderDuring) {
			glRender(pixels);
		}
#endif
	}
    
	// Join threads
	std::cout << std::endl;
	Timer::getInstance()->stop("Carlo");
	Timer::getInstance()->printRealTime("Carlo");
	std::cout << "Mean depth: " << float(_meanRayDepth) / float(_W * _H * _raysPerPixel) << std::endl;
}
