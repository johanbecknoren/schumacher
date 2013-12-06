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

	int countd = 1, counts = 1, countl = 20;

	if(tree.intersect(ray, ip)) {
		
		//ip.setPoint(ip.getPoint() - 0.01f*ray.getDirection());

		if (ip.getMaterial()->getMaterialType() == LIGHT) {
			addToMeanDepth(depth);
			return ip.getMaterial()->getDiffuseColor() * ip.getMaterial()->getEmission();
		}

		// Do russian roulette to terminate rays.
		float russianRandom = _rgen.nextFloat();

		float killRange = 0.9f;
		if (killRange < russianRandom) kill = true;
		
		if( (depth < _minDepth || !kill) && depth < _maxDepth) {
			
			rad = ip.getMaterial()->getEmission();
			bool isInsideObj = ( glm::dot(ray.getDirection(), ip.getNormal() ) >= 0.0f) ? true : false;

			glm::vec3 diffuse_dir;
			IntersectionPoint ip_temp;

			// diffuse indirect light
#if 1
			for(int i=0; i<countd; ++i) {
				float n = 0.5;
				float r1 = _rgen.nextFloat();
				float phi = r1 * PI * 2.f;
				float r2 = _rgen.nextFloat();
				float theta = acos(pow(r2, 1.f/(1.f+n)));

				float cosTheta = cos(theta);//glm::max(0.0f,glm::dot(ip.getNormal(), ray.getDirection()));
				float pdf = ((n+1)/2.f*PI) * pow(cosTheta, n);//1.f/(2.f*PI);//cosTheta / PI;

				// Use PDF when picking sample direction here
				//float thetaPrim = (2.f*_rgen.nextFloat()-1.f) / pdf;
				//float thetaPrim = (PI*_rgen.nextFloat()) / pdf;
				//float phiPrim = (2.f*_rgen.nextFloat()-1.f) / pdf;
				//float phiPrim = (2.f*PI*_rgen.nextFloat()) / pdf;

				/*diffuse_dir = glm::vec3(2.f * _rgen.nextFloat() - 1.f, // [-1, 1]
										2.f * _rgen.nextFloat() - 1.f,
										2.f * _rgen.nextFloat() - 1.f);*/
				/*diffuse_dir = glm::vec3 (
					sin(thetaPrim)*cos(phiPrim),
					sin(thetaPrim)*sin(phiPrim),
					cos(thetaPrim)
					);*/
				diffuse_dir = glm::vec3 (
					sin(phi)*cos(theta),
					sin(phi)*sin(theta),
					cos(phi)
					);

				// Rotate diffuse_dir to distribution of normal vector
				float el = -acos(ip.getNormal().z);
				float az = -atan2(ip.getNormal().y, ip.getNormal().x);

				// Rotate around y
				glm::vec3 diffuse_dir2 = glm::vec3(cos(el) * diffuse_dir.x - sin(el) * diffuse_dir.z, diffuse_dir.y, sin(el) * diffuse_dir.x + cos(el) * diffuse_dir.z);
				// Rotate around z
				diffuse_dir = glm::normalize(glm::vec3(cos(az) * diffuse_dir2.x + sin(az) * diffuse_dir2.y, -sin(az) * diffuse_dir2.x + cos(az) * diffuse_dir2.y, diffuse_dir2.z));
				glm::vec3 reverse_diffuse_dir = -1.f*diffuse_dir;
				//diffuse_dir = glm::normalize(diffuse_dir);
				
				/*if (glm::dot(diffuse_dir, ip.getNormal()) < 0.f) {
					diffuse_dir = -diffuse_dir;
				}*/

				Ray diffuse_ray(ip.getPoint() + 0.01f*diffuse_dir, diffuse_dir);

				float cosA = glm::max(0.f, glm::dot( glm::normalize(ip.getNormal()), diffuse_dir));
				glm::vec3 brdf = ip.getMaterial()->getDiffuseColor() * (1.f-ip.getMaterial()->getSpecular()) / PI;

				if(tree.intersect(diffuse_ray, ip_temp)) {
					if(ip_temp.getMaterial()->getMaterialType() != LIGHT) {
						Ray reverse_diffuse_ray(ip_temp.getPoint(), reverse_diffuse_dir);
						//glm::vec3 val = iterateRay(diffuse_ray, tree, depth+1, kill) * cosA * brdf;// / pdf;
						glm::vec3 val = iterateRay(reverse_diffuse_ray, tree, depth+1, kill) * cosA * brdf / pdf;
						//if(glm::dot(glm::vec3(1,1,1), val)<0.0f) val *= -1.f;
						Lrd += val;//iterateRay(diffuse_ray, tree, depth+1, kill) * cosA * brdf / pdf;
						//Lrd += (2.f * ip.getMaterial()->getDiffuseColor()
							//* (1.f-ip.getMaterial()->getSpecular()) * iterateRay(diffuse_ray, tree, depth+1, kill)) * cosA;
	
					}
				}
			}
			//Lrd = Lrd * PI;
			Lrd /= float(countd);
			//Lrd = glm::clamp(Lrd, 0.f,1.f);
#endif
#if 1
			// perfect specular reflections (även refraktion här)
			int num_refr_rays = 0;
			for(int i=0; i<counts; ++i) {
				// Refraction
				if(ip.getMaterial()->getMaterialType() == GLASS) {
					if(isInsideObj) { // coming from inside object
						ip.setNormal(-ip.getNormal());
						float n2overn1 = REFRACTION_AIR / REFRACTION_GLASS;
						float snell = n2overn1;
						float angle_in = acos(glm::dot( ip.getNormal(), -ray.getDirection()));
						float critical_angle = asin(snell);
						
						ray.setRefractionIndex(REFRACTION_GLASS);
						Ray refl_ray = calculateReflection(ray, ip);

						if(angle_in > critical_angle) {	// Total internal reflection
						} else {
							ip.getMaterial()->setRefractionIndex(REFRACTION_AIR);
							Ray refr_ray = calculateRefraction(ray, ip);
							ip.getMaterial()->setRefractionIndex(REFRACTION_GLASS);
							Ls += (1.0f-ip.getMaterial()->getOpacity())
								* ip.getMaterial()->getDiffuseColor()
								* iterateRay(refr_ray, tree,depth+1,kill);
							++num_refr_rays;
						}
						Ls += (ip.getMaterial()->getOpacity()) *
						ip.getMaterial()->getDiffuseColor() * iterateRay(refl_ray, tree, depth+1, kill);
					} else {	// Coming from outside (air)
						Ray refr_ray = calculateRefraction(ray, ip);
						Ls += (1.0f-ip.getMaterial()->getOpacity())
								* ip.getMaterial()->getDiffuseColor()
								* iterateRay(refr_ray, tree,depth+1,kill);
						++num_refr_rays;
					}
				}
				// Reflection
				Ray refl_ray = calculateReflection(ray, ip);
				//float cosA = glm::dot(ip.getNormal(), refl_ray.getDirection());
				if(tree.intersect(refl_ray, ip_temp)) {
					if(ip_temp.getMaterial()->getMaterialType() != LIGHT) { // Indirect
						Ls += ip.getMaterial()->getDiffuseColor() 
							* ip.getMaterial()->getSpecular() * iterateRay(refl_ray, tree, depth+1, kill)// * cosA/1.f
							* ip.getMaterial()->getOpacity();

					} else { // Direct
						Ls += ip_temp.getMaterial()->getEmission() * ip.getMaterial()->getDiffuseColor() * ip.getMaterial()->getSpecular();// * cosA/1.f;
					}
				}
			}
			Ls /= float(counts);
#endif
#if 1
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
#endif
			return (Lrd+Ls+Ldl);

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
	int NUM_THREADS = std::thread::hardware_concurrency();
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
