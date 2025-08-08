#include <GLEW/glew.h>

#include <stdio.h>
#include <string.h>

#include <sogl/transform/vec4f.hpp>
#include <sogl/rendering/factories/ShaderFactory.h> 
#include <sogl/rendering/gl/GLUniform.h>
#include <sogl/rendering/glUtilities.h>
#include <sogl/rendering/factories/MaterialFactory.h>

namespace sogl {
	hashTable<Material> MaterialFactory::LoadedMaterials(64);
	bool MaterialFactory::SaveOnTerminate = false;
	const char* MaterialFactory::SaveDirectory = "assets/materials";

	Material* MaterialFactory::CreateNew(ShaderProgram* shader, const char* alias, cullingMode cullMode) {
		char* aliasUsed = nullptr;
		if (strcmp(alias, "") == 0) {
			aliasUsed = new char[16];
			snprintf(aliasUsed, 16, "material%lu\0", static_cast<int>(LoadedMaterials.size));
		}
		else {
			aliasUsed = const_cast<char*>(alias);
		}

		printf("[Material Factory]: Creating material \"%s\"\n", aliasUsed);
		if (shader == nullptr) {
			printf("[Material Factory]: Failed to create material: \"%s\"\n", aliasUsed);
			printf("|-- Specified shader is NULL.\n");
			return nullptr;
		}

		Material* m = nullptr;
		if (LoadedMaterials.find(aliasUsed, m)) {
			printf("[Material Factory]: Specified material \"%s\"already exists!\n", aliasUsed);
			return m;
		}

		m = new Material(shader, cullMode);
		LinkUniforms(shader, m);
		LoadedMaterials.insert(aliasUsed, m);
		printf("[Material Factory]: Successfully created material \"%s\"\n", aliasUsed);

		return m;
	}

	Material* MaterialFactory::CreateNew(const char* filePath, const char* alias) {
		return nullptr;
	}

	void MaterialFactory::LinkUniforms(const ShaderProgram* shader, Material* material) {
		const uint32_t prog = shader->programID;
		// get number of uniforms in shader
		int numUniforms = 0;
		glGetProgramiv(prog, GL_ACTIVE_UNIFORMS, &numUniforms);
		shader->use();
		for (uint32_t i = 0; i < numUniforms; i++) {
			GLUniform* unif = new GLUniform();
			//allocate unnecessarily large buffer for uniform name
			char* temp = new char[64];
			int32_t nameLength = 0;
			glGetActiveUniform(prog, i, 64, &nameLength, &unif->size, &unif->type, temp);
			// allocate correct amount this time, and store name
			unif->name = new char[nameLength + 1];
			memcpy(unif->name, temp, nameLength);
			unif->name[nameLength] = '\0';
			unif->nameEnd = unif->name + nameLength;
			delete[] temp;
			// get location for uniform
			unif->location = glGetUniformLocation(prog, unif->name);
			// part of uniform block, ignore
			if (unif->location == -1) {
				delete[] unif->name;
				delete unif;
				continue;
			}
			unif->FindUploadMethod();
			material->AddUniform(unif);
		}
		shader->stop();
	}

	bool MaterialFactory::Find(const char* alias, Material*& outMaterial) {
		return LoadedMaterials.find(alias, outMaterial);
	}

	void MaterialFactory::SetSaveOnTerminate(const bool value) {
		MaterialFactory::SaveOnTerminate = value;
	}

	bool MaterialFactory::WriteToFile(const char* filePath, const Material* material) {
		char* name = nullptr;
		for (uint32_t i = 0; i < LoadedMaterials.size; i++) {
			Material* dbMaterial = nullptr;
			if ((dbMaterial = LoadedMaterials.data[i].value) != nullptr && dbMaterial == material) {
				name = LoadedMaterials.data[i].key;
				dbMaterial = nullptr;
				break;
			}
		}
		if (name == nullptr) {
			printf("[Material Factory]: Specified material not present within database!\n");
			return false;
		}

		char buf[128];
		snprintf(buf, sizeof(buf), "%s/%s.mat", filePath, name);
		FILE* matFile = fopen(buf, "w");
		if (matFile == nullptr) {
			printf("[Material Factory]: Could not open file \"%s\" for writing!\n", buf);
			return false;
		}

		fprintf(matFile, "m %s\n\n", name);
		const char* vertex = ShaderFactory::GetShaderVertexName(material->shader->vertexShaderID);
		const char* fragment = ShaderFactory::GetShaderFragmentName(material->shader->fragmentShaderID);
		const char* shaderName = ShaderFactory::GetShaderName(material->shader);
		fprintf(matFile, "s %s %s %s\n", shaderName, vertex, fragment);

		for (uint32_t i = 0; i < material->uniforms.size; i++) {
			char* key = nullptr;
			if ((key = material->uniforms.data[i].key) == nullptr)
				continue;
			GLUniform* unif = material->uniforms.data[i].value;

			WriteUniformValue(matFile, material->shader->programID, unif);
		}
		fclose(matFile);
		matFile = nullptr;

		printf("[Material Factory]: Successfully wrote material \"%s\" to file: %s\n", name, buf);
		return true;
	}

	void MaterialFactory::WriteUniformValue(FILE* file, const uint32_t programID, GLUniform* uniform) {
		fprintf(file, "\#%s\n", glGetNamedType(uniform->type));
		switch (uniform->type) {
		case GL_FLOAT:
		{

			float val = 0;
			glGetUniformfv(programID, uniform->location, &val);
			fprintf(file, "u %lu %s %f\n", uniform->type, uniform->name, val);
			return;
		}
		case GL_FLOAT_VEC3:
		{

			vec3f val;
			glGetUniformfv(programID, uniform->location, (float*)val);
			fprintf(file, "u %lu %s %f %f %f\n", uniform->type, uniform->name, val.x, val.y, val.z);
			return;
		}
		case GL_FLOAT_VEC4:
		{
			vec4f val;
			glGetUniformfv(programID, uniform->location, (float*)val);
			fprintf(file, "u %lu %s %f %f %f %f\n", uniform->type, uniform->name, val.x, val.y, val.z, val.w);
			return;
		}
		default:
			fprintf(file, "u %lu %s\n", uniform->type, uniform->name);
			return;
		}
	}

	void MaterialFactory::Terminate() {
		printf("[Material Factory]: Cleaning up materials.\n");
		char* key = nullptr;
		for (uint32_t i = 0; i < LoadedMaterials.size; i++) {
			if ((key = LoadedMaterials.data[i].key) == nullptr) {
				continue;
			}

			if (MaterialFactory::SaveOnTerminate) {
				Material* mat = LoadedMaterials.data[i].value;
				WriteToFile(SaveDirectory, mat);
			}

			LoadedMaterials.remove(key);
		}
	}
}