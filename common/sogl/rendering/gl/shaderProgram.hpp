#pragma once

namespace sogl {
	struct vec3f;
	struct vec4f;
	struct matrix3f;
	struct matrix4f;
	struct uniformBufferObject;

	typedef struct shaderProgram {
		unsigned int programID;
		unsigned int vertexShaderID;
		unsigned int fragmentShaderID;

		shaderProgram();

		void uploadUniform(const char* uniformName, const float&			value) const;
		void uploadUniform(const char* uniformName, const bool&				value) const;
		void uploadUniform(const char* uniformName, const struct vec3f&		value) const;
		void uploadUniform(const char* uniformName, const struct vec4f&		value) const;
		void uploadUniform(const char* uniformName, const struct matrix3f&	value, const bool& transposed = false) const;
		void uploadUniform(const char* uniformName, const struct matrix4f&	value, const bool& transposed = false) const;
		void uploadUniform(const char* uniformName, const struct color&		value, const bool& includeAlpha = false) const;
		void uploadUniform(const char* uniformName, const struct color32&	value, const bool& includeAlpha = false) const;

		void use() const;
		void stop() const;
	} shaderProgram;
}