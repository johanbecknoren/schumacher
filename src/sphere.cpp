#include "sphere.h"
#include <sstream>
Sphere::Sphere(const float &radius, const glm::vec3 &position) {
	_radius = radius;
	_position = position;
	createAABB();
}

void Sphere::createAABB() {
	glm::vec3 lLB(_position.x-_radius, _position.y-_radius, _position.z-_radius);
	glm::vec3 uRF(_position.x+_radius, _position.y+_radius, _position.z+_radius);
	this->_boundingBox = new AABB(lLB, uRF, _position);
}

std::string Sphere::asString() const {
	std::stringstream s;
	s << getName() << " p:[" << _position.x << " " << _position.y << " "
      << _position.z << "] r:" << _radius;
	return s.str();
}

bool Sphere::getIntersectionPoint(Ray &ray, IntersectionPoint &ip) const {
	/*Algorithm from http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection*/

	float A,B,C,t;

	Ray ray_cp = Ray(ray);
	IntersectionPoint d;
    bool intersecting = _boundingBox->getIntersection(ray_cp, d);
	glm::vec3 dir = glm::normalize(ray.getDirection());

    if(intersecting) {// Intersection with AABB exists
		A = glm::dot(dir, dir);
		B = 2 * glm::dot((ray.getOrigin() - _position), dir);
		C = glm::dot((ray.getOrigin() - _position),
                 (ray.getOrigin() - _position)) - _radius*_radius;

		float sqrtTerm = B*B - 4*A*C;

		if(sqrtTerm < 0.0f) {// Imaginary root(s)
			return false;
		}

		float t0, t1, q;
		if(B<0)
			q = (-B - sqrtf(sqrtTerm))/2.0f;
		else
			q = (-B + sqrtf(sqrtTerm))/2.0f;

		//To avoid poor numerical prec. when B ~= sqrt(sqrtTerm)
		t0 = q / A;
		t1 = C / q;

		if(t0 > t1) { // If t1 smaller than t0, swap them
			float temp = t0;
			t0 = t1;
			t1 = temp;
		}

		if(t1 < 0.0f) {// Intersection, but in the ray's negative direction
			return false;
		}

		if(t0 < 0.0f) // If t0 <= 0, intersection is at t1
			t = t1;
		else
			t = t0;

		//ray.print();
		glm::vec3 intP = ray.getOrigin() + glm::normalize(ray.getDirection())*t + 0.0001f*ray.getDirection();
		glm::vec3 surfNormal = glm::normalize(intP - _position);
		ip = IntersectionPoint(intP, surfNormal, getMaterial());

		return true;  }
  else { // No intersection with sphere's AABB
    return false;
  }
}
