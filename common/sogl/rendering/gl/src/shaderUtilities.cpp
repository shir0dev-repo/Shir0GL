#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include <sogl/rendering/gl/shaderProgram.hpp>
#include <sogl/rendering/glUtilities.hpp>
#include <sogl/rendering/shaderUtilities.hpp>

namespace sogl {
	static const char* readShader(const char* filePath) {
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
		
		char* c = new char[content.size()];
		content.copy(c, content.size(), 0);

		return c;
	}

	unsigned int loadShader(const unsigned int& type, const char* shaderPath) {
		unsigned int shaderID = 0;
		const char* shaderSrc = readShader(shaderPath);
		
		assert(shaderSrc != nullptr);

		if (type == GL_VERTEX_SHADER) shaderID = glCreateShader(GL_VERTEX_SHADER);
		else if (type == GL_FRAGMENT_SHADER) shaderID = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(shaderID, 1, &shaderSrc, NULL);
		glCompileShader(shaderID);

		delete[] shaderSrc;
		
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