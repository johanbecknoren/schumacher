#ifndef TRACER_H
#define TRACER_H

#include "camera.h"
#include "Ray.h"
#include "intersectionpoint.h"

class Tracer {
	public:
		Tracer(const int W, const int H) : _W(W), _H(H) {
			// Godtycklig FOV (http://www.unknownroad.com/rtfm/graphics/rt_eyerays.html)
			_tanfovx = (float(M_PI)/4.0f);
			_tanfovy = tan((float(H)/float(W)) * _tanfovx);
			_tanfovx = tan(_tanfovx);
		}

		Ray calculateReflection(const Ray &inRay, const IntersectionPoint &ip) {
			glm::vec3 reflection = -(glm::dot(inRay.getDirection(), ip.getPoint()) * ip.getNormal()) + inRay.getDirection();
			Ray r = Ray(ip.getPoint(), reflection);
			r.setOrigin(ip.getPoint() + 0.001f * reflection);
			return r;
		}
		int calculateId(const int u, const int v) const {
			return u * 3 + _W * (_H - 1 - v) * 3;
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
