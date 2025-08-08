#pragma once

#include <sogl/structure/hashTable.hpp>
#include <sogl/structure/linkedList.h>

namespace sogl {
	struct Texture;
	struct ShaderProgram;
	struct Material;
	const uint16_t MAX_TEXTURES = 8U;

	typedef enum cullingMode { back = GL_BACK, front = GL_FRONT, both = GL_FRONT_AND_BACK };

	typedef struct Material {
		hashTable<struct GLUniform> uniforms;
		Texture** textures;
		const ShaderProgram* shader;
		uint16_t numTextures;

		Material();
		Material(const ShaderProgram* ShaderProgram, const uint16_t cullFunc = GL_BACK, const uint16_t depthFunc = GL_LEQUAL);
		~Material();

		void bind() const;
		void prepare();
		void unbind() const;
		
		void AddUniform(GLUniform* uniform);
		void addTexture(Texture* tex);

		void uploadUniformData(const char* name, const void* data);
		void listAllUniforms() const;
	};
}