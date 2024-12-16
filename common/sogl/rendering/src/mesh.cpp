// STD:
#include <iostream>

// SOGL:
#include <sogl/structure/hashTable.hpp>
#include <sogl/transform/vec3f.hpp>
#include <sogl/rendering/mesh.hpp>

namespace sogl {
	mesh::mesh() : size(0), indicesSize(0), indexCount(0), positions(nullptr), texCoords(nullptr), normals(nullptr), indices(nullptr) {}

	mesh* createMesh(const char* filePath, const char* alias) {
		char* aliasUsed;
		if (strcmp(alias, "") == 0) {
			aliasUsed = const_cast<char*>(filePath);
		}
		else {
			aliasUsed = const_cast<char*>(alias);
		}
		
		mesh* m = nullptr;

		if (meshManager::internal_findMesh(alias, m)) {
			std::cout << "[Mesh Manager]: Mesh with name " << alias << " already exists!\n";
			return m;
		}
		else {
			std::cout << "[Mesh Manager]: Loading mesh " << alias << "...\n";
		}

		m = meshManager::internal_createMesh(filePath, aliasUsed);
		if (m != nullptr) {
			meshManager::internal_addMesh(m, alias);
			return m;
			
		} 
		
		return nullptr;
	}

	bool findMesh(const char* alias, mesh*& mesh) {
		return meshManager::internal_findMesh(alias, mesh);
	}
}