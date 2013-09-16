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
	Ray *r2 = new Ray(glm::vec3(0.0f), glm::vec3(-1.0f));
	std::cout << "Creating AABB!\n";
	AABB bb(glm::vec3(-10.0f), glm::vec3(10.0f));

	std::cout<<"main.cpp - getQuadrant, AABB: "<<bb.getQuadrant(point)<<"\n";
	Octree tree(&bb);
	Sphere *sphere = new Sphere(0.001f, glm::vec3(-0.352f));
	Sphere *sp2 = new Sphere(0.001f, glm::vec3(1.02f, 0.13f, 4.23f));
	std::cout << "main.cpp - " << sphere->asString() << std::endl;
	ip = bb.getIntersection(r,true);
	if(ip!=NULL) {
	std::cout << "\nmain.cpp - Ray-AABB intersection. Point = "
		<<ip->getPoint().x<<','<<ip->getPoint().y<<','<<ip->getPoint().z
		<< ", normal = "<<ip->getNormal().x<<','<<ip->getNormal().y<<','
		<<ip->getNormal().z<<std::endl;
	} else
		std::cout << "main.cpp - No intersection!\n";
	tree.addObject(sphere);
	tree.addObject(sp2);
	std::cout << "main.cpp -  \n";
	tree.print();
	std::cout << " --- \n ";
	tree.findIntersection(r);
	return 0;
}
