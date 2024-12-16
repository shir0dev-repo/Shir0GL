#pragma once

#include <sogl/structure/hashTable.hpp>
#include <sogl/rendering/gl/uniformBufferObject.hpp>

namespace sogl {
	typedef class uniformBufferFactory {
		static hashTable<uniformBufferObject> m_uniformBuffers;
	public:
		static uint16_t getBufferSize(const uint16_t program, const uint16_t blockIndex);

		static uniformBufferObject* createNew(const char* bufferName, const uint32_t bufferSize);
		static bool find(const char* bufferName, uniformBufferObject*& outUBO);

		static void terminate();
	} uniformBufferFactory;
}