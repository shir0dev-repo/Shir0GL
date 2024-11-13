// STD:
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

// SOGL:
#include "../rendering/mesh/mesh.hpp"
#include "../transform/vec3f.hpp"

namespace sogl {
	typedef struct faceData {
		GLuint positionIndices[3];
		GLuint texCoordIndices[3];
		GLuint normalIndices[3];
	} faceData;

	mesh::mesh(const char* filePath) {
		std::ifstream obj(filePath, std::ifstream::in);
		assert(obj.good());

		std::string currentLine;

		// v, vt, vn, or f for now
		std::string type;

		// worker threads
		std::thread positionWorker;
		std::thread texCoordWorker;
		std::thread normalWorker;
		std::thread faceWorker;

		// vertex data to be passed to worker threads
		std::string positionBatch;
		std::string texCoordBatch;
		std::string normalBatch;
		std::string faceBatch;

		// the offset before the actual data (i.e. when line begins with "vt ", offset = 2)
		size_t offset;

		GLuint vertexCount = 0, texCoordCount = 0, normalCount = 0;
		std::vector<vec3f> vertices{};
		std::vector<vec3f> texCoords{};
		std::vector<vec3f> normals{};
		std::vector<faceData> faces{};

		while (obj.good()) {
			std::getline(obj, currentLine);

			offset = currentLine.find_first_of(' ');
			if (offset == std::string::npos) continue;

			type = currentLine.substr(0, offset);

			// will erase the type identifier at the start of the string, and in the case of v, the space as well
			currentLine.erase(0, 2);
			// erases up to the first occurence of actual data.

			
			currentLine.erase(0, currentLine.find_first_not_of(' '));

			/* Since validly generated .obj files have data grouped together (afaik), once we hit the next "type",
			we can safely dispatch a thread to convert the gathered string data into our final arrays.*/

			if (type == "v") {
				positionBatch.append(currentLine + '\n');
				vertexCount++;
			}
			else if (type == "vt") {
				if (!positionWorker.joinable()) { // check if thread has been started yet
					positionWorker = std::thread(&mesh::processBatch, this, positionBatch, std::ref(vertices),
						vertexCount, 3);
				}

				texCoordBatch.append(currentLine + '\n');
				texCoordCount++;
			}
			else if (type == "vn") {
				if (!texCoordWorker.joinable()) {
					texCoordWorker = std::thread(&mesh::processBatch, this, texCoordBatch, std::ref(texCoords),
						texCoordCount, 2);
				}

				normalBatch.append(currentLine + '\n');
				normalCount++;
			}
			else if (type == "f") { // face data started, break from loop
				// no need to check if thread joinable, as we break immediately after this.
				normalWorker = std::thread(&mesh::processBatch, this, normalBatch, std::ref(normals),
					normalCount, 3);

				faceBatch.append(currentLine + '\n');
				break;
			}
			else { // ignore all other data
				continue;
			}
		}

		// while the vertex data is being parsed, iterate through the rest of the file on the main thread
		while (obj.good()) {
			std::getline(obj, currentLine);

			// ignore non-face related data
			if (currentLine[0] != 'f') continue;

			offset = currentLine.find_first_of(' ');
			currentLine.erase(0, offset + 1);
			faceBatch.append(currentLine + '\n');
		}

		// file reading done, make sure to close !
		obj.close();

		// start face iteration
		faceWorker = std::thread(&mesh::processFaces, this, faceBatch, std::ref(faces));

		this->positionByteSize = 3 * (GLsizeiptr)vertexCount * sizeof(float);
		this->positions = new float[3 * vertexCount];

		this->texCoordByteSize = 2 * (GLsizeiptr)texCoordCount * sizeof(float);
		this->texCoords = new float[2 * texCoordCount];

		this->normalByteSize = 3 * (GLsizeiptr)normalCount * sizeof(float);
		this->normals = new float[3 * normalCount];

		positionWorker.join();
		texCoordWorker.join();
		normalWorker.join();
		faceWorker.join();
		
		this->indicesByteSize = 3 * (GLsizeiptr)faces.size() * sizeof(GLuint);
		this->indices = new GLuint[3 * faces.size()];

		// order vtn indices to align for openGL
		for (int i = 0; i < faces.size(); i++) {
			faceData currentFace = faces.at(i);

			for (int j = 0; j < 3; j++) {
				int currentVertexIndex = currentFace.positionIndices[j];
				indices[i + j] = currentVertexIndex;

				this->positions[currentVertexIndex * 3] = vertices.at(currentFace.positionIndices[j]).x;
				this->positions[currentVertexIndex * 3 + 1] = vertices.at(currentFace.positionIndices[j]).y;
				this->positions[currentVertexIndex * 3 + 2] = vertices.at(currentFace.positionIndices[j]).z;

				this->texCoords[currentVertexIndex * 2] = texCoords.at(currentFace.texCoordIndices[j]).x;
				this->texCoords[currentVertexIndex * 2 + 1] = 1.0f - texCoords.at(currentFace.texCoordIndices[j]).y;

				this->normals[currentVertexIndex * 3] = normals.at(currentFace.normalIndices[j]).x;
				this->normals[currentVertexIndex * 3 + 1] = normals.at(currentFace.normalIndices[j]).y;
				this->normals[currentVertexIndex * 3 + 2] = normals.at(currentFace.normalIndices[j]).z;
			}
		}
	}

	mesh::~mesh() {
		if (positions != nullptr) {
			delete[] positions;
			positions = nullptr;
		}
		if (texCoords != nullptr) {
			delete[] texCoords;
			texCoords = nullptr;
		}
		if (normals != nullptr) {
			delete[] texCoords;
			texCoords = nullptr;
		}
		if (indices != nullptr) {
			delete[] indices;
			indices = nullptr;
		}
	}

	void mesh::processBatch(std::string batch, std::vector<vec3f>& data, GLuint count, size_t stride) {
		std::thread::id curr_id = std::this_thread::get_id();

		std::cout << "Processing vertex data batch in thread " << curr_id << ".\n";

		using namespace std::chrono_literals;

		std::stringstream ss;
		ss << batch << "eof";

		std::string line;
		int idx = 0;

		while (true) {
			idx++;
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
			data.push_back(v);
		}

		std::cout << "Finished processing vertex data batch in thread " << curr_id << ".\n";
	}

	void mesh::processFaces(std::string faceBatch, std::vector<faceData>& faces) {
		std::thread::id curr_id = std::this_thread::get_id();
		std::cout << "Processing faces in thread " << curr_id << ".\n";

		std::stringstream ss;
		ss << faceBatch << "eof";

		std::string line;
		std::string indices[3];

		const char delim = '/';
		size_t separator;

		size_t end;

		while (true) {
			std::getline(ss, line);

			if (line == "eof") break;

			// split line into 3 entries; one for each vertex
			for (int i = 0; i < 3; i++) {
				separator = line.find_first_of(' ');
				indices[i] = line.substr(0, separator);

				// clear copied entry
				line.erase(0, separator + 1);
			}

			faceData face;

			// parse vertices into usable data
			for (int i = 0; i < 3; i++) {
				GLuint idx[3];

				for (int j = 0; j < 3; j++) {
					end = indices[i].find_first_of('/');

					// empty index
					if (indices[i].size() > 2 && indices[i].at(end + 1) == '/') {
						idx[j] = 0;
					}
					else {
						idx[j] = std::stoi(indices[i].substr(0, end)) - 1;
					}

					indices[i].erase(0, end + 1);
				}

				face.positionIndices[i] = idx[0];
				face.texCoordIndices[i] = idx[1];
				face.normalIndices[i] = idx[2];
			}

			faces.push_back(face);
		}

		std::cout << "Finished processing faces in thread " << curr_id << ".\n";
	}

}