#include "raytracer.h"
#include "progressbar.h"
#include "timer.h"

// Returnera intensitet
float traverseRay(Ray* rayIncoming, IntersectionPoint *ip, Octree* tree, int iterations) {
//	std::cout<<"Calc reflected ray\n";
	glm::vec3 I = glm::normalize(rayIncoming->getDirection());
	glm::vec3 N = glm::normalize(ip->getNormal());
	//glm::vec3 reflDir = rayIncoming->getDirection() - 2 * (glm::dot( glm::normalize(ip->getNormal()), rayIncoming->getDirection() )) * glm::normalize(ip->getNormal());
//	std::cout << "Done.\n";

	glm::vec3 reflDir = I - 2 * glm::dot(N,I) * N;
	
	Ray* reflected = new Ray(ip->getPoint(), reflDir);

//	std::cout<<"Sending ray into octree\n";
	tree->intersect(*reflected, *ip);
	float intensity = 0;
	if(ip != NULL && iterations > 0) {
		intensity = glm::dot(reflected->getDirection(), - ip->getNormal());
//		std::cout << "Recursive call...\n";
		intensity += traverseRay(reflected, ip, tree, --iterations);
	}

	return intensity;
}

void Raytracer::render(float* pixels, Octree *tree, Camera *cam, int &iterations) {

	std::cout << "Populating *pixels...\n";
	// (u,v) are pixel coords for output image

	int pixCounter = 0;
	int total = _W * _H;

	for(int u = 0; u < _W; ++u) {
		Timer::getInstance()->start("Thread", u);
		for(int v = 0; v < _H; ++v) {
			
			float x = ( (2.0f*float(u)-float(_W))/float(_W) ) * _tanfovx;
			float y = ( (2.0f*float(v)-float(_H))/float(_H) ) * _tanfovy;

			Ray *r = new Ray( cam->getPosition(), glm::vec3(x,y,-cam->getDirection().z)-glm::vec3(0.0f));
			IntersectionPoint* ip = nullptr;
			
			if(tree->intersect(*r, *ip)) {
				Material firstMat = ip->getMaterial();
//				std::cout<<"Found intersection\n";
				float intensity = traverseRay(r, ip, tree, iterations);
// 				std::cout << "intensity = " << intensity<<std::endl;
				int id = calculateId(u, v);
				pixels[id + 0] = intensity * firstMat.getDiffuseColor().x;
				pixels[id + 1] = intensity * firstMat.getDiffuseColor().y;
				pixels[id + 2] = intensity * firstMat.getDiffuseColor().z;

			}
			++pixCounter;
			
			ProgressBar::printProgBar(pixCounter, total);	
		
		}
		Timer::getInstance()->stop("Thread", u);
	}
	Timer::getInstance()->printThreadTime("Thread");	
	std::cout << "Done!\n";
}
