// GL:
#include <glew/glew.h>

// STD:
#include <cassert>

// SOGL:
#include "../rendering/mesh/vertexBufferObject.hpp"

namespace sogl {
	vertexBufferObject::vertexBufferObject(const void* data, GLsizeiptr byteSize, GLenum target, GLenum usage) {
		glGenBuffers(1, &ID);
		
		assert(ID > 0);

		this->byteSize = byteSize;
		this->target = target;
		this->usage = usage;

		bind();
		glBufferData(target, byteSize, data, usage);
		unbind();
	}

	void vertexBufferObject::bind() const {
		glBindBuffer(target, ID);
	}

	void vertexBufferObject::unbind() const {
		glBindBuffer(target, 0);
	}

}