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

#define SCANLINE 0
#define PER_SAMPLE 1

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

// 3dim Reflection of a vector on a normal //
glm::vec3 rotate(const glm::vec3 &n, glm::vec3 &v) {
    if (!std::abs(1 - std::abs( glm::dot(n, glm::vec3(0,1,0)) )) <= float(1e-3)) {
	    glm::vec3 bX = glm::cross(n, glm::vec3(0,1,0) );
		glm::vec3 bZ = glm::normalize(glm::cross(n, glm::normalize(bX)));
            
	    return bX * v.x + n * v.y + bZ * v.z;
    } else{
        return v * glm::sign(glm::dot(v, n));
    }
}
glm::vec3 hemisphereRotate(const glm::vec3 &n, const float theta, const float phi) {
    glm::vec3 exitant(std::cos(phi)*std::sin(theta),
                    std::cos(theta),
                    std::sin(phi)*std::sin(theta));
    exitant = rotate(n, exitant);
   
	return glm::normalize(exitant);
}

inline float getFresnel(const float &n1, const float &n2, const float& cosTheta) {
	//float R0 = std::pow( (n1 - n2)/(n1 + n2), float(2.f));
	//return R0 + (1-R0)*std::pow(1-cosTheta, 5.f);
	return std::pow( (n1 - n2)/(n1 + n2), float(2.f));
}

glm::vec3 MonteCarloRayTracer2::iterateRay(Ray &ray, const Octree &tree, int depth, bool kill) {
	IntersectionPoint ip;

	glm::vec3 Lrd(0.0f), Ls(0.0f), Ldl(0.0f);

	glm::vec3 rad(0.0f);

	int countd = 1, counts = 1, countl = 2;

	if(tree.intersect(ray, ip)) {
		
		//ip.setPoint(ip.getPoint() - 0.01f*ray.getDirection());

		if (ip.getMaterial()->getMaterialType() == LIGHT) {
			addToMeanDepth(depth);
			//std::cout <<"Hej!";
			return ip.getMaterial()->getDiffuseColor() * ip.getMaterial()->getEmission();
		}

		// Do russian roulette to terminate rays.
		float russianRandom = _rgen.nextFloat();

		float killRange = 0.9f;
		if (killRange < russianRandom) kill = true;
		
		if( (depth <= _minDepth || !kill) && depth < _maxDepth) {
			
			rad = ip.getMaterial()->getEmission();
			bool isInsideObj = ( glm::dot(ray.getDirection(), ip.getNormal() ) >= 0.0f) ? true : false;

			glm::vec3 diffuse_dir;
			IntersectionPoint ip_temp;

			// diffuse indirect light
#if 1
			for(int i=0; i<countd; ++i) {
				float n = 0.1f;
				float r1 = _rgen.nextFloat();
				float phi = r1 * PI * 2.f;
				float r2 = _rgen.nextFloat();
				float theta = acos(pow(r2, 1.f/(1.f+n))); // USE THIS!!
				//float theta = 0.5f*PI*r2; // DO NOT USE
				//float cosTheta = cos(theta);//glm::max(0.0f,glm::dot(ip.getNormal(), ray.getDirection()));
				//float pdf = cosTheta / PI;//((n+1)/2.f*PI) * pow(cosTheta, n);//1.f/(2.f*PI);

				diffuse_dir = hemisphereRotate(ip.getNormal(), theta, phi);	

				Ray diffuse_ray(ip.getPoint() + 0.01f*diffuse_dir, diffuse_dir);

				//TODO nåt är väldigt galet, *-1 ger samma resultat som *1...
				glm::vec3 reverse_diffuse_dir = -1.f*diffuse_dir;

				//float cosA = glm::dot( glm::normalize(ip.getNormal()), diffuse_dir);
				//glm::vec3 brdf = ip.getMaterial()->getDiffuseColor() * (1.f-ip.getMaterial()->getSpecular()) / PI;
				
				if(tree.intersect(diffuse_ray, ip_temp)) {
					if(ip_temp.getMaterial()->getMaterialType() != LIGHT) {
						/*float d = glm::length(ip_temp.getPoint() - ip.getPoint());
						std::cout<<"d="<<d;*/
						Ray reverse_diffuse_ray(ip_temp.getPoint() - 0.001f*reverse_diffuse_dir, reverse_diffuse_dir);
						glm::vec3 val = iterateRay(reverse_diffuse_ray, tree, depth+1, kill);
						Lrd += val;//glm::clamp(val,0.0f,1.0f);
					}
				}
			}
			Lrd *= ip.getMaterial()->getDiffuseColor() * (1.f-ip.getMaterial()->getSpecular());
			Lrd /= float(countd);
#endif
#if 1
			// perfect specular reflections (refraktion here aswell)
			int num_refr_rays = 0;
			//if(ip.getMaterial()->getSpecular() > EPSILON) {
				for(int i=0; i<counts; ++i) {
					float fresnel_factor = 1.0f;
					// Refraction
					float old_ray_refr_index = ray.getRefractionIndex();
					if(ip.getMaterial()->getMaterialType() == GLASS) {
						if(isInsideObj) { // coming from inside object
							ip.setNormal(-1.0f*ip.getNormal());
							float n2overn1 = REFRACTION_AIR / REFRACTION_GLASS;
							float snell = n2overn1;
							float angle_in = acos(glm::dot( ip.getNormal(), -ray.getDirection()));
							float critical_angle = asin(snell);
						
							ray.setRefractionIndex(REFRACTION_GLASS);
							Ray refl_ray = calculateReflection(ray, ip);

							fresnel_factor = getFresnel(REFRACTION_GLASS, REFRACTION_AIR, 
									glm::dot(ray.getDirection(), ip.getNormal()));
							//std::cout<<"fresnel="<<fresnel_factor;
							if(angle_in > critical_angle) {	// Total internal reflection
								fresnel_factor = 1.f;
								Ls += fresnel_factor//ip.getMaterial()->getOpacity()
								* ip.getMaterial()->getDiffuseColor()
								* iterateRay(refl_ray, tree, depth+1, kill);
							ip.setNormal(-1.0f*ip.getNormal());
							} else {
								ip.getMaterial()->setRefractionIndex(REFRACTION_AIR);
								Ray refr_ray = calculateRefraction(ray, ip);
								ip.getMaterial()->setRefractionIndex(REFRACTION_GLASS);

								Ls += (1.f - fresnel_factor)//(1.0f-ip.getMaterial()->getOpacity())
									* ip.getMaterial()->getDiffuseColor()
									* iterateRay(refr_ray, tree, depth+1, kill);
							}

							// Flytta in denna i satsen för total intern reflektion?!
							/*Ls += fresnel_factor//ip.getMaterial()->getOpacity()
								* ip.getMaterial()->getDiffuseColor()
								* iterateRay(refl_ray, tree, depth+1, kill);
							ip.setNormal(-1.0f*ip.getNormal());*/
						} else { // Coming from outside (air)
							Ray refr_ray = calculateRefraction(ray, ip);
							fresnel_factor = getFresnel(REFRACTION_AIR, REFRACTION_GLASS,
								glm::dot(ray.getDirection(), ip.getNormal()) );

							Ls += (1.f - fresnel_factor)//(1.0f - ip.getMaterial()->getOpacity())
								* ip.getMaterial()->getDiffuseColor()
								* iterateRay(refr_ray, tree, depth+1, kill);
						}
					}
					ray.setRefractionIndex(old_ray_refr_index);

					// Reflection
					Ray refl_ray = calculateReflection(ray, ip);
					//float cosA = glm::dot(ip.getNormal(), refl_ray.getDirection());
					if(tree.intersect(refl_ray, ip_temp)) {
						//if(ip_temp.getMaterial()->getSpecular() > EPSILON
							//&& ip_temp.getMaterial()->getMaterialType() != LIGHT) { // Indirect
						if(ip_temp.getMaterial()->getMaterialType() != LIGHT
							&& ip.getMaterial()->getSpecular() > EPSILON) {
								//std::cout<<"fresnel="<<fresnel_factor;
							Ls += ip.getMaterial()->getDiffuseColor() 
								* ip.getMaterial()->getSpecular()
								//* ip.getMaterial()->getOpacity()
								* fresnel_factor
								* iterateRay(refl_ray, tree, depth+1, kill);// * cosA/1.f
						} else { // Direct
							Ls += ip_temp.getMaterial()->getEmission() 
								* ip_temp.getMaterial()->getDiffuseColor()
								* ip.getMaterial()->getDiffuseColor() 
								* ip.getMaterial()->getSpecular()// * cosA/1.f;
								//* ip.getMaterial()->getOpacity();
								* fresnel_factor;
						}
					}
				}
			//}
			Ls /= float(counts);
#endif
#if 1
			// direct diffuse light (shadow rays)
			for(int i=0; i<countl; ++i) {
				// Slumpad rikning mot ljuskälla
				// Kolla så att strålen verkligen når ljuskällan och inte är skuggad.
				glm::vec3 randLightPoint = _lightQuad.getRandomPoint(_rgen.nextFloat(),_rgen.nextFloat(),_rgen.nextFloat());
				glm::vec3 d = randLightPoint - ip.getPoint();
				float lightDist = glm::length(d);
				glm::vec3 randLightDir = glm::normalize(ip.getPoint() - randLightPoint); // From light source to ip
				Ray lightSourceRay(randLightPoint+0.001f*randLightDir, randLightDir);
				Ray reverseLightSourceRay(ip.getPoint() - 0.001f*randLightDir, -randLightDir);

				if(tree.intersect(reverseLightSourceRay, ip_temp)) {
					float visibility = (ip_temp.getMaterial()->getMaterialType()==LIGHT) ? 1 : 0;
					float cosThetaPrim = glm::dot(-ip_temp.getNormal(), d);
					float cosTheta = glm::dot(ip.getNormal(), d);
					cosThetaPrim = glm::max(0.0f, cosThetaPrim);
					cosTheta = glm::max(0.0f, cosTheta);

					Ldl += visibility*(ip.getMaterial()->getDiffuseColor() * (1.f-ip.getMaterial()->getSpecular())
						* _lightQuad.getArea() * ip_temp.getMaterial()->getEmission()
						* cosTheta * cosThetaPrim) / (lightDist*lightDist*lightDist*lightDist);
				}
				
			}

			Ldl /= float(countl);
#endif
			return (Lrd+Ls+Ldl);
			//return Lrd;

		}
		else {
			addToMeanDepth(depth);
		}
	}

	return rad;
}

#if SCANLINE
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
#endif

#if PER_SAMPLE
void MonteCarloRayTracer2::threadRender(float *pixels, const Octree &tree, const Camera &cam, MonteCarloRayTracer2::ThreadData thd) {
	for (int u = 0; u < _W / thd.NUM_THREADS; ++u) {
		glm::vec3 accumDiffColor(0.0f,0.0f,0.0f);

		float randU, randV;
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
		int id = calculateId(u * thd.NUM_THREADS + thd.tId, thd.row);

		_buffer[id + 0] = glm::min(1.f, accumDiffColor.x);
		_buffer[id + 1] = glm::min(1.f, accumDiffColor.y);
		_buffer[id + 2] = glm::min(1.f, accumDiffColor.z);		
	}
}
#endif

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
#if SCANLINE
	//  10+3*_H/4
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
#endif
#if PER_SAMPLE
	for(int rpp=1; rpp<=_raysPerPixel; ++rpp) {
		std::cout<<std::endl<<"Processing view-ray no.: "<<rpp<<std::endl;
		//  10+3*_H/4
		for (int row = _H - 1; row >= 0 ; --row) {
			for (int i = 0; i < NUM_THREADS; ++i) {
				ThreadData thd(i, row, NUM_THREADS);
				threads.push_back(std::thread(&MonteCarloRayTracer2::threadRender, this,
											  _buffer, *tree, *cam, thd));
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
		// Vikta buffer mot pixels RAD FÖR RAD; EJ HELA BUFFERTEN
		for(int id=0; id<_W*_H*3; id+=3) {
			pixels[id + 0] = (pixels[id + 0]*(rpp-1) + _buffer[id + 0])/rpp;
			pixels[id + 1] = (pixels[id + 1]*(rpp-1) + _buffer[id + 1])/rpp;
			pixels[id + 2] = (pixels[id + 2]*(rpp-1) + _buffer[id + 2])/rpp;
		}
	}
#endif

	std::cout << std::endl;
	Timer::getInstance()->stop("Carlo");
	Timer::getInstance()->printRealTime("Carlo");
	std::cout << "Mean depth: " << float(_meanRayDepth) / float(_W * _H * _raysPerPixel) << std::endl;
}
