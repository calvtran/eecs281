/* 
 * join_ropes.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 4 Written.
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

#include "join_ropes.h"

using namespace std;

int join_ropes(const vector<int>& rope_lengths) {
	int cost = 0;
	priority_queue<int, vector<int>, greater<int>> values;
	vector<int> sums;
	for (size_t i = 0; i < rope_lengths.size(); ++i) {
		values.push(rope_lengths[i]);
	}

	while (!values.empty()) {
		int a = values.top();
		values.pop();
		if (values.empty())
			break;
		int b = values.top();
		int c = a + b;
		values.push(c);
		sums.push_back(c);
		values.pop();
	}

	for (size_t i = 0; i < sums.size(); i++) {
		cost += sums[i];
	}

	return cost;
}
