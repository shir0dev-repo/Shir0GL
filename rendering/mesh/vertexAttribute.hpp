#pragma once

namespace sogl {
	typedef unsigned int GLuint;
	typedef unsigned int GLenum;

	struct vertexBufferObject;

	struct vertexAttribute {
		GLuint attributeSize;
		GLuint attributeID;

		vertexBufferObject* buffer;

		vertexAttribute(GLuint vaoID, GLuint attributeID, const void* data, GLuint byteSize, GLenum dataType,
			GLuint attributeSize, GLuint stride);

		~vertexAttribute();		
	};
}