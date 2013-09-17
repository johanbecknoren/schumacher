#include "simpleraycaster.h"

void SimpleRaycaster::render(float* pixels, Octree *tree, const int W, const int H, Camera *cam){
	// Skicka en stråle per pixel, hämta normal samt material
	// (främst diffuse color) från IntersectionPoint.
	
	// Godtycklig FOV (http://www.unknownroad.com/rtfm/graphics/rt_eyerays.html)
	float fovx = 3.14159f/4.0f; // 45 grader
	float fovy = (float(H)/float(W)) * fovx;

	int counter = 0;
	for(int u=0; u<W; ++u) {
		for(int v=0; v<H; ++v) {
			counter++;
			float x = ( (2.0f*float(u)-float(W))/float(W) ) * tan(fovx);
			float y = ( (2.0f*float(v)-float(H))/float(H) ) * tan(fovy);
			
			// TODO: Lägg detta i en funktion som returnerar intensitet (floats) och som
			// kan anropas rekursivt. Spawna nya strålar (reflekterade och brutna) till
			// en iterations-tröskel nås
			Ray *r = new Ray( glm::vec3(0.0f), glm::vec3(x,y,1.0f)-glm::vec3(0.0f) );
			IntersectionPoint* ip;
			ip = tree->findIntersection(r);
			
			if(ip != NULL )	{
				float intensity = glm::dot(r->getDirection(), - ip->getNormal());
				pixels[u*3 + W*(H-v)*3 + 0] = intensity*ip->getMaterial().getDiffuseColor().x;
				pixels[u*3 + W*(H-v)*3 + 1] = intensity*ip->getMaterial().getDiffuseColor().y;
				pixels[u*3 + W*(H-v)*3 + 2] = intensity*ip->getMaterial().getDiffuseColor().z;
			}
		}
	}
	std::cout << "Num pixels: "<<W*H<<", num rays: "<<counter<<"\n";
	

}
