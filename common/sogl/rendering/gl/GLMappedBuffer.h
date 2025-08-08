#pragma once

#include <stdint.h>

namespace sogl {
	typedef struct GLMappedBuffer {
		uint32_t ID;
		uint32_t size;
		uint32_t flags;
		void* pointer;
	private:
		GLsync syncObj;
	public:
		GLMappedBuffer(const uint32_t size, const uint32_t flags, const void* data = nullptr);
		void update();
	};

}