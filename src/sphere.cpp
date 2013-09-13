#include "sphere.h"

Sphere::Sphere(const float &radius, const glm::vec3 &position) {
	_radius = radius;
	_position = position;
	createAABB();
}

void Sphere::createAABB() {
	glm::vec3 lLB(_position.x-_radius, _position.y-_radius, _position.z-_radius);
	glm::vec3 uRF(_position.x+_radius, _position.y+_radius, _position.z+_radius);
	this->boundingBox = new AABB(lLB, uRF, _position);
}

IntersectionPoint *Sphere::getIntersectionPoint(Ray *ray) const {
	/*Algorithm from http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection*/

	float A,B,C,t;
	IntersectionPoint* d = boundingBox->getIntersection(*ray);
	glm::vec3 dir = ray->getDirection();
//	std::cout << "Sphere intersection distance: " << d << std::endl;
	if(d != NULL) {// Intersection with AABB exists
		A = glm::dot(dir, dir);
		B = 2 * glm::dot((ray->getOrigin() - _position), dir);
		C = glm::dot((ray->getOrigin() - _position), (ray->getOrigin() - _position)) - _radius*_radius;

		float sqrtTerm = B*B - 4*A*C;

		if(sqrtTerm >= 0.0f) { // Positive, real, roots
			float t0, t1;
			t0 = (-B - sqrtTerm) / (2*A);
			t1 = (-B + sqrtTerm) / (2*A);

			if(t0 > t1) { // If t1 smaller than t0, swap them
				float temp;
				temp = t0;
				t0 = t1;
				t1 = temp;
			}

			if(t1 < 0.0f) // Intersection, but in the ray's negative direction
				return NULL;

			if(t0 < 0.0f) // If t0 == 0, intersection is at t1
				t = t1;
			else
				t = t0;

			glm::vec3 intP = ray->getOrigin() + glm::normalize(ray->getDirection())*t;
			glm::vec3 surfNormal = glm::normalize(intP - _position);

      return new IntersectionPoint(intP, surfNormal);

		}
    else { // Imaginary roots, no intersections
			return NULL;
		}
  }
  else { // Description of this plz
    return NULL;
  }
}
