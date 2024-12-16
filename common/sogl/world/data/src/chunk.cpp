#include <GLEW/glew.h>

#include <sogl/world/data/chunk.h>
#include <sogl/rendering/color.hpp>
#include <sogl/debug/debug.h>
#include <sogl/rendering/gl/mappedBuffer.h>
#include <sogl/rendering/factories/shaderFactory.hpp>
#include <sogl/rendering/mesh.hpp>
#include <sogl/rendering/gl/vertexArrayObject.hpp>
#include <sogl/noise/fastNoise.h>

namespace sogl {
	shaderProgram* chunk::chunkShader = nullptr;
	mesh* chunk::cubeMesh = nullptr;

	color chunk::voxelColors[5] = {
		color::WHITE,
		color(0.6, 0.6, 0.6, 1),
		color(0.588, 0.294, 0, 1),
		color::GREEN,
		color(0.3, 0.3, 0.3, 1)
	};

	color voxel::getColorForVoxel(const voxel* v) {
		switch (v->type) {
		case AIR:
			return color::WHITE;
		case STONE:
			return color(0.6, 0.6, 0.6);
		case DIRT:
			return color(0.588, 0.294, 0);
		case GRASS:
			return color::GREEN;
		case COBBLESTONE:
			return color(0.3, 0.3, 0.3);
		}
	}

	void chunk::initialize() {
		chunkShader = shaderFactory::createNew("assets/shader/voxel.vert", "assets/shader/voxel.frag", "chunkShader");
		chunkShader->use();
		glUniform3i(glGetUniformLocation(chunkShader->programID, "chunkSize"), CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
		chunkShader->stop();
		noiseData = new FastNoise(time(0));
		
	}

	chunk::chunk(const vec3f& chunkCoords) : chunkCoords(chunkCoords) {
		const uint32_t totalVoxels = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;
		uint32_t flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
		this->voxelBuffer = nullptr;

		this->voxels = new voxel[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];

		voxel* v = nullptr;
		for (int y = 0; y < CHUNK_SIZE_Y; y++) {
			for (int x = 0; x < CHUNK_SIZE_X; x++) {
				for (int z = 0; z < CHUNK_SIZE_Z; z++) {
					if ((v = getVoxel(x, y, z)) != nullptr) {
						float noise = noiseData->GetPerlin(x, y, z);
						
						if (noise < 0.2)
							v->type = AIR;
						else if (noise < 0.4)
							v->type = STONE;
						else if (noise < 0.6)
							v->type = DIRT;
						else if (noise < 0.8)
							v->type = GRASS;
						else
							v->type = COBBLESTONE;
					}
				}
			}
		}
		if (cubeMesh == nullptr) {
			findMesh("cube", cubeMesh);
		}

		assert(cubeMesh != nullptr);

		vao = new vertexArrayObject(*cubeMesh);
		glBindVertexArray(vao->ID);

		glGenBuffers(1, &this->voxelBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, voxelBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(voxel) * totalVoxels, this->voxels, GL_STATIC_DRAW);
		glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
		
		glVertexAttribDivisor(3, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	voxel* const chunk::getVoxel(const uint16_t x, const uint16_t y, const uint16_t z) {
		if (voxels == nullptr)
			return nullptr;

		int index = (z * CHUNK_SIZE_X * CHUNK_SIZE_Y) + (y * CHUNK_SIZE_X) + x;
		if (!indexInRange(index)) {
			return nullptr;
		}

		return &voxels[index];
	}

	bool chunk::indexInRange(const uint16_t index) {
		return index >= 0 && index < CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;
	}

	void chunk::draw() {
		chunkShader->use();
		chunkShader->uploadUniform("chunkCoord", chunkCoords);

		//glBindBuffer(GL_ARRAY_BUFFER, vboID);
		vao->bind();
		glEnableVertexAttribArray(3);
		
		uint32_t count = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;
		glDrawElementsInstanced(GL_TRIANGLES, vao->pointCount, GL_UNSIGNED_INT, 0, count);
		
		glDisableVertexAttribArray(3);
		vao->unbind();
		chunkShader->stop();
	}
}