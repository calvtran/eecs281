/* 
 * two_pair_sums.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 6 Written.
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

#include "two_pair_sums.h"

using namespace std;

void two_pair_sums(const vector<int>& nums, ostream& os) {
  // TODO
  using myPair = pair<int, int>;
  using sum = int;
  unordered_map<sum, myPair> map;

  if (!nums.empty()) {
	  for (size_t i = 0; i < nums.size() - 1; ++i) {
		  for (size_t j = i + 1; j < nums.size(); ++j) {
			  auto iter = map.find(nums[i] + nums[j]);
			  if (iter != map.end()) {
				  os << "(" << iter->second.first << ", " << iter->second.second << ") and (" << nums[i] << ", " << nums[j] << ")\n";
				  map[nums[i] + nums[j]] = make_pair(nums[i], nums[j]);
			  }
			  else
				  map[nums[i] + nums[j]] = make_pair(nums[i], nums[j]);
		  }
	  }
  }
}