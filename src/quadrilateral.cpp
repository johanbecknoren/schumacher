#include "quadrilateral.h"
#include <sstream>

Quadrilateral::Quadrilateral( const glm::vec3& v00, const glm::vec3& v01, const glm::vec3& v10, const glm::vec3& v11) {
	_v00 = v00;
	_v01 = v01;
	_v10 = v10;
	_v11 = v11;
	createAABB();
	_name = "Quadrilateral";
}

std::string Quadrilateral::asString() const {
	std::stringstream ss;
	ss << "Quad with verts: "<< glm::to_string(_v00);
	return ss.str();
}

void Quadrilateral::scale(float v) {
	_v00 *= v;
	_v01 *= v;
	_v10 *= v;
	_v11 *= v;
}

void Quadrilateral::translate(const glm::vec3& t){
	_v00 += t;
	_v01 += t;
	_v10 += t;
	_v11 += t;
}

IntersectionPoint* Quadrilateral::getIntersectionPoint(Ray *ray) const {
	//std::cout << "Quad: intersection\n";
	// Intersection algorithm from http://graphics.cs.kuleuven.be/publications/LD05ERQIT/LD05ERQIT_code.cpp
	float epsilon = 0.0000001,u,v,t;
	glm::vec3 E_01 = _v10 - _v00;
	glm::vec3 E_03 = _v01 - _v00;
	glm::vec3 P = glm::cross(ray->getDirection(), E_03);
	float det = glm::dot(E_01, P);

	if(fabs(det) < epsilon)
		return NULL;

	float inv_det = 1.0f/det;
	glm::vec3 T = ray->getOrigin() -_v00;
	float alpha = glm::dot(T,P) * inv_det;
	
	if(alpha < 0.0f)
		return NULL;

	glm::vec3 Q = glm::cross(T, E_01);
	float beta = glm::dot(ray->getDirection(), Q) * inv_det;
	
	if(beta < 0.0f)
		return NULL;

	if( alpha+beta > 1.0f ) {
		// Rejects rays that intersect the plane of Q either on the
	    // left of the line V11V10 or on the right of the line V11V01.

		glm::vec3 E_23 = _v01 - _v11;
		glm::vec3 E_21 = _v10 - _v11;
		glm::vec3 P_prime = glm::cross(ray->getDirection(), E_21);
		float det_prime = glm::dot(E_23, P_prime);

		if(fabs(det_prime) < epsilon)
			return NULL;

		float inv_det_prime = 1.0f/det_prime;
		glm::vec3 T_prime = ray->getOrigin() - _v11;
		float alpha_prime = glm::dot(T_prime, P_prime) * inv_det_prime;
		
		if(alpha_prime < 0.0f)
			return NULL;

		glm::vec3 Q_prime = glm::cross(T_prime, E_23);
		float beta_prime = glm::dot(ray->getDirection(), Q_prime) * inv_det_prime;
		
		if(beta_prime < 0.0f)
			return NULL;
	}

	// Compute the ray parameter of the intersection point, and
	// reject the ray if it does not hit Q.

	t = glm::dot(E_03, Q);
	
	if(t < 0.0f)
		return NULL;

	// Compute the barycentric coordinates of the fourth vertex.
	// These do not depend on the ray, and can be precomputed
	// and stored with the quadrilateral.  

	float alpha_11, beta_11;
	glm::vec3 E_02 = _v11 - _v00;
	glm::vec3 n = glm::cross(E_01, E_03);

	if( fabs(n.x) >= fabs(n.y) && fabs(n.x) >= fabs(n.z) ) {
		alpha_11 = ( (E_02.y * E_03.z) - (E_02.z * E_03.y) ) / n.x;
		beta_11 = ( (E_01.y * E_02.z) - (E_01.z * E_02.y) ) / n.x;
	} else if( fabs(n.y) >= fabs(n.x) && fabs(n.y) >= fabs(n.z) ) {
		alpha_11 = ( (E_02.z * E_03.x) - (E_02.x * E_03.z) ) / n.y;
		beta_11 = ( (E_01.y * E_02.z) - (E_01.z * E_02.y) ) / n.y;
	} else {
		alpha_11 = ( (E_02.x * E_03.y) - (E_02.y * E_03.x) ) / n.z;
		beta_11 = ( (E_01.x * E_02.y) - (E_01.y * E_02.x) ) / n.z;
	}

	// Compute the bilinear coordinates of the intersection point.

	if(fabs(alpha_11 - 1.0f) < epsilon) {
		// Q is a trapezium
		u = alpha_11;
		if( fabs(beta_11-1.0f) < epsilon )
			v = beta; // Q is a parallelogram
		else
			v = beta / ( (u * (beta_11 - 1.0f)) + 1.0f); // Q is a trapezium
	} else if(fabs(beta_11 - 1.0f) < epsilon) {
		// Q is a trapezium
		v = beta;
		u = alpha / ((v * (alpha_11 - 1.0f)) + 1.0f);
	} else {
		float A = 1.0f - beta_11;
		float B = (alpha * (beta_11 - 1.0f)) - (beta * (alpha_11 - 1.0f)) - 1.0f;
		float C = alpha;
		float D = (B * B) - (4.0f * A * C);
		float Q = -0.5f * (B + ((B < 0.0f ? -1.0f : 1.0f) * sqrt(D)));

		u = Q / A;

    	if ((u < 0.0f) || (u > 1.0f))
			u = C / Q;

	    v = beta / ((u * (beta_11 - 1.0f)) + 1.0f);
	}

	//std::cout << "Done\n";
	glm::vec3 intP = ray->getOrigin() + glm::normalize(ray->getDirection())*t;
	glm::vec3 surfNormal = glm::normalize( glm::cross(_v00-_v01, _v00-_v10) );

	//std::cout<<"surfNormal = "<<glm::to_string(surfNormal)<<std::endl;

	return new IntersectionPoint(intP,-1.0f*surfNormal,getMaterial());
	//return NULL;
}

void Quadrilateral::createAABB(){
	glm::vec3 lLB(
		glm::min( glm::min(_v00.x,_v01.x), glm::min(_v10.x,_v11.x) ),
		glm::min( glm::min(_v00.y,_v01.y), glm::min(_v10.y,_v11.y) ),
		glm::min( glm::min(_v00.z,_v01.z), glm::min(_v10.z,_v11.z) ));
	glm::vec3 uRF(
		glm::max( glm::max(_v00.x,_v01.x), glm::max(_v10.x,_v11.x) ),
		glm::max( glm::max(_v00.y,_v01.y), glm::max(_v10.y,_v11.y) ),
		glm::max( glm::max(_v00.z,_v01.z), glm::max(_v10.z,_v11.z) ));

	this->_boundingBox = new AABB(lLB, uRF);
}
