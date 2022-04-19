// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7

#include "Pathfinder.h"
#include <getopt.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	std::ios_base::sync_with_stdio(false);

	Pathfinder path;

	path.getOptions(argc, argv);
	path.readMap();

	return 0;
}