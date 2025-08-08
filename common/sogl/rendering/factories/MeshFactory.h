#pragma once

#include <stdint.h>
#include <vector>

#include <sogl/structure/Dictionary.h>
#include <sogl/structure/hashTable.hpp>
#include <sogl/structure/linkedList.h>
#include <sogl/structure/pair.h>

#include <sogl/rendering/gl/mesh/Mesh.h>
#include <sogl/rendering/gl/mesh/InstancedMesh.h>

namespace sogl {
	class MeshFactory {
		static hashTable<Mesh> LoadedMeshes;
		static Dictionary<const Mesh*, linkedList<InstancedMesh*>*> MeshInstances;
		static Dictionary<const Mesh*, const struct VertexArray> MeshBufferDictionary;

		static void LoadToBuffer(const Mesh* meshAsset, const char* alias);
	public:
		static const Mesh* CreateNew( const char* filePath, const char* alias = "" );
		static const Mesh* CreateNew( const float* vertices, const float* uvs, const float* normals, const uint64_t* indices, const char* alias = "" );
		static bool Find(const char* alias, Mesh*& outMesh);
		static bool DeleteMesh(Mesh* mesh);

		static bool FindBuffer(const Mesh* meshAsset, const struct VertexArray*& outVAO);

		static InstancedMesh* const CreateInstance(const Mesh* baseAsset);
		static InstancedMesh* const Clone(InstancedMesh*& copy, const InstancedMesh* base);
		static void UpdateInstance(InstancedMesh* instance, const Mesh* meshAsset);
		static bool FindAllInstances(const char* alias, InstancedMesh*& outMeshArray, uint32_t* outArraySize);
		static bool DeleteInstance(InstancedMesh* instance);

		static bool Remove(const char* alias);
		static bool Remove(Mesh* mesh);
		
		static void Terminate();
	};
}