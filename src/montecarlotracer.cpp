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
		
		if (ip.getMaterial()->getMaterialType() == LIGHT) {
			addToMeanDepth(depth);
			return ip.getMaterial()->getDiffuseColor();
		} else if(ip.getMaterial()->getMaterialType() == GLASS) {
		//	return glm::vec3(0.0f,0.0f,1.0f);
		}

		// Do russian roulette to terminate rays.
		float russianRandom = _rgen.nextFloat();

		float killRange = 0.9f;
		if (killRange < russianRandom) kill = true;
		
		if(depth < _maxDepth || !kill) {
			
			rad = ip.getMaterial()->getEmission();
			bool isInsideObj = ( glm::dot(ray.getDirection(), ip.getNormal() ) > 0.0f) ? true : false;

			if(isInsideObj && ip.getMaterial()->getMaterialType() == GLASS) { // If coming from inside obj going out into air (refraction needed)
				float n2overn1 = REFRACTION_AIR / ip.getMaterial()->getRefractionIndex(); // Should always be < 1
				float n1overn2 = ray.getRefractionIndex() / REFRACTION_AIR;
				float snell = n1overn2;
				ip.setNormal(-1.0f*ip.getNormal());
//				float critical_angle = asin(n2overn1);
				float critical_angle = acos(snell);
				float angle_in = glm::dot(ip.getNormal(), ray.getDirection() * -1.0f);
//TODO: correct reflecance acc. to Fresnel eq.	float reflectance_s = ray.get
				Ray refl_ray = calculateReflection(ray, ip);		
				
				if(acos(angle_in) > critical_angle) {// Calc and spawn refracted and reflected rays
					// Only internal reflection
				} else {
					ip.getMaterial()->setRefractionIndex(REFRACTION_AIR);
					Ray refr_ray = calculateRefraction(ray, ip);
					rad += (1.0f-ip.getMaterial()->getOpacity()) *
						ip.getMaterial()->getDiffuseColor()*iterateRay(refr_ray, tree, depth + 1, kill);
				} 
				rad += ip.getMaterial()->getOpacity() * 
					ip.getMaterial()->getDiffuseColor() * iterateRay(refl_ray, tree, depth + 1, kill);

			}
			else { // Ray coming from air
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
				float t = ip.getMaterial()->getSpecular();
				glm::vec3 dir = glm::normalize(diffuse_dir*(1.0f-t) + refl_ray.getDirection()*t);
				
				refl_ray = Ray(origin + dir * 0.0001f, dir);

				if(ip.getMaterial()->getOpacity() < 1.f-0.0001f) { // Do refraction + reflection						
					Ray refr_ray = calculateRefraction(ray, ip);
					rad += (1.0f-ip.getMaterial()->getOpacity()) * 
						ip.getMaterial()->getDiffuseColor()*iterateRay(refr_ray, tree, depth + 1, kill);
				}
				rad += ip.getMaterial()->getOpacity()*ip.getMaterial()->getDiffuseColor() * 
					iterateRay(refl_ray, tree, depth + 1, kill);
			}

		}
		else {
			addToMeanDepth(depth);
		}
	}

	return rad;
}

void MonteCarloRayTracer::threadRender(float *pixels, const Octree &tree, const Camera &cam, MonteCarloRayTracer::ThreadData thd) {
	for (int u = 0; u < _W / thd.NUM_THREADS; ++u) {
		glm::vec3 accumDiffColor(0.0f,0.0f,0.0f);

		float randU, randV;
		for(int rpp=1; rpp<=_raysPerPixel; ++rpp) {

			randU = _rgen.nextFloat() / 1.f;
			randV = _rgen.nextFloat() / 1.f;
				
			float u2 = u * thd.NUM_THREADS + thd.tId + randU;
			float v2 = thd.row + randV;
				
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
		int id = calculateId(u * thd.NUM_THREADS + thd.tId, thd.row);

		pixels[id + 0] = accumDiffColor.x/float(_raysPerPixel);
		pixels[id + 1] = accumDiffColor.y/float(_raysPerPixel);
		pixels[id + 2] = accumDiffColor.z/float(_raysPerPixel);

		
	}
}

void MonteCarloRayTracer::glRender(float *pixels) {
#ifdef USE_OPENGL
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawPixels(_W, _H, GL_RGB, GL_FLOAT, pixels);
	glfwSwapBuffers();
#endif
}
 
void MonteCarloRayTracer::render(float *pixels, Octree *tree, Camera *cam, bool singleThread, bool renderDuring) {
    Timer::getInstance()->start("Carlo");
    int NUM_THREADS = std::thread::hardware_concurrency();
    if (singleThread || NUM_THREADS == 1) {
        std::cout << "Starting carlo tracer with " << 1 << " thread.\n";
        for (int row = 0; row < _H; ++row) {
            ThreadData td(0, row, 1);
            threadRender(pixels, *tree, *cam, td);
#ifdef USE_OPENGL
            if (renderDuring) {
                glRender(pixels);
            }
#endif
        }
    }
    else {
        std::cout << "Starting carlo tracer with " << NUM_THREADS << " threads.\n";
	    _rgen = Rng();
	    std::vector<std::thread> threads;
	    // Start threads
	    
	    for (int row = 0; row < _H; ++row) {
		    for (int i = 0; i < NUM_THREADS; ++i) {
                MonteCarloRayTracer::ThreadData thd(i, row, NUM_THREADS);
			    threads.push_back(std::thread(&MonteCarloRayTracer::threadRender, this,
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
    }
	// Join threads
	
	std::cout << std::endl;
	Timer::getInstance()->stop("Carlo");
	Timer::getInstance()->printRealTime("Carlo");
	std::cout << "Mean depth: " << float(_meanRayDepth) / float(_W * _H * _raysPerPixel) << std::endl;
}
