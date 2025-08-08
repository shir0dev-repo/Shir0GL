#pragma once

#include <string>
#include <sogl/structure/hashTable.hpp>
#include <sogl/rendering/Texture.h>

namespace sogl {
	typedef class TextureFactory {
		static Texture* DefaultTexture;
		static hashTable<Texture> LoadedTextures;

		static bool glLoadSTBITextureData(const char* filePath, Texture* refTexture);
	public:
		static Texture* loadTexture(const char* filePath, const char* alias = "");
		static Texture* getDefaultTexture();
		static bool findTexture(const char* alias, Texture*& outTexture);
		static void terminate();
	};
}