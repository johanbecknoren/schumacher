#include "AABB.h"
#include <vector>

AABB::AABB() {
	this->_lowerLeftBack = Vec3(real(-1));
	this->_upperRightFront = Vec3(real(1));
}

AABB::AABB(const Vec3& lLB, const Vec3& uRF, const Vec3& trash) :	_lowerLeftBack(lLB),
																	_upperRightFront(uRF)
{
	this->_origin = (_lowerLeftBack + _upperRightFront)/real(2);
}

// Check if point is inside AABB
bool AABB::isInside(const Vec3& p) const {
	return (p[0] >= _lowerLeftBack[0] && p[0] < _upperRightFront[0])
			&& (p[1] >= _lowerLeftBack[1] && p[1] < _upperRightFront[1])
			&& (p[2] >= _lowerLeftBack[2] && p[2] < _upperRightFront[2]);
}

int AABB::getQuadrant(const Vec3& p) const {
	Vec3 diff = p - this->_origin; // vector from origin to point p;

	if(!isInside(p))
		return -1;
	
	if(diff.x < real(0)) {
		if(diff.y < real(0)) {
			if(diff.z < real(0))
				return 0;
			else
				return 2;
		} else { // y >= 0.0
			if(diff.z < real(0))
				return 4;
			else
				return 6;
		}
	} else { // x >= 0.0
		if(diff.y < real(0)) {
			if(diff.z < real(0))
				return 1;
			else
				return 3;
		} else { // y >= 0.0
			if(diff.z < real(0))
				return 5;
			else
				return 7;
		}
	}

	return 0;
}

bool AABB::IntersectT(Ray *ray, real *tmin, real *tmax) const {
	real t0 = ray->getTMin();
	real t1 = ray->getTMax();
	for(int i = 0; i < 3; ++i) {
		real invRayDir = real(1) / ray->getDirection()[i];
		real tNear = (_lowerLeftBack[i] - ray->getOrigin()[i]) * invRayDir;
		real tFar = (_upperRightFront[i] - ray->getOrigin()[i]) * invRayDir;
		if (tNear > tFar) std::swap(tNear, tFar);
		
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;
		if (t0 > t1) return false;
	}
	if (tmin) *tmin = t0;
	if (tmax) *tmax = t1;

	return true;
}


bool AABB::getIntersection(Ray& ray, IntersectionPoint &ip, bool getIntersectionNormal)  const {
	/* Algorithm from http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms */
	Vec3 direction = glm::normalize(ray.getDirection());
	Vec3 dirfrac;
	real t;

	dirfrac.x = real(1) / direction.x;
	dirfrac.y = real(1) / direction.y;
	dirfrac.z = real(1) / direction.z;

	real t1 = (_lowerLeftBack.x - ray.getOrigin().x)*dirfrac.x;
	real t2 = (_upperRightFront.x - ray.getOrigin().x)*dirfrac.x;
	real t3 = (_lowerLeftBack.y - ray.getOrigin().y)*dirfrac.y;
	real t4 = (_upperRightFront.y - ray.getOrigin().y)*dirfrac.y;
	real t5 = (_lowerLeftBack.z - ray.getOrigin().z)*dirfrac.z;
	real t6 = (_upperRightFront.z - ray.getOrigin().z)*dirfrac.z;

	real tmin = glm::max( glm::max( glm::min(t1,t2), glm::min(t3,t4)), glm::min(t5,t6) );
	real tmax = glm::min( glm::min( glm::max(t1,t2), glm::max(t3,t4)), glm::max(t5,t6) );

	if(tmax < real(0)) { // Ray intersection, but whole AABB is behind us
		t = tmax;
		//std::cout<<"AABB.cpp - Intersection, but AABB behind us\n";
		return false;
	}

	if(tmin > tmax) { // No intersection
		t = tmax;
		//std::cout<<"AABB.cpp - No intersection\n";
		return false;
	}

	t = tmin; // Store length of ray until intersection in t
	
	Vec3 intP = ray.getOrigin() + direction*t; // intersection point
	Vec3 surfNormal = Vec3(real(0));
	
	if(getIntersectionNormal) {
	// Räkna ut korrekt normal för kollisionen.
		std::vector<Vec3> midpoints; // Store all normals for AABB
		midpoints.push_back( Vec3(real(0), _lowerLeftBack.y, real(0)) - _origin );
		midpoints.push_back( Vec3(real(0), _upperRightFront.y, real(0)) - _origin );
		midpoints.push_back( Vec3(_lowerLeftBack.x, real(0), real(0)) - _origin );
		midpoints.push_back( Vec3(_upperRightFront.x, real(0), real(0)) - _origin );
		midpoints.push_back( Vec3(real(0),real(0), _lowerLeftBack.z) - _origin );
		midpoints.push_back( Vec3(real(0),real(0), _upperRightFront.z) - _origin );

		for(size_t i=0; i<midpoints.size(); ++i) { // Remove impossible normal results
			if(glm::dot(intP, midpoints.at(i)) <= 0.0f) {
				midpoints.erase(midpoints.begin()+i);
				--i;
			}
		}
		if(midpoints.size() == 1) // Onödig, kommer typ aldrig ske :P
			surfNormal = midpoints.at(0);
		else {	// midpoints borde vara <=3 stor här
			float t_max = real(FLT_MIN);
			int t_max_index = 0;
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
    ip = IntersectionPoint(intP, surfNormal);
	return true;
}
void AABB::print() const {
	std::cout << _lowerLeftBack.x << " " 
			<<  _lowerLeftBack.y << " " <<_lowerLeftBack.z << " " 
			<< " " << _upperRightFront.x << " "  << _upperRightFront.y 
			<< " " << _upperRightFront.z << " " << std::endl;
}
