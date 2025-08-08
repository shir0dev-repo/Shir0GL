#include <GLEW/glew.h>

#include <sogl/transform/transform.hpp>
#include <sogl/rendering/gl/VertexArray.h>
#include <sogl/rendering/factories/MeshFactory.h>
#include <sogl/rendering/gl/Model.h>

namespace sogl {
	Model::Model() {
		m_meshInstance = nullptr;
		m_material = nullptr;
		m_meshBuffers = nullptr;

		m_transform = new transform();
	}

	Model::Model(const Mesh* meshAsset, const Material* material) : m_meshBuffers(nullptr) {
		m_meshInstance = MeshFactory::CreateInstance(meshAsset);
		m_material = material;
		MeshFactory::FindBuffer(meshAsset, m_meshBuffers);

		m_transform = new transform();
	}

	Model::~Model() {
		MeshFactory::DeleteInstance(m_meshInstance);
		m_meshInstance = nullptr;
		m_material = nullptr;
		m_meshBuffers = nullptr;
		delete m_transform;
	}

	const Mesh* const Model::GetMesh() const {
		return m_meshInstance->GetMesh();
	}

	void Model::SetMesh(const Mesh* meshAsset) {
		MeshFactory::UpdateInstance(m_meshInstance, meshAsset);
		MeshFactory::FindBuffer(meshAsset, m_meshBuffers);
	}

	const InstancedMesh* const Model::GetMeshInstance() const {
		return m_meshInstance;
	}

	const Material* const Model::GetMaterial() const {
		return m_material;
	}

	void Model::SetMaterial(const Material* material) {
		m_material = material;
	}

	transform* const Model::Transform() {
		return m_transform;
	}
}