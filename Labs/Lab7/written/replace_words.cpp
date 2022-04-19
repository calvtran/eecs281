/*
 * replace_words.cpp
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 7 Written.
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

#include "replace_words.h"

using namespace std;

vector<string> replace_words(const vector<string>& prefixes,
                             const vector<string>& sentence) {
  // TODO
    vector<string> out = sentence;
    
    for (size_t i = 0; i < out.size(); ++i) {              // N
        for (size_t j = 0; j < prefixes.size(); ++j) {          // P
            if (out[i].size() >= prefixes[j].size()) {
                if (out[i].find(prefixes[j]) != string::npos && out[i].find(prefixes[j]) == 0) {
                    out[i] = prefixes[j];
                }
            }
        }
    }
    return out;
}
