#pragma once

#include <sogl/structure/Hasher.h>

namespace sogl {
	template<typename K, typename V>
	struct KeyValuePair {
		K key;
		V value;
		KeyValuePair* next;

		inline KeyValuePair(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
	};

	template <typename TKey, typename TValue>
	class Dictionary {
		typedef KeyValuePair<TKey, TValue> Entry;

		Entry** m_buckets;

		uint32_t m_count;
		uint32_t m_capacity;

		inline void Expand() {
			uint32_t newCapacity = m_capacity << 1;
			Entry** temp = new Entry* [newCapacity];
			uint32_t i = 0;
			
			for (i; i < m_capacity; i++) {
				temp[i] = m_buckets[i];
			}
			for (i; i < newCapacity; i++) {
				temp[i] = nullptr;
			}

			m_capacity = newCapacity;
			delete[] m_buckets;
			m_buckets = temp;
			temp = nullptr;
		}

		inline uint32_t GetIndex(const TKey& key) {
			return Hasher::FNVHash(&key, sizeof(TKey)) % m_capacity;
		}

	public:
		inline uint32_t Count() { return m_count; }
		
		inline Dictionary(uint32_t initialCapacity = 32) {
			m_capacity = initialCapacity;
			m_buckets = new Entry*[m_capacity]();
			m_count = 0;
		}

		inline void Add(const TKey& key, TValue value) {
			if (ContainsKey(key)) {
				return;
			}
			if (++m_count > m_capacity) {
				Expand();
			}
			uint64_t index = GetIndex(key);
			Entry* kvp = new Entry(key, value);
			// ensure collisions are stored in the last entry of the bucket
			Entry* targetPosition = nullptr;
			if ((targetPosition = m_buckets[index]) != nullptr) {
				// find last valid entry
				while (targetPosition->next != nullptr) {
					targetPosition = targetPosition->next;
				}
				targetPosition->next = kvp;
			}
			else {
				m_buckets[index] = kvp;
			}
		}

		inline bool Remove(const TKey& key) {
			uint64_t index = GetIndex(key);

			Entry* kvp = m_buckets[index];
			return false;
		}

		inline void Clear() {
			for (uint32_t i = 0; i < m_count; i++) {
				if (m_buckets[i])
					delete[] m_buckets[i];
			}

			m_count = 0;
		}

		inline bool ContainsKey(const TKey& key) {
			for (uint32_t i = 0; i < m_count; i++) {
				if (m_buckets[i] == nullptr) continue;

				if (key == m_buckets[i]->key)
					return true;
			}

			return false;
		}

		inline TValue& operator[](const TKey& key) {
			uint32_t index = GetIndex(key);
			assert(m_buckets[index] != nullptr);

			return m_buckets[index]->value;
		}
	};
}