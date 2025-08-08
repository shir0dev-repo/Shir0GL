#pragma once

namespace sogl {

	struct Texture {
		int32_t width;
		int32_t height;
		int32_t nrChannels;
		uint32_t ID;
		uint8_t* data;

		Texture();
		Texture(const char* filePath);
		void bind() const;
		void unbind() const;
		~Texture();
	};
}