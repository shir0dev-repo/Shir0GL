#pragma once

#include <sogl/structure/hashTable.hpp>
#include <sogl/structure/linkedList.h>
#include <sogl/structure/pair.hpp>

namespace sogl {
	struct texture;
	struct shaderProgram;
	struct material;
	const uint16_t MAX_TEXTURES = 8U;

	typedef void (*uniformUploadMethod)(struct uniform* uniform, const void* value);

	typedef struct uniform {
		char* name;
		char* nameEnd;
		uint32_t type;
		int32_t size;
		int32_t location;

		static void UploadFloat(uniform* u, const void* data);
		static void UploadBool(uniform* u, const void* data);
		static void UploadVec3f(uniform* u, const void* data);
		static void UploadVec4f(uniform* u, const void* data);
		static void UploadMatrix3f(uniform* u, const void* data);
		static void UploadMatrix4f(uniform* u, const void* data);

		uniformUploadMethod uploadMethod;
	} uniform;

	typedef enum cullingMode { back = GL_BACK, front = GL_FRONT, both = GL_FRONT_AND_BACK };

	typedef struct material {
		using uniformEntry = pair<const char*, uniform*>;

		hashTable<uniform> uniforms;
		texture** textures;
		const shaderProgram* shader;
		uint16_t numTextures;

		material();
		material(const shaderProgram* shaderProgram, const uint16_t cullFunc = GL_BACK, const uint16_t depthFunc = GL_LEQUAL);
		~material();

		void bind() const;
		void prepare();
		void unbind() const;
		
		void addTexture(texture* tex);

		void getUploadMethodForUniform(uniform* unif) const;
		void uploadUniformData(const char* name, const void* data);
		const char* listAllUniforms(std::ostream& os) const;
	} material;

	namespace materialManager {
		bool internal_findMaterial(const char* alias, material*& outMaterial);
		void internal_deleteMaterial(const char* alias, material* mat);
	}

	material* createMaterial(const shaderProgram* shader, const char* alias = "", uint16_t cullFunction = GL_BACK, uint16_t depthFunction = GL_LEQUAL, uint16_t numTextures = 0, texture** textures = nullptr);
	bool findMaterial(const char* alias, material*& outMaterial);
	void terminateMaterials();
}