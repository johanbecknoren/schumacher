#include <iostream>
#include "octree.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "lightsource.h"

int main() {
	IntersectionPoint *ip = new IntersectionPoint();
	Ray *r = new Ray(glm::vec3(-15.f,-15.f,-15.f), glm::vec3(1.0,0.7f,0.8f));
	glm::vec3 point(-9.5f,9.5f,9.5f);


	std::cout << "Creating AABB!\n";
	AABB bb(glm::vec3(-10.0f), glm::vec3(10.0f));

	std::cout<<"main.cpp - getQuadrant, AABB: "<<bb.getQuadrant(point)<<"\n";
	Octree tree(&bb);
	Sphere *sphere = new Sphere(1.0f, glm::vec3(0.0f));

	/*ip = sphere->getIntersectionPoint(r);
	
	if(ip!=NULL) {
	std::cout << "\nmain.cpp - Ray-Sphere intersection. Point = "
		<<ip->getPoint().x<<','<<ip->getPoint().y<<','<<ip->getPoint().z
		<< ", normal = "<<ip->getNormal().x<<','<<ip->getNormal().y<<','
		<<ip->getNormal().z<<std::endl;
	} else
		std::cout << "main.cpp - No intersection!\n";*/

	ip = bb.getIntersection(r,true);
	if(ip!=NULL) {
	std::cout << "\nmain.cpp - Ray-AABB intersection. Point = "
		<<ip->getPoint().x<<','<<ip->getPoint().y<<','<<ip->getPoint().z
		<< ", normal = "<<ip->getNormal().x<<','<<ip->getNormal().y<<','
		<<ip->getNormal().z<<std::endl;
	} else
		std::cout << "main.cpp - No intersection!\n";

	tree.addObject(sphere);
	return 0;
}
