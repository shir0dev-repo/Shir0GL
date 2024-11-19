// GL:
#include <GLEW/glew.h>

// STD:
#include <cassert>

// SOGL:
#include <sogl/rendering/vertexBufferObject.hpp>

namespace sogl {
	vertexBufferObject::vertexBufferObject(const void* data, const GLsizeiptr byteSize, const GLenum target, const GLenum usage) {
		glGenBuffers(1, &ID);
		
		assert(ID > 0);

		this->byteSize = byteSize;
		this->target = target;
		this->usage = usage;
		this->count = byteSize / sizeof(float);

		bind();
		glBufferData(target, byteSize, data, usage);
	}

	vertexBufferObject::~vertexBufferObject() {
		glDeleteBuffers(1, &this->ID);
	}

	void vertexBufferObject::bind() const {
		glBindBuffer(target, ID);
	}

	void vertexBufferObject::unbind() const {
		glBindBuffer(target, 0);
	}

}