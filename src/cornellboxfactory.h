#ifndef CORNELLBOXFACTORY_H
#define CORNELLBOXFACTORY_H

#include "quadrilateral.h"
#include "triangle.h"


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
		f.updateAABB();
		return f;
	}

	static Quadrilateral createFront() {
		Quadrilateral f = Quadrilateral(
										glm::vec3(0.f, 0.f, 0.f),
										glm::vec3(0.f, 548.8f, 0.f),
										glm::vec3(556.f, 548.8f, 0.f),
										glm::vec3(552.8, 0.f, 0.f));
		f.scale(_scaleFactor);
		f.translate(_t);
		f.updateAABB();
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
		c.updateAABB();
		return c;
	}

	static Quadrilateral createBack() {
		Quadrilateral b = Quadrilateral(

			glm::vec3(549.6, 	0.0, 	559.2),
			glm::vec3(0.0,   	0.0, 	559.2),
			glm::vec3(0.0, 		548.8, 	559.2), 		
			glm::vec3(556.0, 	548.8, 	559.2));
		b.scale(_scaleFactor);
		b.translate(_t);
		b.updateAABB();
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
		r.updateAABB();
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
		l.updateAABB();
		return l;
	}

	static Quadrilateral createShortBlock1() {
		Quadrilateral sb1 = Quadrilateral(
			glm::vec3(130.0f,	165.0f,	65.0f),
			glm::vec3(82.0f,	165.0f,	225.0f),
			glm::vec3(240.0f,	165.0f,	272.0f),
			glm::vec3(290.0f,	165.0f,	114.0f));
		sb1.scale(_scaleFactor);
		sb1.translate(_t);
		sb1.updateAABB();
		return sb1;
	}
	static Quadrilateral createShortBlock2() {
		Quadrilateral sb2 = Quadrilateral(
			glm::vec3(290.0f,	0.0f,	114.0f),
			glm::vec3(290.0f,	165.0f,	114.0f),
			glm::vec3(240.0f,	165.0f,	272.0f),
			glm::vec3(240.0f,	0.0f,	272.0f));
		sb2.scale(_scaleFactor);
		sb2.translate(_t);
		sb2.updateAABB();
		return sb2;
	}
	static Quadrilateral createShortBlock3() {
		Quadrilateral sb3 = Quadrilateral(
			glm::vec3(130.0f,	0.0f,	65.0f),
			glm::vec3(130.0f,	165.0f,	65.0f),
			glm::vec3(290.0f,	165.0f,	114.0f),
			glm::vec3(290.0f,	0.0f,	114.0f));
		sb3.scale(_scaleFactor);
		sb3.translate(_t);
		sb3.updateAABB();
		return sb3;
	}
	static Quadrilateral createShortBlock4() {
		Quadrilateral sb4 = Quadrilateral(
			glm::vec3(82.0f,	0.0f,	225.0f),
			glm::vec3(82.0f,	165.0f,	225.0f),
			glm::vec3(130.0f,	165.0f,	65.0f),
			glm::vec3(130.0f,	0.0f,	65.0f));
		sb4.scale(_scaleFactor);
		sb4.translate(_t);
		sb4.updateAABB();
		return sb4;
	}
	static Quadrilateral createShortBlock5() {
		Quadrilateral sb5 = Quadrilateral(
			glm::vec3(240.0f,	0.0f,	272.0f),
			glm::vec3(240.0f,	165.0f,	272.0f),
			glm::vec3(82.0f,	165.0f,	225.0f),
			glm::vec3(82.0f,	0.0f,	225.0f));
		sb5.scale(_scaleFactor);
		sb5.translate(_t);
		sb5.updateAABB();
		return sb5;
	}

	static Quadrilateral createTallBlock1() {
		Quadrilateral tb1 = Quadrilateral(
			glm::vec3(423.0f,	330.0f,	247.0f),
			glm::vec3(265.0f,	330.0f,	296.0f),
			glm::vec3(314.0f,	330.0f,	456.0f),
			glm::vec3(472.0f,	330.0f,	406.0f));
		tb1.scale(_scaleFactor);
		tb1.translate(_t);
		tb1.updateAABB();
		return tb1;
	}

	static Quadrilateral createTallBlock2() {
		Quadrilateral tb2 = Quadrilateral(
			glm::vec3(423.0f,	0.0f,	247.0f),
			glm::vec3(423.0f,	330.0f,	247.0f),
			glm::vec3(472.0f,	330.0f,	406.0f),
			glm::vec3(472.0f,	0.0f,	406.0f));
		tb2.scale(_scaleFactor);
		tb2.translate(_t);
		tb2.updateAABB();
		return tb2;
	}

	static Quadrilateral createTallBlock3() {
		Quadrilateral tb3 = Quadrilateral(
			glm::vec3(472.0f,	0.0f,	406.0f),
			glm::vec3(472.0f,	330.0f,	406.0f),
			glm::vec3(314.0f,	330.0f,	456.0f),
			glm::vec3(314.0f,	0.0f,	456.0f));
		tb3.scale(_scaleFactor);
		tb3.translate(_t);
		tb3.updateAABB();
		return tb3;
	}

	static Quadrilateral createTallBlock4() {
		Quadrilateral tb4 = Quadrilateral(
			glm::vec3(314.0f,	0.0f,	456.0f),
			glm::vec3(314.0f,	330.0f,	456.0f),
			glm::vec3(265.0f,	330.0f,	296.0f),
			glm::vec3(265.0f,	0.0f,	296.0f));
		tb4.scale(_scaleFactor);
		tb4.translate(_t);
		tb4.updateAABB();
		return tb4;
	}

	static Quadrilateral createTallBlock5() {
		Quadrilateral tb5 = Quadrilateral(
			glm::vec3(265.0f,	0.0f,	296.0f),
			glm::vec3(265.0f,	330.0f,	296.0f),
			glm::vec3(423.0f,	330.0f,	247.0f),
			glm::vec3(423.0f,	0.0f,	247.0f));
		tb5.scale(_scaleFactor);
		tb5.translate(_t);
		tb5.updateAABB();
		return tb5;
	}

	static Quadrilateral createLightSourceQuad() {
		Quadrilateral ls = Quadrilateral(
			glm::vec3(343.0f, 548.8f, 227.0f),
			glm::vec3(343.0f, 548.8f, 332.0f),
			glm::vec3(213.0f, 548.8f, 332.0f),
			glm::vec3(213.0f, 548.8f, 227.0f));
		ls.scale(_scaleFactor);
		ls.translate(_t);
		ls.translate(glm::vec3(0.0f,-0.01f,0.0f));
		ls.updateAABB();
		return ls;
	}


private:
		static float _scaleFactor;
		static glm::vec3 _t;
};


float CornellBoxFactory::_scaleFactor = 0.025f;
glm::vec3 CornellBoxFactory::_t = glm::vec3(-7.f,-6.8f,15.6f);//glm::vec3(-278.f,-273.f,800.f);
#endif
