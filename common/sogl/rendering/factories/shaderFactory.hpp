#pragma once

#include <sogl/structure/hashTable.hpp>
#include <sogl/rendering/gl/shaderProgram.hpp>

namespace sogl {
	struct shaderProgram;

	typedef class shaderFactory {
		static hashTable<unsigned int> m_vertexShaders;
		static hashTable<unsigned int> m_fragmentShaders;
		static hashTable<shaderProgram> m_loadedShaders;

		static unsigned int createShaderSource(const char* filePath, const unsigned int sourceType);
		static bool findSource(const char* alias, const unsigned int sourceType, unsigned int*& outSourceID);
		static bool readShader(const char* filePath, char*& outSource);
		
	public:
		static shaderProgram* createNew(const char* vertexFilePath, const char* fragmentFilePath, const char* alias);
		static bool find(const char* alias, shaderProgram*& outShader);
		
		static void terminate();

	} shaderFactory;
}