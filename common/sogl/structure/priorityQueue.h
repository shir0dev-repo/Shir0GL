#pragma once

#include "linkedList.h"

enum priorityMode {
	// Sorts by lower priority first (i.e., 0 will be popped before 1, etc.)
	ascending,
	// Sorts by higher priority first (i.e., 1 will be popped before 0, etc.)
	descending
};

template<typename T>
class priorityQueue {
public:
	/// <summary>
	/// <para>When implementing this delegate, it should return the following:</para>
	/// <para>-1: a has lower priority</para>
	/// <para> 0: a and b have equal priority</para>
	/// <para> 1: a has higher priority</para>
	/// </summary>
	typedef int (*priorityFunc)(T a, T second);

private:
	struct priorityNode {
		T item;
		uint32_t priority;

		priorityNode() = default;
		priorityNode(T item, uint32_t priority) {
			this->item = item;
			this->priority = priority;
		}
	};

	priorityFunc m_comparer = nullptr;
	linkedList<priorityNode> m_queue;
	const priorityMode m_mode;

	uint32_t getPriorityIndex(uint32_t priority) {
		uint32_t size = m_queue.size();
		if (size == 0) {
			return 0;
		}

		uint32_t lastIndex = 0;
		uint32_t lastPrio;
		// find index such that: (index - 1).priority > priority > (index + 1).priority
		if (m_mode == priorityMode::ascending) {
			lastIndex = 0;
			
			// find first entry of equal or greater priority
			while ((lastPrio = m_queue[lastIndex].priority) < priority) {
				if (++lastIndex == size) {
					return lastIndex;
				}
			}

			if (lastPrio > priority) {
				return lastIndex;
			}
			// if more than 1 entry exists for this priority, follow FIFO rule
			while ((lastPrio = m_queue[lastIndex].priority) == priority) {
				if (++lastIndex == size) {
					return lastIndex;
				}
			}

			return lastIndex;
		}
		// find index such that:(index - 1).priority < priority <= (index + 1).priority
		else {
			// find first entry of equal or lesser priority
			while ((lastPrio = m_queue[lastIndex].priority) > priority) {
				if (++lastIndex == size) {
					return lastIndex;
				}
			}

			if (lastPrio > priority) {
				return lastIndex;
			}

			while ((lastPrio = m_queue[lastIndex].priority) == priority) {
				if (++lastIndex == size) {
					return lastIndex;
				}
			}

			return lastIndex;
		}
	}

	uint32_t getPriorityIndex(const T item, priorityFunc pFunc) {
		uint32_t size = m_queue.size();
		if (size == 0)
			return 0;

		uint32_t lastIndex = 0;
		uint32_t lastPrio;
		if (m_mode == priorityMode::ascending) {
			lastPrio = pFunc(item, m_queue[0].item);
			// priority is lowest found
			if (lastPrio < 0) {
				return 0;
			}

			priorityNode current = m_queue[lastIndex];
			// find first item that is equal priority
			while ((lastPrio = current.priority + pFunc(item, current.item)) < current.priority) {
				// reached end
				if (++lastIndex == size) {
					return lastIndex;
				}

				current = m_queue[lastIndex];
			}

			if (lastPrio >= lastIndex || lastIndex >= size) {
				return lastIndex;
			}
			// find first item that is lower priority
			while ((lastPrio = current.priority + pFunc(item, current.item)) == current.priority) {
				if (++lastIndex == size) {
					return lastIndex;
				}

				current = m_queue[lastIndex];
			}

			return lastIndex;
		}
		else { // priorityMode::descending
			priorityNode current = *m_queue.last();
			lastPrio = current.priority + pFunc(item, current.item);
			if (lastPrio < 0) {
				return size - 1;
			}
			// find first item of equal priority
			while ((lastPrio = current.priority + pFunc(item, current.item)) > current.priority) {
				if (++lastIndex == size) {
					return lastIndex;
				}

				current = m_queue[lastIndex];
			}
			if (lastPrio >= lastIndex || lastIndex >= size) {
				return lastIndex;
			}

			while ((lastPrio = current.priority + pFunc(item, current.item)) == current.priority) {
				if (++lastIndex == size) {
					return lastIndex;
				}

				current = m_queue[lastIndex];
			}

			return lastIndex;
		}
	}
public:
	priorityQueue(priorityMode mode) : m_queue(), m_mode(mode) {}
	priorityQueue(priorityMode mode, priorityFunc comparer) : m_queue(), m_mode(mode), m_comparer(comparer) {}

	bool empty() const {
		return m_queue.size() == 0;
	}

	void enqueue(T item, priorityFunc pFunc) {
		priorityNode node(item, getPriorityIndex(item, pFunc));
		m_queue.emplace(node, node.priority);
	}
	void enqueue(T item, uint32_t priority) {
		priorityNode node(item, priority);	
		m_queue.emplace(node, getPriorityIndex(priority));
	}

	bool peek(T& outValue) {
		if (empty()) {
			return false;
		}

		outValue = m_queue[0].item;
		return true;
	}

	T dequeue() {
		T val = m_queue[0].item;
		m_queue.removeAt(0);
		return val;
	}
};