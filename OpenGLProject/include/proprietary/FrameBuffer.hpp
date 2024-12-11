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
	void bind(bool normal = true);
	void un_bind();
	unsigned int get_texture(bool normal = true);
	
	void normalFrameBuffer(unsigned int width, unsigned int height, unsigned int samplePoints);
	void shadowFrameBuffer();

private:
	unsigned int normal_samplePoints;
	unsigned int normal_id;
	unsigned int normal_texColorBuffer;
	unsigned int normal_rbo;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int shadow_depthMapFBO;
	unsigned int shadow_depthMap;
};
