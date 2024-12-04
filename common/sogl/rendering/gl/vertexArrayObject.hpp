#pragma once

namespace sogl {
	typedef int GLint;
	typedef unsigned int GLuint;
	typedef unsigned int GLenum;

	struct mesh;
	typedef struct vertexBufferObject {
		unsigned int size;
		unsigned int ID;
		unsigned int target;

		vertexBufferObject();
		vertexBufferObject(const unsigned int& size, const void* data, const unsigned int& target);		
		void bind() const;
		void unbind() const;
	};

	struct vertexArrayObject {
		vertexBufferObject positions;
		vertexBufferObject texCoords;
		vertexBufferObject normals;
		vertexBufferObject indices;

		unsigned int ID;
		unsigned int pointCount;

		vertexArrayObject(const mesh& mesh);

		void bind() const;
		void unbind() const;
	};
}