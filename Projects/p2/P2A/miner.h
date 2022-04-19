// Project Identifier: 19034C8F3B1196BF8E0C6E1C0F973D2FD550B88F

#include "P2random.h"
#include <queue>
#include <vector>
#include <getopt.h>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <limits>

using namespace std;

class MineEscape {
private:
	struct tile {
		int rubble = 0;
		bool discovered = false;
		bool TNTcleared = false;
		bool investigated = false;
	};
	vector<vector<tile>> map;

	struct PQ {
		int rubble = 0;
		int row = 0, col = 0;
	};

	// Functor comparator
	class LessRubble {
	public:
		bool operator()(PQ left, PQ right) const {
			if (left.rubble > right.rubble) {
				return true;
			}
			else if (left.rubble < right.rubble) {
				return false;
			}
			else {	// left.rubble == right.rubble
				if (left.col > right.col) {
					return true;
				}
				else if (left.col < right.col) {
					return false;
				}
				else {	// left.col == right.col
					if (left.row > right.row) {
						return true;
					}
					else if (left.row < right.row) {
						return false;
					}
					else
						return false;
				}
			}
		}
	};

	priority_queue<PQ, vector<PQ>, LessRubble> q;
	priority_queue<PQ, vector<PQ>, LessRubble> TNTq;
	priority_queue<double, vector<double>, greater<double>> up;
	priority_queue<double, vector<double>, less<double>> low;

	int statsN = 0;
	double median = -1;
	int rubble_cleared = 0;
	int numCleared = 0;
	vector<PQ> cleared_tiles;

	int floor_size;
	int start_row, start_col;
	bool statsM = false, medianM = false, verboseM = false;
public:
	void getOptions(int argc, char* argv[]) {

		int gotopt = 0;
		int option_index = 0;
		opterr = false;
		struct option long_opts[] = {
			{"stats", required_argument, nullptr, 's'},
			{"median", no_argument, nullptr, 'm'},
			{"verbose", no_argument, nullptr, 'v'},
			{"help", no_argument, nullptr, 'h'},
			{nullptr, 0, nullptr, '\0'}
		};

		while ((gotopt = getopt_long(argc, argv, "s:mvh", long_opts, &option_index)) != -1) {
			switch (gotopt) {
			case 's':
				if (optarg[0] == '\0') {	// No num specified
					cerr << "error : Stats flag requires an argument N.\n"
						<< "[--stats | -s] <N>\n";
					exit(1);
				}
				else {
					statsM = true;
					statsN = atoi(optarg);
				}
				break;

			case 'm':
				medianM = true;
				up.push(std::numeric_limits<double>::max());
				low.push(std::numeric_limits<double>::min());
				break;

			case 'v':
				verboseM = true;
				break;

			case 'h':
				cout << "This program reads a 'MINEFILE' from a TXT file, \n"
					<< "which should specify the type of map input (map (​M​) and pseudo-random (​R​)), \n"
					<< "the size of the map (N x N), and the starting coordinate.\n"
					<< "In R mode, the seed number, the maximum rubble value a tile can have, \n"
					<< "and the chance of TNT should also be specified.\n"
					<< "This program outputs the summary of the miner's path of escape.\n"
					<< "Usage: \'./MineEscape\n"
					<< "\t[--stats | -s] <N>\n"
					<< "\t[--median | -m]\n"
					<< "\t[--verbose | -v]\n"
					<< "\t[--help | -h]\n"
					<< "\t< infile.txt (> outfile.txt) \'\n";
				exit(0);
			}
		} // while
	};

	void readMap() {
		char mode;
		cin >> mode;
		if (mode != 'M' && mode != 'R') {
			cerr << "Invalid input mode\n";
			exit(1);
		}
		string junk;
		cin >> junk;		// "Size:"
		cin >> floor_size;
		map.resize(floor_size, vector<tile>(floor_size));
		cin >> junk;		// "Start:"
		cin >> start_row;
		cin >> start_col;
		if (start_row >= floor_size || start_row < 0) {
			cerr << "Invalid starting row\n";
			exit(1);
		}
		
		if (start_col >= floor_size || start_col < 0) {
			cerr << "Invalid starting column\n";
			exit(1);
		}

		stringstream ss;
		istream& in = (mode == 'M') ? cin : ss;

		if (mode == 'R') {
			unsigned int seed = 0, max_rubble = 0, tnt_chance = 0;
			cin >> junk;	// "Seed:"
			cin >> seed;
			cin >> junk;	// "Max_Rubble:"
			cin >> max_rubble;
			cin >> junk;	// "TNT:"
			cin >> tnt_chance;
			P2random::PR_init(ss, floor_size, seed, max_rubble, tnt_chance);
		}

		for (int r = 0; r < floor_size; ++r) {
			for (int c = 0; c < floor_size; ++c) {
				in >> map[r][c].rubble;
			}
		}
	}

private:

	void getMedian(const int row, const int col) {
		if (map[row][col].rubble >= up.top()) {
			up.push((double)map[row][col].rubble);
		}
		else
			low.push((double)map[row][col].rubble);

		if (low.size() - up.size() == 2) {
			up.push(low.top());
			low.pop();
		}
		else if (up.size() - low.size() == 2) {
			low.push(up.top());
			up.pop();
		}

		if (up.size() > low.size()) {
			median = up.top();
		}
		else if (up.size() < low.size()) {
			median = low.top();
		}
		else {
			median = ((up.top() + low.top()) / 2.00);
		}

		cout << "Median difficulty of clearing rubble is: " << fixed << setprecision(2)
			<< median << '\n';
	}

	void verboseOUT(const int row, const int col, const bool exploded) {
		if (exploded) {
			cout << "Cleared by TNT: " << map[row][col].rubble
				<< " at [" << row << "," << col << "]\n";
		}
		else {
			if (map[row][col].rubble > 0) {
				cout << "Cleared: " << map[row][col].rubble << " at [" << row << "," << col << "]\n";
			}
			else if (map[row][col].rubble == -1) {
				cout << "TNT explosion at [" << row << "," << col << "]!\n";
			}
		}
	}

	void clearTile(const int r, const int c, const bool exploded) {
		
		if (map[r][c].rubble != 0) {
			if (verboseM)
				verboseOUT(r, c, exploded);

			if (map[r][c].rubble > 0) {
				rubble_cleared += map[r][c].rubble;
				++numCleared;
				if (medianM)
					getMedian(r, c);
			}
			cleared_tiles.push_back({ map[r][c].rubble, r, c });
			map[r][c].rubble = 0;
		}
		
		map[r][c].discovered = true;
	}

	void addTiles(const int r, const int c) {
		// Add surrounding undiscovered tiles to q
		PQ temp;
		if (c > 0) {
			if (!map[r][c - 1].discovered) {
				temp = { map[r][c - 1].rubble, r, c - 1 };	// left
				map[r][c - 1].discovered = true;
				q.push(temp);
			}
		}

		if (r > 0) {
			if (!map[r - 1][c].discovered) {
				temp = { map[r - 1][c].rubble, r - 1, c };	// top
				map[r - 1][c].discovered = true;
				q.push(temp);
			}
		}

		if (r < floor_size - 1) {
			if (!map[r + 1][c].discovered) {
				temp = { map[r + 1][c].rubble, r + 1, c };	// bottom
				map[r + 1][c].discovered = true;
				q.push(temp);
			}
		}

		if (c < floor_size - 1) {
			if (!map[r][c + 1].discovered) {
				temp = { map[r][c + 1].rubble, r, c + 1 };	// right
				map[r][c + 1].discovered = true;
				q.push(temp);
			}
		}
	}

	void emptyTNTQ() {
		while (!TNTq.empty()) {
			if (TNTq.top().rubble != 0) {
				int r = TNTq.top().row, c = TNTq.top().col;

				if (TNTq.top().rubble == -1) {
					TNTq.pop();
					TNTclear(r, c);
				}
				else {
					clearTile(r, c, true);
					TNTq.pop();
				}

				q.push({ 0, r, c });

			}
			else
				TNTq.pop();
		}
	}

	void TNTclear(const int row, const int col) {
		if (map[row][col].rubble != -1) {
			cerr << "error : Tile is not TNT\n";
			exit(1);
		}
		else {
			clearTile(row, col, false);
			map[row][col].TNTcleared = true;

			// Push exploded rubble > 0 to TNTq
			{
				PQ temp;
				if (row > 0) {
					if (!map[row - 1][col].TNTcleared && map[row - 1][col].rubble != 0) {
						map[row - 1][col].TNTcleared = true;
						temp = { map[row - 1][col].rubble, row - 1, col };	// top
						TNTq.push(temp);
					}
				}

				if (row < floor_size - 1) {
					if (!map[row + 1][col].TNTcleared && map[row + 1][col].rubble != 0) {
						map[row + 1][col].TNTcleared = true;
						temp = { map[row + 1][col].rubble, row + 1, col };	// bottom
						TNTq.push(temp);
					}
				}

				if (col > 0) {
					if (!map[row][col - 1].TNTcleared && map[row][col - 1].rubble != 0) {
						map[row][col - 1].TNTcleared = true;
						temp = { map[row][col - 1].rubble, row, col - 1 };	// left
						TNTq.push(temp);
					}
				}

				if (col < floor_size - 1) {
					if (!map[row][col + 1].TNTcleared && map[row][col + 1].rubble != 0) {
						map[row][col + 1].TNTcleared = true;
						temp = { map[row][col + 1].rubble, row, col + 1 };	// right
						TNTq.push(temp);
					}
				}
			}
		}
	}

	void summary() {
		cout << "First tiles cleared:\n";
		for (int i = 0; i < (int)cleared_tiles.size() && i < statsN; ++i) {
			if (cleared_tiles[i].rubble == -1)
				cout << "TNT";
			else
				cout << cleared_tiles[i].rubble;

			cout << " at [" << cleared_tiles[i].row << "," << cleared_tiles[i].col << "]\n";
		}

		cout << "Last tiles cleared:\n";
		for (int i = 0, j = (int)cleared_tiles.size() - 1; j >= 0 && i < statsN; ++i, --j) {
			if (cleared_tiles[j].rubble == -1)
				cout << "TNT";
			else
				cout << cleared_tiles[j].rubble;

			cout << " at [" << cleared_tiles[j].row << "," << cleared_tiles[j].col << "]\n";
		}

		sort(cleared_tiles.rbegin(), cleared_tiles.rend(), LessRubble());
		cout << "Easiest tiles cleared:\n";
		for (int i = 0; i < (int)cleared_tiles.size() && i < statsN; ++i) {
			if (cleared_tiles[i].rubble == -1)
				cout << "TNT";
			else
				cout << cleared_tiles[i].rubble;

			cout << " at [" << cleared_tiles[i].row << "," << cleared_tiles[i].col << "]\n";
		}

		cout << "Hardest tiles cleared:\n";
		for (int i = 0, j = (int)cleared_tiles.size() - 1; j >= 0 && i < statsN; ++i, --j) {
			if (cleared_tiles[j].rubble == -1)
				cout << "TNT";
			else
				cout << cleared_tiles[j].rubble;

			cout << " at [" << cleared_tiles[j].row << "," << cleared_tiles[j].col << "]\n";
		}
	}

public:
	void output() {
		PQ temp;
		// Starting Pos
		map[start_row][start_col].discovered = true;
		map[start_row][start_col].investigated = true;

		// If start is TNT...
		if (map[start_row][start_col].rubble == -1) {
			TNTclear(start_row, start_col);
			emptyTNTQ();
		}

		addTiles(start_row, start_col);

		clearTile(start_row, start_col, false);

		if (start_row == 0 || start_row == floor_size - 1 || start_col == 0 || start_col == floor_size - 1) {
			cout << "Cleared " << numCleared << " tiles containing "
				<< rubble_cleared << " rubble and escaped.\n"; 
			if (statsM)
				summary();
			return;
		}

		for (int row = q.top().row, col = q.top().col; !q.empty(); row = q.top().row, col = q.top().col) {
			if (!map[row][col].investigated) {
				map[row][col].investigated = true;
				map[row][col].discovered = true;

				q.pop();

				if (map[row][col].rubble == -1) {
					TNTclear(row, col);
					emptyTNTQ();
				}

				if (map[row][col].rubble != 0) {
					clearTile(row, col, false);
				}

				addTiles(row, col);

				// Check if @ edge
				if (row == 0 || row == floor_size - 1 || col == 0 || col == floor_size - 1) {
					break;
				}
			}
			else
				q.pop();
		}
		cout << "Cleared " << numCleared << " tiles containing "
			<< rubble_cleared << " rubble and escaped.\n";

		if (statsM)
			summary();
		
	}
};
