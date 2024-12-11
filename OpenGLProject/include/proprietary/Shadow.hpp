#pragma once

//cpp standard libraries
#include <iostream>

//Window libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//glm Maths Librariess
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

class Shadow {
public:
	Shadow(unsigned int width = 0, unsigned int height = 0);
	~Shadow();
	void bind();
	void un_bind();
	unsigned int get_depth_map();
	void rescale_framebuffer(int width, int height);
private:

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	unsigned int depthMap;
	//unsigned int rbo;
};
