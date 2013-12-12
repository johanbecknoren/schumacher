#include "whittedtracer.h"
#include "progressbar.h"
#include "timer.h"

void WhittedTracer::render(float *pixels, Octree *tree, Camera *cam) {
	int total = _W * _H;
	int rayCounter = 0;

	// (u,v) are pixel coords for output image
	Timer::getInstance()->start("whitted");
	for(int u=0; u<_W; ++u) {
		for(int v=0; v<_H; ++v) {
			rayCounter++;
			real x;
			real y;			
			calculateXnY(u, v, x, y);
			Ray r = cam->createRay(x, y);
			Vec3 intensity = iterateRay(r, tree, 0);
			int id = calculateId(u, v);
			pixels[id + 0] = intensity.x;
			pixels[id + 1] = intensity.y;
			pixels[id + 2] = intensity.z;

			ProgressBar::printProgBar(rayCounter, total);	
		}
	}
	Timer::getInstance()->stop("whitted");
	
	std::cout << std::endl;
	std::cout << "Num pixels: "<<_W*_H<<", num rays: "<<rayCounter<<"\n";
	Timer::getInstance()->printRealTime("whitted");
}

Vec3 WhittedTracer::iterateRay(Ray &ray, Octree *tree, int depth) {
	IntersectionPoint ip;
	Vec3 color(0.0f);
 	const int maxDepth = 10;
	if (tree->intersect(ray, ip)) {
		if (depth < maxDepth) {
			color = phongShader(ray, ip, tree);
			color *= ip.getMaterial()->getAbsorbtion();

			Ray reflection = calculateReflection(ray, ip);
			real absor = real(1.0) - ip.getMaterial()->getAbsorbtion();
// 			if (color.x < 1.f && color.y < 1.f && color.y < 1.f) 
			{
				Vec3 rayColor = iterateRay(reflection, tree, depth + 1);
				color += absor * rayColor;
			}
			
		}
		else {
			color = ip.getMaterial()->getAbsorbtion() * phongShader(ray, ip, tree);
		}
	}
	return color;
}

Vec3 WhittedTracer::phongShader(Ray &incoming, IntersectionPoint &ip, Octree *tree) {
	const std::vector<PointLight *> &lights = tree->getPointLights();
	Vec3 surfaceNormal = ip.getNormal();
	Vec3 surfacePosition = ip.getPoint();

	Vec3 color = Vec3(0.0f);
	for (size_t i = 0; i < lights.size(); ++i) {
		Vec3 L1 = (lights[i]->getPosition() - surfacePosition);
// 		float length = glm::length(L);	
		Vec3 L = glm::normalize(L1);
		// Shadow ray test
		Ray shadowRay = Ray(surfacePosition + L * real(0.0001), L);
		IntersectionPoint shadowIntersection;

		if (tree->intersect(shadowRay, shadowIntersection)) {
// 			color += ip.getMaterial().getDiffuseColor() * nDotL * 0.01f; // Diffuse component
			
		}
		else 
		{
			Vec3 V = glm::normalize(incoming.getPosition() - surfacePosition);
			real nDotL = glm::max(glm::dot(surfaceNormal, L), real(0.0));
			real a = glm::dot(L, surfaceNormal);
			Vec3 R = 2 * a * surfaceNormal - L;
			real vDotR =0 * pow(glm::max(glm::dot(R, V), real(0.0)), 4);
			
			color += ip.getMaterial()->getDiffuseColor() * nDotL; // Diffuse component
			color += vDotR * ip.getMaterial()->getDiffuseColor(); // Specular component
			
		}

	}
	return color; 
}
