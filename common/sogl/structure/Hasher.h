#pragma once

#include <stdint.h>
namespace sogl {
	namespace Hasher {
		uint64_t FNVHash(const void* data, const uint32_t size);
	}
}
