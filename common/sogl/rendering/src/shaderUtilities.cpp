#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include <sogl/rendering/shaderProgram.hpp>
#include <sogl/rendering/glUtilities.hpp>
#include <sogl/rendering/shaderUtilities.hpp>
#include <sogl/database.hpp>

namespace sogl {
	
	void deleteShader(shaderProgram* shader) {
		shader->glDelete();
	}

	static database<shaderProgram> LoadedShaders(4, &deleteShader);
	
	void terminateShaders() {
		LoadedShaders.destroy();
	}

	int directoryLength() {
		static int length = 0;
		if (length == 0)
			length = strlen(ShaderDirectory);

		return length;
	}

	void addShader(const char* shaderName, const char* vertexShader, const char* fragmentShader) {
		if (LoadedShaders.containsKey(shaderName)) {
			std::cout << "Shader " << shaderName << "is already loaded.\n";
			return;
		}
		
		LoadedShaders.add(shaderName, new shaderProgram(shaderName, vertexShader, fragmentShader));
		std::cout << "Successfully added " << shaderName << " to the database.\n";
	}

	shaderProgram* const findShader(const char* shaderName) {
		if (shaderProgram* const shader = LoadedShaders.get(shaderName)) {
			return shader;
		}
		else {
			std::cout << "Shader " << shaderName << " not found!\n";
			return nullptr;
		}
	}

	void removeShader(const char* shaderName) {
		LoadedShaders.remove(shaderName);
	}

	const char* readShader(char* c, std::string filePath) {
		std::string content = "";
		std::ifstream fileStream(filePath, std::ios::in);

		if (!fileStream.is_open()) {
			std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
			return "";
		}

		std::string line = "";
		while (!fileStream.eof()) {
			std::getline(fileStream, line);
			content.append(line + "\n");
		}

		fileStream.close();
		content.append("\0");
		std::cout << "'" << content << "'" << std::endl;
		
		c = new char[content.size()];
		content.copy(c, content.size(), 0);

		return c;
	}

	unsigned int loadShader(unsigned int type, std::string shaderPath) {
		unsigned int shaderID = 0;
		char* c = nullptr;
		const char* shaderSrc = readShader(c, shaderPath);
		

		assert(shaderSrc != nullptr);

		if (type == GL_VERTEX_SHADER) shaderID = glCreateShader(GL_VERTEX_SHADER);
		else if (type == GL_FRAGMENT_SHADER) shaderID = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(shaderID, 1, &shaderSrc, NULL);
		glCompileShader(shaderID);

		delete[] c;
		
		// check for compiler errors
		int logSize = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);

		// check for GL errors
		unsigned int err = glGetError();

		// OpenGL error found
		if (err != GL_FALSE) {
			glErrorCB(err);
		}

		// shader failed to compile
		if (logSize > 0) {
			char* shaderOutput = new char[logSize];
			glGetShaderInfoLog(shaderID, logSize, &logSize, shaderOutput);
			std::cout << shaderOutput << '\n';
			delete[] shaderOutput;

			glDeleteShader(shaderID);

			throw err;
		}
		// shader loaded successfully
		return shaderID;
	}
}