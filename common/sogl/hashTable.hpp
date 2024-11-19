#pragma once

#include <cassert>
#include <string>

#define CAPACITY 50000

template<typename T>
T nextPowTwo(T num) {
	assert(num > 1);

	uint64_t iterations = sizeof(T) * 8;
	num--;

	for (int i = 1; i < iterations; i <<= 1) {
		num |= num >> i;
	}

	return ++num; 
}

inline char* findBufferEnd(const char* buffer) {
	char* end = const_cast<char*>(buffer);

	// 0xFFFF is max ushort value
	for (uint16_t i = 0; i < 0xFFFF; i++) {
		if (*end == '\0') {
			return end;
		}

		end++;
	}

	return end;
}

namespace sogl {
	template<typename T> class hashTable {
		struct hashTableItem {
			char* key;
			char* keyEnd;
			T* value;
			bool isManaged;
			uint64_t keyLength;

			inline hashTableItem() : key(nullptr), keyEnd(nullptr), value(nullptr), isManaged(true), keyLength(0) {}

			inline ~hashTableItem() {
				if (key) {
					delete[] key;
				}

				key = nullptr;
				keyEnd = nullptr;
				value = nullptr;
			}
		};

		inline uint64_t getHash(const char* buffer, const char* const bufferEnd) {
			const uint64_t prime = 0x00000100000001B3;
			uint64_t hash = 0xCBF29CE484222325;

			char* bufIter = const_cast<char*>(buffer);

			for (; bufIter < bufferEnd; bufIter++) {
				hash = (hash ^ (*bufIter)) * prime;
			}

			return hash;
		}

		inline bool compareKeys(const hashTableItem* item, const char* key, const char* keyEnd) {
			if (item->keyLength != (keyEnd - key)) {
				return false;
			}

			char* iterA = item->key;
			char* iterB = const_cast<char*>(key);

			for (; iterA < item->keyEnd; iterA++) {
				if (*iterB != *iterA) {
					return false;
				}

				iterB++;
			}

			return true;
		}
		inline void expand() {
			uint64_t newSize = size << 1;
			hashTable<T>::hashTableItem* temp = new hashTable<T>::hashTableItem[newSize];

			for (uint64_t i = 0; i < size; i++) {
				if (data[i].key == nullptr) {
					continue;
				}

				uint64_t hash = getHash(data[i].key, data[i].keyEnd) % newSize;
				uint64_t startingHash = hash;

				while (temp[hash].key != nullptr) {
					hash = (hash + 1) % newSize;

					if (hash == startingHash) {
						assert(false);
					}
				}

				temp[hash] = data[i];
			}

			delete[] data;
			data = temp;
			temp = nullptr;
			size = newSize;
		}
	public:
		uint64_t count;
		uint64_t size;
		hashTableItem* data;

		inline hashTable(uint64_t size) {
			this->size = size < 32 ? 32 : nextPowTwo(size);
			this->data = new hashTable<T>::hashTableItem[size];
			this->count = 0;
		}

		inline ~hashTable() {
			for (uint64_t i = 0; i < count; i++) {
				if (data[i].value == nullptr) {
					continue;
				}

				if (data[i].isManaged) {
					delete data[i].value;
				}

				delete[] data;
				data = nullptr;
			}
		}
		inline char* insert(const char* key, T* value, bool isManaged = false) {
			if (++count == size) {
				expand();
			}

			char* keyEnd = findBufferEnd(key);
			uint64_t hash = getHash(key, keyEnd);
			uint64_t startingHash = hash;

			while (data[hash].key != nullptr) {
				hash = (hash + 1) % size;

				if (hash == startingHash) {
					assert(false);
				}
			}

			data[hash].keyLength = keyEnd - key;
			data[hash].key = new char[data[hash].keyLength + 1];

			memcpy(data[hash].key, key, data[hash].keyLength + 1);
			data[hash].keyEnd = data[hash].key + data[hash].keyLength;
			data[hash].value = value;
			data[hash].isManaged = isManaged;

			return data[hash].key;
		}

		inline bool remove(const char* key) {
			char* keyEnd = findBufferEnd(key);

			uint64_t hash = getHash(key, keyEnd);
			uint64_t startingHash = hash;

			while (data[hash].key != nullptr) {
				if (compareKeys(&data[hash], key, keyEnd)) {
					delete[] data[hash].key;
					data[hash].key = nullptr;
					data[hash].keyEnd = nullptr;

					if (data[hash].isManaged == false) {
						delete data[hash].value;
					}

					data[hash].value = nullptr;
					data[hash].isManaged = true;
					count--;
				
					return true;
				}

				hash = (hash + 1) % size;
				if (hash == startingHash) {
					return false;
				}
			}

			return false;
		}

		inline bool contains(const char* key, T*& foundValue) {
			char* keyEnd = findBufferEnd(key);

			uint64_t hash = getHash(key, keyEnd) % size;
			uint16_t startingHash = hash;

			while (data[hash].value != nullptr) {
				if (compareKeys(&data[hash], key, keyEnd)) {
					foundValue = data[hash].value;
					return true;
				}

				hash = (hash + 1) % size;

				if (hash == startingHash) {
					return false;
				}
			}

			return false;
		}
	};
}