#pragma once

#include <stdint.h>
#include <sogl/structure/octree.h>

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

	typedef struct chunk {
	private:
		static struct shaderProgram* chunkShader;
		static struct mesh* cubeMesh;
		static struct FastNoise* noiseData;
	public:
		static const uint16_t CHUNK_SIZE_X = 16;
		static const uint16_t CHUNK_SIZE_Z = 16;
		static const uint16_t CHUNK_SIZE_Y = 64;

		static struct color voxelColors[5];
	private:
		vec3f chunkCoords;
		struct vertexArrayObject* vao;
		uint32_t voxelBufferID;
		struct mappedBuffer* voxelBuffer;
		voxel* voxels;
		static bool indexInRange(const uint16_t index);
	public:
		static void initialize();
		chunk(const vec3f& chunkCoords);
		void draw();

		voxel* const getVoxel(const uint16_t x, const uint16_t y, const uint16_t z);
		void setVoxel(voxel* voxel, const voxelType type);
	};
}