#ifndef TRACER_H
#define TRACER_H

#include "camera.h"
#include "Ray.h"
#include "intersectionpoint.h"

class Tracer {
	public:
		Tracer(const int W, const int H) : _W(W), _H(H) {
			// Godtycklig FOV (http://www.unknownroad.com/rtfm/graphics/rt_eyerays.html)
			_tanfovx = -(float(M_PI)/8.0f);
			_tanfovy = tan((float(H)/float(W)) * _tanfovx);
			_tanfovx = tan(_tanfovx);
		}

		Ray calculateReflection(const Ray &inRay, const IntersectionPoint &ip) {
			glm::vec3 reflection = inRay.getDirection() - 2 * glm::dot(inRay.getDirection(), ip.getNormal()) * ip.getNormal();			
			Ray r = Ray(ip.getPoint() + 0.001f * reflection, reflection);
			return r;
		}

		Ray calclulateRefraction(const Ray &inRay, const IntersectionPoint &ip) {
			float cosIn = glm::dot(ip.getNormal(), -inRay.getDirection());
			float n1overn2 = inRay.getRefractionIndex()/ip.getMaterial().getRefractionIndex();
			float cosOut = sqrtf(1 - n1overn2*(1 - cosIn*cosIn));
			
			glm::vec3 refr_dir;
			if(cosIn > 0.0f)
				refr_dir = (n1overn2) * inRay.getDirection() + (n1overn2*cosIn - cosOut)*ip.getNormal();
			else		
				refr_dir = (n1overn2) * inRay.getDirection() + (n1overn2*cosIn + cosOut)*ip.getNormal();

			return Ray(ip.getPoint(), glm::normalize(refr_dir), ip.getMaterial().getRefractionIndex());
		}

		int calculateId(const int u, const int v) const {
			return u * 3 + _W * v * 3;
		}

		void calculateXnY(const int u, const int v, float &x, float &y) {
			x = ( (2.0f*float(u)-float(_W))/float(_W) ) * _tanfovx;
			y = -( (2.0f*float(v)-float(_H))/float(_H) ) * _tanfovy;
		}

		void calculateXnY(const float u, const float v, float &x, float &y) {
			x = ( (2.0f*float(u)-float(_W))/float(_W) ) * _tanfovx;
			y = -( (2.0f*float(v)-float(_H))/float(_H) ) * _tanfovy;
		}

// 		Ray calculateRefraction() {
// 
// 		}
	protected:
		const int _W;
		const int _H;
		float _tanfovx;
		float _tanfovy;
};

#endif
