#pragma once

#include <string>
#include <vector>

namespace sogl {
	typedef struct mesh {
		uint64_t size;
		uint64_t indicesSize;
		uint64_t indexCount;


		uint32_t* indices;
		float* positions;
		float* texCoords;
		float* normals;

		mesh();
	} mesh;

	struct vec3f;
	namespace meshManager {
		struct faceData;
		
		void internal_processBatch(std::vector<std::string>& batch, std::vector<vec3f>& outData, size_t stride);
		void internal_processFaces(std::vector<std::string>& faces, std::vector<faceData>& outData);

		bool internal_parseOBJ(const char* filePath,
			std::vector<std::string>* vertices,
			std::vector<std::string>* texCoords,
			std::vector<std::string>* normals,
			std::vector<std::string>* faces);

		mesh* internal_createMesh(const char* filePath, const char* alias);
		void internal_addMesh(mesh* mesh, const char* alias);
		bool internal_findMesh(const char* alias, mesh*& mesh);
		void internal_removeMesh(const char* alias, mesh* mesh);
	}

	mesh* createMesh(const char* filePath, const char* alias = "");
	bool findMesh(const char* alias, mesh*& outMesh);
	
	void terminateMeshes();
}