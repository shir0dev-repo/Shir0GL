#pragma once

#include <string>
#include <vector>

namespace sogl {
	typedef unsigned int GLuint;
	typedef unsigned long long GLsizeiptr;
	
	struct faceData;
	struct vec3f;

	struct mesh {
		GLsizeiptr positionByteSize;
		GLsizeiptr texCoordByteSize;
		GLsizeiptr normalByteSize;
		GLsizeiptr indicesByteSize;

		float* positions;
		float* texCoords;
		float* normals;
		GLuint* indices;

		mesh(const char* filePath);
		mesh(int positionCount, const float* positions, const float* texCoords, const float* normals, 
			const uint16_t* indices, const int iCount);

		~mesh();

	private:
		void processBatch(std::string stringData, std::vector<vec3f>& outData, GLuint count, size_t stride);
		void processFaces(std::string faceBatch, std::vector<faceData>& outData);
	};
}