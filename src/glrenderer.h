#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <GL/glfw.h>
#include <iostream>

class GlRenderer {
	public:
		GlRenderer(const int W, const int H);
		~GlRenderer();	
		void render(int *image);
		GLubyte *texture;
	private:
		const int _W;
		const int _H;
};

#endif
