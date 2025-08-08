#pragma once

#include <stdint.h>

namespace sogl {
	struct Mesh {
	private:
		friend class MeshFactory;
		friend struct MeshUtils;

		float* m_vertices;
		float* m_texCoords;
		float* m_normals;
		uint32_t* m_indices;

		uint32_t m_vertexCount;
		uint32_t m_texCoordCount;
		uint32_t m_normalCount;
		uint32_t m_indexCount;

	public:
		Mesh();		
		Mesh(const Mesh&) = delete;
		Mesh(const Mesh&&) = delete;
		~Mesh();

		inline const float* const Vertices() const { return m_vertices; }
		inline void SetVertices(float* vertices, uint32_t length) { 
			if (m_vertices != nullptr) delete[] m_vertices;
			m_vertices = vertices;
			m_vertexCount = length;
		}
		inline void SetVertex(int index, float value) {
			if (m_vertices == nullptr) return;

			m_vertices[index] = value;
		}

		inline const uint32_t VerticesSize() const { return m_vertexCount * sizeof(float); }
		inline const uint32_t VertexCount() const { return m_vertexCount; }

		inline const float* const TexCoords() const { return m_texCoords; }
		inline const uint32_t TexCoordsSize() const { return m_texCoordCount * sizeof(float); }
		inline const uint32_t TexCoordCount() const { return m_texCoordCount; }

		inline const float* const Normals() const { return m_normals; }
		inline const uint32_t NormalsSize() const { return m_normalCount * sizeof(float); }
		inline const uint32_t NormalCount() const { return m_normalCount; }

		inline const uint32_t* const Indices() const { return m_indices; }
		inline const uint32_t IndicesSize() const { return m_indexCount * sizeof(uint32_t); }
		inline const uint32_t IndexCount() const { return m_indexCount; }
	};
}