#pragma once

#include <stdint.h>

namespace sogl {
	typedef struct mappedBuffer {
		uint32_t ID;
		uint32_t size;
		uint32_t flags;
		void* pointer;
	private:
		GLsync syncObj;
	public:
		mappedBuffer(const uint32_t size, const uint32_t flags, const void* data = nullptr);
		void update();
	};

}