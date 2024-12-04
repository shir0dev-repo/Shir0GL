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

#include <sogl/hashTable.hpp>
#include <sogl/rendering/mesh.hpp>
#include <sogl/transform/vec3f.hpp>

namespace sogl {
	typedef struct vertex {
		uint32_t p;
		uint32_t t;
		uint32_t n;
		inline uint32_t& operator[](const uint8_t& i) {
			if (i == 0) return p;
			if (i == 1) return t;
			if (i == 2) return n;
			else throw;
		}
	} vertex;

	typedef struct meshManager::faceData {
		vertex a;
		vertex b;
		vertex c;
		inline vertex& operator[](const uint8_t& i) {
			if (i == 0) return a;
			if (i == 1) return b;
			if (i == 2) return c;
			else throw;
		}
	} faceData;

	static hashTable<mesh> LoadedMeshes(64);

	bool meshManager::internal_findMesh(const char* alias, mesh*& m) {
		mesh* m1 = nullptr;
		if (LoadedMeshes.find(alias, m)) {
			m1 = m;
		}
		else {
			m1 = nullptr;
		}

		return m1;
	}

	void meshManager::internal_addMesh(mesh* mesh, const char* alias) {
		if (mesh != nullptr) {
			LoadedMeshes.insert(alias, mesh);
		} 
		else {
			std::cerr << "[Mesh Manager]: Error! Tried to add mesh " << alias << " to database, but " << alias << " is a null pointer!\n";
		}
	}

	void meshManager::internal_removeMesh(const char* alias, mesh* mesh) {
		if (mesh->positions != nullptr) {
			delete[] mesh->positions;
			mesh->positions = nullptr;
		}
		if (mesh->texCoords != nullptr) {
			delete[] mesh->texCoords;
			mesh->texCoords = nullptr;
		}
		if (mesh->normals != nullptr) {
			delete[] mesh->texCoords;
			mesh->texCoords = nullptr;
		}
		if (mesh->indices != nullptr) {
			delete[] mesh->indices;
			mesh->indices = nullptr;
		}

		LoadedMeshes.remove(alias);
	}

	void terminateMeshes() {
		for (uint64_t i = 0; i < LoadedMeshes.size; i++) {
			if (LoadedMeshes.data[i].key == nullptr) {
				continue;
			}

			mesh* mesh = LoadedMeshes.data[i].value;

			if (LoadedMeshes.data[i].isManaged) {
				std::cout << "[Mesh Manager]: Deleting mesh " << LoadedMeshes.data[i].key << ".\n";
				meshManager::internal_removeMesh(LoadedMeshes.data[i].key, mesh);
			}
		}
	}

	mesh* meshManager::internal_createMesh(const char* filePath, const char* alias) {
		std::vector<std::string> vertexList{};
		std::vector<std::string> texCoordList{};
		std::vector<std::string> normalList{};
		std::vector<std::string> faceList{};
		
		bool readSuccess = internal_parseOBJ(filePath, &vertexList, &texCoordList, &normalList, &faceList);
		if (!readSuccess) {
			std::cerr << "[Mesh Manager]: Could not read mesh " << alias << ". Specified file path does not exist.\n";
			return nullptr;
		}
		else {
			std::cout << "[Mesh Manager]: Successfully parsed " << alias << ". Converting to Mesh.\n";
		}

		// create vectors to store parsed data
		std::vector<vec3f> vertices{};
		std::vector<vec3f> texCoords{};
		std::vector<vec3f> normals{};
		std::vector<faceData> faces{};

		//meshManager::internal_processBatch(vertexList, vertices, 3);
		//meshManager::internal_processBatch(texCoordList, texCoords, 2);
		//meshManager::internal_processBatch(normalList, normals, 3);
		// send positions to worker thread
		std::thread positionWorker(&meshManager::internal_processBatch, std::ref(vertexList), std::ref(vertices), 3);
		std::cout << "|-- Parsing vertex positions in thread " << positionWorker.get_id() << "...\n";
		// send texcoords to worker thread
		std::thread texCoordWorker(&meshManager::internal_processBatch, std::ref(texCoordList), std::ref(texCoords), 2);
		std::cout << "|-- Parsing texture coordinates in thread " << texCoordWorker.get_id() << "...\n";
		// send normals to worker thread
		std::thread normalWorker(&meshManager::internal_processBatch, std::ref(normalList), std::ref(normals), 3);
		std::cout << "|-- Parsing vertex normals in thread " << normalWorker.get_id() << "...\n";
		
		std::thread faceWorker(&meshManager::internal_processFaces, std::ref(faceList), std::ref(faces));
		std::cout << "|-- Parsing triangle data in thread " << faceWorker.get_id() << "...\n";

		positionWorker.join();
		std::cout << "|-- Finished processing vertex positions.\n";
		texCoordWorker.join();
		std::cout << "|-- Finished processing texture coordinates.\n";
		normalWorker.join();
		std::cout << "|-- Finished processing vertex normals.\n";
		faceWorker.join();
		std::cout << "|-- Finished processing face data.\n";

		// create mesh and initialize values
		mesh* mesh = new sogl::mesh();

		uint64_t pointElementCount = vertices.size() * 3;
		uint64_t indicesElementCount = faces.size() * 3;

		mesh->size = pointElementCount * sizeof(float);

		mesh->positions = new float[pointElementCount];
		mesh->texCoords = new float[pointElementCount];
		mesh->normals = new float[pointElementCount];

		mesh->indexCount = indicesElementCount;
		mesh->indicesSize = indicesElementCount * sizeof(uint32_t);
		mesh->indices = new unsigned int[indicesElementCount];

		size_t faceCount = faces.size();

		// iterate through faces
		for (int i = 0; i < faceCount; i++) {
			faceData currentFace = faces.at(i);

			// iterate through face vertices
			for (int j = 0; j < 3; j++) {
				uint32_t currentVertexIndex = currentFace[j].p - 1;

				mesh->indices[(i * 3) + j] = currentVertexIndex;

				vec3f v = vertices.at(currentVertexIndex);
				mesh->positions[currentVertexIndex * 3] = v.x;
				mesh->positions[currentVertexIndex * 3 + 1] = v.y;
				mesh->positions[currentVertexIndex * 3 + 2] = v.z;

				vec3f vt = texCoords.at(currentFace[j].t - 1);
				mesh->texCoords[currentVertexIndex * 2] = vt.x;
				mesh->texCoords[currentVertexIndex * 2 + 1] = 1.0f - vt.y;

				vec3f n = normals.at(currentFace[j].n - 1);
				mesh->normals[currentVertexIndex * 3] = n.x;
				mesh->normals[currentVertexIndex * 3 + 1] = n.y;
				mesh->normals[currentVertexIndex * 3 + 2] = n.z;
			}
		}
		std::cout << "|-- Successfully created mesh " << alias << ".\n";
		return mesh;
	}

	bool meshManager::internal_parseOBJ(const char* filePath,
		std::vector<std::string>* vertices,
		std::vector<std::string>* texCoords,
		std::vector<std::string>* normals,
		std::vector<std::string>* faces)
	{
		std::ifstream obj(filePath, std::ifstream::in);
		if (!obj.is_open()) {
			return false;
		}

		std::string currentLine;

		// v, vt, vn, or f for now
		std::string type;
		size_t offset;

		while (std::getline(obj, currentLine)) {
			offset = currentLine.find_first_of(' ');
			if (offset == std::string::npos) continue;

			type = currentLine.substr(0, offset);

			// will erase the type identifier at the start of the string, and in the case of v, the space as well
			currentLine.erase(0, 2);
			// erases up to the first occurence of actual data.

			currentLine.erase(0, currentLine.find_first_not_of(' '));

			if (type == "v") {
				vertices->push_back(currentLine + '\n');
			}
			else if (type == "vt") {
				texCoords->push_back(currentLine + '\n');
			}
			else if (type == "vn") {
				normals->push_back(currentLine + '\n');
			}
			else if (type == "f") {
				faces->push_back(currentLine + '\n');
			}
			else continue;  // ignore all other data
		}

		obj.close();
		return true;
	}

	void meshManager::internal_processBatch(std::vector<std::string>& batch, std::vector<vec3f>& outData, size_t stride) {
		std::stringstream ss;
		for (std::string face : batch) {
			ss << face;
		}
		ss << "eof";
		
		std::string line;
		while (true) {
			std::getline(ss, line);
			if (line == "eof") break;

			vec3f v;
			for (int i = 0; i < stride; i++) {
				size_t separator = line.find_first_of(' ');
				// parse float out of substring
				std::string valStr = line.substr(0, separator);
				v[i] = std::stof(valStr);
				// delete entry after setting component of v
				line.erase(0, separator + 1);
			}

			// add data to list
			outData.push_back(v);
		}
	}

	void meshManager::internal_processFaces(std::vector<std::string>& faces, std::vector<faceData>& outData) {
		std::stringstream ss;
		for (std::string face : faces) {
			ss << face;
		}
		ss << "eof";

		std::string indices[3];

		size_t separator;
		size_t end;

		std::string line;
		while (true) {
			std::getline(ss, line);

			if (line == "eof") break;

			// split line into 3 entries; one for each vertex
			for (int i = 0; i < 3; i++) {
				// find space and copy entry to indices
				separator = line.find_first_of(' ');
				indices[i] = line.substr(0, separator);
				// clear copied entry
				line.erase(0, separator + 1);
			}
			faceData currentFace;

			// index into indices array
			for (int i = 0; i < 3; i++) {
				vertex v;

				// index into current indices
				for (int j = 0; j < 3; j++) {
					end = indices[i].find_first_of('/');

					if (indices[i].find("//") == std::string::npos) {
						v[j] = std::stoi(indices[i].substr(0, end));
					}
					else { // empty index
						v[j] = 0;
					}
					indices[i].erase(0, end + 1);
				}
				currentFace[i] = v;
			}

			outData.push_back(currentFace);
		}
	}
}