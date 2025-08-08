#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <sogl/structure/hashTable.hpp>
#include <sogl/transform/vec3f.hpp>
#include <sogl/transform/vec4f.hpp>
#include <sogl/transform/matrix3f.hpp>
#include <sogl/transform/matrix4f.hpp>
#include <sogl/rendering/glUtilities.h>
#include <sogl/rendering/gl/UniformBuffer.h>

namespace sogl {
	UniformBuffer::UniformBuffer() : name(""), ID(0), bufferSize(0), bindingIndex(0) {}

	bool UniformBuffer::bufferData(const void* data, const uint64_t dataSize, const uint64_t offset) {
		if (dataSize == 0) {
			std::cout <<
				"[GLERROR]: Failed to buffer data to " << name << '\n' <<
				"|-- Cannot buffer data with a size of zero!\n";
			return false;
		}
		else if (data == nullptr) {
			std::cout <<
				"[GLERROR]: Failed to buffer data to " << name << "\n" <<
				"|-- Cannot buffer NULL data!\n";
			return false;
		}
		else if (ID == 0 || bufferSize == 0) {
			std::cout <<
				"[GLERROR]: Failed to buffer data to " << name << "\n" <<
				"|-- Buffer is not initialized!\n" <<
				"  |-- Initialize this buffer with a maximum size in bytes.\n";
			return false;
		}
		else if ((uint64_t)(dataSize + offset) > bufferSize) {
			std::cout <<
				"[GLERROR]: Specified size (" << dataSize << " at the given offset " << offset << ") would overflow the buffer!\n" <<
				"|-- Size of buffer: " << bufferSize << '\n' <<
				"|-- Specified data range: " << offset << " to " << (dataSize + offset) << '\n' <<
				"|-- Size difference: " << (((uint64_t)offset + dataSize) - bufferSize) << " bytes\n";
			return false;
		}

		// ensure this buffer is currently bound
		glBindBuffer(GL_UNIFORM_BUFFER, this->ID);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSize, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}