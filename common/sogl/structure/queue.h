#pragma once

#include "linkedList.h"

template<typename T>
class queue {
	struct node {
		T item;

		node() = default;
		node(T item) {
			this->item = item;
		}
	};

	linkedList<T> m_queue;
	
public:
	queue() : m_queue() {}

	bool empty() const {
		return m_queue.size() == 0;
	}

	void enqueue(T item) {
		m_queue.add(item);		
	}

	bool peek(T& outValue) {
		if (empty()) {
			return false;
		}

		outValue = m_queue[0];
		return true;
	}

	T dequeue() {
		T val = m_queue[0];
		m_queue.removeAt(0);
		return val;
	}
};