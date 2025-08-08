#pragma once

namespace sogl {
	struct ShaderProgram {
		unsigned int programID;
		unsigned int vertexShaderID;
		unsigned int fragmentShaderID;

		ShaderProgram();

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
	};
}