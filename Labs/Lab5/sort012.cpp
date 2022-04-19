/* 
 * sort012.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 5 Written.
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

#include "sort012.h"

using namespace std;

void sort012(vector<int>& nums) {
	if (!nums.empty()) {
		unsigned int next = 0;
		bool done = false;
		for (unsigned int i = 0; i < (int)nums.size() - 1; ++i) {
			if (nums[i] != 0) {
				done = false;
				for (unsigned int j = nums.size() - 1; j > i && !done; --j) {
					if (nums[j] == 0) {
						swap(nums[i], nums[j]);
						done = true;
					}
				}

				if (!done) {
					next = i;
					break;
				}
			}
		}

		for (unsigned int i = next; i < (int)nums.size() - 1; ++i) {
			if (nums[i] != 1) {
				done = false;
				for (unsigned int j = nums.size() - 1; j > i && !done; --j) {
					if (nums[j] == 1) {
						swap(nums[i], nums[j]);
						done = true;
					}
				}

				if (!done) {
					next = i;
					break;
				}
			}
		}
	}
}
