#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

// STD:
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>

//SOGL: 
#include <sogl/hashTable.hpp>
#include <sogl/transform/vec3f.hpp>
#include <sogl/transform/vec4f.hpp>
#include <sogl/transform/matrix3f.hpp>
#include <sogl/transform/matrix4f.hpp>
#include <sogl/rendering/GLError.hpp>
#include <sogl/rendering/glUtilities.hpp>
#include <sogl/rendering/shaderUtilities.hpp>
#include <sogl/rendering/shaderProgram.hpp>

namespace sogl {
	static hashTable<shaderProgram> Shaders(32);
	shaderProgram* createShader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* alias = "") {
		char* aliasUsed;
		shaderProgram* shader = nullptr;

		if (strcmp(alias, "")) {
			aliasUsed = const_cast<char*>(vertexShaderPath);
		}
		else {
			aliasUsed = const_cast<char*>(alias);
		}

		if (Shaders.contains(aliasUsed, shader)) {
			return nullptr;
		}

		shader = new shaderProgram(vertexShaderPath, fragmentShaderPath);
		shaderManager::internal_createShader(shader, false, aliasUsed);

		return shader;
	}

	shaderProgram::shaderProgram(const char* vertexFilePath, const char* fragmentFilePath) {
		try {
			vertexShaderID = loadShader(GL_VERTEX_SHADER, vertexFilePath);
			fragmentShaderID = loadShader(GL_FRAGMENT_SHADER, fragmentFilePath);
		}
		catch (const unsigned int& e) {
			vertexShaderID = 0;
			fragmentShaderID = 0;
		}

		assert(vertexShaderID != 0 && fragmentShaderID != 0);

		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);

		GLint isLinked = 0;
		glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);

		if (isLinked == GL_FALSE) {
			std::cout << "[GLERROR]: Failed to link shader program!\n";

			GLint logLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
			
			std::vector<GLchar> infoLog(logLength);
			glGetProgramInfoLog(programID, logLength, &logLength, &infoLog[0]);

			std::cout << "Reason: \n";
			for (char c : infoLog)
				std::cout << c;

			std::cout << "\n Deleting program and it's related shaders.\n" << std::endl;

			glDetachShader(programID, vertexShaderID);
			glDetachShader(programID, fragmentShaderID);
			
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);

			glDeleteProgram(programID);

			programID = vertexShaderID = fragmentShaderID = 0;

			return;
		}

		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
	}

	void shaderProgram::glDelete() {
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		glDeleteProgram(programID);

		programID = 0;
		vertexShaderID = 0;
		fragmentShaderID = 0;
	}

	void shaderProgram::uploadUniformFloat(const float& value, const char* uniformName) const {
		int location = glGetUniformLocation(programID, uniformName);

		if (location >= 0) {
			glUniform1f(location, value);
		}
		else {
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}	
	
	void shaderProgram::uploadUniformBool(const bool& value, const char* uniformName) const {
		int location = glGetUniformLocation(programID, uniformName);
		
		if (location >= 0) {
			glUniform1i(location, value ? 1 : 0);
		}
		else {
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}

	void shaderProgram::uploadUniformVec3f(const vec3f& value, const char* uniformName) const {
		int location = glGetUniformLocation(programID, uniformName);

		if (location >= 0) {
			glUniform3f(location, value.x, value.y, value.z);
		}
		else {
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}

	void shaderProgram::uploadUniformVec4f(const vec4f& value, const char* uniformName) const {
		int location = glGetUniformLocation(programID, uniformName);

		if (location >= 0) {
			glUniform4f(location, value.x, value.y, value.z, value.w);
		}
		else {
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}

	void shaderProgram::uploadUniformMatrix3f(const matrix3f& value, const bool& transposed, const char* uniformName) const {
		unsigned int location = glGetUniformLocation(programID, uniformName);
		if (location >= 0) {
			glUniformMatrix3fv(location, 1, transposed, value.getPointer());
		}
		else {
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}

	void shaderProgram::uploadUniformMatrix4f(const matrix4f& value, const bool& transposed, const char* uniformName) const {
		unsigned int location = glGetUniformLocation(programID, uniformName);
		if (location >= 0) {
			glUniformMatrix4fv(location, 1, transposed, value.getPointer());
		}
		else {
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}


	void shaderProgram::use() const {
		glUseProgram(programID);
	}

	void shaderProgram::stop() const {
		glUseProgram(0);
	}
}