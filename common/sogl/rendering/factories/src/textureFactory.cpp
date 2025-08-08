#include <GLEW/glew.h>
#include <stbi/stb_image.h>

#include <iostream>

#include <sogl/rendering/factories/TextureFactory.h>

namespace sogl {
	hashTable<Texture> TextureFactory::LoadedTextures(32);
	Texture* TextureFactory::DefaultTexture = nullptr;

	bool TextureFactory::glLoadSTBITextureData(const char* filePath, Texture* refTexture) {
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

	Texture* TextureFactory::getDefaultTexture() {
		if (DefaultTexture != nullptr) {
			return DefaultTexture;
		}

		DefaultTexture = new Texture();
		if (!glLoadSTBITextureData("assets/tex/default.png", DefaultTexture)) {
			std::cout <<
				"[Texture Manager FATAL]: Default texture could not be loaded! Cannot proceed in initialization!\n" <<
				"|-- Expected file path: \"assets/tex/\" had no default texture within it!\n";
			assert(false);
		}

		return DefaultTexture;
	}

	Texture* TextureFactory::loadTexture(const char* filePath, const char* alias) {
		char* aliasUsed;
		if (strcmp(alias, "") == 0) {
			aliasUsed = const_cast<char*>(filePath);
		}
		else {
			aliasUsed = const_cast<char*>(alias);
		}

		Texture* tex = new Texture();

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

	bool TextureFactory::findTexture(const char* alias, Texture*& outTexture) {
		return LoadedTextures.find(alias, outTexture);
	}

	void TextureFactory::terminate() {
		for (uint64_t i = 0; i < LoadedTextures.size; i++) {
			Texture* tex = nullptr;
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