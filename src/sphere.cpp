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
	glm::vec3 dir = glm::normalize(ray->getDirection());

	if(d != NULL) {// Intersection with AABB exists
		std::cout << "\nSphere.cpp - Found intersection with AABB\n";
		A = glm::dot(dir, dir);
		B = 2 * glm::dot((ray->getOrigin() - _position), dir);
		C = glm::dot((ray->getOrigin() - _position), (ray->getOrigin() - _position)) - _radius*_radius;
		
		std::cout<<"A = "<<A<<", B = "<<B<<", C = "<<C<<"\n";

		std::cout <<"1\n";
		float sqrtTerm = B*B - 4*A*C;

		if(sqrtTerm < 0.0f) {// Imaginary root(s)
			std::cout << "Sphere.cpp - No intersection, imaginary roots\n";
			return NULL;
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
			std::cout << "Sphere.cpp -Intersection in ray's negative direction\n";
			return NULL;
		}

		if(t0 < 0.0f) // If t0 <= 0, intersection is at t1
			t = t1;
		else
			t = t0;

		//ray->print();
		std::cout<<"t = "<<t<<", t0 = "<<t0<<", t1 = "<<t1<<"\n";
		glm::vec3 intP = ray->getOrigin() + glm::normalize(ray->getDirection())*t;
		glm::vec3 surfNormal = glm::normalize(intP - _position);

   		return new IntersectionPoint(intP, surfNormal);	
  }
  else { // No intersection with sphere's AABB
	std::cout << "Sphere.cpp - No intersection with AABB\n";
    return NULL;
  }
}
