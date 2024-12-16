#pragma once

#include <stdint.h>
#include <iostream>
#include <sogl/structure/linkedList.h>

namespace sogl {
	template<typename T>
	class runLengthEncoding {
		struct RLENode {
			T object;
			uint32_t runLength;
			
			RLENode() {
				runLength = 0;
			}

			RLENode(uint32_t runLength, T object) {
				this->runLength = runLength;
				this->object = object;
			}
		};
	public:
		typedef bool (*comparer)(const T& a, const T& b);
	private:
		linkedList<RLENode> m_data;
		uint32_t m_totalLength;
		comparer m_comp;
	public:
		inline runLengthEncoding() : m_data(), m_totalLength(0) {}
		inline runLengthEncoding(T* objArray, const uint32_t arraySize, comparer comp) : m_data(), m_totalLength(0), m_comp(comp) {
			if (arraySize <= 0) return;
			else if (m_comp == nullptr) return;

			encode(objArray, arraySize);
		}

		void add(T obj) {
			if (m_data.last() && m_data.last()->object == obj) {
				m_data.last()->runLength++;
				return;
			}

			m_data.add(RLENode(1, obj));
			m_totalLength++;
		}

		void encode(T* objArray, const uint32_t arraySize) {
			if (arraySize <= 0) return;

			T* prev = const_cast<T*>(&objArray[0]);
			T* curr = nullptr;

			uint32_t run = 0;
			uint32_t index = 1;
			while ((curr = &objArray[index]) != nullptr && index <= arraySize) {
				index++;
				run++;
				if (!m_comp(*prev, *curr)) {
					RLENode rle(run, *prev);
					m_data.add(rle);
					m_totalLength += run;
					run = 0;
					prev = curr;
					continue;
				}

				
			}
		}

		bool writeToFile(const char* filePath, const char* fileExtension) {
			char buf[256];
			snprintf(buf, sizeof(buf), "%s%s", filePath, fileExtension);

			FILE* file = fopen(buf, "wb");

			if (file == nullptr) {
				printf("Could not write to file! Resulting path: %s", buf);
				return false;
			}

			RLENode* current = nullptr;
			uint32_t index = 0;
			RLENode* nodeArr = nullptr;
			m_data.toArray(nodeArr);

			fwrite(nodeArr, sizeof(RLENode), m_data.size(), file);
			//while (index < m_data.size()) {
			//	current = &m_data[index++];
			//	fwrite(&current->object, sizeof(T), 1, file);
			//	fwrite(&current->runLength, sizeof(uint32_t), 1, file);
			//}

			fclose(file);
			printf("Finished writing binary file! Output:\n");
			printf(buf);
		}
	};
}