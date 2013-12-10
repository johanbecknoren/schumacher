#include "quadrilateral.h"
#include <sstream>

Quadrilateral::Quadrilateral( const Vec3& v00, const Vec3& v10, const Vec3& v11, const Vec3& v01) {
	_v00 = v00;
	_v01 = v01;
	_v10 = v10;
	_v11 = v11;
	_boundingBox = NULL;
	createAABB();
}

std::string Quadrilateral::asString() const {
	std::stringstream ss;
	ss << "Quad with verts: \n\t"<< glm::to_string(_v00) << " \n\t" << glm::to_string(_v01) 
		<<" \n\t" << glm::to_string(_v10) 
		<<" \n\t" << glm::to_string(_v11);
	return ss.str();
}

void Quadrilateral::scale(const real v) {
	_v00 *= v;
	_v01 *= v;
	_v10 *= v;
	_v11 *= v;
	createAABB();
}

void Quadrilateral::translate(const Vec3& t){
	_v00 += t;
	_v01 += t;
	_v10 += t;
	_v11 += t;
	createAABB();
}

Vec3 Quadrilateral::getRandomPoint(const real &s, const real &t, const real &randomFloat) {
	Vec3 randS = s*_v00 + (1.f-s)*_v10;
	Vec3 randT = t*_v11 + (1.f-t)*_v01;

	return randomFloat*randS + (real(1)-randomFloat)*randT;
}

real Quadrilateral::getArea() {
	return glm::length(_v11 - _v01) * glm::length(_v00 - _v01);
}


bool Quadrilateral::getIntersectionPoint(Ray &ray, IntersectionPoint &ip) const {
	// Algorithm from http://graphics.cs.kuleuven.be/publications/LD05ERQIT/LD05ERQIT_code.cpp

	static const real eps = real(10e-6);
	real t,u,v;
	// Rejects rays that are parallel to Q, and rays that intersect the plane of
	// Q either on the left of the line V00V01 or on the right of the line V00V10.

	Vec3 E_01 = _v10 - _v00;
	Vec3 E_03 = _v01 - _v00;
	Vec3 P = glm::cross(ray.getDirection(), E_03);
	real det = glm::dot(E_01, P);

	if (std::fabs(det) < eps) return false;

	real inv_det = real(1.0) / det;
	Vec3 T = ray.getOrigin() - _v00;
	real alpha = glm::dot(T, P) * inv_det;

	if (alpha < real(0.0)) return false;

	// if (alpha > real(1.0)) return false; // Uncomment if VR is used.
	Vec3 Q = glm::cross(T, E_01);
	real beta = glm::dot(ray.getDirection(), Q) * inv_det;

	if (beta < real(0.0)) return false; 

	// if (beta > real(1.0)) return false; // Uncomment if VR is used.

	if ((alpha + beta) > real(1.0)) {

		// Rejects rays that intersect the plane of Q either on the
		// left of the line V11V10 or on the right of the line V11V01.

		Vec3 E_23 = _v01 - _v11;
		Vec3 E_21 = _v10 - _v11;
		Vec3 P_prime = glm::cross(ray.getDirection(), E_21);
		real det_prime = glm::dot(E_23, P_prime);

		if (std::fabs(det_prime) < eps) return false;

		real inv_det_prime = real(1.0) / det_prime;
		Vec3 T_prime = ray.getOrigin() - _v11;
		real alpha_prime = glm::dot(T_prime, P_prime) * inv_det_prime;

		if (alpha_prime < real(0.0)) return false;

		Vec3 Q_prime = glm::cross(T_prime, E_23);
		real beta_prime = glm::dot(ray.getDirection(), Q_prime) * inv_det_prime;

		if (beta_prime < real(0.0)) return false;
	}

	// Compute the ray parameter of the intersection point, and
	// reject the ray if it does not hit Q.

	t = glm::dot(E_03, Q) * inv_det;
	if (t < real(0.0)) return false; 

	// Compute the barycentric coordinates of the fourth vertex.
	// These do not depend on the ray, and can be precomputed
	// and stored with the quadrilateral.  

	real alpha_11, beta_11;
	Vec3 E_02 = _v11 - _v00;
	Vec3 n = glm::cross(E_01, E_03);

	if ((std::abs(n.x) >= std::abs(n.y))
	  && (std::abs(n.x) >= std::abs(n.z))) {

		alpha_11 = ((E_02.y * E_03.z) - (E_02.z * E_03.y)) / n.x;
		beta_11  = ((E_01.y * E_02.z) - (E_01.z * E_02.y)) / n.x;
	} else if ((std::abs(n.y) >= std::abs(n.x))
		&& (std::abs(n.y) >= std::abs(n.z))) {  

		alpha_11 = ((E_02.z * E_03.x) - (E_02.x * E_03.z)) / n.y;
		beta_11  = ((E_01.z * E_02.x) - (E_01.x * E_02.z)) / n.y;
	} else {
		alpha_11 = ((E_02.x * E_03.y) - (E_02.y * E_03.x)) / n.z;
		beta_11  = ((E_01.x * E_02.y) - (E_01.y * E_02.x)) / n.z;
	}

	// Compute the bilinear coordinates of the intersection point.

	if (std::abs(alpha_11 - real(1.0)) < eps) {    
		// Q is a trapezium.
		u = alpha;
		if (std::abs(beta_11 - real(1.0)) < eps) v = beta; // Q is a parallelogram.
		else v = beta / ((u * (beta_11 - real(1.0))) + real(1.0)); // Q is a trapezium.
	} else if (std::abs(beta_11 - real(1.0)) < eps) {
		// Q is a trapezium.
		v = beta;
		u = alpha / ((v * (alpha_11 - real(1.0))) + real(1.0));
	} else {
		real A = real(1.0) - beta_11;
		real B = (alpha * (beta_11 - real(1.0)))
			- (beta * (alpha_11 - real(1.0))) - real(1.0);
		real C = alpha;
		real D = (B * B) - (real(4.0) * A * C);
		real Q = real(-0.5) * (B + ((B < real(0.0) ? real(-1.0) : real(1.0))
			* std::sqrt(D)));
		u = Q / A;
		if ((u < real(0.0)) || (u > real(1.0))) u = C / Q;
		v = beta / ((u * (beta_11 - real(1.0))) + real(1.0)); 
	}

	Vec3 iPoint = ray.getOrigin() + t * ray.getDirection();// - 0.0001f*ray.getDirection();
	Vec3 sN = glm::normalize( glm::cross(_v00-_v10, _v00-_v11) );

/*	if(glm::dot(ray.getDirection(), sN) > 0.0f) // Draw the quad from both directions
		sN *= -1.0f;*/
	ip = IntersectionPoint(iPoint, sN, this->getMaterial());

	return true;
}
void Quadrilateral::createAABB(){
	real eps = real(1e06);
	Vec3 lLB(
		glm::min( glm::min(_v00.x,_v01.x), glm::min(_v10.x,_v11.x) ),
		glm::min( glm::min(_v00.y,_v01.y), glm::min(_v10.y,_v11.y) ),
		glm::min( glm::min(_v00.z,_v01.z), glm::min(_v10.z,_v11.z) ));
	Vec3 uRF(
		glm::max( glm::max(_v00.x,_v01.x), glm::max(_v10.x,_v11.x) ),
		glm::max( glm::max(_v00.y,_v01.y), glm::max(_v10.y,_v11.y) ),
		glm::max( glm::max(_v00.z,_v01.z), glm::max(_v10.z,_v11.z) ));

	if ( (lLB.x - uRF.x) < eps){
		uRF.x += 0.0001f;
		lLB.x -= 0.0001f;
	}
	if ( (lLB.y - uRF.y) < eps) {
		uRF.y += 0.0001f;
		lLB.x -= 0.0001f;
	}
	if ( (lLB.z - uRF.z) < eps) {
		uRF.z += 0.0001f;
		lLB.x -= 0.0001f;
	}

	if((_boundingBox == NULL))
		this->_boundingBox.reset( new AABB(lLB, uRF));
	else {
		_boundingBox->setLowerLeftBack(lLB);
		_boundingBox->setUpperRightFront(uRF);
	}
}
