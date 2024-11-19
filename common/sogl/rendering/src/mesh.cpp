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
#include <sogl/rendering/mesh.hpp>
#include <sogl/transform/vec3f.hpp>

namespace sogl {
	typedef struct vertex {
		GLuint p;
		GLuint t;
		GLuint n;
		inline GLuint& operator[](const unsigned& i) {
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
		inline vertex& operator[](const unsigned& i) {
			if (i == 0) return a;
			if (i == 1) return b;
			if (i == 2) return c;
			else throw;
		}
	} faceData;


	mesh::mesh(int pCount, const float* positions, const float* texCoords, const float* normals, 
		const uint16_t* indices, const int iCount) {
		const float* points = positions;

		this->positions = new float[pCount * 3];
		this->positionByteSize = sizeof(float) * 3 * pCount;
		if (positions != nullptr)
			for (int i = 0; i < pCount * 3; i++) {
				this->positions[i] = positions[i];
			}

		this->texCoords = new float[pCount * 2];
		this->texCoordByteSize = sizeof(float) * 2 * pCount;
		if (texCoords)
			for (int i = 0; i < pCount * 3; i++) {
				this->texCoords[i] = texCoords[i];
			}

		this->normals = new float[pCount * 3];
		this->normalByteSize = sizeof(float) * 3 * pCount;
		if (normals)
			for (int i = 0; i < pCount * 3; i++) {
				this->normals[i] = normals[i];
			}

		this->indices = new GLuint[iCount * 3];
		this->indicesByteSize = sizeof(GLuint) * iCount * 3;
		if (indices != nullptr) {
			for (int i = 0; i < iCount * 3; i++) {
				this->indices[i] = (GLuint) indices[i];
			}
		}
			
	}

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
		std::vector<vec3f> vertexList{};
		std::vector<vec3f> texCoordList{};
		std::vector<vec3f> normalList{};
		std::vector<faceData> faceList{};

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
					positionWorker = std::thread(&mesh::processBatch, this, positionBatch, std::ref(vertexList),
						vertexCount, 3);
				}

				texCoordBatch.append(currentLine + '\n');
				texCoordCount++;
			}
			else if (type == "vn") {
				if (!texCoordWorker.joinable()) {
					texCoordWorker = std::thread(&mesh::processBatch, this, texCoordBatch, std::ref(texCoordList),
						texCoordCount, 2);
				}

				normalBatch.append(currentLine + '\n');
				normalCount++;
			}
			else if (type == "f") { // face data started, break from loop
				// no need to check if thread joinable, as we break immediately after this.
				normalWorker = std::thread(&mesh::processBatch, this, normalBatch, std::ref(normalList),
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
		faceWorker = std::thread(&mesh::processFaces, this, faceBatch, std::ref(faceList));

		this->positionByteSize = 3 * (GLsizeiptr)vertexCount * sizeof(float);
		this->positions = new float[3 * vertexCount];

		this->texCoordByteSize = 2 * (GLsizeiptr)vertexCount * sizeof(float);
		this->texCoords = new float[2 * vertexCount];

		this->normalByteSize = 3 * (GLsizeiptr)vertexCount * sizeof(float);
		this->normals = new float[3 * vertexCount];

		positionWorker.join();
		texCoordWorker.join();
		normalWorker.join();
		faceWorker.join();

		this->indicesByteSize = 3 * (GLsizeiptr)faceList.size() * sizeof(GLuint);
		this->indices = new GLuint[3 * faceList.size()];

		size_t faceCount = faceList.size();

		// iterate through faces
		for (int i = 0; i < faceCount; i++) {
			faceData currentFace = faceList.at(i);

			// iterate through face vertices
			for (int j = 0; j < 3; j++) {
				GLuint currentVertexIndex = currentFace[j].p - 1;

				this->indices[(i * 3) + j] = currentVertexIndex;

				vec3f v = vertexList.at(currentVertexIndex);
				this->positions[currentVertexIndex * 3] = v.x;
				this->positions[currentVertexIndex * 3 + 1] = v.y;
				this->positions[currentVertexIndex * 3 + 2] = v.z;

				vec3f vt = texCoordList.at(currentFace[j].t - 1);
				this->texCoords[currentVertexIndex * 2] = vt.x;
				this->texCoords[currentVertexIndex * 2 + 1] = 1.0f - vt.y;

				vec3f n = normalList.at(currentFace[j].n - 1);
				this->normals[currentVertexIndex * 3] = n.x;
				this->normals[currentVertexIndex * 3 + 1] = n.y;
				this->normals[currentVertexIndex * 3 + 2] = n.z;
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

		std::stringstream ss;
		ss << batch << "eof";

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

		size_t separator;
		size_t end;

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

					// empty index
					if (indices[i].find("//") == std::string::npos) {
						v[j] = std::stoi(indices[i].substr(0, end));
					}
					else { // valid value found
						v[j] = 0;
						// obj indices start at 1, convert to standard zero-index
						//v[j] -= 1;
					}

					indices[i].erase(0, end + 1);
				}

				currentFace[i] = v;
			}

			faces.push_back(currentFace);
		}

		std::cout << "Finished processing faces in thread " << curr_id << ".\n";
	}

}