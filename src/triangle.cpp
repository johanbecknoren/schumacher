#include "triangle.h"

Triangle::Triangle(glm::vec3 ix0, glm::vec3 ix1, glm::vec3 ix2) : x1(ix1), x2(ix2), x0(ix0) {
	normal = -glm::cross(x1 - x0, x2 - x0);

	createAABB();
}

bool Triangle::getIntersectionPoint(Ray &ray, IntersectionPoint &ip) const {
    glm::vec3 edge1 = x1 - x0;
    glm::vec3 edge2 = x2 - x0;
    glm::vec3 pvec = glm::cross(ray.getDirection(), edge2);
    float det = glm::dot(edge1, pvec);
    if (det == 0.f) return false;
    float invDet = 1.0f / det;
    glm::vec3 tvec = ray.getOrigin() - x0;
	float U = glm::dot(tvec, pvec) * invDet;
    if (U < 0.f || U > 1.f) return false;
    glm::vec3 qvec = glm::cross(tvec, edge1);
    float V = glm::dot(ray.getDirection(), qvec) * invDet;
    if (V < 0.f || U + V > 1.f) return false;
    float T = glm::dot(edge2, qvec) * invDet;
	if (T < 0.f) return false;
	glm::vec3 newPos = ray.getOrigin() + T * ray.getDirection();
	ip = IntersectionPoint(newPos, normal, getMaterial()); 
	
	return true;
}


void Triangle::createAABB() {
	glm::vec3 lower(glm::min(glm::min(x1.x, x2.x), x0.x),
			glm::min(glm::min(x1.y, x2.y), x0.y),
			glm::min(glm::min(x1.z, x2.z), x0.z));
	glm::vec3 upper(glm::max(glm::max(x1.x, x2.x), x0.x),
			glm::max(glm::max(x1.y, x2.y), x0.y),
			glm::max(glm::max(x1.z, x2.z), x0.z));
	if (upper.x == lower.x) upper.x += 0.1f;
	if (upper.y == lower.y) upper.y += 0.1f;
	if (upper.z == lower.z) upper.z += 0.1f;
	this->_boundingBox = new AABB(lower, upper);
} 

void Triangle::translate(const glm::vec3 &t) {
	x1 + t;
	x2 + t;
	x0 + t;
}

void Triangle::scale(const float s) {
	x1 *= s;
	x2 *= s;
	x0 *= s;
}
