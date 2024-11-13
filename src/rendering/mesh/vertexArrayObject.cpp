#include <glew/glew.h>
#include <glfw/glfw3.h>

#include "../rendering/mesh/mesh.hpp"
#include "../rendering/mesh/vertexAttribute.hpp"
#include "../rendering/mesh/vertexArrayObject.hpp"
#include "../rendering/mesh/vertexBufferObject.hpp"

namespace sogl {
	vertexArrayObject::vertexArrayObject(const mesh& mesh) {		
		glGenVertexArrays(1, &this->ID);

		m_positionAttribute = new vertexAttribute(ID, 0, mesh.positions, mesh.positionByteSize, GL_FLOAT, 3, 0);
		m_texCoordAttribute = new vertexAttribute(ID, 1, mesh.texCoords, mesh.texCoordByteSize, GL_FLOAT, 2, 0);
		m_normalAttribute = new vertexAttribute(ID, 2, mesh.normals, mesh.normalByteSize, GL_FLOAT, 3, 0);
		m_indicesBuffer = new vertexBufferObject(mesh.indices, mesh.indicesByteSize, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
	}

	vertexArrayObject::~vertexArrayObject() {
		if (m_positionAttribute != nullptr) {
			delete m_positionAttribute;
			m_positionAttribute = nullptr;
		}
		if (m_texCoordAttribute != nullptr) {
			delete m_texCoordAttribute;
			m_texCoordAttribute = nullptr;
		}
		if (m_normalAttribute != nullptr) {
			delete m_normalAttribute;
			m_normalAttribute = nullptr;
		}
		if (m_indicesBuffer != nullptr) {
			delete m_indicesBuffer;
			m_indicesBuffer = nullptr;
		}
	}
}