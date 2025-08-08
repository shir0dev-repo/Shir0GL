#include <GLEW/glew.h>

#include <iostream>

#include <sogl/rendering/glUtilities.h>
#include <sogl/rendering/factories/uniformBufferFactory.hpp>

namespace sogl {
	hashTable<UniformBuffer> uniformBufferFactory::m_uniformBuffers(32);

	uint16_t uniformBufferFactory::getBufferSize(const uint16_t program, const uint16_t blockIndex) {
		int size = 0;
		glGetActiveUniformBlockiv(program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
		return size;
	}

	UniformBuffer* uniformBufferFactory::createNew(const char* bufferName, const uint32_t bufferSize) {
		glErrorCB(glGetError());
		UniformBuffer* buf = nullptr;
		if (find(bufferName, buf)) {
			std::cout <<
				"[Uniform Buffer Manager]: Failed to create uniform buffer!\n" <<
				"|-- Specified buffer " << bufferName << " already exists!\n";
			return buf;
		}
		
		// create a new blank buffer
		buf = new UniformBuffer();
		buf->name = bufferName;
		buf->bufferSize = bufferSize;

		glGenBuffers(1, &buf->ID);
		glBindBuffer(GL_UNIFORM_BUFFER, buf->ID);
		glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
		
		// until i think of a better way to set this up, im just directly mapping these to how many there are
		// we do this before the buffer is inserted into the hashtable so that the uniform binding points correctly start at zero
		buf->bindingIndex = m_uniformBuffers.count;
		glBindBufferBase(GL_UNIFORM_BUFFER, buf->bindingIndex, buf->ID);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		m_uniformBuffers.insert(buf->name, buf);
		
		return buf;
	}

	bool uniformBufferFactory::find(const char* bufferName, UniformBuffer*& outUBO) {
		return m_uniformBuffers.find(bufferName, outUBO);
	}

	void uniformBufferFactory::terminate() {
		uint32_t countRemoved = 0, initialCount = m_uniformBuffers.count;
		for (uint32_t i = 0; i < m_uniformBuffers.size; i++) {
			const char* key;
			if ((key = m_uniformBuffers.data[i].key) == nullptr)
				continue;

			std::cout << "[Uniform Buffer Manager]: Deleting buffer \"" << key << "\"...\n";

			UniformBuffer* buf = m_uniformBuffers.data[i].value;

			if (glIsBuffer(buf->ID)) {
				glDeleteBuffers(1, &buf->ID);
			}
			

			buf->ID = 0;
			buf->bufferSize = 0;
			buf->bindingIndex = -1;
			std::cout << "|-- Deleted buffer \"" << key << "\".\n";
			m_uniformBuffers.remove(key);

			if (m_uniformBuffers.count == 0)
				break;
		}
	}
}