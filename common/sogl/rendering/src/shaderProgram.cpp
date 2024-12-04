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
#include <sogl/rendering/color.hpp>
#include <sogl/rendering/color32.hpp>
#include <sogl/rendering/glUtilities.hpp>
#include <sogl/rendering/shaderUtilities.hpp>
#include <sogl/rendering/gl/shaderProgram.hpp>

namespace sogl {
	
	shaderProgram::shaderProgram() : vertexShaderID(0), fragmentShaderID(0), programID(0) {}

	void shaderProgram::uploadUniform(const char* uniformName, const float& value) const {
		int location = glGetUniformLocation(programID, uniformName);

		if (location >= 0) {
			glUniform1f(location, value);
		}
		else {
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}

	void shaderProgram::uploadUniform(const char* uniformName, const bool& value) const {
		int location = glGetUniformLocation(programID, uniformName);

		if (location >= 0) {
			glUniform1i(location, value ? 1 : 0);
		}
		else {
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}

	void shaderProgram::uploadUniform(const char* uniformName, const color& color, const bool& includeAlpha) const {
		int location = glGetUniformLocation(programID, uniformName);
		if (location >= 0) {
			if (includeAlpha) {
				glUniform4f(location, color.r, color.g, color.b, color.a);
			}
			else {
				glUniform3f(location, color.r, color.g, color.b);
			}
		}
		else {
			glErrorCB(glGetError());
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}

	void shaderProgram::uploadUniform(const char* uniformName, const color32& clr32, const bool& includeAlpha) const {
		int location = glGetUniformLocation(programID, uniformName);
		if (location >= 0) {
			color clr = (color)clr32;
			if (includeAlpha) {
				glUniform4f(location, clr.r, clr.g, clr.b, clr.a);
			}
			else {
				glUniform3f(location, clr.r, clr.g, clr.b);
			}
		}
		else {
			glErrorCB(glGetError());
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}

	void shaderProgram::uploadUniform(const char* uniformName, const vec3f& value) const {
		int location = glGetUniformLocation(programID, uniformName);

		if (location >= 0) {
			glUniform3f(location, value.x, value.y, value.z);
		}
		else {
			glErrorCB(glGetError());
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}

	void shaderProgram::uploadUniform(const char* uniformName, const vec4f& value) const {
		int location = glGetUniformLocation(programID, uniformName);

		if (location >= 0) {
			glUniform4f(location, value.x, value.y, value.z, value.w);
		}
		else {
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}

	void shaderProgram::uploadUniform(const char* uniformName, const matrix3f& value, const bool& transposed) const {
		unsigned int location = glGetUniformLocation(programID, uniformName);
		if (location >= 0) {
			glUniformMatrix3fv(location, 1, transposed, value.getPointer());
		}
		else {
			std::cout << "Could not find uniform: " << uniformName << ".\n";
		}
	}

	void shaderProgram::uploadUniform(const char* uniformName, const matrix4f& value, const bool& transposed) const {
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