// Project Identifier: 19034C8F3B1196BF8E0C6E1C0F973D2FD550B88F

#include "miner.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	std::ios_base::sync_with_stdio(false);
	MineEscape ME;

	ME.getOptions(argc, argv);
	ME.readMap();
	ME.output();
	return 0;
}