#include <GLEW/glew.h>

#include <sogl/rendering/gl/mesh/Mesh.h>
#include <sogl/rendering/factories/MeshFactory.h>

namespace sogl {
	Mesh::Mesh() {
		m_vertices = nullptr;
		m_vertexCount = 0;

		m_texCoords = nullptr;
		m_texCoordCount = 0;
		
		m_normals = nullptr;
		m_normalCount = 0;
		
		m_indices = nullptr;
		m_indexCount = 0;
	}
	Mesh::~Mesh() {
		MeshFactory::DeleteMesh(this);
	}
}