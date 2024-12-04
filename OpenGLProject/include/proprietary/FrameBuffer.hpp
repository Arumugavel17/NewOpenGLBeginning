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

class FrameBuffer {
public:
	FrameBuffer(unsigned int width, unsigned int height,unsigned int samplePoints = 1);
	~FrameBuffer();
	void bind();
	void un_bind();
	unsigned int get_tex_color_buffer();
	void rescale_framebuffer(int width,int height);
private:
	unsigned int textureType;
	unsigned int samplePoints;
	unsigned int id;
	unsigned int texColorBuffer;
	unsigned int rbo;
};
