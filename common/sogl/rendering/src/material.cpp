#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <concepts>

#include <sogl/hashTable.hpp>
#include <sogl/rendering/glUtilities.hpp>
#include <sogl/rendering/texture.hpp>
#include <sogl/rendering/color.hpp>
#include <sogl/rendering/color32.hpp>
#include <sogl/rendering/gl/shaderProgram.hpp>
#include <sogl/transform/matrix3f.hpp>
#include <sogl/transform/matrix4f.hpp>
#include <sogl/rendering/material.hpp>

#define GET_CULL_FLAG(flag/*uint16_t*/) 1024 | ((back & (front >> 1)) << 3 | ((back ^ (front >> 1)) << 2)) | ((back ^ (front >> 1)) & back) 
#define GET_DEPTH_FLAG(flag) ((flag & 0b01110000) >> 4) + GL_NEVER

namespace sogl {
	static hashTable<material> LoadedMaterials(32);
	uint32_t DefaultTexture;
	bool materialManager::internal_findMaterial(const char* alias, material*& outMaterial) {
		return LoadedMaterials.find(alias, outMaterial);
	}

	void materialManager::internal_deleteMaterial(const char* alias, material* mat) {
		std::cout << "|-- [Material Manager]: Deleting material " << alias << "...\n";


		if (!LoadedMaterials.find(alias, mat)) {
			std::cout <<
				"|-- [Material Manager]: Could not delete material " << alias << '\n' <<
				"	|-- " << alias << " does not exist in the material database.\n";
			return;
		}
		// would be weird for this to happen but just in case
		else if (mat == nullptr) {
			std::cout <<
				"|-- [Material Manager]: Could not delete material " << alias << '\n' <<
				"	|-- Material " << alias << " is NULL!\n";
			return;
		}

		LoadedMaterials.remove(alias);
		// as stated in createMaterial, the material struct is self-managing
		delete mat;
		std::cout << "|-- [Material Manager]: Successfully deleted material " << alias << ".\n";
	}

	void terminateMaterials() {
		std::cout << "[Material Manager]: Terminating all material instances...\n";
		for (uint32_t i = 0; i < LoadedMaterials.size; i++) {
			if (LoadedMaterials.data[i].key == nullptr) {
				continue;
			}

			const char* alias = LoadedMaterials.data[i].key;
			LoadedMaterials.remove(alias);
		}
	}

	material* createMaterial(const shaderProgram* shader, const char* alias, uint16_t cullFunction, uint16_t depthFunction, uint16_t numTextures, texture** textures) {
		std::cout << "[Material Manager]: Creating new material...\n";

		if (shader == nullptr) {
			std::cout <<
				"[Material Manager]: Could not create material!\n" <<
				"|-- Specified shader is NULL!\n";
			return nullptr;
		}

		char* aliasUsed = nullptr;
		if (strcmp(alias, "") == 0) {
			aliasUsed = const_cast<char*>("shaderName");
		}
		else {
			aliasUsed = const_cast<char*>(alias);
		}

		material* m = nullptr;
		if (findMaterial(aliasUsed, m)) {
			std::cout <<
				"[Material Manager]: Could not create material!\n" <<
				"|-- Specified material already exists!\n" <<
				"|-- Will return the found material pointer instead.\n";
			return m;
		}

		m = new material(shader);
		LoadedMaterials.insert(aliasUsed, m);
		std::cout << "[Material Manager]: Material " << aliasUsed << " created.\n";

		return m;
	}

	bool findMaterial(const char* alias, material*& outMaterial) {
		return materialManager::internal_findMaterial(alias, outMaterial);
	}

	material::material() : textures(nullptr), shader(nullptr), uniforms(32), numTextures(0) {}

	material::material(const shaderProgram* shader, const uint16_t glCullFunc, const uint16_t glDepthFunc) : uniforms(32) {
		this->shader = shader;
		
		this->textures = new texture * [MAX_TEXTURES];
		this->numTextures = 0;
		const uint32_t prog = shader->programID;

		int numUniforms = 0;
		glGetProgramiv(prog, GL_ACTIVE_UNIFORMS, &numUniforms);
		shader->use();
		for (int u = 0; u < numUniforms; u++) {
			uniform* unif = new uniform();
			int32_t nameSize;

			// allocate more memory than what's probably needed
			char* n = new char[64];

			// glGetActiveUniform(program, index, 64, &realNameSize, &sizeOfU, &typeOfU, name*)
			// get info about the current uniform
			// retrieved:
			//	- size of uniform name
			//	- uniform type
			//	- size of uniform type
			//	- uniform name
			glGetActiveUniform(prog, u, 64, &nameSize, &unif->size, &unif->type, n);
			// take the correct name size to assign the correct size for uniform's name
			unif->name = new char[nameSize + 1];
			memcpy(unif->name, n, nameSize);
			unif->name[nameSize] = '\0';
			unif->nameEnd = unif->name + nameSize;
			delete[] n;
			// get the location from the given GL name
			unif->location = glGetUniformLocation(prog, const_cast<const char*>(unif->name));
			// any uniform with location -1 means they are part of a uniform block. ignore them for now
			if (unif->location == -1) {
				delete[] unif->name;
				delete unif;
				continue;
			}
			// for now, this method just ensures the uniform is a supported type
			getUploadMethodForUniform(unif);
			uniforms.insert(const_cast<const char*>(unif->name), unif, false);
		}

		shader->stop();
	}

	material::~material() {
		for (uint32_t i = 0; i < uniforms.size; i++) {
			// these uniforms are managed externally, meaning the hashtable wont delete them upload removal
			uniform* unif;
			while ((unif = uniforms.data[i].value) != nullptr) {
				uniforms.remove(unif->name);
			}
		}

		// this deletes the array of pointers, not the actual textures
		delete[] textures;
		shader = nullptr;
	}

	void material::getUploadMethodForUniform(uniform* unif) const {
		uniformUploadMethod uploadMethod = nullptr;
		switch (unif->type) {
		case GL_FLOAT:
			uploadMethod = uniform::UploadFloat;
			break;
		case GL_INT:
			uploadMethod = &uniform::UploadBool;
			break;
		case GL_FLOAT_VEC3:
			uploadMethod = &uniform::UploadVec3f;
			break;
		case GL_FLOAT_VEC4:
			uploadMethod = &uniform::UploadVec4f;
			break;
		case GL_FLOAT_MAT3:
			uploadMethod = &uniform::UploadMatrix3f;
			break;
		case GL_FLOAT_MAT4:
			uploadMethod = &uniform::UploadMatrix4f;
			break;
		default:
			std::cout <<
				"[GLERROR]: Failed to assign uniform " << unif->name << "an upload method.\n" <<
				"|-- Uniform type: " << unif->type << "is unsupported!\n";
			uploadMethod = nullptr;
			break;
		}

		unif->uploadMethod = uploadMethod;
	}

	void material::uploadUniformData(const char* name, const void* data) {
		uniform* unif = nullptr;
		if (!uniforms.find(name, unif)) {
			std::cout <<
				"[GLERROR]: Could not find specified uniform " << name << ".\n" <<
				"|-- Use material::listAllUniforms() to see material's uniforms.\n";
			return;
		}

		unif->uploadMethod(unif, data);
	}

	void material::bind() const {
		shader->use();
	}

	void material::prepare() {
		for (int i = 0; i < numTextures; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]->ID);
		}
	}

	void material::unbind() const {
		for (int i = 0; i < numTextures; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		shader->stop();
	}

	void material::addTexture(texture* tex) {
		textures[numTextures++] = tex;
		
	}

	const char* material::listAllUniforms(std::ostream& os) const {
		os << "|-- Stored uniforms:\n";

		for (uint32_t i = 0; i < uniforms.size; i++) {
			uniform* unif;
			if ((unif = uniforms.data[i].value) != nullptr) {
				os << "  |-- [" << glGetNamedType(unif->type) << "]: " << uniforms.data[i].key << '\n';
			}
		}
		
		return "";
	}

	void uniform::UploadFloat(uniform* u, const void* data) {
		float* f = (float*)data;
		if (f)
			glUniform1f(u->location, *f);
	}
	void uniform::UploadBool(uniform* u, const void* data) {
		int* i = (int*)data;
		if (i)
			glUniform1i(u->location, *i);
	}
	void uniform::UploadVec3f(uniform* u, const void* data) {
		const vec3f* v = (vec3f*)data;
		if (v)
			glUniform3f(u->location, v->x, v->y, v->z);
	}
	void uniform::UploadVec4f(uniform* u, const void* data) {
		const vec4f* v = (vec4f*)data;
		if (v)
			glUniform4f(u->location, v->x, v->y, v->z, v->w);
	}
	void uniform::UploadMatrix3f(uniform* u, const void* data) {
		const matrix3f* m = (matrix3f*)data;
		if (m)
			glUniformMatrix3fv(u->location, 1, GL_FALSE, m->getPointer());
	}
	void uniform::UploadMatrix4f(uniform* u, const void* data) {
		const matrix4f* m = (matrix4f*)data;
		if (m)
			glUniformMatrix4fv(u->location, 1, GL_FALSE, m->getPointer());
	}
}