#include <FrameBuffer.hpp>

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, unsigned int samplePoints) {
    normalFrameBuffer(width, height, samplePoints);
    shadowFrameBuffer();
}

void FrameBuffer::normalFrameBuffer(unsigned int width, unsigned int height, unsigned int samplePoints) {
    this->normal_samplePoints = samplePoints;

    // Sanity check for sample points
    if (samplePoints < 1) {
        std::cerr << "ERROR::FRAMEBUFFER:: Invalid samplePoints value. Must be >= 1." << std::endl;
        return;
    }

    // Check maximum supported samples
    int maxSamples;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
    if (samplePoints > maxSamples) {
        std::cerr << "ERROR::FRAMEBUFFER:: samplePoints exceeds GL_MAX_SAMPLES (" << maxSamples << ")" << std::endl;
        return;
    }

    glGenFramebuffers(1, &normal_id);
    glBindFramebuffer(GL_FRAMEBUFFER, normal_id);

    // Texture creation
    glGenTextures(1, &normal_texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, normal_texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, normal_texColorBuffer, 0);

    glGenRenderbuffers(1, &normal_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, normal_rbo);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, normal_rbo);

    // Framebuffer completeness check
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::shadowFrameBuffer() {
    glGenFramebuffers(1, &shadow_depthMapFBO);
    // create depth texture
    glGenTextures(1, &shadow_depthMap);
    glBindTexture(GL_TEXTURE_2D, shadow_depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1920,1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL Construct Error Code: " << err << std::endl;
    }
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1,&normal_id);
	glDeleteTextures(1, &normal_texColorBuffer);
}

void FrameBuffer::bind(bool normal) {
    if (normal) {
	    glBindFramebuffer(GL_FRAMEBUFFER, normal_id);
    }
    else {
        glBindFramebuffer(GL_FRAMEBUFFER, shadow_depthMapFBO);
    }
}

void FrameBuffer::un_bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBuffer::get_texture(bool normal) {
    if (normal) {
	    return normal_texColorBuffer;
    }
    else {
        return shadow_depthMap;
    }
}
