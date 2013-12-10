#include "triangle.h"
#include "glmprint.h"

Triangle::Triangle(Vec3 ix0, Vec3 ix1, Vec3 ix2) : x1(Vec3(ix1)),
																  x2(Vec3(ix2)),
																  x0(Vec3(ix0)) {
	normal = glm::normalize(glm::cross((x1) - (x0), (x2) - (x0)));
	createAABB();
}

bool Triangle::getIntersectionPoint(Ray &ray, IntersectionPoint &ip) const {
    Vec3 edge1 = (x1) - (x0);
    Vec3 edge2 = (x2) - (x0);
    Vec3 pvec = glm::cross(ray.getDirection(), edge2);
    real det = glm::dot(edge1, pvec);
    if (det == 0.f) return false;
    real invDet = real(1) / det;
    Vec3 tvec = ray.getOrigin() - (x0);
	real U = glm::dot(tvec, pvec) * invDet;
    if (U < real(0) || U > real(1)) return false;
    Vec3 qvec = glm::cross(tvec, edge1);
    real V = glm::dot(ray.getDirection(), qvec) * invDet;
    if (V < real(0) || U + V > real(1)) return false;
    float T = glm::dot(edge2, qvec) * invDet;
	if (T < real(0)) return false;

	real dt = glm::dot(x1 - ray.getOrigin(), normal) /
		glm::dot(ray.getDirection(), normal);
	
	Vec3 newPos = ray.getOrigin() + dt * ray.getDirection();
	// std::cout << newPos << " " << ray.getOrigin() << std::endl;
	
	real v = real(1);
	if(glm::dot(ray.getDirection(), normal) > 0.0f) // Draw the quad from both directions
		v = -1.0f;

	ip = IntersectionPoint(newPos, normal * v, this->getMaterial()); 

	return true;
}


void Triangle::createAABB() {
	Vec3 lower(glm::min(glm::min(x1.x, x2.x), x0.x),
			glm::min(glm::min(x1.y, x2.y), x0.y),
			glm::min(glm::min(x1.z, x2.z), x0.z));
	Vec3 upper(glm::max(glm::max(x1.x, x2.x), x0.x),
			glm::max(glm::max(x1.y, x2.y), x0.y),
			glm::max(glm::max(x1.z, x2.z), x0.z));
	if (upper.x == lower.x) upper.x += 0.1f;
	if (upper.y == lower.y) upper.y += 0.1f;
	if (upper.z == lower.z) upper.z += 0.1f;

	this->_boundingBox.reset( new AABB(lower, upper));
} 

void Triangle::translate(const Vec3 &t) {
	x1 += t;
	x2 += t;
	x0 += t;
	createAABB();
}

void Triangle::scale(const real s) {
	x1 *= s;
	x2 *= s;
	x0 *= s;
	createAABB();
}
