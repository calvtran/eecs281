#include "knapsack.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
	vector<int> values = { 60, 100, 120 };
	vector<int> weights = { 10, 20, 30 };
	int CAP = 50;

	cout << "Expected: 220, Actual: " << knapsack(CAP, values, weights) << endl;

	vector<int> values2 = { 1, 2, 3, 4, 5, 6 };
	vector<int> weights2 = { 1, 3, 4, 7, 9, 10 };
	CAP = 20;

	cout << "Expected: 12, Actual: " << knapsack(CAP, values2, weights2) << endl;

	vector<int> values3 =  { 79, 92, 50, 37, 41, 86, 62, 48, 35, 31 };
	vector<int> weights3 = { 15, 20, 15,  8,  4, 19, 11, 21,  5,  7 };

	cout << "Expected: 138, Actual: " << knapsack(CAP, values3, weights3) << endl;

	return 0;
}