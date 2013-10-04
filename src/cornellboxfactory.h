#ifndef CORNELLBOXFACTORY_H
#define CORNELLBOXFACTORY_H

#include "quadrilateral.h"



class CornellBoxFactory {

	public: 
		static Quadrilateral createFloor() {
		Quadrilateral f = Quadrilateral(
			glm::vec3(552.8f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f), 			
			glm::vec3(0.0f, 0.0f, 559.2f), 		
			glm::vec3(549.6f, 0.0f, 559.2f));
		
		f.translate(_t);
		f.scale(_scaleFactor);
		return f;
	}

	static Quadrilateral createCeil() {
		Quadrilateral c = Quadrilateral(
			glm::vec3(556.0f,   548.8f, 0.0f),
			glm::vec3(556.0f,   548.8f, 559.2f), 			
			glm::vec3(0.0f,     548.8f, 559.2f), 		
			glm::vec3(0.0f,     548.8f, 0.0f));
		c.translate(_t);
		c.scale(_scaleFactor);
		return c;
	}

	static Quadrilateral createBack() {
		Quadrilateral b = Quadrilateral(
			glm::vec3(549.6, 0.0, 559.2),
			glm::vec3(0.0,   0.0, 559.2), 			
			glm::vec3(0.0, 548.8, 559.2), 		
			glm::vec3(556.0, 548.8, 559.2));
		b.translate(_t);
		b.scale(_scaleFactor);
		return b;
	}

	static Quadrilateral createRight() {
		Quadrilateral r = Quadrilateral(
			glm::vec3(0.0, 0.0, 559.2),
			glm::vec3(0.0, 0.0, 0.0), 			
			glm::vec3(0.0, 548.8, 0.0), 		
			glm::vec3(0.0, 548.8, 559.2));
		r.translate(_t);
		r.scale(_scaleFactor);
		return r;
	}
	static Quadrilateral createLeft() {
		Quadrilateral l = Quadrilateral(
			glm::vec3(552.8, 0.0, 0.0),
			glm::vec3(549.6, 0.0, 559.2), 			
			glm::vec3(556.0, 548.8, 559.2), 		
			glm::vec3(556.0, 548.8, 0.0));
		l.translate(_t);
		l.scale(_scaleFactor);
		return l;
	}

	private:
		static float _scaleFactor;
		static glm::vec3 _t;
};
float CornellBoxFactory::_scaleFactor = 0.005;
glm::vec3 CornellBoxFactory::_t = glm::vec3(-278.f,-273.f,800.f);
#endif
