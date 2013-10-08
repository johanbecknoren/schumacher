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
		
		f.scale(_scaleFactor);
		f.translate(_t);
		return f;
	}

	static Quadrilateral createCeil() {
		Quadrilateral c = Quadrilateral(

			glm::vec3(556.0f,   548.8f, 0.0f),
			glm::vec3(556.0f,   548.8f, 559.2f), 			
			glm::vec3(0.0f,     548.8f, 559.2f), 		
			glm::vec3(0.0f,     548.8f, 0.0f));
		c.scale(_scaleFactor);
		c.translate(_t);
		return c;
	}

	static Quadrilateral createBack() {
		Quadrilateral b = Quadrilateral(
			glm::vec3(549.6, 0.0, 559.2),
			glm::vec3(556.0, 548.8, 559.2), 			
			glm::vec3(0.0, 548.8, 559.2), 		
			glm::vec3(0.0,   0.0, 559.2));
		b.scale(_scaleFactor);
		b.translate(_t);
		return b;
	}

	static Quadrilateral createRight() {
		Quadrilateral r = Quadrilateral(

			glm::vec3(0.0, 0.0, 559.2),
			glm::vec3(0.0, 0.0, 0.0), 			
			glm::vec3(0.0, 548.8, 0.0), 		
			glm::vec3(0.0, 548.8, 559.2));
			r.scale(_scaleFactor);
		r.translate(_t);
		return r;
	}
	static Quadrilateral createLeft() {
		Quadrilateral l = Quadrilateral(

			glm::vec3(552.8, 0.0, 0.0),
			glm::vec3(549.6, 0.0, 559.2), 			
			glm::vec3(556.0, 548.8, 559.2), 		
			glm::vec3(556.0, 548.8, 0.0));
		l.scale(_scaleFactor);
		l.translate(_t);
		return l;
	}

	private:
		static float _scaleFactor;
		static glm::vec3 _t;
};
float CornellBoxFactory::_scaleFactor = 0.02;
glm::vec3 CornellBoxFactory::_t = glm::vec3(-4.5f,-4.5f,1.6f);//glm::vec3(-278.f,-273.f,800.f);
#endif
