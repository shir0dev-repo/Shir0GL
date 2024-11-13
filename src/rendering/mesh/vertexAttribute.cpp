#include <glew/glew.h>

#include "../rendering/mesh/vertexBufferObject.hpp"
#include "../rendering/mesh/vertexAttribute.hpp"

namespace sogl {
	vertexAttribute::vertexAttribute(GLuint vaoID, GLuint attributeID, const void* data, GLuint byteSize,
		GLenum dataType, GLuint attributeSize, GLuint stride) {
		// store this attribute's target ID (position, uv, etc.)
		this->attributeID = attributeID;
		this->attributeSize = attributeSize;

		// enable vao to allow writing to attributes
		glEnableVertexAttribArray(vaoID);

		// create vbo
		this->buffer = new vertexBufferObject(data, byteSize, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		
		// bind vbo, assign attribute pointer to vao
		buffer->bind();
		glVertexAttribPointer(attributeID, attributeSize, dataType, GL_FALSE, stride, (void*)0);
		
		// unbind vao and vbo
		buffer->unbind();
		glDisableVertexAttribArray(vaoID);
	}

	vertexAttribute::~vertexAttribute() {
		// delete the data stored in the gl buffer
		glDeleteBuffers(1, &buffer->ID);
		
		// delete buffer object in heap
		delete buffer;
		buffer = nullptr;
	}
}