#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <format>
#include <limits>
#include <vector>
#include <thread>
#include <functional> // for std::ref
#include <cassert>

#include <sogl/structure/Array.h>
#include <sogl/structure/hashTable.hpp>
#include <sogl/transform/vec3f.hpp>
#include <sogl/rendering/gl/VertexArray.h>
#include <sogl/rendering/factories/FactoryUtils.h>
#include <sogl/rendering/gl/mesh/MeshUtils.h>
#include <sogl/rendering/factories/MeshFactory.h>

namespace sogl {

	hashTable<Mesh> MeshFactory::LoadedMeshes(64);
	Dictionary<const Mesh*, linkedList<InstancedMesh*>*> MeshFactory::MeshInstances = Dictionary<const Mesh*, linkedList<InstancedMesh*>*>();
	Dictionary<const Mesh*, const VertexArray> MeshFactory::MeshBufferDictionary = Dictionary<const Mesh*, const VertexArray>();

	void MeshFactory::LoadToBuffer(const Mesh* meshAsset, const char* alias) {
		if (MeshBufferDictionary.ContainsKey(meshAsset)) {
			printf("|-- Mesh \"%s\" already exists on the GPU!\n", alias);
			return;
		}

		VertexArray meshBuffers(*meshAsset);
		MeshBufferDictionary.Add(meshAsset, meshBuffers);
	}

	const Mesh* MeshFactory::CreateNew(const char* filePath, const char* alias) {
		char* aliasUsed = nullptr;
		FactoryUtils::CreateAlias(alias, "Mesh", LoadedMeshes.size, aliasUsed);

		printf("[Mesh Factory]: Creating new mesh \"%s\".\n", aliasUsed);
		Mesh* m = nullptr;
		if (LoadedMeshes.find(aliasUsed, m)) {
			printf("[Mesh Factory]: Failed to create mesh \"%s\".\n", aliasUsed);
			printf("|-- A mesh with name \"%s\" already exists!\n", aliasUsed);
			return m;
		}

		if (MeshUtils::CreateOBJMesh(filePath, m)) {
			LoadedMeshes.insert(aliasUsed, m);

			printf("[Mesh Factory]: Sending mesh data to OpenGL.\n");
			LoadToBuffer(m, aliasUsed);

			printf("[Mesh Factory]: Successfully created mesh \"%s\".\n", aliasUsed);
			printf("|-- Vertices: %lu\n", m->VertexCount());
			printf("|-- Texture Coordinates: %lu\n", m->TexCoordCount());
			printf("|-- Normals: %lu\n", m->NormalCount());
			printf("|-- Indices: %lu\n", m->IndexCount());
			
			return m;
		}
	}

	const Mesh* MeshFactory::CreateNew(const float* vertices, const float* uvs, const float* normals, const uint64_t* indices, const char* alias) {
		uint32_t verticesLength = Array<float>::Length(vertices);
		uint32_t uvsLength = Array<float>::Length(uvs);
		uint32_t normalsLength = Array<float>::Length(normals);
		uint32_t indicesLength = Array<uint64_t>::Length(indices);

		if ((verticesLength | uvsLength | normalsLength | indicesLength) == 0) {
			std::cout << "[Mesh Factory]: Cannot create mesh with no data!" << std::endl;
			return nullptr;
		}

		char* aliasUsed = nullptr;
		FactoryUtils::CreateAlias(alias, "Mesh", LoadedMeshes.size, aliasUsed);

		printf("[Mesh Factory]: Creating new mesh \"%s\".\n", aliasUsed);

		Mesh* m = new Mesh();

		if (verticesLength > 0) {
			m->m_vertices = new float[verticesLength];
			m->m_vertexCount = verticesLength;
		}
		if (uvsLength > 0) {
			m->m_texCoords = new float[verticesLength];
			m->m_texCoordCount = uvsLength;
		}
		if (normalsLength > 0) {
			m-> m_normals = new float[verticesLength];
			m-> m_normalCount = normalsLength;
		}
		if (indicesLength > 0) {
			m->m_indices = new uint32_t[indicesLength];
			m->m_indexCount = indicesLength;
		}
		
		LoadedMeshes.insert(aliasUsed, m);
		printf("[Mesh Factory]: Successfully created mesh \"%s\".\n", aliasUsed);
		printf("|-- Vertices: %lu\n", m->m_vertexCount);
		printf("|-- Texture Coordinates: %lu\n", m->m_texCoordCount);
		printf("|-- Normals: %lu\n", m->m_normalCount);
		printf("|-- Indices: %lu\n", m->m_indexCount);

		return m;
	}

	bool MeshFactory::Find(const char* alias, Mesh*& outMesh) {
		return LoadedMeshes.find(alias, outMesh);
	}

	bool MeshFactory::DeleteMesh(Mesh* mesh) {
		if (mesh->m_vertices) {
			delete[] mesh->m_vertices;
			mesh->m_vertices = nullptr;
		}
		if (mesh->m_texCoords) {
			delete[] mesh->m_texCoords;
			mesh->m_texCoords = nullptr;
		}
		if (mesh->m_normals) {
			delete[] mesh->m_normals;
			mesh->m_texCoords = nullptr;
		}
		if (mesh->m_indices) {
			delete[] mesh->m_indices;
			mesh->m_indexCount = 0;
		}
		
		for (uint64_t i = 0; i < LoadedMeshes.size; i++) {
			char* key = nullptr;
			if ((LoadedMeshes.data[i].key = key) == nullptr) continue;

			else if (LoadedMeshes.data[i].value != mesh) continue;
			
			LoadedMeshes.remove(key);
			return true;
		}

		return false;
	}

	bool MeshFactory::FindBuffer(const Mesh* meshAsset, const VertexArray*& outVAO) {
		if (MeshBufferDictionary.ContainsKey(meshAsset)) {
			outVAO = &MeshBufferDictionary[meshAsset];
			return true;
		}
		
		return false;
	}

	InstancedMesh* const MeshFactory::CreateInstance(const Mesh* baseAsset) {
		InstancedMesh* iMesh = new InstancedMesh();
		iMesh->SetMesh(baseAsset);
		
		if (!MeshInstances.ContainsKey(baseAsset)) {
			linkedList<InstancedMesh*>* instances = new linkedList<InstancedMesh*>();
			MeshInstances.Add(baseAsset, instances);
		}
		else {
			MeshInstances[baseAsset]->add(iMesh);
		}

		return iMesh;
	}

	InstancedMesh* const MeshFactory::Clone(InstancedMesh*& copy, const InstancedMesh* base) {
		assert(copy == nullptr && base != nullptr);
		InstancedMesh* iMesh = new InstancedMesh();
		
		const Mesh* asset = base->GetMesh();
		iMesh->SetMesh(asset);

		//key guaranteed as this mesh has the same asset
		MeshInstances[asset]->add(iMesh);
		copy = iMesh;
		return copy;
	}

	void MeshFactory::UpdateInstance(InstancedMesh* instance, const Mesh* newMeshAsset) {
		linkedList<InstancedMesh*>* currentEntry = MeshInstances[instance->GetMesh()];
		for (uint32_t i = 0; i < currentEntry->size(); i++) {
			if ((*currentEntry)[i] == instance) {
				currentEntry->remove(instance);
				break;
			}
		}
		instance->SetMesh(newMeshAsset);

		if (!MeshInstances.ContainsKey(newMeshAsset)) {
			linkedList<InstancedMesh*>* instances = new linkedList<InstancedMesh*>();
			instances->add(instance);
			MeshInstances.Add(newMeshAsset, instances);
		}
		else {
			MeshInstances[newMeshAsset]->add(instance);
		}
	}

	//NYI
	bool MeshFactory::FindAllInstances(const char* alias, InstancedMesh*& outMeshArray, uint32_t* outArraySize) {
		return false;
	}

	bool MeshFactory::DeleteInstance(InstancedMesh* instance) {
		MeshInstances[instance->GetMesh()]->remove(instance);
		instance->SetMesh(nullptr);

		::operator delete(instance);
		return true;
	}

	bool MeshFactory::Remove(const char* alias) {
		Mesh* mesh = nullptr;
		if (!Find(alias, mesh)) {
			printf("[Mesh Factory]: Could not remove mesh %s!\n", alias);
			printf("|-- Mesh %s does not exist!\n", alias);
			return false;
		}

		return DeleteMesh(mesh);
	}

	bool MeshFactory::Remove(Mesh* mesh) {
		for (uint64_t i = 0; i < LoadedMeshes.size; i++) {
			if (LoadedMeshes.data[i].value == mesh) {
				return DeleteMesh(mesh);
			}
		}

		printf("[Mesh Factory]: Could not remove referenced mesh!\n");
		printf("|-- Mesh does not exist!\n");
		return false;
	}

	void MeshFactory::Terminate() {
		MeshBufferDictionary.Clear();

		for (uint64_t i = 0; i < LoadedMeshes.size; i++) {
			char* key = nullptr;
			if ((key = LoadedMeshes.data[i].key) == nullptr) {
				continue;
			}

			if (LoadedMeshes.data[i].isManaged) {
			}

			std::cout << "[Mesh Manager]: Deleting mesh " << key << ".\n";
			LoadedMeshes.remove(key);
		}
	}
}