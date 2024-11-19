#pragma once

namespace sogl {
	struct vec3f;
	struct vec4f;
	struct matrix3f;
	struct matrix4f;

	typedef struct shaderProgram {
	private:
		unsigned int programID;
		unsigned int vertexShaderID;
		unsigned int fragmentShaderID;

	public:
		shaderProgram(const char* vertexFilePath, const char* fragmentFilePath);
		
		void uploadUniformFloat(const float& value, const char* uniformName) const;
		void uploadUniformBool(const bool& value, const char* uniformName) const;
		void uploadUniformVec3f(const vec3f& value, const char* uniformName) const;
		void uploadUniformVec4f(const vec4f& value, const char* uniformName) const;
		void uploadUniformMatrix3f(const matrix3f& value, const bool& transposed, const char* uniformName) const;
		void uploadUniformMatrix4f(const matrix4f& value, const bool& transposed, const char* uniformName) const;

		void use() const;
		void stop() const;
		void glDelete();

		friend void addShader(const char* shaderName, const char* vertexShader, const char* fragmentShader);
	} shaderProgram;

	namespace shaderManager {
		void internal_UploadShader(shaderProgram* shader);
		void internal_deleteShader(shaderProgram* shader);
		void internal_createShader(shaderProgram* shader, const bool isManaged, const char* alias);
	}

	shaderProgram* createShader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* alias = "");
}