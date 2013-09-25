#include "raytracer.h"
// #include "progressbar.h"
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
	tree->intersect(&reflected, &ip);
	float intensity = 0;
	if(ip != NULL && iterations > 0) {
		intensity = glm::dot(reflected->getDirection(), - ip->getNormal());
//		std::cout << "Recursive call...\n";
		intensity += traverseRay(reflected, ip, tree, --iterations);
	}
	
	return intensity;
}

void Raytracer::render(float* pixels, Octree *tree, const int W, const int H, Camera *cam, int &iterations) {

	// Godtycklig FOV (http://www.unknownroad.com/rtfm/graphics/rt_eyerays.html)
	float fovx = float(M_PI)/4.0f; // 45 grader
	float fovy = (float(H)/float(W)) * fovx;
	std::cout << "Populating *pixels...\n";
	// (u,v) are pixel coords for output image

	int pixCounter = 0;
	int total = W*H;

	for(int u=0; u<W; ++u) {
		Timer::getInstance()->start("Thread", u);
		for(int v=0; v<H; ++v) {
			
			float x = ( (2.0f*float(u)-float(W))/float(W) ) * tan(fovx);
			float y = ( (2.0f*float(v)-float(H))/float(H) ) * tan(fovy);

			Ray *r = new Ray( cam->getPosition(), glm::vec3(x,y,-cam->getDirection().z)-glm::vec3(0.0f));
			IntersectionPoint* ip;
			
			if(tree->intersect(r, ip)) {
				Material firstMat = ip->getMaterial();
//				std::cout<<"Found intersection\n";
				float intensity = traverseRay(r, &ip, tree, iterations);
// 				std::cout << "intensity = " << intensity<<std::endl;
				pixels[u*3 + W*(H-v)*3 + 0] = intensity * firstMat.getDiffuseColor().x;
				pixels[u*3 + W*(H-v)*3 + 1] = intensity * firstMat.getDiffuseColor().y;
				pixels[u*3 + W*(H-v)*3 + 2] = intensity * firstMat.getDiffuseColor().z;

			}
			++pixCounter;
			
// 			ProgressBar::printProgBar(pixCounter, total);	
		
		}
		Timer::getInstance()->stop("Thread", u);
	}
	Timer::getInstance()->printThreadTime("Thread");	
	std::cout << "Done!\n";
}
