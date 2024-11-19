#pragma once

namespace sogl {
	typedef unsigned int GLuint;
	typedef unsigned int GLenum;
	typedef unsigned long long GLsizeiptr;

	struct vertexBufferObject {
		GLsizeiptr byteSize;
		GLuint ID;
		GLenum usage;
		GLenum target;
		GLuint count;
		
	public:

		// Don't forget to unbind!
		vertexBufferObject(const void* data, const GLsizeiptr byteSize, const GLenum target, const GLenum usage);
		~vertexBufferObject();

		void bind() const;
		void unbind() const;
	};
}