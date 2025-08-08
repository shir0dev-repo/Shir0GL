#include <stdint.h>
#include <bitset>

namespace sogl {
	uint64_t trailing_zeroes(const uint64_t value) {
		std::bitset<64> b;
		b |= value;
		uint64_t bits = 0;
		for (uint64_t i = 0; i < 64; i++) {
			if (b[i] == 0)
				bits++;
			else
				break;
		}

		return bits;
	}

	uint64_t trailing_ones(const uint64_t value) {
		std::bitset<64> b;
		b |= value;
		uint64_t bits = 0;
		for (uint64_t i = 0; i < 64; i++) {
			if (b[i] == 1)
				bits++;
			else
				break;
		}

		return bits;
	}
}