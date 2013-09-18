#include "simpleraycaster.h"
#include "progressbar.h"

void SimpleRaycaster::render(float* pixels, Octree *tree, const int W, const int H, Camera *cam){
	// Skicka en stråle per pixel, hämta normal samt material
	// (främst diffuse color) från IntersectionPoint.
	
	// Godtycklig FOV (http://www.unknownroad.com/rtfm/graphics/rt_eyerays.html)
	float fovx = float(M_PI)/4.0f; // 45 grader
	float fovy = (float(H)/float(W)) * fovx;

	int total = W * H;
	int rayCounter = 0;

	// (u,v) are pixel coords for output image
	for(int u=0; u<W; ++u) {
		for(int v=0; v<H; ++v) {
			rayCounter++;
			float x = ( (2.0f*float(u)-float(W))/float(W) ) * tan(fovx);
			float y = ( (2.0f*float(v)-float(H))/float(H) ) * tan(fovy);
			
			// TODO: Lägg detta i en funktion som returnerar intensitet (floats) och som
			// kan anropas rekursivt. Spawna nya strålar (reflekterade och brutna) till
			// en iterations-tröskel nås
			//Ray *r = new Ray( glm::vec3(0.0f), glm::vec3(x,y,1.0f)-glm::vec3(0.0f) );
			Ray *r = new Ray( cam->getPosition(), glm::vec3(x,y,-cam->getDirection().z)-glm::vec3(0.0f) );

			IntersectionPoint* ip;
			ip = tree->findIntersection(r);
			
			if(ip != NULL )	{
				float intensity = glm::dot(r->getDirection(), - ip->getNormal());
				pixels[u*3 + W*(H-v)*3 + 0] = intensity*ip->getMaterial().getDiffuseColor().x;
				pixels[u*3 + W*(H-v)*3 + 1] = intensity*ip->getMaterial().getDiffuseColor().y;
				pixels[u*3 + W*(H-v)*3 + 2] = intensity*ip->getMaterial().getDiffuseColor().z;
			}
			ProgressBar::printProgBar(int(100.0f * float(rayCounter) / float(total)));	
		}
	}
	std::cout << std::endl;
	std::cout << "Num pixels: "<<W*H<<", num rays: "<<rayCounter<<"\n";
}
