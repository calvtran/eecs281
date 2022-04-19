//	Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <iostream>
#include <getopt.h>
#include <cstring>
#include "sillyQL.h"

using namespace std;

int main(int argc, char* argv[]) {
	ios_base::sync_with_stdio(false);
	cin >> std::boolalpha;
	cout << std::boolalpha;

	SillyQL silly;

	if (argc > 1) {
		if (strcmp(argv[1], "-q") == 0 || strcmp(argv[1], "--quiet") == 0)
			silly.STFU();
		else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
			silly.help();
		else {
			cerr << "Error: Invalid command line option\n";
			exit(1);
		}

		if (argc > 2) {
			if (strcmp(argv[2], "-q") == 0 || strcmp(argv[2], "--quiet") == 0)
				silly.STFU();
			else if (strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0)
				silly.help();
			else {
				cerr << "Error: Invalid command line option\n";
				exit(1);
			}
		}
	}

	string cmd;
	string cmds = "CIDRJ#PGQ";
	do {
		cout << "% ";
		cin >> cmd;

		switch (cmd[0]) {
		case 'C':
				silly.createTable();
				break;
		case 'I':
				silly.insert_into_table();
				break;
		case 'D':
				silly.delete_from_table();
				break;
		case 'R':
				silly.removeTable();
				break;
		case 'J':
				silly.joinTables();
				break;
		case 'P':
				silly.print();
				break;
		case 'G':
				silly.genIndex();
				break;
		case '#':
			break;
		case 'Q':
			break;
		default:
			cout << "Error: unrecognized command\n";
			//getline(cin, cmd);
			break;
		}

		if (cmd != "QUIT")
			getline(cin, cmd);

	} while (cmd != "QUIT");

	cout << "Thanks for being silly!\n";

	return 0;
}