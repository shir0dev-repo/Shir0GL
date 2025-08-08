#include <GLEW/glew.h>

#include <sogl/rendering/gl/GLBuffer.h>

namespace sogl {
	GLBuffer::GLBuffer() : size(0), target(0), ID(0) {}
	GLBuffer::GLBuffer(const unsigned int& size, const void* data, const unsigned int& target)
	{
		this->size = size;
		this->target = target;
		glGenBuffers(1, &ID);
		glBindBuffer(target, ID);
		glBufferData(target, size, data, GL_STATIC_DRAW);
	}

	void GLBuffer::bind() const {
		glBindBuffer(target, ID);
	}

	void GLBuffer::bufferData(const unsigned int& size, const unsigned int& offset, const void* data) {
		bind();
		if (size <= this->size) {
			glBufferSubData(this->target, offset, size, data);
		}
		else {
			// reallocate the buffer, and reflect changes on vbo
			this->size = size;
			glBufferData(this->target, size, data, GL_STATIC_DRAW);
		}
		unbind();
	}

	void GLBuffer::unbind() const {
		glBindBuffer(target, 0);
	}
}