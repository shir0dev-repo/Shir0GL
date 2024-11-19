#pragma once

namespace sogl {
	typedef int GLint;
	typedef unsigned int GLuint;
	typedef unsigned char GLuchar;

	struct texture {
		GLint width;
		GLint height;
		GLint nrChannels;
		GLuint ID;
		GLuchar* data;

		texture(const char* filePath);
		void bind() const;
		void unbind() const;
		~texture();
	};
}