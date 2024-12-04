#pragma once

#include <string>

namespace sogl {
	template<typename T>
	class linkedList {
		
		/// <summary>Pointer wrapper for an item within the arrayList.</summary>
		struct node {
			/// <summary>The data held by this node.</summary>
			T* item;
			/// <summary>The next node in the arrayList.</summary>
			node* next;
			/// <summary><para>Lets the arrayList know whether "item" is being managed or not.</para><para> See linkedList.add().</para></summary>
			bool managed;
			
			inline node() {
				this->item = nullptr;
				this->next = nullptr;
				this->managed = false;
			}

			inline node(T*& item, bool isManaged = false) {
				this->next = nullptr;
				this->item = item;
				this->managed = isManaged;
			}
		};

		/// <summary>The amount of nodes that hold data.</summary>
		int m_count;
		/// <summary>The maximum nodes this arrayList can hold.</summary>
		int m_capacity;
		/// <summary>The array of nodes this arrayList points to.</summary>
		node** m_nodes;

		/// <summary>
		/// <para>Expands the size of this arrayList.</para>
		/// <para>Expanded size will always be double the current size. (m_capacity &lt;&lt; 1)</para>
		/// </summary>
		inline void expand() {
			uint32_t newCapacity = m_capacity << 1;

			node** temp = new node* [newCapacity];
			temp[0] = new node(m_nodes[0]->item, m_nodes[0]->managed);
			delete m_nodes[0];
			m_nodes[0] = nullptr;
			
			node* previous = temp[0];
			for (uint32_t i = 1; i < m_capacity; i++) {
				node* n = new node(m_nodes[i]->item, m_nodes[i]->managed);
				temp[i] = n;
				previous->next = n;
				previous = n;
			}
			// previous will equal the last node in m_nodes
			for (int i = m_capacity; i < newCapacity; i++) {
				temp[i] = new node();
				previous->next = temp[i];
				previous = temp[i];
			}

			delete[] m_nodes;
			m_nodes = temp;
			temp = nullptr;
			m_capacity = newCapacity;
		}

		/// <summary>
		/// <para>Retrieves the underlying node that the specified item resides in.</para>
		/// </summary>
		/// <param name="item">-- The item to search for.</param>
		/// <param name="outNode">-- The pointer to return after searching. Will be null if item could not be found.</param>
		/// <returns>
		/// <para>TRUE:</para>
		/// <para>- The specified item was found.</para>
		/// <para>- Parameter outNode will be set to the node containing the item.</para>
		/// <para>FALSE:</para>
		/// <para>- The specified item could not be found.</para>
		/// <para>- Parameter outNode will be SET to nullptr. (Never pass in a pointer you need!)</para>
		/// </returns>
		inline bool find(const T& item, node*& outNode) {
			if (m_count == 0) return false;
			
			outNode = m_nodes[0];
			if (*(outNode->item) == item) return true;

			while (outNode->next != nullptr) {
				if (*outNode->item == item) {
					return true;
				}
				else {
					outNode = outNode->next;
				}
			}

			outNode = nullptr;
			return false;
		}
	public:
		linkedList(uint32_t cap) {
			m_capacity = cap;
			m_count = 0;
			m_nodes = new node*[m_capacity];

			// setting the first node and starting iteration at 1 lets us avoid checking if i > 0 every run
			m_nodes[0] = new node();
			node* previous = m_nodes[0];

			// iterate through list and chain together references
			for (uint32_t i = 1; i < m_capacity; i++) {
				m_nodes[i] = new node();
				previous->next = m_nodes[i];
				previous = m_nodes[i];
			}
		}

		~linkedList() {
			clear();
			for (uint32_t i = 0; i < m_capacity; i++) {
				delete m_nodes[i];
			}

			delete[] m_nodes;
		}

		/// <summary>
		/// <para>Adds the specified item to the array list.</para>
		/// <para> </para>
		/// <para>If isManaged, removing the item from the list will not delete its value and only remove the pointer.</para>
		/// <para>The default is not managed, meaning the list will handle the pointer's underlying value.</para>
		/// </summary>
		/// <param name="item">-- The item to add to the array list.</param>
		/// <param name="isManaged">-- Specifies whether the data is being externally managed.</param>
		/// <returns></returns>
		inline bool add(T* item, bool isManaged = false) {
			if (++m_count > m_capacity) {
				expand();
			}

			node* n = nullptr;
			node* prev = nullptr;

			for (uint32_t i = 0; i < m_capacity; i++) {
				n = m_nodes[i];

				if (n->item != nullptr) {
					// store reference to previous node, so we can link it's "next" member to our inserted item
					prev = m_nodes[i];
					continue;
				}
				else { // m_nodes[i] == nullptr
					m_nodes[i] = new node(item, isManaged);
					
					if (prev != nullptr) {
						prev->next = m_nodes[i];
					}
					return true;
				}
			}
			//should never happen
			m_count--;
			return false;
		}

		/// <summary>
		/// <para>Removes the specified item within the list.</para>
		/// <para> </para>
		/// <para>Note: If the item was previously added with the managed specifier,</para>
		/// <para>only the pointer itself will be removed, not the data that this list points to.</para>
		/// </summary>
		/// <param name="item">-- The item to search for.</param>
		/// <returns>
		/// <para>TRUE:</para>
		/// <para>- The specified item was successfully removed from the list.</para>
		/// <para>FALSE:</para>
		/// <para>- The specified item was not in the list.</para>
		/// </returns>
		inline bool remove(const T& item) {
			node* n = nullptr;

			if (find(item, n)) {
				if (!n->managed) {
					delete n->item;
				}

				n->item = nullptr;
				n->managed = false;
				m_count--;
				return true;
			}
			else {
				return false;
			}
		}
		/// <summary>
		/// <para>Removes the data at a specified index, so long as the index is valid.</para>
		/// <para>Will not throw if index provided is outside of list's range.</para>
		/// </summary>
		/// <param name="index">-- The index within the list.</param>
		/// <returns>
		/// <para>TRUE:</para>
		/// <para>- The specified index contained a value.</para>
		/// <para>FALSE:</para>
		/// <para>- The current size of this list is zero.</para>
		/// <para>- The specified index was out of range.</para>
		/// <para>- The item at the specified index is NULL.</para>
		/// </returns>
		inline bool removeAt(uint32_t index) noexcept {
			if (m_count <= 0 || index < 0 || index >= m_capacity) {
				return false;
			}
			
			node* n = m_nodes[index];

			if (m_nodes->item == nullptr) {
				return false;
			}

			m_count--;
			if (!n->managed) {
				delete n->item;
			}
			
			n->item = nullptr;
			n->managed = false;
			return true;
		}

		/// <summary>
		/// <para>Empties the list of all its data, but keeps the nodes intact.</para>
		/// <para> </para>
		/// <para>Note: This does not remove the nodes within the list, only the data contained within them.</para>
		/// </summary>
		inline void clear() {
			if (m_count == 0) {
				return;
			}

			node* n = m_nodes[0];
			do {
				if (!n->managed) {
					delete n->item;
				}
				n->item = nullptr;
				n->managed = false;
			} while (m_count > 0 && (n = n->next) != nullptr);
		}

		inline T*& operator[](const uint32_t& index) {
			if (index < 0 || index >= m_capacity) {
				throw std::range_error("Specified index was out of range!\n");
			}

			return m_nodes[index]->item;
		}
	};
}