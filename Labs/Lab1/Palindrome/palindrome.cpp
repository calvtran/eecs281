/* 
 * palindrome.cpp 
 * Univeristy of Michigan, Ann Arbor
 * EECS 281 Lab 1 Written.
 * SUBMIT ONLY THIS FILE TO GRADESCOPE.
 */

// Common #includes for convience.
// No need to use them. 
// Remove any of them if you want.
#include "palindrome.h"

using namespace std;

bool isPalindrome(Node* start, Node* end) {
	// TODO
	if (start == end) {	// When word is odd, return true if at middle char
		return true;
	}
	else if (start->prev == end && start == end->next) {	// When word is even, return true if pass start passes end
		return true;
	}
	else if (start->value == end->value) {					// Recursive call if chars at start and end are equal
		return isPalindrome(start->next, end->prev);
	}
	else {
		return false;
	}
}
