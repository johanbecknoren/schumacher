#include "glrenderer.h"

GlRenderer::GlRenderer(const int W, const int H) : _W(W), _H(H) {
	GLFWvidmode mode;
    if(!glfwInit())
    {
        glfwTerminate();
        return;
    }
    glfwGetDesktopMode(&mode);

    if( !glfwOpenWindow(W, H, 8,8,8,8, 32, 0, GLFW_WINDOW) )
    {
        glfwTerminate(); // glfwOpenWindow failed, quit the program.
        return;
    }

    if( !glfwGetWindowParam( GLFW_OPENED ) ) {
		std::cout << "Window unable to open ... exiting!" << std::endl;
        return;
    }
	
	glEnable(GL_TEXTURE_2D);
	
	texture = (GLubyte*)malloc(sizeof(GLubyte) * 3 * W * H);
}

GlRenderer::~GlRenderer() {
	delete texture;
}

void GlRenderer::render(int *image) {
	// Convert image to GLdata
// 	for (int i = 0; i < _H; ++i) {
// 		for (int j = 0; j < _W; ++j) {
// 			int id = _W * 3 * i + j + 3;
// 			for (int k = 0; k < 3; ++k) {
// 				texture[id + k] = 
// 			}
// 			 
// 		}
// 	}
	std::copy(image, image + 3 * _W * _H, texture);
	GLuint texId = 0;
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _W, _H, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);    
    
    GLenum loaderError = glGetError();
    if(loaderError){
        printf("Error while loading texture!\n");
	}
	int running = GL_TRUE;
	while (running) {
		                                                
		glLoadIdentity();                                               
		glMatrixMode(GL_MODELVIEW);                                  
		glLoadIdentity();

		glBindTexture(GL_TEXTURE_2D, 0);  //Retrieved from your object
		glEnable(GL_TEXTURE_2D);
// 		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buff);

		glBegin(GL_QUADS);
			glTexCoord2f(0,1);
			glVertex2f(-1, -1); 
			glTexCoord2f(0, 0);
			glVertex2f(-1, 1);
			glTexCoord2f(1, 0);   
			glVertex2f(1, 1);
			glTexCoord2f(1, 1);      
			glVertex2f(1, -1);
		glEnd();

	}
}
