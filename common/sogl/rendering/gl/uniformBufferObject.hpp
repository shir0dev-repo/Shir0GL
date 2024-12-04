#pragma once

namespace sogl {
	typedef struct uniformBufferObject {
		// In OpenGL, the uniform buffer is treated like an array. This index specifies the "index" into that array
		// Two shaders using the same binding index will result in sharing the same data
		uint32_t bindingIndex;
		uint32_t ID;
		uint64_t bufferSize;
		const char* name;
		
		uniformBufferObject();

		bool bufferData(const void* data, const uint64_t dataSize, const uint64_t offset);
	} uniformBufferObject;
}