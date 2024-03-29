#include "FrameBuffer.h"
#include <glad/glad.h>

namespace engine {

	FrameBuffer::FrameBuffer() {
		glGenFramebuffers(1, &this->id);
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &this->id);
	}

	void FrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, this->id);
	}

	void FrameBuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::addDepthTexture(std::shared_ptr<Texture> texture) {
		glBindFramebuffer(GL_FRAMEBUFFER, this->id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getId(), 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::addDepthCubeMap(std::shared_ptr<CubeMap> cubeMap) {
		glBindFramebuffer(GL_FRAMEBUFFER, this->id);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeMap->getId(), 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::bindDefaultFrameBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
