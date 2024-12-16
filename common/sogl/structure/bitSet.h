#pragma once

#include <stdint.h>
#include <sogl/mathutil.hpp>

namespace sogl {
	typedef class bitSet8_t {
		uint8_t m_size;
		uint8_t m_count;
		uint8_t m_start;

	public:
		inline bitSet8_t(const uint8_t startingSize = 4) {
			if (startingSize < 4) {
				m_size = 4;
			}
			else {
				m_size = nextPowTwo(startingSize);
			}
			m_count = 0;
			m_start = 0;			
		}
		inline void push(const bool val) {
			m_start = (m_start << 1) | val;
		}
		inline bool pop() {
			if (m_count == 0) return false;
			bool hasBit = 1 & m_start;
			m_start >>= 1;
			return hasBit;
		}
	} bitSet;
}