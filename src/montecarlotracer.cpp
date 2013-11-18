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
		
		if( (depth < _minDepth || !kill) && depth < _maxDepth) {
			
			rad = ip.getMaterial()->getEmission();
			bool isInsideObj = ( glm::dot(ray.getDirection(), ip.getNormal() ) >= 0.0f) ? true : false;

			if(isInsideObj && ip.getMaterial()->getMaterialType() == GLASS) { // If coming from inside obj going out into air (refraction needed)
				float n2overn1 = REFRACTION_AIR / REFRACTION_GLASS;//ip.getMaterial()->getRefractionIndex(); // Should always be < 1
				float n1overn2 = ip.getMaterial()->getRefractionIndex() / REFRACTION_AIR;
				float snell = n2overn1;
				ip.setNormal(-1.0f*ip.getNormal());
				float angle_in = acos(glm::dot( glm::normalize(ip.getNormal()), glm::normalize(ray.getDirection() * -1.0f) ));
				float critical_angle = asin(snell);

				ray.setRefractionIndex(REFRACTION_GLASS);

				//TODO: correct reflectance acc. to Fresnel eq.
				float reflectance_s = ip.getMaterial()->getOpacity();
				Ray refl_ray = calculateReflection(ray, ip);
//				std::cout<<"ray refract="<<ray.getRefractionIndex()<<",angle_in="<<angle_in<<",critical_angle="<<critical_angle<<std::endl;
				if(angle_in > critical_angle) {// Calc and spawn refracted and reflected rays
					// Only internal reflection
					//std::cout<<"HERRRKKKA\n";
				} else {
					float old_refr_idx = float(ip.getMaterial()->getRefractionIndex());
					ip.getMaterial()->setRefractionIndex(REFRACTION_AIR);
					Ray refr_ray = calculateRefraction(ray, ip);
					ip.getMaterial()->setRefractionIndex(REFRACTION_GLASS); // TODO should not be hardcoded, but old_refr_idx always returns REFRACTION_AIR and never glass.
					//std::cout<<"ip mat type:"<<ip.getMaterial()->getMaterialType()<<"old ip refr idx"<<old_refr_idx<<"ip refr idx:"<<ip.getMaterial()->getRefractionIndex();
//					std::cout<<"derrp";
					float reflectance_s_nom = (ray.getRefractionIndex()*angle_in) -
		 				(ip.getMaterial()->getRefractionIndex() * glm::dot(refr_ray.getDirection(), ip.getNormal()));
					float reflectance_s_denom = (ray.getRefractionIndex()*angle_in) +
						(ip.getMaterial()->getRefractionIndex() * glm::dot(refr_ray.getDirection(), ip.getNormal()));
//					reflectance_s = (reflectance_s_nom/reflectance_s_denom) * (reflectance_s_nom/reflectance_s_denom);
//					rad += (1.0f-reflectance_s) *
					rad += (1.0f - ip.getMaterial()->getOpacity()) * 
						ip.getMaterial()->getDiffuseColor()*iterateRay(refr_ray, tree, depth + 1, kill);
				} 
//				rad += reflectance_s * 
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
				float reflectance_s = ip.getMaterial()->getOpacity();

				if(ip.getMaterial()->getOpacity() < 1.f-EPSILON) { // Do refraction + reflection

					//std::cout<<"ip mat type:"<<ip.getMaterial()->getMaterialType()<<"ip refr idx:"<<ip.getMaterial()->getRefractionIndex();
					Ray refr_ray = calculateRefraction(ray, ip);
					//std::cout<<"orig dir: "<<glm::to_string(ray.getDirection())<<"refr dir: "<<glm::to_string(refr_ray.getDirection())<<std::endl;
					float angle_in = glm::dot(ip.getNormal(), ray.getDirection()*-1.0f);
					//std::cout<<"angle_in = "<<angle_in<<std::endl;

					float reflectance_s_nom = (ray.getRefractionIndex()*angle_in) -
		 				(ip.getMaterial()->getRefractionIndex() * glm::dot(refr_ray.getDirection(), -ip.getNormal()));
					float reflectance_s_denom = (ray.getRefractionIndex()*angle_in) +
						(ip.getMaterial()->getRefractionIndex() * glm::dot(refr_ray.getDirection(), -ip.getNormal()));
//					reflectance_s = (reflectance_s_nom/reflectance_s_denom) * (reflectance_s_nom/reflectance_s_denom);

//					std::cout<<"reflectance_s = "<<reflectance_s<<std::endl;
//					rad += (1.0f-reflectance_s) * 
					rad += (1.0f - ip.getMaterial()->getOpacity()) * 
						ip.getMaterial()->getDiffuseColor()*iterateRay(refr_ray, tree, depth + 1, kill);
				}

//				rad += reflectance_s * 
				rad += ip.getMaterial()->getOpacity() * 
					ip.getMaterial()->getDiffuseColor()*iterateRay(refl_ray, tree, depth + 1, kill);
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
