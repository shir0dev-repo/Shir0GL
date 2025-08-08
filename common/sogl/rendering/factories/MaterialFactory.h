#pragma once

#include <sogl/structure/hashTable.hpp>
#include <sogl/rendering/Material.h>

namespace sogl {
	class MaterialFactory {
		static hashTable<Material> LoadedMaterials;
		static bool SaveOnTerminate;
		static const char* SaveDirectory;
		static void LinkUniforms( const struct ShaderProgram* shader, Material* material );
		static void WriteUniformValue(FILE* file, const uint32_t programID, GLUniform* uniform);
	public:
		static void SetSaveOnTerminate(const bool value);
		static Material* CreateNew( ShaderProgram* shader, const char* alias = "", cullingMode cullMode = back );
		static Material* CreateNew( const char* filePath, const char* alias = "" );

		static bool Find(const char* alias, Material*& outMaterial);
		static bool WriteToFile( const char* filePath, const Material* material );

		static void Terminate();
	};
}