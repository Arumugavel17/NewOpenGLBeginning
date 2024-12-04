#include <FrameBuffer.hpp>

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, unsigned int samplePoints) {
    this->samplePoints = samplePoints;

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

    textureType = (samplePoints > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    // Texture creation
    glGenTextures(1, &texColorBuffer);
    glBindTexture(textureType, texColorBuffer);

    if (textureType == GL_TEXTURE_2D_MULTISAMPLE) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samplePoints, GL_RGB, width, height, GL_TRUE);
    }
    else if (textureType == GL_TEXTURE_2D) {
        glTexImage2D(textureType, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureType, texColorBuffer, 0);

    // Renderbuffer creation
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    if (textureType == GL_TEXTURE_2D_MULTISAMPLE) {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samplePoints, GL_DEPTH24_STENCIL8, width, height);
    }
    else {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Framebuffer completeness check
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1,&id);
	glDeleteTextures(1, &texColorBuffer);
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::un_bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBuffer::get_tex_color_buffer() {
	return texColorBuffer;
}

void FrameBuffer::rescale_framebuffer(int width, int height) {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glBindTexture(textureType, texColorBuffer);

	if (textureType == GL_TEXTURE_2D_MULTISAMPLE) {
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, this->samplePoints, GL_RGB, width, height, GL_TRUE);
	}
	else if (textureType == GL_TEXTURE_2D) {
		glTexImage2D(textureType, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	if (textureType == GL_TEXTURE_2D_MULTISAMPLE) {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, this->samplePoints, GL_DEPTH24_STENCIL8, width, height);
	}
	else {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureType, texColorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete after rescaling!" << std::endl;
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
