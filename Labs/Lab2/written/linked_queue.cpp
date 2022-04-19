/*
 * linked_queue.cpp
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 2 Written.
 * SUBMIT ONLY THIS FILE TO GRADESCOPE.
 */

// Common #includes for convience.
// No need to use them.
// Remove any of them if you want.
#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <math.h>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "linked_queue.h"

using namespace std;

template <typename T>
T LinkedQueue<T>::front() const {
  assert(!empty());
  return tail->value;
}

template <typename T>
void LinkedQueue<T>::pop() {	// pop tail (front) for queue
	assert(!empty());
	Node<T>* victim = tail;
	delete victim;
	if (tail != head) {	// If at least two nodes in queue
		// Find 2nd to front node to reassign new tail
		Node<T>* tailPrev = head;
		for (int i = 1; i < (size() - 1); ++i) {
			tailPrev = tailPrev->next;
		}
		tail = tailPrev;
		tail->next = nullptr;
	}
	else {	// If only one node in queue
		head = nullptr;
		tail = nullptr;
	}
	count -= 1;
}

template <typename T>
void LinkedQueue<T>::push(T x) {	// push new head (back) for queues
	Node<T>* node = new Node<T>;
	node->value = x;
	if (!empty()) {	// If queue is not empty
		node->next = head;
	}
	else {	// If queue is empty, need to assign new tail
		tail = node;
		node->next = nullptr;
	}
	head = node;
	count += 1;
}

template <typename T>
LinkedQueue<T>::~LinkedQueue() {
	while (!empty())
		pop();
}
