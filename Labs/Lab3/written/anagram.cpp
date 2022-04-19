/*
 * anagram.cpp
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 3 Written.
 * SUBMIT ONLY THIS FILE TO GRADESCOPE.
 */

// Common #includes for convenience.
// No need to use them.
// Remove any of them if you want.
#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
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

#include "anagram.h"

using namespace std;

bool isAnagram(const string &s1, const string &s2) {
	string str = s1, str2 = s2;
	int len = s1.length();

	for (size_t i = 0; i < len; ++i) {
		if (str.at(i) == ' ') {
			str.erase(i, 1);
			--len;

		}
	}

	len = s2.length();
	for (size_t i = 0; i < len; ++i) {
		if (str2.at(i) == ' ') {
			str2.erase(i, 1);
			--len;
		}
	}

	if (str.length() == str2.length()) {
		while (!str.empty() && !str2.empty()) {
			if (str2.find(str.at(0), 0) != string::npos) {
				str2.erase(str2.find(str.at(0), 0), 1);
				str.erase(0, 1);
			}
			else
				return false;
		}

		if (str.empty() && str2.empty())
			return true;
		else
			return false;
	}
	else
		return false;
}
