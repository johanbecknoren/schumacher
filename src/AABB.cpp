#include "AABB.h"
#include <vector>

AABB::AABB() {
	this->_lowerLeftBack = glm::vec3(-1.0f);
	this->_upperRightFront = glm::vec3(1.0f);
}

AABB::AABB(const glm::vec3& lLB, const glm::vec3& uRF, const glm::vec3& trash)//,
//          const glm::vec3 o)
{
	this->_lowerLeftBack = lLB;
	this->_upperRightFront = uRF;
	this->_origin = (_lowerLeftBack + _upperRightFront)/2.0f;
}

// Check if point is inside AABB
bool AABB::isInside(const glm::vec3& p) const {
	return (p[0] >= _lowerLeftBack[0] && p[0] < _upperRightFront[0])
			&& (p[1] >= _lowerLeftBack[1] && p[1] < _upperRightFront[1])
			&& (p[2] >= _lowerLeftBack[2] && p[2] < _upperRightFront[2]);
}

int AABB::getQuadrant(const glm::vec3& p) const {
	glm::vec3 diff = p - this->_origin; // vector from origin to point p;

	if(!isInside(p))
		return -1;
	
	if(diff.x < 0.0f) {
		if(diff.y < 0.0f) {
			if(diff.z < 0.0f)
				return 0;
			else
				return 2;
		} else { // y >= 0.0
			if(diff.z < 0.0f)
				return 4;
			else
				return 6;
		}
	} else { // x >= 0.0
		if(diff.y < 0.0f) {
			if(diff.z < 0.0f)
				return 1;
			else
				return 3;
		} else { // y >= 0.0
			if(diff.z < 0.0f)
				return 5;
			else
				return 7;
		}
	}

	return 0;
}

bool AABB::IntersectT(Ray *ray, float *tmin, float *tmax) const {
	float t0 = ray->getTMin();
	float t1 = ray->getTMax();
	for(int i = 0; i < 3; ++i) {
		float invRayDir = 1.0f / ray->getDirection()[i];
		float tNear = (_lowerLeftBack[i] - ray->getOrigin()[i]) * invRayDir;
		float tFar = (_upperRightFront[i] - ray->getOrigin()[i]) * invRayDir;
		if (tNear > tFar) std::swap(tNear, tFar);
		
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;
		if (t0 > t1) return false;
	}
	if (tmin) *tmin = t0;
	if (tmax) *tmax = t1;

	return true;
}

IntersectionPoint* AABB::getIntersection(Ray* ray, bool getIntersectionNormal)  const {
	/* Algorithm from http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms */
	glm::vec3 direction = glm::normalize(ray->getDirection());
	glm::vec3 dirfrac;
	float t;

	dirfrac.x = 1.0f / direction.x;
	dirfrac.y = 1.0f / direction.y;
	dirfrac.z = 1.0f / direction.z;

	float t1 = (_lowerLeftBack.x - ray->getOrigin().x)*dirfrac.x;
	float t2 = (_upperRightFront.x - ray->getOrigin().x)*dirfrac.x;
	float t3 = (_lowerLeftBack.y - ray->getOrigin().y)*dirfrac.y;
	float t4 = (_upperRightFront.y - ray->getOrigin().y)*dirfrac.y;
	float t5 = (_lowerLeftBack.z - ray->getOrigin().z)*dirfrac.z;
	float t6 = (_upperRightFront.z - ray->getOrigin().z)*dirfrac.z;

	float tmin = glm::max( glm::max( glm::min(t1,t2), glm::min(t3,t4)), glm::min(t5,t6) );
	float tmax = glm::min( glm::min( glm::max(t1,t2), glm::max(t3,t4)), glm::max(t5,t6) );

	if(tmax < 0.0f) { // Ray intersection, but whole AABB is behind us
		t = tmax;
		//std::cout<<"AABB.cpp - Intersection, but AABB behind us\n";
		return NULL;
	}

	if(tmin > tmax) { // No intersection
		t = tmax;
		//std::cout<<"AABB.cpp - No intersection\n";
		return NULL;
	}

	t = tmin; // Store length of ray until intersection in t
	
	glm::vec3 intP = ray->getOrigin() + direction*t; // intersection point
	glm::vec3 surfNormal = glm::vec3(0.0f);
	
	if(getIntersectionNormal) {
	// Räkna ut korrekt normal för kollisionen.
		std::vector<glm::vec3> midpoints; // Store all normals for AABB
		midpoints.push_back( glm::vec3(0.0f, _lowerLeftBack.y, 0.0f) - _origin );
		midpoints.push_back( glm::vec3(0.0f, _upperRightFront.y, 0.0f) - _origin );
		midpoints.push_back( glm::vec3(_lowerLeftBack.x, 0.0f, 0.0f) - _origin );
		midpoints.push_back( glm::vec3(_upperRightFront.x, 0.0f, 0.0f) - _origin );
		midpoints.push_back( glm::vec3(0.0f, 0.0f, _lowerLeftBack.z) - _origin );
		midpoints.push_back( glm::vec3(0.0f, 0.0f, _upperRightFront.z) - _origin );

		for(size_t i=0; i<midpoints.size(); ++i) { // Remove impossible normal results
			if(glm::dot(intP, midpoints.at(i)) <= 0.0f) {
				midpoints.erase(midpoints.begin()+i);
				--i;
			}
		}
		if(midpoints.size() == 1) // Onödig, kommer typ aldrig ske :P
			surfNormal = midpoints.at(0);
		else {	// midpoints borde vara <=3 stor här
			float t_max = FLT_MIN;
			int t_max_index;
			for(size_t i=0; i<midpoints.size(); ++i) {
				float nom = glm::dot(midpoints.at(i), midpoints.at(i));
				float denom = glm::dot(intP, midpoints.at(i));
				float dist = nom/denom;
				if(dist > t_max) {
					t_max = dist;
					t_max_index = i;
				}
			}
			surfNormal = midpoints.at(t_max_index);
		}
	}
	//return t;
	return new IntersectionPoint(intP, surfNormal);
}
void AABB::print() const {
	std::cout << _lowerLeftBack.x << " " 
			<<  _lowerLeftBack.y << " " <<_lowerLeftBack.z << " " 
			<< " " << _upperRightFront.x << " "  << _upperRightFront.y 
			<< " " << _upperRightFront.z << " " << std::endl;
}
