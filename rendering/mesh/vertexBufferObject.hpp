#pragma once

namespace sogl {
	typedef unsigned int GLuint;
	typedef unsigned int GLenum;
	typedef long long GLsizeiptr;

	struct vertexBufferObject {
		GLsizeiptr byteSize;
		GLuint ID;
		GLenum usage;
		GLenum target;
	private:
		int padding;
	public:

		vertexBufferObject(const void* data, GLsizeiptr byteSize, GLenum target, GLenum usage);

		void bind() const;
		void unbind() const;
	};
}