#include <GLEW/glew.h>

#include <vector>
#include <map>
#include <sogl/bitmanip.hpp>

#include <sogl/world/data/FaceDirection.hpp>
#include <sogl/rendering/gl/mesh/Mesh.h>
#include <sogl/structure/bitSet.h>
#include <sogl/world/data/chunk.h>
#include <sogl/transform/vec3f.hpp>
#include <sogl/world/data/chunkMesh.h>
#include <sogl/rendering/factories/MeshFactory.h>

namespace sogl {
	static const uint64_t CHUNK_SIZE_2 = Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE;
	static const uint64_t CHUNK_SIZE_3 = Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE;
	static const uint64_t U_ONE = 1UL;
	
	struct GreedyQuad {
		uint64_t x;
		uint64_t y;
		uint64_t w;
		uint64_t h;

		inline GreedyQuad(uint64_t x, uint64_t y, uint64_t w, uint64_t h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};

	ChunkMesh::ChunkMesh(Chunk& chunkData) {
		uint64_t* axisColumns = nullptr;

		ConstructAxisBitset(&chunkData, axisColumns);
		assert(axisColumns);

		uint64_t* columnFaceMasks = new uint64_t[3 * CHUNK_SIZE_3 * 2]{ 0UL };
		for (uint64_t axis = 0; axis < 3; axis++) {
			for (uint64_t i = 0; i < CHUNK_SIZE_2; i++) {
				uint64_t column = axisColumns[(CHUNK_SIZE_2 * axis) + i];
				columnFaceMasks[(CHUNK_SIZE_2 * (axis * 2 + 1)) + i] = column & ~(column >> 1);
				columnFaceMasks[(CHUNK_SIZE_2 * (axis * 2 + 0)) + i] = column & ~(column << 1);
			}
		}
		
		std::map<uint64_t, std::map<uint64_t, uint64_t[Chunk::CHUNK_SIZE]>> data[6]{ std::map<uint64_t, std::map<uint64_t, uint64_t[Chunk::CHUNK_SIZE]>>()};
		
		for (uint64_t axis = 0; axis < 6; axis++) {
			for (uint64_t z = 0; z < Chunk::CHUNK_SIZE; z++) {
				for (uint64_t x = 0; x < Chunk::CHUNK_SIZE; x++) {
					uint64_t colIndex = 1 + x + ((z + 1) * Chunk::CHUNK_SIZE) + CHUNK_SIZE_2 * axis;
					uint64_t column = columnFaceMasks[colIndex];
					while (column != 0) {
						uint64_t y = trailing_zeroes(column);
						column &= column - 1;
						vec3i pos;
						switch (axis) {
							case 0:
							case 1:
								pos.set(x, y, z); // down, up
								break;
							case 2:
							case 3:
								pos.set(y, z, x); // left, right
								break;
							default:
								pos.set(x, z, y); // forward, back
								break;
						}
						voxel* current = chunkData.getVoxel(pos.x, pos.y, pos.z);
						auto d = data[axis]
							[current->type]
							[y];
						d[x] |= U_ONE << z;
					}
				}
			}
		}

		std::vector<vec3f> vertices{};
		std::vector<uint32_t> normals{};
		for (uint64_t axis = 0; axis < 6; axis++) {
			auto& blockData = data[axis];
			FaceDirection faceDir;
			switch (axis) {
				case 0:
					faceDir = FaceDirection::down;
					break;
				case 1:
					faceDir = FaceDirection::up;
					break;
				case 2:
					faceDir = FaceDirection::left;
					break;
				case 3:
					faceDir = FaceDirection::right;
					break;
				case 4:
					faceDir = FaceDirection::forward;
					break;
				default:
					faceDir = FaceDirection::back;
					break;
			}
			for (auto& pair : blockData) {
				uint64_t blockType = pair.first;
				for (auto& axisPlane : pair.second) {
					std::vector<GreedyQuad> quadVertices{};
					GreedyMeshBinaryPlane(&quadVertices, &axisPlane.second[0]);
					for (auto& q : quadVertices) {
						AppendVertices(q, &vertices, &normals, faceDir, axis, blockType);
					}
				}
			}
		}

		this->meshData = new Mesh();
		const uint32_t size = vertices.size() * 3;
		meshData->SetVertices(new float[size], size);
		for (uint32_t i = 0; i < vertices.size(); i += 3) {
			meshData->SetVertex(i, vertices.at(i).x);
			meshData->SetVertex(i + 1, vertices.at(i).y);
			meshData->SetVertex(i + 2, vertices.at(i).z);
		}
	}

	void ChunkMesh::ConstructAxisBitset(Chunk* chunkData, uint64_t*& outBitset) {
		outBitset = new uint64_t[3 * CHUNK_SIZE_3]{ 0UL };

		for (uint64_t y = 0; y < Chunk::CHUNK_SIZE; y++) {
			for (uint64_t z = 0; z < Chunk::CHUNK_SIZE; z++) {
				for (uint64_t x = 0; x < Chunk::CHUNK_SIZE; x++) {
					voxel* const v = chunkData->getVoxel(x, y, z);
					if (v->type != AIR) {
						// xz - y axis
						outBitset[x + (z * Chunk::CHUNK_SIZE)] |= U_ONE << y;
						// zy - x axis
						outBitset[z + (y * Chunk::CHUNK_SIZE) + CHUNK_SIZE_2] |= U_ONE << x;
						// xy - z axis
						outBitset[x + (y * Chunk::CHUNK_SIZE) + CHUNK_SIZE_2 * 2] |= U_ONE << z;
					}
				}
			}
		}
	}

	void ChunkMesh::GreedyMeshBinaryPlane(std::vector<GreedyQuad>* quadVertices, uint64_t* planeData) {
		uint64_t dataLen = 64;
		for (uint64_t row = 0; row < dataLen; row++) {
			uint64_t y = 0;
			while (y < Chunk::CHUNK_SIZE) {
				y += trailing_zeroes(planeData[row] >> y);
				
				if (y >= Chunk::CHUNK_SIZE)
					continue;

				uint64_t h = trailing_ones(planeData[row] >> y);
				uint64_t hMask = (h < 64) ? ((U_ONE << h) - 1) : ~0ull;
				uint64_t mask = hMask << y;

				uint64_t w = 1;

				while ((row + w) < Chunk::CHUNK_SIZE) {
					uint64_t nextRowH = (planeData[row + w] >> y) & hMask;
					if (nextRowH != hMask) break;

					planeData[row + w] &= ~mask;
					w += 1;
				}
				quadVertices->push_back(GreedyQuad(row, y, w, h));
				y += h;
			}
		}
	}

	vec3f ChunkMesh::WorldToSample(FaceDirection dir, uint64_t axis, uint64_t x, uint64_t y) {
		switch (dir) {
			case FaceDirection::up:
				return vec3f(x, axis + 1, y);
			case FaceDirection::down:
				return vec3f(x, axis, y);
			case FaceDirection::left:
				return vec3f(axis, y, x);
			case FaceDirection::right:
				return vec3f(axis + 1, y, x);
			case FaceDirection::forward:
				return vec3f(x, y, axis);
			case FaceDirection::back:
				return vec3f(x, y, axis + 1);
		}
	}

	void ChunkMesh::AppendVertices(GreedyQuad quad, std::vector<vec3f>* vertices, std::vector<uint32_t>* normals, FaceDirection faceDir, uint64_t axis, uint64_t blockType) {
		vertices->push_back(WorldToSample(faceDir, axis, quad.x, quad.y));
		vertices->push_back(WorldToSample(faceDir, axis, quad.x + quad.w, quad.y + quad.h));
		vertices->push_back(WorldToSample(faceDir, axis, quad.x, quad.y + quad.h));
		vertices->push_back(WorldToSample(faceDir, axis, quad.x + quad.w, quad.y));
		normals->push_back(NormalIndex(faceDir));
	}
}