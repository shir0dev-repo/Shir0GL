#pragma once

#include <sogl/structure/hashTable.hpp>
#include <sogl/rendering/gl/ShaderProgram.h>

namespace sogl {
	struct ShaderProgram;

	typedef class ShaderFactory {
		static hashTable<unsigned int> m_vertexShaders;
		static hashTable<unsigned int> m_fragmentShaders;
		static hashTable<ShaderProgram> m_loadedShaders;

		static unsigned int createShaderSource(const char* filePath, const unsigned int sourceType);
		static bool findSource(const char* alias, const unsigned int sourceType, unsigned int*& outSourceID);
		static bool readShader(const char* filePath, char*& outSource);
		
	public:
		static ShaderProgram* createNew(const char* vertexFilePath, const char* fragmentFilePath, const char* alias);
		static bool find(const char* alias, ShaderProgram*& outShader);
		static const char* GetShaderVertexName(const uint32_t vertexID);
		static const char* GetShaderFragmentName(const uint32_t fragmentID);
		static const char* GetShaderName(const ShaderProgram* shader);
		static void terminate();

	} ShaderFactory;
}