
#pragma once

#include <GLEW/glew.h>
#include <string>
#include <sogl/rendering/shaderProgram.hpp>

namespace sogl {
	static const char* ShaderDirectory = "assets/shader/";

	int directoryLength();

	// Opens the specified shader file and writes it's contents to c.
	const char* readShader(std::string filePath);
	// Loads a shader into the current OpenGL context. Throws an unsigned int (GLenum) if error is encountered.
	unsigned int loadShader(unsigned int type, std::string shaderPath);

	// Loads a shader for use at runtime, and returns a reference to it.
	void addShader(const char* shaderName, const char* vertexShader, const char* fragmentShader);
	// Attempts to locate a requested shader. Returns the shader if it exists, otherwise returns the default shader.
	shaderProgram* const findShader(const char* name);

	void removeShader(const char* name);
	// Removes all currently stored shader programs from the current OpenGL context.
	void terminateShaders();
}