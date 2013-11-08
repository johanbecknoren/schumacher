#include "quadrilateral.h"
#include <sstream>

Quadrilateral::Quadrilateral( const glm::vec3& v00, const glm::vec3& v10, const glm::vec3& v11, const glm::vec3& v01) {
	_v00 = v00;
	_v01 = v01;
	_v10 = v10;
	_v11 = v11;
	createAABB();
	_name = "Quadrilateral";
}

std::string Quadrilateral::asString() const {
	std::stringstream ss;
	ss << "Quad with verts: \n\t"<< glm::to_string(_v00) << " \n\t" << glm::to_string(_v01) 
		<<" \n\t" << glm::to_string(_v10) 
		<<" \n\t" << glm::to_string(_v11);
	return ss.str();
}

void Quadrilateral::scale(const float v) {
	_v00 *= v;
	_v01 *= v;
	_v10 *= v;
	_v11 *= v;
//	createAABB();
}

void Quadrilateral::translate(const glm::vec3& t){
	_v00 += t;
	_v01 += t;
	_v10 += t;
	_v11 += t;
//	createAABB();
}

//typedef float real;
//typedef glm::vec3 vector;

bool Quadrilateral::getIntersectionPoint(Ray &ray, IntersectionPoint &ip) const {
	// Algorithm from http://graphics.cs.kuleuven.be/publications/LD05ERQIT/LD05ERQIT_code.cpp

	static const float eps = float(10e-6);
	float t,u,v;
	// Rejects rays that are parallel to Q, and rays that intersect the plane of
	// Q either on the left of the line V00V01 or on the right of the line V00V10.

	glm::vec3 E_01 = _v10 - _v00;
	glm::vec3 E_03 = _v01 - _v00;
	glm::vec3 P = glm::cross(ray.getDirection(), E_03);
	float det = glm::dot(E_01, P);

	if (std::fabs(det) < eps) return false;

	float inv_det = float(1.0) / det;
	glm::vec3 T = ray.getOrigin() - _v00;
	float alpha = glm::dot(T, P) * inv_det;

	if (alpha < float(0.0)) return false;

	// if (alpha > real(1.0)) return false; // Uncomment if VR is used.
	glm::vec3 Q = glm::cross(T, E_01);
	float beta = glm::dot(ray.getDirection(), Q) * inv_det;

	if (beta < float(0.0)) return false; 

	// if (beta > real(1.0)) return false; // Uncomment if VR is used.

	if ((alpha + beta) > float(1.0)) {

		// Rejects rays that intersect the plane of Q either on the
		// left of the line V11V10 or on the right of the line V11V01.

		glm::vec3 E_23 = _v01 - _v11;
		glm::vec3 E_21 = _v10 - _v11;
		glm::vec3 P_prime = glm::cross(ray.getDirection(), E_21);
		float det_prime = glm::dot(E_23, P_prime);

		if (std::fabs(det_prime) < eps) return false;

		float inv_det_prime = float(1.0) / det_prime;
		glm::vec3 T_prime = ray.getOrigin() - _v11;
		float alpha_prime = glm::dot(T_prime, P_prime) * inv_det_prime;

		if (alpha_prime < float(0.0)) return false;

		glm::vec3 Q_prime = glm::cross(T_prime, E_23);
		float beta_prime = glm::dot(ray.getDirection(), Q_prime) * inv_det_prime;

		if (beta_prime < float(0.0)) return false;
	}

	// Compute the ray parameter of the intersection point, and
	// reject the ray if it does not hit Q.

	t = glm::dot(E_03, Q) * inv_det;
	if (t < float(0.0)) return false; 

	// Compute the barycentric coordinates of the fourth vertex.
	// These do not depend on the ray, and can be precomputed
	// and stored with the quadrilateral.  

	float alpha_11, beta_11;
	glm::vec3 E_02 = _v11 - _v00;
	glm::vec3 n = glm::cross(E_01, E_03);

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

	if (std::abs(alpha_11 - float(1.0)) < eps) {    
		// Q is a trapezium.
		u = alpha;
		if (std::abs(beta_11 - float(1.0)) < eps) v = beta; // Q is a parallelogram.
		else v = beta / ((u * (beta_11 - float(1.0))) + float(1.0)); // Q is a trapezium.
	} else if (std::abs(beta_11 - float(1.0)) < eps) {
		// Q is a trapezium.
		v = beta;
		u = alpha / ((v * (alpha_11 - float(1.0))) + float(1.0));
	} else {
		float A = float(1.0) - beta_11;
		float B = (alpha * (beta_11 - float(1.0)))
			- (beta * (alpha_11 - float(1.0))) - float(1.0);
		float C = alpha;
		float D = (B * B) - (float(4.0) * A * C);
		float Q = float(-0.5) * (B + ((B < float(0.0) ? float(-1.0) : float(1.0))
			* std::sqrt(D)));
		u = Q / A;
		if ((u < float(0.0)) || (u > float(1.0))) u = C / Q;
		v = beta / ((u * (beta_11 - float(1.0))) + float(1.0)); 
	}

	glm::vec3 iPoint = ray.getOrigin() + t * ray.getDirection();
	glm::vec3 sN = glm::normalize( glm::cross(_v00-_v10, _v00-_v11) );

/*	if(glm::dot(ray.getDirection(), sN) > 0.0f) // Draw the quad from both directions
		sN *= -1.0f;*/
	ip = IntersectionPoint(iPoint, sN, this->getMaterial());

	return true;
}
void Quadrilateral::createAABB(){
	float eps = float(1e06);
	glm::vec3 lLB(
		glm::min( glm::min(_v00.x,_v01.x), glm::min(_v10.x,_v11.x) ),
		glm::min( glm::min(_v00.y,_v01.y), glm::min(_v10.y,_v11.y) ),
		glm::min( glm::min(_v00.z,_v01.z), glm::min(_v10.z,_v11.z) ));
	glm::vec3 uRF(
		glm::max( glm::max(_v00.x,_v01.x), glm::max(_v10.x,_v11.x) ),
		glm::max( glm::max(_v00.y,_v01.y), glm::max(_v10.y,_v11.y) ),
		glm::max( glm::max(_v00.z,_v01.z), glm::max(_v10.z,_v11.z) ));

	if ( (lLB.x - uRF.x) < eps)
		uRF.x += 0.0001f;
	if ( (lLB.y - uRF.y) < eps)
		uRF.y += 0.0001f;
	if ( (lLB.z - uRF.z) < eps)
		uRF.z += 0.0001f;

	if(!(_boundingBox = NULL))
		this->_boundingBox = new AABB(lLB, uRF);
	else {
		_boundingBox->setLowerLeftBack(lLB);
		_boundingBox->setUpperRightFront(uRF);
	}
}
