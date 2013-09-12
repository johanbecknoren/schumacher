#include "AABB.h"

AABB::AABB() {
	this->_lowerLeftBack = glm::vec3(-1.0f);
	this->_upperRightFront = glm::vec3(1.0f);
}

AABB::AABB(const glm::vec3& lLB, const glm::vec3& uRF, const glm::vec3& o = glm::vec3(0.0f))
{
	this->_lowerLeftBack = lLB;
	this->_upperRightFront = uRF;
	this->_origin = o;
}

// Check if point is inside AABB
bool AABB::isInside(const glm::vec3& p) {
	return (p[0] > _lowerLeftBack[0] && p[0] < _upperRightFront[0])
			&& (p[1] > _lowerLeftBack[1] && p[1] < _upperRightFront[1])
			&& (p[2] > _lowerLeftBack[2] && p[2] < _upperRightFront[2]);
}

float AABB::getIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) {
	/* Algorithm from http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms */
	glm::vec3 direction = glm::normalize(rayDirection);
	glm::vec3 dirfrac;
	float t;
	
	dirfrac.x = 1.0f / rayDirection.x;
	dirfrac.y = 1.0f / rayDirection.y;
	dirfrac.z = 1.0f / rayDirection.z;

	float t1 = (_lowerLeftBack.x - rayOrigin.x)*dirfrac.x;
	float t2 = (_upperRightFront.x - rayOrigin.x)*dirfrac.x;
	float t3 = (_lowerLeftBack.y - rayOrigin.y)*dirfrac.y;
	float t4 = (_upperRightFront.y - rayOrigin.y)*dirfrac.y;
	float t5 = (_lowerLeftBack.z - rayOrigin.z)*dirfrac.z;
	float t6 = (_upperRightFront.z - rayOrigin.z)*dirfrac.z;

	float tmin = glm::max( glm::max( glm::min(t1,t2), glm::min(t3,t4)), glm::min(t5,t6) );
	float tmax = glm::min( glm::min( glm::max(t1,t2), glm::max(t3,t4)), glm::max(t5,t6) );

	if(tmax < 0.0f) { // Ray intersection, but whole AABB is behind us
		t = tmax;
		return -1.0f;
	}

	if(tmin > tmax) { // No intersection
		t = tmax;
		return -1.0f;
	}

	t = tmin; // Store length of ray until intersection in t

	return t;
}
