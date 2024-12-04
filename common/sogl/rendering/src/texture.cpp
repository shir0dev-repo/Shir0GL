#include "stbi/stb_image.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <sogl/rendering/texture.hpp>

namespace sogl {
	texture::texture() {
		
	}

	texture::texture(const char* filePath) {
		glGenTextures(1, &this->ID);
		glBindTexture(GL_TEXTURE_2D, this->ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		this->data = stbi_load(filePath, &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture : " << filePath << ".\n";
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}

	void texture::bind() const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->ID);
	}

	void texture::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	texture::~texture() {
		glDeleteTextures(1, &this->ID);
	}
}