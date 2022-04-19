/* 
 * cycle.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 9 Written.
 * SUBMIT ONLY THIS FILE TO GRADESCOPE.
 */

// Common #includes for convience.
// No need to use them. 
// Remove any of them if you want.
#include <algorithm>
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
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "cycle.h"

using namespace std;

bool visit(size_t vertex, vector<bool> visited, const vector<vector<int>>& adj_list, size_t visitor) {
	visited[vertex] = true;

	for (size_t i = 0; i < adj_list[vertex].size(); ++i) {
		if (visited[adj_list[vertex][i]] == false) {
			if (visit(adj_list[vertex][i], visited, adj_list, vertex))
				return true;
		}
		else if (adj_list[vertex][i] != visitor)
			return true;
	}
	return false;
};

bool is_graph_cyclic(const vector<vector<int>> &adj_list) {
	vector<bool> visited(adj_list.size(), false);

	for (size_t i = 0; i < adj_list.size(); ++i) {
		if (visited[i] == false)
			if (visit(i, visited, adj_list, -1))
				return true;
	}

	return false;
}


//int main() {
//	vector<vector<int>> graph1 = { {1,2}, {0,2}, {0,1,3}, {2} };
//	cout << "Expected = 1, Actual = " << is_graph_cyclic(graph1) << endl;
//
//	vector<vector<int>> graph2 = { {1}, {0,2}, {1,3}, {2} };
//	cout << "\nExpected = 0, Actual = " << is_graph_cyclic(graph2) << endl;
//
//	vector<vector<int>> graph3 = { {1,4}, {0,2}, {1,3}, {2,4}, {0,3} };
//	cout << "\nExpected = 1, Actual = " << is_graph_cyclic(graph3) << endl;
//
//	return 0;
//}