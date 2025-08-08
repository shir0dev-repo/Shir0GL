#pragma once

namespace sogl {
	struct GLBuffer {
		unsigned int size;
		unsigned int ID;
		unsigned int target;

		GLBuffer();
		GLBuffer(const unsigned int& size, const void* data, const unsigned int& target);
		void bind() const;
		void bufferData(const unsigned int& size, const unsigned int& offset, const void* data);
		void unbind() const;
	};
}