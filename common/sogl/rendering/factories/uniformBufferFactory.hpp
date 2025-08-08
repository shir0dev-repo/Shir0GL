#pragma once

#include <sogl/structure/hashTable.hpp>
#include <sogl/rendering/gl/UniformBuffer.h>

namespace sogl {
	typedef class uniformBufferFactory {
		static hashTable<UniformBuffer> m_uniformBuffers;
	public:
		static uint16_t getBufferSize(const uint16_t program, const uint16_t blockIndex);

		static UniformBuffer* createNew(const char* bufferName, const uint32_t bufferSize);
		static bool find(const char* bufferName, UniformBuffer*& outUBO);

		static void terminate();
	} uniformBufferFactory;
}