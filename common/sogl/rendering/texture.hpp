#pragma once

namespace sogl {

	struct texture {
		int32_t width;
		int32_t height;
		int32_t nrChannels;
		uint32_t ID;
		uint8_t* data;

		texture();
		texture(const char* filePath);
		void bind() const;
		void unbind() const;
		~texture();
	};
}