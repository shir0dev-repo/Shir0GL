#pragma once

#include <stdint.h>

namespace sogl {
	struct InstancedMesh {
	private:
		friend class MeshFactory;
		const struct Mesh* m_baseAsset;
	public:
		InstancedMesh();
		InstancedMesh(const InstancedMesh& otherInstance);
		InstancedMesh(const struct Mesh& baseAsset);
		InstancedMesh(const InstancedMesh&&) = delete;
		~InstancedMesh();

		const Mesh* const GetMesh() const;
		void SetMesh(const Mesh* meshAsset);
	};
}