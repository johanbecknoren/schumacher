#include "glrenderer.h"
/*
GlRenderer::GlRenderer(const int W, const int H) : texId(0), _W(W), _H(H)  {
    if(!glfwInit())
    {
        glfwTerminate();
        return;
    }
	if( !glfwOpenWindow(W, H, 8,8,8,8, 0, 0, GLFW_WINDOW) )
    {
        glfwTerminate(); // glfwOpenWindow failed, quit the program.
        return;
    }

    if( !glfwGetWindowParam( GLFW_OPENED ) ) {
		std::cout << "Window unable to open ... exiting!" << std::endl;
        return;
    }
	glEnable(GL_DEPTH_TEST);	
	glEnable(GL_TEXTURE_2D);

	texture = (GLubyte*)malloc(sizeof(GLubyte) * 4 * W * H);

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
		
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _W, _H, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);    
    
	GLenum loaderError = glGetError();
	if(loaderError){
		printf("Error while loading texture!\n");
	}
}

GlRenderer::~GlRenderer() {
	delete texture;
}

void GlRenderer::render(float *image, std::vector<bool> *done) {
	// Convert image to GLdata
	int running = GL_TRUE;
	while (running) {
		for (int i = 0; i < _H; ++i) {
			for (int j = 0; j < _W; ++j) {
				int id = _W * 4 * i + j * 4;
				int id2 = _W * 3 * i + j * 3;
				for (int k = 0; k < 3; ++k) {
					texture[id + k] = int(image[id2 + k] + 40); 
				}
				texture[id + 3] = 255.f; 
			}
		}
		// 	std::copy(image, image + 3 * _W * _H, texture);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _W, _H, GL_RGBA, GL_UNSIGNED_BYTE, texture);
    
		GLenum loaderError = glGetError();
		if(loaderError){
			printf("Error while loading texture!\n");
		}

		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);                                           

		glMatrixMode(GL_MODELVIEW);                                  
		glLoadIdentity();

		glBindTexture(GL_TEXTURE_2D, texId);  //Retrieved from your object
// 		glColor4f(0.f, 1.f, 1.f, 1.f);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex2f(-1, -1); 

			glTexCoord2f(0, 1);
			glVertex2f(-1, 1);
			
			glTexCoord2f(1, 1);   
			glVertex2f(1, 1);
			
			glTexCoord2f(1, 0);      
			glVertex2f(1, -1);
		glEnd();  
		glfwSwapBuffers();
		running = glfwGetWindowParam( GLFW_OPENED );

		for(auto b : *done) {
			if (!b) continue;
		}
		running = GL_FALSE;
	}

}

void GlRenderer::render(int *image) {
	// Convert image to GLdata
	for (int i = 0; i < _H; ++i) {
		for (int j = 0; j < _W; ++j) {
			int id = _W * 4 * i + j * 4;
			int id2 = _W * 3 * i + j * 3;
			for (int k = 0; k < 3; ++k) {
				texture[id + k] = image[id2 + k]; 
			}
			texture[id + 3] = 255.f; 
		}
	}
// 	std::copy(image, image + 3 * _W * _H, texture);
	GLuint texId = 0;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _W, _H, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);    
    
    GLenum loaderError = glGetError();
    if(loaderError){
        printf("Error while loading texture!\n");
	}
	int running = GL_TRUE;
	while (running) {
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);                                           

		glMatrixMode(GL_MODELVIEW);                                  
		glLoadIdentity();

		glBindTexture(GL_TEXTURE_2D, texId);  //Retrieved from your object
// 		glColor4f(0.f, 1.f, 1.f, 1.f);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex2f(-1, -1); 

			glTexCoord2f(0, 1);
			glVertex2f(-1, 1);
			
			glTexCoord2f(1, 1);   
			glVertex2f(1, 1);
			
			glTexCoord2f(1, 0);      
			glVertex2f(1, -1);
		glEnd();  
		glfwSwapBuffers();
		running = glfwGetWindowParam( GLFW_OPENED );
	}
}
*/
