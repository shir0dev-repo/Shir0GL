#pragma once

#include <string>
#include <sogl/structure/hashTable.hpp>
#include <sogl/rendering/texture.hpp>

namespace sogl {
	typedef class textureFactory {
		static texture* DefaultTexture;
		static hashTable<texture> LoadedTextures;

		static bool glLoadSTBITextureData(const char* filePath, texture* refTexture);
	public:
		static texture* loadTexture(const char* filePath, const char* alias = "");
		static texture* getDefaultTexture();
		static bool findTexture(const char* alias, texture*& outTexture);
		static void terminate();
	} textureFactory;
}