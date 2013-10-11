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
			float x = ( (2.0f*float(u)-float(_W))/float(_W) ) * _tanfovx;
			float y = ( (2.0f*float(v)-float(_H))/float(_H) ) * _tanfovy;

			Ray r = cam->createRay(x, y);
			glm::vec3 intensity = iterateRay(r, tree, 0);
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

glm::vec3 WhittedTracer::iterateRay(Ray &ray, Octree *tree, int depth) {
	IntersectionPoint ip;
	glm::vec3 color(0.0f);
 	const int maxDepth = 5;
	if (tree->intersect(ray, ip)) {
		if (depth < maxDepth) {
			color = phongShader(ray, ip, tree);
			color *= ip.getMaterial().getAbsorbtion();

			Ray reflection = calculateReflection(ray, ip);
			float absor = 1.0f - ip.getMaterial().getAbsorbtion();
// 			if (color.x < 1.f && color.y < 1.f && color.y < 1.f) 
			{
				glm::vec3 rayColor = iterateRay(reflection, tree, depth + 1);
				color += absor * rayColor;
			}
			
		}
		else {
			color = ip.getMaterial().getAbsorbtion() * phongShader(ray, ip, tree);
		}
	}
	return color;
}

glm::vec3 WhittedTracer::phongShader(Ray &incoming, IntersectionPoint &ip, Octree *tree) {
	const std::vector<PointLight *> &lights = tree->getPointLights();
	glm::vec3 surfaceNormal = ip.getNormal();
	glm::vec3 surfacePosition = ip.getPoint();

	glm::vec3 color = glm::vec3(0.0f);
	for (size_t i = 0; i < lights.size(); ++i) {
		glm::vec3 L = (lights[i]->getPosition() - surfacePosition);
// 		float length = glm::length(L);	
		L = glm::normalize(L);
		// Shadow ray test
		Ray shadowRay = Ray(surfacePosition + L * 0.0001f, L);
		IntersectionPoint shadowIntersection;
		glm::vec3 V = glm::normalize(incoming.getPosition() - surfacePosition);
		float nDotL = glm::max(glm::dot(surfaceNormal, L), 0.0f);
		if (tree->intersect(shadowRay, shadowIntersection)) {
// 			color += ip.getMaterial().getDiffuseColor() * nDotL * 0.01f; // Diffuse component

		}
		else 
		{
			float a = glm::dot(L, surfaceNormal);
			glm::vec3 R = 2 * a * surfaceNormal - L;
			float vDotR = pow(glm::max(glm::dot(R, V), 0.0f), 10);
			
			color += ip.getMaterial().getDiffuseColor() * nDotL; // Diffuse component
			color += vDotR * ip.getMaterial().getDiffuseColor(); // Specular component
			
		}

	}
	return color; 
}
