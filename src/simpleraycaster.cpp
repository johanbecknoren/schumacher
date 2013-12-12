#include "simpleraycaster.h"
#include "progressbar.h"
#include "timer.h"

void SimpleRaycaster::render(float* pixels, Octree *tree, Camera *cam){
	// Skicka en stråle per pixel, hämta normal samt material
	// (främst diffuse color) från IntersectionPoint.

	int total = _W * _H;
	int rayCounter = 0;

	// (u,v) are pixel coords for output image
	Timer::getInstance()->start("Simple");
	for(int u=0; u<_W; ++u) {
		for(int v=0; v<_H; ++v) {
			rayCounter++;

			real x;
			real y;			
			calculateXnY(u, v, x, y);
			// TODO: Lägg detta i en funktion som returnerar intensitet (floats) och som
			// kan anropas rekursivt. Spawna nya strålar (reflekterade och brutna) till
			// en iterations-tröskel nås
			//Ray *r = new Ray( glm::vec3(0.0f), glm::vec3(x,y,1.0f)-glm::vec3(0.0f) );
			Ray r = cam->createRay(x, y);
// 			Ray *r = new Ray( cam->getPosition(), glm::vec3(x,y,-cam->getDirection().z)-glm::vec3(0.0f) );
			IntersectionPoint ip;

			if (tree->intersect(r, ip)) {
				float intensity = glm::dot(r.getDirection(), - ip.getNormal());
				int id = calculateId(u, v);
				pixels[id + 0] = intensity*ip.getMaterial()->getDiffuseColor().x;
				pixels[id + 1] = intensity*ip.getMaterial()->getDiffuseColor().y;
				pixels[id + 2] = intensity*ip.getMaterial()->getDiffuseColor().z;
			}
			ProgressBar::printProgBar(rayCounter, total);	
		}
	}
	Timer::getInstance()->stop("Simple");
	std::cout << std::endl;
	Timer::getInstance()->printRealTime("Simple");
	std::cout << "Num pixels: "<<_W*_H<<", num rays: "<<rayCounter<<"\n";
}
