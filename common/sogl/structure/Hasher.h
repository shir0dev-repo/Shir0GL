#pragma once

#include <stdint.h>
namespace sogl {
	namespace Hasher {
		static uint64_t FNVHash(const void* data, const uint32_t size) {
			const uint64_t prime = 0x00000100000001B3;
			uint64_t hash = 0xCBF29CE484222325;
			char* begin = const_cast<char*>(reinterpret_cast<const char*>(data));

			for (char* c = begin; c != &(*(begin + 1)); c++) {
				hash = (hash ^ (*c)) * prime;
			}

			return hash;
		}
	}
}
