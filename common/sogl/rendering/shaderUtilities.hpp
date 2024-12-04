
#pragma once

#include <GLEW/glew.h>
#include <string>
#include <sogl/rendering/gl/shaderProgram.hpp>

namespace sogl {
	static const char* ShaderDirectory = "assets/shader/";
	
	unsigned int loadShader(const unsigned int& shaderType, const char* shaderPath);
}