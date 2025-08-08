#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <concepts>

#include <sogl/transform/matrix3f.hpp>
#include <sogl/transform/matrix4f.hpp>
#include <sogl/rendering/glUtilities.h>
#include <sogl/rendering/Texture.h>
#include <sogl/rendering/color.hpp>
#include <sogl/rendering/color32.hpp>
#include <sogl/rendering/gl/ShaderProgram.h>
#include <sogl/rendering/gl/GLUniform.h>
#include <sogl/rendering/Material.h>

namespace sogl {
	static hashTable<Material> LoadedMaterials(32);
	
	Material::Material() : textures(nullptr), shader(nullptr), uniforms(32), numTextures(0) {}

	Material::Material(const ShaderProgram* shader, const uint16_t glCullFunc, const uint16_t glDepthFunc) : uniforms(32) {
		this->shader = shader;
		this->textures = new Texture* [MAX_TEXTURES];
		this->numTextures = 0;
	}

	Material::~Material() {
		for (uint64_t i = 0; i < uniforms.size; i++) {
			GLUniform* unif = nullptr;
			if ((unif = uniforms.data[i].value) != nullptr) {
				const char* key = uniforms.data[i].key;
				delete[] unif->name;
				uniforms.remove(key);

			}
		}
		// this deletes the array of pointers, not the actual textures
		delete[] textures;
		textures = nullptr;
		shader = nullptr;
	}

	void Material::uploadUniformData(const char* name, const void* data) {
		GLUniform* unif = nullptr;

		if (!uniforms.find(name, unif)) {			
			printf("[Material]: Could not find specified uniform \"%s\".\n", name);
			printf("|-- Use material::listAllUniforms() to see material's uniforms.\n");
			return;
		}

		unif->uploadMethod(unif, data);
	}

	void Material::bind() const {
		shader->use();
	}

	void Material::prepare() {
		for (int i = 0; i < numTextures; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]->ID);
		}
	}

	void Material::unbind() const {
		for (int i = 0; i < numTextures; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		shader->stop();
	}

	void Material::AddUniform(GLUniform* uniform) {
		uniforms.insert(uniform->name, uniform);
	}

	void Material::addTexture(Texture* tex) {
		textures[numTextures++] = tex;

	}

	void Material::listAllUniforms() const {
		uint64_t size = uniforms.size;

		for (uint32_t i = 0; i < size; i++) {
			GLUniform* unif = nullptr;
			if ((unif = uniforms.data[i].value) != nullptr) {
				printf("  |-- [%s]: %s\n", glGetNamedType(unif->type), uniforms.data[i].key);
			}
		}
	}
}