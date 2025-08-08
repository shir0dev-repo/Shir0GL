#include <sogl/rendering/gl/mesh/InstancedMesh.h>
#include <sogl/rendering/factories/MeshFactory.h>

namespace sogl {
	InstancedMesh::InstancedMesh() {
		m_baseAsset = nullptr;
	}

	InstancedMesh::InstancedMesh(const InstancedMesh& otherInstance) : m_baseAsset(nullptr) {
		InstancedMesh* self = this;
		MeshFactory::Clone(self, &otherInstance);
	}

	InstancedMesh::InstancedMesh(const Mesh& baseAsset) {
		m_baseAsset = &baseAsset;
	}

	InstancedMesh::~InstancedMesh() {
		MeshFactory::DeleteInstance(this);		
	}

	const Mesh* const InstancedMesh::GetMesh() const { return m_baseAsset; }
	void InstancedMesh::SetMesh(const Mesh* meshAsset) { m_baseAsset = meshAsset; }
}