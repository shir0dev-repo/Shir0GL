#pragma once

#include <stdint.h>
#include <sogl/structure/octree.h>

struct FastNoise;

namespace sogl {
	enum voxelType : uint8_t {
		AIR = 0,
		STONE = 1,
		DIRT = 2,
		GRASS = 3,
		COBBLESTONE = 4
	};
	
	typedef struct voxel {
		uint8_t type;
		static color getColorForVoxel(const voxel* v);
	} voxel;

	typedef struct Chunk {
	private:
		static struct ShaderProgram* chunkShader;
		static struct Mesh* cubeMesh;
		static FastNoise* noiseData;
	public:
		static const uint64_t CHUNK_SIZE = 64;
		static const uint16_t CHUNK_SIZE_X = 64;
		static const uint16_t CHUNK_SIZE_Z = 64;
		static const uint16_t CHUNK_SIZE_Y = 64;

		static struct color voxelColors[5];
	private:
		vec3f chunkCoords;
		struct VertexArray* vao;
		uint32_t voxelBufferID;
		struct GLMappedBuffer* voxelBuffer;
		voxel* voxels;
		static bool indexInRange(const uint16_t index);
	public:
		static void initialize();
		Chunk(const vec3f& chunkCoords);
		void draw();

		voxel* const getVoxel(const uint16_t x, const uint16_t y, const uint16_t z);
		bool getVoxelNeighbours(const uint16_t x, const uint16_t y, const uint16_t z, voxel**& outNeighbours);
		void setVoxel(voxel* voxel, const voxelType type);
	};
}