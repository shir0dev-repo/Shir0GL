#pragma once

#include <string>
#include <iostream>
#include <cassert>

template<typename T>
class linkedList {
	struct node {
		T item;
		node* next;
		inline node() {
			next = nullptr;
		}
		inline node(T item, node* next) {
			this->item = item;
			this->next = next;
		}
	};

	int m_count;
	node* head;
	node* tail;

public:
	typedef bool (*Comparer)(T t1, T t2);

	linkedList() : head(NULL), tail(NULL), m_count(0) {}
	
	~linkedList() {
		clear();
	}

	uint32_t size() const { return m_count; }
	inline const T* last() const {
		return &tail->item;
	}
	
	inline T* last() {
		return &tail->item;
	}

	void add(T item) {
		node* newNode = new node(item, nullptr);
		newNode->item = item;
		newNode->next = nullptr;

		if (!head) {
			head = newNode;
			m_count++;
			tail = head;
			return;
		}

		node* temp = head;
		while (temp->next) {
			temp = temp->next;
		}
		
		temp->next = tail = newNode;
		m_count++;
	}

	void addRange(const T* arr, uint32_t size) {
		for (uint32_t i = 0; i < size; i++) {
			add(arr[i]);
		}
	}

	void emplace(T item, uint32_t index) {
		if (index < 0) {
			std::cout << "Invalid index!\n";
			return;
		}
		// ignore above range indices, append to tail
		else if (index >= m_count) {
			add(item);
			return;
		}

		node* newNode = new node();
		newNode->item = item;
		
		// make new entry list head
		if (index == 0) {
			newNode->next = head;
			head = newNode;
			m_count++;
			return;
		}
		// make new entry list tail
		else if (index == m_count - 1) {
			tail->next = newNode;
			tail = newNode;
			m_count++;
			return;
		}

		node* temp = head;
		node* lastTemp = nullptr;
		uint32_t current = 0;

		while (temp && current++ < index) {
			lastTemp = temp;
			temp = temp->next;
		}

		assert(lastTemp);
		newNode->next = lastTemp->next;
		lastTemp->next = newNode;
		m_count++;
	}

	bool remove(T item) {
		node* temp = head;
		node* lastTemp = nullptr;

		// empty list
		if (!temp) {
			return false;
		}

		// remove head
		if (temp->item == item) {
			head = temp->next;
			delete temp;
			m_count--;
			return true;
		}
		else {
			lastTemp = temp;
			temp = temp->next;
		}

		while (temp) {
			if (temp->item == item) {
				if (temp->next) {
					lastTemp->next = temp->next;
				}
				else {
					lastTemp->next = nullptr;
				}

				delete temp;
				m_count--;
				return true;
			}
			else {
				lastTemp = temp;
				temp = temp->next;
			}
		}
		// couldn't find item
		return false;
	}

	bool removeAt(const uint32_t index) {
		// index invalid
		if (index < 0 || index >= m_count) {
			return false;
		}
		// empty list
		else if (m_count == 0) {
			return false;
		}

		// deleting from beginning, make head->next new head
		if (index == 0) {
			node* temp = head->next;
			delete this->head;
			m_count--;
			this->head = temp;
			return true;
		}

		node* temp = head;
		node* lastTemp = nullptr;
		for (uint32_t i = 0; i < index; i++) {
			lastTemp = temp;
			temp = temp->next;
		}

		if (temp->next) {
			lastTemp->next = temp->next;
		}

		delete temp;
		m_count--;
		return true;
	}

	bool contains(T value, Comparer comp) {
		node* current = head;
		while (current->next != nullptr) {
			if (comp(value, current)) {
				return true;
			}
		}

		return false;
	}

	void clear() {
		if (!head) {
			return;
		}

		node* temp = head;
		node* n = nullptr;
		while (temp) {
			n = temp->next;
			delete temp;
			temp = n;
		}

		m_count = 0;
		head = tail = nullptr;

	}
	inline void toArray(T*& outArray) const {
		if (m_count <= 0) return;

		if (outArray != nullptr) {
			delete[] outArray;
		}

		outArray = new T[this->m_count];

		int index = 0;
		node* current = head;
		while (index < m_count && current != nullptr) {
			outArray[index++] = current->item;
			current = current->next;
		}
	}

	inline T operator[] (uint32_t index) const {
		assert(m_count > 0 && index >= 0 && index < m_count);
		
		node* temp = head;
		uint32_t current = 0;
		
		// prefix increment so we get the last valid node
		while (temp && ++current < index) {
			temp = temp->next;
		}

		assert(temp);
		return temp->item;
	}

	inline T& operator[](uint32_t index) {
		assert(m_count > 0 && index >= 0 && index < m_count);

		node* temp = head;
		uint32_t current = 0;
		while (temp && ++current < index) {
			temp = temp->next;
		}

		return temp->item;
	}
};