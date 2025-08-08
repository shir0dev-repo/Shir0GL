#pragma once

#include <stdint.h>

namespace sogl {
	template <typename T>
	class Array {
		T* m_values;
		uint32_t m_capacity;

	public:
		template <class K>
		static inline uint32_t Length(K* arr) {
			return (arr == nullptr) ? 0 : *(&arr + 1) - arr;
		}

		inline Array(uint32_t capacity = 1) {
			if (capacity < 1) {
				throw;
			}
			m_capacity = capacity;
			m_values = new T[m_capacity];
		}

		inline const uint32_t Length() const { return m_capacity; }
		
		inline T& operator[](uint32_t index) {
			if (index < 0 || index > m_capacity) {
				throw;
			}

			return m_values[index];
		}
	};
}