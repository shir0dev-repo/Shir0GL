#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>

#include <sogl/transform/vec3f.hpp>
#include <sogl/rendering/gl/mesh/Mesh.h>
#include <sogl/rendering/gl/mesh/InstancedMesh.h>

#define VECTOR(x) std::vector<x>

namespace sogl {
	typedef struct vertex {
		uint64_t p;
		uint64_t t;
		uint64_t n;
		inline uint64_t& operator[](const uint8_t& i) {
			if (i == 0) return p;
			if (i == 1) return t;
			if (i == 2) return n;
			else throw;
		}
	} vertex;

	typedef struct faceData {
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

	struct MeshUtils {
		template<class T>
		using Vec = std::vector<T>;
		using StrVec = std::vector<std::string>;

		static bool CreateOBJMesh(const char* filePath, Mesh*& outMesh) {
			StrVec vertexList{};
			StrVec texCoordList{};
			StrVec normalList{};
			StrVec faceList{};
			// ensure file gets parsed correctly
			bool readOK = MeshUtils::ParseOBJFile(filePath, &vertexList, &texCoordList, &normalList, &faceList);
			// couldn't parse file
			if (!readOK) {
				return false;
			}
			Vec<vec3f> vertices{};
			Vec<vec3f> texCoords{};
			Vec<vec3f> normals{};
			Vec<faceData> faces{};

			// split OBJ parsing to multiple threads
			CreateOBJData(vertexList, texCoordList, normalList, faceList, vertices, texCoords, normals, faces);

			Mesh* mesh = new Mesh();

			// apply the parsed data to the mesh object
			ApplyDataToMesh(mesh, vertices, texCoords, normals, faces);
			outMesh = mesh;
			return true;
		}

		static void CreateOBJData(StrVec& vertexStrVec, StrVec& tCoordStrVec, StrVec& normalStrVec, StrVec facesStrVec,
			Vec<vec3f>& vertices, Vec<vec3f>& texCoords, Vec<vec3f>& normals, Vec<faceData>& faces) {
			
			// send positions to worker thread
			std::thread positionWorker(&ParseBatch, std::ref(vertexStrVec), std::ref(vertices), 3);
			printf("|-- Parsing vertex positions in thread %lu.\n", positionWorker.get_id());
			// send texcoords to worker thread
			std::thread texCoordWorker(&MeshUtils::ParseBatch, std::ref(tCoordStrVec), std::ref(texCoords), 2);
			printf("|-- Parsing texture coordinates in thread %lu.\n", texCoordWorker.get_id());
			// send normals to worker thread
			std::thread normalWorker(&MeshUtils::ParseBatch, std::ref(normalStrVec), std::ref(normals), 3);
			printf("|-- Parsing vertex normals in thread %lu.\n", normalWorker.get_id());
			// send faces to worker thread
			std::thread faceWorker(&MeshUtils::ParseFaceBatch, std::ref(facesStrVec), std::ref(faces));
			printf("|-- Parsing triangle data in thread %lu.\n", faceWorker.get_id());
			// wait for threads to finish (should be done already, depending on the mesh.)
			// here, we wait for the face worker last because it typically contains the most data		
			positionWorker.join();
			printf("|-- Finished processing vertex positions.\n");
			texCoordWorker.join();
			printf("|-- Finished processing texture coordinates.\n");
			normalWorker.join();
			printf("|-- Finished processing vertex normals.\n");
			faceWorker.join();
			printf("|-- Finished processing face data.\n");
		}

		static bool ParseOBJFile(const char* filePath, Vec<std::string>* vertices, Vec<std::string>* texCoords, Vec<std::string>* normals,
			Vec<std::string>* faces)
		{
			std::ifstream obj(filePath, std::ifstream::in);
			if (!obj.is_open()) {
				return false;
			}

			std::string currentLine;

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

		static void ParseBatch(Vec<std::string>& stringBatch, Vec<vec3f>& outData, const uint8_t stride) {
			std::stringstream ss;
			for (std::string face : stringBatch) {
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

		static void ParseFaceBatch(Vec<std::string>& faces, Vec<faceData>& outData) {
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

		static void ApplyDataToMesh(Mesh* mesh, Vec<vec3f>& vertices, Vec<vec3f>& texCoords, Vec<vec3f>& normals, Vec<faceData>& faces) {
			mesh->m_vertexCount = vertices.size() * 3;
			mesh->m_vertices = new float[mesh->m_vertexCount];
			mesh->m_texCoordCount = texCoords.size() * 3;
			mesh->m_texCoords = new float[mesh->m_vertexCount];
			mesh->m_normalCount = normals.size() * 3;
			mesh->m_normals = new float[mesh->m_vertexCount];
			mesh->m_indexCount = faces.size() * 3;
			mesh->m_indices = new uint32_t[mesh->m_indexCount];
			
			size_t faceCount = faces.size();

			// iterate through faces
			for (int i = 0; i < faceCount; i++) {
				faceData currentFace = faces.at(i);

				// iterate through face vertices
				for (int j = 0; j < 3; j++) {
					uint64_t currentVertexIndex = currentFace[j].p - 1;

					mesh->m_indices[(i * 3) + j] = currentVertexIndex;

					vec3f v = vertices.at(currentVertexIndex);
					mesh->m_vertices[currentVertexIndex * 3] = v.x;
					mesh->m_vertices[currentVertexIndex * 3 + 1] = v.y;
					mesh->m_vertices[currentVertexIndex * 3 + 2] = v.z;

					vec3f vt = texCoords.at(currentFace[j].t - 1);
					mesh->m_texCoords[currentVertexIndex * 2] = vt.x;
					mesh->m_texCoords[currentVertexIndex * 2 + 1] = 1.0f - vt.y;

					vec3f n = normals.at(currentFace[j].n - 1);
					mesh->m_normals[currentVertexIndex * 3] = n.x;
					mesh->m_normals[currentVertexIndex * 3 + 1] = n.y;
					mesh->m_normals[currentVertexIndex * 3 + 2] = n.z;
				}
			}
		}
	};
}