#pragma once

namespace sogl {
	typedef class ChunkMesh {
		void ConstructAxisBitset(Chunk* chunk, uint64_t*& outBitset);
		void GreedyMeshBinaryPlane(std::vector<struct GreedyQuad>* quadVerts, uint64_t* planeData);
		static vec3f WorldToSample(enum FaceDirection dir, uint64_t axis, uint64_t x, uint64_t y);
		static void AppendVertices(GreedyQuad quad, std::vector<vec3f>* vertices, std::vector<uint32_t>* normals, FaceDirection faceDir, uint64_t axis, uint64_t blockType);
	public:
		struct Mesh* meshData;
		ChunkMesh(struct Chunk& chunk);
	};
}