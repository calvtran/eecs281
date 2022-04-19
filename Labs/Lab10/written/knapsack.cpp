/* 
 * knapsack.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 10 Written.
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

#include "knapsack.h"

using namespace std;

int max(int a, int b) {
    return (a < b) ? b : a;
}

int knapsack(int C, const vector<int>& value, const vector<int>& weight) {
    vector<vector<int>> K(value.size() + 1);

    for (size_t i = 0; i <= value.size(); i++)
    {
        for (int w = 0; w <= C; w++)
        {
            if (i == 0 || w == 0)
                K[i].push_back(0);
            else if (weight[i - 1] <= w)
                K[i].push_back(max(value[i - 1] + K[i - 1][w - weight[i - 1]], K[i - 1][w]));
            else
                K[i].push_back(K[i - 1][w]);
        }
    }

    return K[value.size()][C];
}
