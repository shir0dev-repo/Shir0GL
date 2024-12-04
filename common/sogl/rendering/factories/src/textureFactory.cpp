#include <GLEW/glew.h>
#include <stbi/stb_image.h>

#include <iostream>

#include <sogl/rendering/factories/textureFactory.hpp>

namespace sogl {
	hashTable<texture> textureFactory::LoadedTextures(32);
	texture* textureFactory::DefaultTexture = nullptr;

	bool textureFactory::glLoadSTBITextureData(const char* filePath, texture* refTexture) {
		glGenTextures(1, &refTexture->ID);
		glBindTexture(GL_TEXTURE_2D, refTexture->ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		refTexture->data = stbi_load(filePath, &refTexture->width, &refTexture->height, &refTexture->nrChannels, 0);
		if (refTexture->data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, refTexture->width, refTexture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, refTexture->data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(refTexture->data);
			refTexture->data = nullptr;
			return true;
		}
		else {
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &refTexture->ID);
			return false;
		}
	}

	texture* textureFactory::getDefaultTexture() {
		if (DefaultTexture != nullptr) {
			return DefaultTexture;
		}

		DefaultTexture = new texture();
		if (!glLoadSTBITextureData("assets/tex/default.png", DefaultTexture)) {
			std::cout <<
				"[Texture Manager FATAL]: Default texture could not be loaded! Cannot proceed in initialization!\n" <<
				"|-- Expected file path: \"assets/tex/\" had no default texture within it!\n";
			assert(false);
		}

		return DefaultTexture;
	}

	texture* textureFactory::loadTexture(const char* filePath, const char* alias) {
		char* aliasUsed;
		if (strcmp(alias, "") == 0) {
			aliasUsed = const_cast<char*>(filePath);
		}
		else {
			aliasUsed = const_cast<char*>(alias);
		}

		texture* tex = new texture();

		if (glLoadSTBITextureData(filePath, tex)) {
			LoadedTextures.insert(aliasUsed, tex);
			return tex;
		}
		else {
			std::cout <<
				"[Texture Manager]: Failed to load texture \"" << aliasUsed << "\".\n" <<
				"|-- STB Images could not load the image data:\n" <<
				stbi_failure_reason() << '\n';
			delete tex;
			return getDefaultTexture();
		}
	}

	bool textureFactory::findTexture(const char* alias, texture*& outTexture) {
		return LoadedTextures.find(alias, outTexture);
	}

	void textureFactory::terminate() {
		for (uint64_t i = 0; i < LoadedTextures.size; i++) {
			texture* tex = nullptr;
			char* alias = nullptr;
			if ((alias = LoadedTextures.data[i].key) != nullptr) {
				if (LoadedTextures.find(alias, tex)) {
					glDeleteTextures(1, &tex->ID);
				}

				LoadedTextures.remove(alias);
			}
		}
	}
}