#pragma once

#include <stdint.h>
#include <sogl/linkedList.h>

namespace sogl {
	template <class T>
	class AssetFactory {
		struct node {
			linkedList<T> bucket;
			node* next;
		};

		uint32_t m_size;
		uint32_t m_capacity;

		node* m_data;

		AssetFactory(uint64_t maxSize = 32) {
			m_capacity = maxSize;
		}
	};
}