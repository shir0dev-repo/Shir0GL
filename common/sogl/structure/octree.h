#pragma once

#define DEBUG 1

#include <stdint.h>
#include <sogl/transform/vec3i.hpp>
#include <sogl/structure/pair.h>

#if DEBUG
#include <GLEW/glew.h>
#include <sogl/debug/debug.h>
#include <sogl/rendering/color.hpp>
#endif
namespace sogl {

	template <typename T>
	class octree {
		typedef struct node {
			vec3f center;
			vec3f size;
			uint8_t depth;

			pair<T*, vec3f>** objects;
			uint8_t objectCount;
			node** children;

			inline node() : center(), size(1, 1, 1), children(nullptr), objects(nullptr), objectCount(0) {}
			inline node(vec3f pos, vec3f extents, uint8_t depth = 0) : center(pos), size(extents), objectCount(0), depth(depth), children(nullptr), objects(nullptr) {}

			inline bool insert(T* obj, const vec3f& position, const uint8_t& maxObjects, const uint8_t& maxDepth, uint8_t& currentTreeDepth) {
				// prioritize children
				if (children != nullptr) {
					uint8_t childIndex = getChildIndex(position);

					return children[childIndex]->insert(obj, position, maxObjects, maxDepth, currentTreeDepth);
				}
				// no objects added to node
				if (objects == nullptr) {
					objects = new pair<T*, vec3f>* [maxObjects];
					for (int i = 0; i < maxObjects; i++) {
						objects[i] = new pair<T*, vec3f>{ nullptr, vec3f() };
					}

					objects[objectCount++] = new pair<T*, vec3f>(obj, position);
					return true;
				}
				// node has space for object
				if (objectCount < maxObjects) {
					objects[objectCount++] = new pair<T*, vec3f>(obj, position);
					return true;
				}
				// node reached max objects
				else {
					if (!subdivide(maxDepth)) {
						return false;
					}
					currentTreeDepth++;

					for (uint8_t i = 0; i < maxObjects; i++) {
						uint8_t index = getChildIndex(objects[i]->second);
						pair<T*, vec3f>* temp = new pair<T*, vec3f>(objects[i]->first, objects[i]->second);

						children[index]->insert(temp->first, temp->second, maxObjects, maxDepth, currentTreeDepth);
						this->objects[i]->first = nullptr;
						delete objects[i];
					}

					delete[] objects;
					objects = nullptr;
					objectCount = 0;
					return true;
				}
			}

			bool query(const vec3f& position, T*& outVal, uint8_t maxObjects) {
				if (children != nullptr) {
					int childIndex = getChildIndex(position);
					return children[childIndex]->query(position, outVal, maxObjects);
				}
				else if (objects == nullptr) {
					outVal = nullptr;
					return false;
				}

				for (int i = 0; i < maxObjects; i++) {
					if (objects[i] != nullptr && objects[i]->second == position) {
						outVal = objects[i]->first;
						return true;
					}
				}

				outVal = nullptr;
				return false;
			}

			void iterate(void (*func)(octree<T>::node* n)) {
				func(this);
				if (children) {
					for (int i = 0; i < 8; i++) {
						children[i]->iterate(func);
					}
				}
			}

		private:
			inline bool subdivide(const uint8_t& maxDepth) {
				if (children != nullptr) {
					return false;
				}
				else if (this->depth >= maxDepth) {
					return false;
				}

				children = new node * [8];
				vec3f newSize = this->size * 0.5f;
				uint8_t newDepth = this->depth + 1;

				for (uint8_t i = 0; i < 8; i++) {
					vec3f offset = vec3f(
						(i & 1) == 0 ? -newSize.x * 0.5f : newSize.x * 0.5f,
						(i & 2) == 0 ? -newSize.y * 0.5f : newSize.y * 0.5f,
						(i & 4) == 0 ? -newSize.z * 0.5f : newSize.z * 0.5f);

					children[i] = new node(this->center + offset, newSize, newDepth);
				}

				return true;
			}

			uint8_t getChildIndex(const vec3f& position) {
				uint8_t index = 0;
				if (position.x >= center.x) index |= 1;
				if (position.y >= center.y) index |= 2;
				if (position.z >= center.z) index |= 4;

				return index;
			}
		};

		node* m_rootNode;
		uint8_t m_maxObjects;
		uint8_t m_maxDepth;
		uint8_t m_currentDepth;
	public:
		octree(vec3f center, vec3f initialSize, uint8_t maxObjects = 8, uint8_t maxDepth = 8) :
			m_maxDepth(maxDepth), m_currentDepth(0), m_maxObjects(maxObjects) {
			m_rootNode = new node(center, initialSize);
		}

		bool insert(T* obj, const vec3f& center) {
			return m_rootNode->insert(obj, center, m_maxObjects, m_maxDepth, m_currentDepth);
		}

		bool query(const vec3f& position, T*& outVal) {
			return m_rootNode->query(position, outVal, m_maxObjects);
		}

		void drawOutline() {
			m_rootNode->iterate(&octree::drawFunc);
		}


		static void drawFunc(octree<T>::node* node) {
			color boxColor(0, 1, 0, 0.015f);
			color pointColor(1, 0, 0, 1);

			debug::drawCube(node->center, node->size, boxColor);
			
			for (int i = 0; i < node->objectCount; i++) {
				debug::drawPoint(node->objects[i]->second, color::RED);
			}
		}
	};
}