#ifndef TRACER_H
#define TRACER_H

#include "camera.h"
#include "Ray.h"
#include "intersectionpoint.h"

class Tracer {
	public:
		Tracer(const int W, const int H) : _W(W), _H(H) {
			// Godtycklig FOV (http://www.unknownroad.com/rtfm/graphics/rt_eyerays.html)
			_tanfovx = -(real(PI)/real(8));
			_tanfovy = tan((real(H)/real(W)) * _tanfovx);
			_tanfovx = tan(_tanfovx);
		}

		Ray calculateReflection(const Ray &inRay, const IntersectionPoint &ip) {
			Vec3 reflection = inRay.getDirection() - real(2) * glm::dot(inRay.getDirection(), ip.getNormal()) * ip.getNormal();			
			Ray r = Ray(ip.getPoint() + real(0.001) * reflection, glm::normalize(reflection), inRay.getRefractionIndex());
			return r;
		}

		Ray calculateRefraction(const Ray &inRay, const IntersectionPoint &ip) {
			// Snell's Law
			real cosIn = glm::dot(ip.getNormal(), glm::normalize(inRay.getDirection()*real(-1)) );
			real n1overn2 = inRay.getRefractionIndex()/ip.getMaterial()->getRefractionIndex();
			real cosOut = sqrtf(real(1) - (n1overn2*n1overn2)*(real(1) - cosIn*cosIn));

			Vec3 refr_dir;
			if(cosIn > real(0))
				refr_dir = (n1overn2) * inRay.getDirection() + (n1overn2*cosIn - cosOut)*ip.getNormal();
			else		
				refr_dir = (n1overn2) * inRay.getDirection() + (n1overn2*cosIn + cosOut)*ip.getNormal();

			return Ray(ip.getPoint()+real(0.001)*glm::normalize(refr_dir), glm::normalize(refr_dir), ip.getMaterial()->getRefractionIndex());
		}

		inline int calculateId(const int u, const int v) const {
			return u * 3 + _W * v * 3;
		}

		inline void calculateXnY(const int u, const int v, float &x, float &y) {
			x = ( (2.0f*real(u)-real(_W))/real(_W) ) * _tanfovx;
			y = -( (2.0f*real(v)-real(_H))/real(_H) ) * _tanfovy;
		}

		inline void calculateXnY(const real u, const real v, real &x, real &y) {
			x = ( (2.0f*real(u)-real(_W))/real(_W) ) * _tanfovx;
			y = -( (2.0f*real(v)-real(_H))/real(_H) ) * _tanfovy;
		}

	protected:
		const int _W;
		const int _H;
		real _tanfovx;
		real _tanfovy;
};

#endif
