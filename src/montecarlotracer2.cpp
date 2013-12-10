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
Vec3 rotate(const Vec3 &n, Vec3 &v) {
    if (!std::abs(1 - std::abs( glm::dot(n, Vec3(0,1,0)) )) <= float(1e-3)) {
	    Vec3 bX = glm::cross(n, Vec3(0,1,0) );
		Vec3 bZ = glm::normalize(glm::cross(n, glm::normalize(bX)));
            
	    return bX * v.x + n * v.y + bZ * v.z;
    } else{
        return v * glm::sign(glm::dot(v, n));
    }
}
Vec3 hemisphereRotate(const Vec3 &n, const real theta, const real phi) {
    Vec3 exitant(std::cos(phi)*std::sin(theta),
                    std::cos(theta),
                    std::sin(phi)*std::sin(theta));
    exitant = rotate(n, exitant);
   
	return glm::normalize(exitant);
}

// Ray going from n1 to n2
inline real getFresnel(const real &n1, const real &n2, const real& cosTheta) {
	//float R0 = std::pow( (n1 - n2)/(n1 + n2), real(2));
	//return R0 + (1-R0)*std::pow(1-cosTheta, 5.f);
	return std::pow( (n1 - n2)/(n1 + n2), real(2));
}
Vec3 MonteCarloRayTracer2::iterateAirToGlass(Ray &ray, const Octree &tree, int depth, bool kill) {
	Vec3 Lr(real(0.0)); // reflected radiance
	Vec3 Lt(real(0.0)); // refracted/trasmitted radiance;
	//std::cout<<"In glass->air! depth="<<depth<<"kill="<<kill<<std::endl;
	float R=1.f,T=0.f;

	IntersectionPoint ip, ip_temp, iIp;

	real fresnel_factor = real(1);

	// Do russian roulette to terminate rays.
	float russianRandom = _rgen.nextFloat();

	float killRange = 0.9f;
	if (killRange < russianRandom) kill = true;
		
	if( (depth <= _minDepth || !kill) && depth < _maxDepth) {
		if(tree.intersect(ray, ip)) {
			// Reflection
			/*Vec3 refl_dir = glm::reflect(ray.getDirection(), ip.getNormal());
			Ray refl_ray(ip.getPoint(), refl_dir);*/
			Ray refl_ray = calculateReflection(ray, ip);
			//if(glm::dot(ip.getNormal(), refl_ray.getDirection()) < 0.0f) std::cout <<"Error in air->glass reflection 1!\n";
			//if(glm::dot(ip.getNormal(), refl_ray.getDirection()) < 0.0f) refl_ray.setDirection(-refl_ray.getDirection());
			//std::cout<<"refl->norm dot: "<<glm::to_string(glm::dot(ip.getNormal(), refl_ray.getDirection()))<<std::endl;
			if(tree.intersect(refl_ray, ip_temp)) {
				// Indirect illumination
				if(ip_temp.getMaterial()->getMaterialType() != LIGHT) {
					Vec3 val = iterateRay(refl_ray, tree, depth+1, kill);
					Lr = val;
				} else { // Direct illumination
					Lr = ip_temp.getMaterial()->getEmission()
						* ip_temp.getMaterial()->getDiffuseColor();
				}
			}

			// Refraction
			Ray refr_ray = calculateRefraction(ray, ip); // flippa normalen på ip här? tror inte det...
			//if(glm::dot(refr_ray.getDirection(), ip.getNormal()) >= 0.0f) std::cout<<"Error in air->glass refraction ray 2!\n";
			//if(glm::dot(refr_ray.getDirection(), ip.getNormal()) >= 0.0f) refr_ray.setDirection(-refr_ray.getDirection());
			//if(glm::dot(refr_ray.getDirection(), -ip.getNormal()) < glm::dot(refl_ray.getDirection(), ip.getNormal())) std::cout << "Felaktig refraktion beräkning1" << std::endl;
			if(tree.intersect(refr_ray, ip_temp)) {
				fresnel_factor = getFresnel(REFRACTION_AIR, REFRACTION_GLASS, real(0));
				float cosA = glm::dot(-ip.getNormal(), refr_ray.getDirection());
				//if(cosA < 0.0f) std::cout<<"Error in air->glass refraction ray!\n";

				Vec3 val = iterateGlassToAir(refr_ray, tree, depth+1, kill);
				Lt = val;
			}
		}
		Vec3 ret = (fresnel_factor*Lr + (real(1)-fresnel_factor)*Lt) * ip.getMaterial()->getDiffuseColor();
	} else {
	return Vec3(real(0));
	}
}
Vec3 MonteCarloRayTracer2::iterateGlassToAir(Ray &ray, const Octree &tree, int depth, bool kill) {
	Vec3 Lr(real(0.0)); // reflected radiance
	Vec3 Lt(real(0.0)); // refracted/trasmitted radiance;
	//std::cout<<"In glass->air! depth="<<depth<<"kill="<<kill<<std::endl;
	float R=real(1),T=real(0);

	IntersectionPoint ip, ip_temp, iIp;
	Vec3 iN; // inverted normal direction

	real fresnel_factor = real(1.0);

	// Do russian roulette to terminate rays.
	float russianRandom = _rgen.nextFloat();

	float killRange = 0.9f;
	if (killRange < russianRandom) kill = true;
		
	if( (depth <= _minDepth || !kill) && depth < _maxDepth) {

		// Inner reflection
		if(tree.intersect(ray, ip)) {
			iIp = ip;
			iIp.setNormal(real(-1.0)*ip.getNormal());

			// Inner reflection
			Ray refl_ray = calculateReflection(ray, iIp);
			real rCosA = std::min(glm::dot(iIp.getNormal(), refl_ray.getDirection()), real(1.0));
			//if(glm::dot(refl_ray.getDirection(), iIp.getNormal()) < 0.0f) std::cout<<"Error in internal reflection3!\n";
			//if(glm::dot(refl_ray.getDirection(), iIp.getNormal()) < 0.0f) refl_ray.setDirection(-refl_ray.getDirection());
			if(tree.intersect(refl_ray, ip_temp)) {
				Vec3 val = iterateGlassToAir(refl_ray, tree, depth+1, kill);
				Lr = val;
			}

			// Refraction
			float critical_angle = asin(REFRACTION_AIR/REFRACTION_GLASS);
			float angle_in = acos(rCosA);//acos(glm::dot( ip.getNormal(), -ray.getDirection()));
			if(angle_in < critical_angle) {
				fresnel_factor = getFresnel(REFRACTION_GLASS, REFRACTION_AIR, 0.0f);
				iIp.getMaterial()->setRefractionIndex(REFRACTION_AIR);
				Ray refr_ray = calculateRefraction(ray, iIp);
				iIp.getMaterial()->setRefractionIndex(REFRACTION_GLASS);
				//if(glm::dot(refr_ray.getDirection(), ip.getNormal()) >= 0.0f) std::cout<<"Error in glass->air refraction4!\n";
				//if(glm::dot(refr_ray.getDirection(), ip.getNormal()) >= 0.0f) refr_ray.setDirection(-refr_ray.getDirection());

				if(tree.intersect(refr_ray, ip_temp)) {
					real tCosA = glm::dot(iIp.getNormal(), refr_ray.getDirection());

					// Indirect illumination
					if(ip_temp.getMaterial()->getMaterialType() != LIGHT) {
						Vec3 val = iterateRay(refr_ray, tree, depth+1, kill);
						Lt = val;
					} else { // Direct illumination, ip_temp on light source
						Lt = ip_temp.getMaterial()->getEmission()
							* ip_temp.getMaterial()->getDiffuseColor();
							//* PI/_lightQuad.getArea();
					}
				}
			}
			
		}
		Vec3 ret = //Vec3(0.f,0.0f,1000.f)*
			(fresnel_factor*Lr + (1.f-fresnel_factor)*Lt) * ip.getMaterial()->getDiffuseColor();
		//std::cout<<glm::to_string(ret)<<std::endl;
		return ret;
	}
	else
	{
		return Vec3(0.0f);
	}
}

Vec3 MonteCarloRayTracer2::iterateRay(Ray &ray, const Octree &tree, int depth, bool kill) {
	IntersectionPoint ip;

	Vec3 Lrd(real(0.0)), Ls(real(0.0)), Ldl(real(0.0));

	Vec3 rad(real(0.0));

	int num_diffuse_rays = 1, num_specular_rays = 1, num_shadow_rays = 2;

	if(tree.intersect(ray, ip)) {
		

		if (ip.getMaterial()->getMaterialType() == LIGHT) {
			addToMeanDepth(depth);
			return ip.getMaterial()->getDiffuseColor() * ip.getMaterial()->getEmission();
		} else if(ip.getMaterial()->getMaterialType() == GLASS) {
			//return iterateAirToGlass(ray, tree,depth,kill);
		}

		// Do russian roulette to terminate rays.
		float russianRandom = _rgen.nextFloat();

		float killRange = 0.9f;
		if (killRange < russianRandom) kill = true;
		
		if( (depth <= _minDepth || !kill) && depth < _maxDepth) {
			
			rad = ip.getMaterial()->getEmission();
			bool isInsideObj = ( glm::dot(ray.getDirection(), ip.getNormal() ) >= 0.0f) ? true : false;

			Vec3 diffuse_dir;
			IntersectionPoint ip_temp;

			// diffuse indirect light
#if 1
			for(int i=0; i<num_diffuse_rays; ++i) {
				real n = real(0.1);
				real r1 = _rgen.nextFloat();
				real phi = r1 * PI * real(2);
				real r2 = _rgen.nextFloat();
				real theta = acos(pow(r2, real(1)/(real(1)+n))); // USE THIS!!
				//float theta = 0.5f*PI*r2; // DO NOT USE
				//float cosTheta = cos(theta);//glm::max(0.0f,glm::dot(ip.getNormal(), ray.getDirection()));
				//float pdf = cosTheta / PI;//((n+1)/2.f*PI) * pow(cosTheta, n);//1.f/(2.f*PI);

				diffuse_dir = hemisphereRotate(ip.getNormal(), theta, phi);	

				Ray diffuse_ray(ip.getPoint() + real(0.01)*diffuse_dir, diffuse_dir);

				Vec3 reverse_diffuse_dir = real(-1)*diffuse_dir;

				//float cosA = glm::dot( glm::normalize(ip.getNormal()), diffuse_dir);
				//Vec3 brdf = ip.getMaterial()->getDiffuseColor() * (1.f-ip.getMaterial()->getSpecular()) / PI;
				
				if(tree.intersect(diffuse_ray, ip_temp)) {
					if(ip_temp.getMaterial()->getMaterialType() != LIGHT) {
						Ray reverse_diffuse_ray(ip_temp.getPoint() - real(0.001)*reverse_diffuse_dir, reverse_diffuse_dir);
						Vec3 val = iterateRay(reverse_diffuse_ray, tree, depth+1, kill);
						Lrd += val;
					}
				}
			}
			Lrd *= ip.getMaterial()->getDiffuseColor() * (1.f-ip.getMaterial()->getSpecular());
			Lrd /= real(num_diffuse_rays);
#endif
#if 1
			// perfect direct and indirect specular reflections (refraktion here aswell)
			int num_refr_rays = 0;
			for(int i=0; i<num_specular_rays; ++i) {
#if 1
				real fresnel_factor = 1.0f;
				// Refraction
				real old_ray_refr_index = ray.getRefractionIndex();
				if(ip.getMaterial()->getMaterialType() == GLASS) {

					if(isInsideObj) { // coming from inside object GLASS->AIR
						/*
						Vec3 radiance_reflected(real(0.0));
						Vec3 radiance_refracted(real(0.0));

						ip.setNormal(real(-1.0)*ip.getNormal());

						real n2overn1 = REFRACTION_AIR / REFRACTION_GLASS;
						real snell = n2overn1;
						real angle_in = acos(glm::dot( ip.getNormal(), -ray.getDirection()));
						real critical_angle = asin(snell);
						
						ray.setRefractionIndex(REFRACTION_GLASS);
						// Inner reflection
						// Denna stråle vet vi kommer behöva refrakteras igen. 
						// Lägga GLASS->AIR i separat funktion och anropa denna explicit?
						Ray refl_ray = calculateReflection(ray, ip);
						if(tree.intersect(refl_ray, ip_temp)) {
							radiance_reflected += fresnel_factor//ip.getMaterial()->getOpacity()
								* iterateRay(refl_ray, tree, depth+1, kill);
						}

						// Refraction GLASS -> AIR
						if(angle_in < critical_angle) {	// Do refraction
							fresnel_factor = getFresnel(REFRACTION_GLASS, REFRACTION_AIR, glm::dot(ray.getDirection(), ip.getNormal()));
							ip.getMaterial()->setRefractionIndex(REFRACTION_AIR);
							Ray refr_ray = calculateRefraction(ray, ip);
							ip.getMaterial()->setRefractionIndex(REFRACTION_GLASS);
							
							radiance_refracted += (real(1) - fresnel_factor)//(1.0f-ip.getMaterial()->getOpacity())
								* iterateRay(refr_ray, tree, depth+1, kill);
						}

						Ls += (fresnel_factor*radiance_reflected + (real(1)-fresnel_factor)*radiance_refracted)
							*ip.getMaterial()->getDiffuseColor();
						//ip.setNormal(real(-1)*ip.getNormal());
						*/
					} else { // Coming from outside AIR->GLASS

						Vec3 radiance_reflected(real(0.0));
						Vec3 radiance_refracted(real(0.0));

						// Reflection
						Ray refl_ray = calculateReflection(ray, ip);
						if(tree.intersect(refl_ray, ip_temp)) {
							// Indirect specular reflection
							radiance_reflected += iterateRay(refl_ray, tree, depth+1, kill);

							// Direct specular reflection
							if(ip_temp.getMaterial()->getMaterialType() == LIGHT) {
								radiance_reflected += ip_temp.getMaterial()->getEmission() 
									* ip_temp.getMaterial()->getDiffuseColor();
							}
						}
						
						// Refraction
						Ray refr_ray = calculateRefraction(ray, ip);
						if(tree.intersect(refr_ray, ip_temp)) {
							fresnel_factor = getFresnel(REFRACTION_AIR, REFRACTION_GLASS, glm::dot(ray.getDirection(), ip.getNormal()) );
							//radiance_refracted += iterateRay(refr_ray, tree, depth+1, kill);
							radiance_refracted = iterateGlassToAir(refr_ray, tree, depth, kill);
						}

						Ls += ((fresnel_factor*radiance_reflected) + radiance_refracted)
							*ip.getMaterial()->getDiffuseColor();
					}

					return iterateAirToGlass(ray, tree, depth, kill);
				} else { // Not GLASS
#endif
					if(ip.getMaterial()->getMaterialType() != GLASS) {
						// Reflection
						if(ip.getMaterial()->getSpecular() > EPSILON) { // Not diffuse materials
							Ray refl_ray = calculateReflection(ray, ip);
							if(tree.intersect(refl_ray, ip_temp)) {
								// Indirect specular reflection
								if(ip_temp.getMaterial()->getMaterialType() != LIGHT) {
									Ls += ip.getMaterial()->getDiffuseColor() 
										* ip.getMaterial()->getSpecular()
										* iterateRay(refl_ray, tree, depth+1, kill);// * cosA/1.f
								} else { // Direct specular reflection, ip_temp is on light source
									Ls += ip_temp.getMaterial()->getEmission() 
										* ip_temp.getMaterial()->getDiffuseColor()
										* ip.getMaterial()->getDiffuseColor() 
										* ip.getMaterial()->getSpecular();// * cosA/1.f;
								}
							}
						}
					}
				}
			}
			Ls /= real(num_specular_rays);
#endif
#if 1
			// direct diffuse light (shadow rays)
			for(int i=0; i<num_shadow_rays; ++i) {
				// Slumpad rikning mot ljuskälla
				// Kolla så att strålen verkligen når ljuskällan och inte är skuggad.
				Vec3 randLightPoint = _lightQuad.getRandomPoint(_rgen.nextFloat(),_rgen.nextFloat(),_rgen.nextFloat());
				Vec3 d = randLightPoint - ip.getPoint();
				real lightDist = glm::length(d);
				Vec3 randLightDir = glm::normalize(ip.getPoint() - randLightPoint); // From light source to ip
				Ray lightSourceRay(randLightPoint+real(0.001)*randLightDir, randLightDir);
				Ray reverseLightSourceRay(ip.getPoint() - real(0.001)*randLightDir, -randLightDir);

				if(tree.intersect(reverseLightSourceRay, ip_temp)) {
					real visibility = (ip_temp.getMaterial()->getMaterialType()==LIGHT) ? 1 : 0;
					real cosThetaPrim = glm::dot(-ip_temp.getNormal(), d);
					real cosTheta = glm::dot(ip.getNormal(), d);
					cosThetaPrim = glm::max(real(0.0), cosThetaPrim);
					cosTheta = glm::max(real(0.0), cosTheta);

					Ldl += visibility*(ip.getMaterial()->getDiffuseColor() * (real(1)-ip.getMaterial()->getSpecular())
						* _lightQuad.getArea() * ip_temp.getMaterial()->getEmission()
						* cosTheta * cosThetaPrim) / (lightDist*lightDist*lightDist*lightDist);
				}
				
			}

			Ldl /= real(num_shadow_rays);
#endif
			return (Lrd+Ls+Ldl);
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
void MonteCarloRayTracer2::threadRender(real *pixels, const Octree &tree, const Camera &cam, MonteCarloRayTracer2::ThreadData thd) {
	for (int u = 0; u < _W / thd.NUM_THREADS; ++u) {
		Vec3 accumDiffColor(real(0.0));

		real randU, randV;
		real x;
		real y;
		{
			randU = _rgen.nextFloat() / real(1);
			randV = _rgen.nextFloat() / real(1);
				
			real u2 = u * thd.NUM_THREADS + thd.tId + randU;
			real v2 = thd.row + randV;
				
			calculateXnY(u2, v2, x, y);
		}
		Ray r = cam.createRay(x, y);
		IntersectionPoint ip;
			
		if (tree.intersect(r, ip)) {
			r.setOrigin(ip.getPoint() + r.getDirection() * real(0.00001));

			Vec3 color = iterateRay(r, tree, 0, false);
			accumDiffColor += color;

			//std::cout<<"Intersection, color="<<glm::to_string(accumDiffColor);
			
		}
		addToCount();
				
		ProgressBar::printTimedProgBar(_rayCounter, _W * _H * _raysPerPixel, "Carlo");
		int id = calculateId(u * thd.NUM_THREADS + thd.tId, thd.row);

		_buffer[id + 0] = glm::min(real(1), accumDiffColor.x);
		_buffer[id + 1] = glm::min(real(1), accumDiffColor.y);
		_buffer[id + 2] = glm::min(real(1), accumDiffColor.z);		
	}
}
#endif

void MonteCarloRayTracer2::glRender(real *pixels) {
#ifdef USE_OPENGL
	glClearColor(real(0.0), real(1.0), real(0.0), real(0.0));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawPixels(_W, _H, GL_RGB, GL_FLOAT, pixels);
	glfwSwapBuffers();
#endif
}
 
void MonteCarloRayTracer2::render(real *pixels, Octree *tree, Camera *cam, bool singleThread, bool renderDuring) {
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
			pixels[id + 0] = (pixels[id + 0]*(rpp-1) + _buffer[id + 0])/(rpp);
			pixels[id + 1] = (pixels[id + 1]*(rpp-1) + _buffer[id + 1])/(rpp);
			pixels[id + 2] = (pixels[id + 2]*(rpp-1) + _buffer[id + 2])/(rpp);
		}
	}
#endif

	std::cout << std::endl;
	Timer::getInstance()->stop("Carlo");
	Timer::getInstance()->printRealTime("Carlo");
	std::cout << "Mean depth: " << float(_meanRayDepth) / float(_W * _H * _raysPerPixel) << std::endl;
}
