#pragma once

namespace sogl {
	typedef int GLint;
	typedef unsigned int GLuint;
	typedef unsigned int GLenum;

	struct mesh;
	struct vertexAttribute;

	struct vertexArrayObject {
	private:
		vertexAttribute* m_positionAttribute;
		vertexAttribute* m_texCoordAttribute;
		vertexAttribute* m_normalAttribute;
		vertexBufferObject* m_indicesBuffer;
		GLuint ID;

	public:
		vertexArrayObject(const mesh& mesh);
		~vertexArrayObject();
	};
}