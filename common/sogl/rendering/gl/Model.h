#pragma once

namespace sogl {
	class Model {
		friend class MeshFactory;

		struct InstancedMesh* m_meshInstance;
		const struct Material* m_material;
		const struct VertexArray* m_meshBuffers;
		struct transform* m_transform;

	public:
		Model();
		Model(const struct Mesh* meshAsset, const struct Material* material);
		~Model();

		const struct Mesh* const GetMesh() const;
		void SetMesh(const struct Mesh* newMesh);
		const struct InstancedMesh* const GetMeshInstance() const;

		const struct Material* const GetMaterial() const;
		void SetMaterial(const struct Material* const material);

		struct transform* const Transform();
	};
}