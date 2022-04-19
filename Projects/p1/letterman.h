// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68768FC9

#include <iostream>
#include <vector>
#include <deque>
#include <getopt.h>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

class Letterman {
private:
	string begin, end;
	size_t begin_ind = 0, end_ind = 0;
	size_t num_words = 0;
	int numDiscovered = 0;
	char outputForm = 'W';
	char route = '\0', dictType = '\0';
	// route = [ s | q ], dictType = [ S | C ]
	bool change = false, swap = false, length = false;

	struct word_struct {
		string word;
		size_t foundbyInd = 0, pos = 0;
		char method = '\0', letter = '\0';
		bool discovered = false;
	};

	vector<word_struct> dictionary;

public:
	//============ Initialization ============
	void getOptions(int argc, char* argv[]) {
		readDict();

		int gotopt = 0;
		int option_index = 0;
		opterr = false;
		struct option long_opts[] = {
			{"stack", no_argument, nullptr, 's'},
			{"queue", no_argument, nullptr, 'q'},
			{"change", no_argument, nullptr, 'c'},
			{"swap", no_argument, nullptr, 'p'},
			{"length", no_argument, nullptr, 'l'},
			{"output", required_argument, nullptr, 'o'},
			{"begin", required_argument, nullptr, 'b'},
			{"end", required_argument, nullptr, 'e'},
			{"help", no_argument, nullptr, 'h'},
			{nullptr, 0, nullptr, '\0'}
		};

		bool route_set_stack = false;
		bool route_set_queue = false;
		bool begin_specified = false;
		bool end_specified = false;

		while ((gotopt = getopt_long(argc, argv, "sqcplo:b:e:h", long_opts, &option_index)) != -1) {
			switch (gotopt) {
			case 's':
				if (!route_set_stack && !route_set_queue) {	// No route set yet
					route = 's';
					route_set_stack = true;
				}
				else if (route_set_queue) {		// Queue route already set
					cerr << "error : Only one routing scheme can be used at a time.\n"
						<< "[--queue | -q] or [--stack | -s]\n";
					exit(1);
				}
				else if (route_set_stack) {		// Stack route already set (stack twice)
					cerr << "error : Routing scheme only needs to be specified once.\n";
					exit(1);
				}
				break;

			case 'q':
				if (!route_set_stack && !route_set_queue) {	// No route set yet
					route = 'q';
					route_set_queue = true;
				}
				else if (route_set_stack) {		// Stack route already set
					cerr << "error : Only one routing scheme can be used at a time.\n"
						<< "[--queue | -q] or [--stack | -s]\n";
					exit(1);
				}
				else if (route_set_queue) {		// Queue route already set (queue twice)
					cerr << "error : Routing scheme only needs to be specified once.\n";
					exit(1);
				}
				break;

			case 'c':
				change = true;
				break;

			case 'p':
				swap = true;
				break;

			case 'l':
				length = true;
				break;

			case 'o':	// Argument required
				if (optarg[1] == '\0') {	// A char was passed as an argument
					if (!strcmp(optarg, "W")) {
						// default 'W'
						break;
					}
					else if (!strcmp(optarg, "M")) {
						outputForm = 'M';
					}
					else {
						cerr << "error : Output file format not specified.\n"
							<< "[--output (W|M) | -o (W|M)]\n";
						exit(1);
					}
				}
				else {		// Something other than a char was passed
					cerr << "error : Output file format must be specified by a char.\n"
						<< "[--output (W|M) | -o (W|M)]\n";
					exit(1);
				}
				break;

			case 'b':	// Argument required
				if (optarg != NULL && optarg[0] == '\0') {	// No word specified
					cerr << "error : Starting word not specified.\n"
						<< "[--begin | -b] <word>\n";
					exit(1);
				}
				else {
					begin = optarg;
					for (size_t i = 0; i < dictionary.size(); ++i) {
						if (dictionary[i].word == begin) {	// If begin word exists in dictionary
							begin_ind = i;
							begin_specified = true;
							break;
						}
					}
				}
				break;

			case 'e':	// Argument required
				if (optarg != NULL && optarg[0] == '\0') {	// No word specified
					cerr << "error : Starting word not specified.\n"
						<< "[--end | -e] <word>\n";
					exit(1);
				}
				else {
					end = optarg;
					for (size_t i = 0; i < dictionary.size(); ++i) {
						if (dictionary[i].word == end) {	// If end word exists in dictionary
							end_ind = i;
							end_specified = true;
							break;
						}
					}
				}
				break;

			case 'h':
				cout << "This program reads a dictionary from a TXT file, \n"
					<< "which specifes the type of dictionary (complex (​C​) and simple (​S​)), \n"
					<< "the number of words in the dictionary, and contains the list of words.\n"
					<< "It outputs the path of words taken from start to finish to change the\n"
					<< "beginning word into the ending word based on the routing scheme.\n"
					<< "Only one routing scheme can be specifed, and\n"
					<< "at least one morph mode option should be specifed.\n"
					<< "Output defaults to word output ('W').\n"
					<< "Usage: \'./letter\n\t[(--stack | -s) | (--queue | -q)]\n"
					<< "\t[--change | -c]\n"
					<< "\t[--swap | -p]\n"
					<< "\t[--length | -l]\n"
					<< "\t[--output (W|M) | -o (W|M)]\n"
					<< "\t[--begin | -b] <word>\n"
					<< "\t[--end | -e] <word>\n"
					<< "\t[--help | -h]\n"
					<< "\t<Dictionary .txt file>\'\n";
				exit(0);
			}
		} // while

		if (!route_set_queue && !route_set_stack) {	// Routing scheme not specified
			cerr << "error : Routing scheme was not specified.\n"
				<< "[--queue | -q] or [--stack | -s]\n";
			exit(1);
		}

		if (!change && !swap && !length) {	// Morph mode option not specified
			cerr << "error : No morph mode option specified. At least one option must be specified.\n"
				<< "\t[--change | -c]\n"
				<< "\t[--swap | -p]\n"
				<< "\t[--length | -l]\n";
			exit(1);
		}

		// The change and/or swap flags are specified, 
		// but length is not, and the start/end words do not match in length
		if ((change || swap) && !length && (begin.size() != end.size())) {
			cerr << "error : The change and/or swap flags are specified, but length is not,"
				<< " and the start/end words do not match in length. (Impossible situation)\n";
			exit(1);
		}

		if ( !(begin_specified && end_specified) ) {	// Begin and/or end words not specified
			cerr << "error : Beginning and ending words must be specified and must exist in given dictionary.\n"
				<< "\t[--begin | -b] <word>\n"
				<< "\t[--end | -e] <word>\n";
			exit(1);
		}
	};

private:
	void readDict() {
		cin >> dictType;	// Simple or Complex
		cin >> num_words;	// Number of words

		string line;			// Ignore comments
		cin >> std::ws;

		while (getline(cin, line)) {	// If line starts with "/" read next line
			if (line[0] != '/' && line[1] != '/') {
				if (dictType == 'S') {
					dictionary.reserve(num_words);
					word_struct temp = { line, 0, 0, '\0', '\0', false };
					dictionary.push_back(temp);
				}
				else
					complexRead(line);
			}
		}

		//dictionary.shrink_to_fit();
	};

	void complexRead(string word) {
		word_struct temp = { word, 0, 0, '\0', '\0', false };
		if (word.back() == '&') {	// Reversal (&)
			word.pop_back();					// remove &
			temp.word = word;
			dictionary.push_back(temp);			// add word
			reverse(word.begin(), word.end());	// reverse word
			temp.word = word;
			dictionary.push_back(temp);			// add reversed word
		}
		else if (word.find('[') != string::npos) {	// Insert-each ([])
			size_t pos = word.find('[');
			int num_letters = 0;

			for (size_t i = pos + 1; i < word.find(']'); ++i) {
				++num_letters;
			}

			// Generate words with each letter and load dictionary
			string inserted = word, ending = word.substr(word.find(']') + 1, string::npos);
			for (int j = 0; j < num_letters; ++j) {
				inserted = word.substr(0, pos);
				inserted += word[pos + 1 + j];
				inserted += ending;
				temp.word = inserted;
				dictionary.push_back(temp);
			}
		}
		else if (word.find('!') != string::npos) {	// Swap (!)
			size_t bang = word.find('!');
			size_t swapA = bang - 2;
			size_t swapB = bang - 1;

			word.erase(bang, 1);		// Remove ! from word
			temp.word = word;
			dictionary.push_back(temp);
			std::swap(word[swapA], word[swapB]);	// Swap letters
			temp.word = word;
			dictionary.push_back(temp);
		}
		else if (word.find('?') != string::npos) {	// Double (?)
			size_t mark = word.find('?');
			string doubleLetter(1, word[mark - 1]);

			word.erase(mark, 1);		// Remove ? from word
			temp.word = word;
			dictionary.push_back(temp);
			word.insert(mark, doubleLetter);		// Insert letter where ? was
			temp.word = word;
			dictionary.push_back(temp);
		}
		else {			// simple word
			temp.word = word;
			dictionary.push_back(temp);
		}
	};

	//============ Morph Modes ============

	bool changeM(const size_t curr_ind, const size_t word_ind) {
		if (dictionary[curr_ind].word.size() == dictionary[word_ind].word.size()) {	// If same lengths
			int letterOff = 1;					// Words must only differ by one letter
			for (size_t i = 0; i < dictionary[curr_ind].word.size(); ++i) {	// Compare two words
				if (dictionary[curr_ind].word[i] != dictionary[word_ind].word[i]) {			// Check if words differ by a char
					--letterOff;					// Keep track of how many chars are different

					dictionary[word_ind].pos = i;
					dictionary[word_ind].letter = dictionary[word_ind].word[i];

					if (letterOff < 0) {			// False if words has more than one letter different
						dictionary[word_ind].pos = 0;
						dictionary[word_ind].letter = '\0';
						return false;
					}
					
				}
			}
			return true;	// Return true if words are off by only one letter
		}
		else				// False if words not same length
			return false;

	};

	bool swapM(const size_t curr_ind, const size_t word_ind) {
		if (dictionary[curr_ind].word.size() == dictionary[word_ind].word.size()) {	// Check if words match in length
			int letterOff = 1;
			for (size_t i = 0; i < dictionary[word_ind].word.size(); ++i) {
				if (dictionary[curr_ind].word[i] != dictionary[word_ind].word[i]) {					// If chars are different
					--letterOff;

					if (letterOff < 0) {			// False if words has more than one letter different
						dictionary[word_ind].pos = 0;
						return false;
					}
					
					if (dictionary[curr_ind].word[i] == dictionary[word_ind].word[i + 1] && 
						dictionary[curr_ind].word[i + 1] == dictionary[word_ind].word[i]) {
						dictionary[word_ind].pos = i;
						++i;
					}
					else
						return false;	// Return false if letters are not swappable
				}
			}
			return true;		// Return true all char are the sam except for swappables
		}
		else
			return false;		// Return false if words do not match in length
	};

	bool insertM(const size_t curr_ind, const size_t word_ind) {
		if (dictionary[curr_ind].word.size() + 1 == dictionary[word_ind].word.size()) {	// Check if words match in length if base added a letter
			int letterOff = 1;
			for (size_t curr = 0, i = 0; curr < dictionary[word_ind].word.size(); ++i, ++curr) {
				if (dictionary[curr_ind].word[curr] != dictionary[word_ind].word[i]) {
					--letterOff;

					if (letterOff < 0) {			// False if words has more than one letter different
						dictionary[word_ind].pos = 0;
						dictionary[word_ind].letter = '\0';
						return false;
					}

					if (dictionary[curr_ind].word[curr] != dictionary[word_ind].word[i + 1])
						return false;		// Return false if char after inserted char are not equal
					else {
						dictionary[word_ind].pos = i;
						dictionary[word_ind].letter = dictionary[word_ind].word[i];
						++i;
					}
				}
			}
			return true;		// Return true if words are only one letter off
			
		}
		else				// Return false if words would not match in length
			return false;
	};

	bool deleteM(const size_t curr_ind, const size_t word_ind) {
		if (dictionary[curr_ind].word.size() - 1 == dictionary[word_ind].word.size()) {	// Check if words match in length if base deler a letter
			int letterOff = 1;
			for (size_t curr = 0, i = 0; i < dictionary[curr_ind].word.size(); ++i, ++curr) {
				if (dictionary[curr_ind].word[curr] != dictionary[word_ind].word[i]) {
					--letterOff;

					if (letterOff < 0) {			// False if words has more than one letter different
						dictionary[word_ind].pos = 0;
						dictionary[word_ind].letter = '\0';
						return false;
					}

					if (dictionary[curr_ind].word[curr + 1] != dictionary[word_ind].word[i])
						return false;		// Return false if char after inserted char are not equal
					else {
						dictionary[word_ind].pos = curr;
						++curr;
					}
				}
			}
			return true;		// Return true if words are only one letter off

		}
		else				// Return false if words would not match in length
			return false;
	};

	//============ Search and Output ============
public:

	bool search() {
		// Initialize deque w/ begin index
		deque<size_t> data = { begin_ind };
		dictionary[begin_ind].discovered = true;	// Mark 'begin' as discovered
		++numDiscovered;

		// [Start loop] Remove top()/front from deque and make it curr_word_ind
		size_t curr_word_ind;
		while (!data.empty()) {

			if (route == 's') {		// Stack route
				curr_word_ind = data.back();
				data.pop_back();		// top = front()
			}
			else {					// Queue route
				curr_word_ind = data.front();
				data.pop_front();		// front = back()
			}

			// Find similar words to curr_word
			for (size_t j = 0; j < dictionary.size(); ++j) {
				if (!dictionary[j].discovered) {		// If word has been undiscovered, perform morphs
					bool similar = false;					// Prevents words from being pushed twice into deque
					if (change) {
						if (changeM(curr_word_ind, j) && !similar) {
							similar = true;
							dictionary[j].method = 'c';
							data.push_back(j);					// push index to deque
							dictionary[j].discovered = true;	// mark it as discovered
							++numDiscovered;
							dictionary[j].foundbyInd = curr_word_ind;	// remember which index found it
						}

						// Check if end was discovered
						if (dictionary[end_ind].discovered)
							return true;
					}
					if (swap) {
						if (swapM(curr_word_ind, j) && !similar) {
							similar = true;
							dictionary[j].method = 's';
							data.push_back(j);					// push index to deque
							dictionary[j].discovered = true;	// mark it as discovered
							++numDiscovered;
							dictionary[j].foundbyInd = curr_word_ind;	// remember which index found it
						}

						// Check if end was discovered
						if (dictionary[end_ind].discovered)
							return true;
					}
					if (length) {
						if (insertM(curr_word_ind, j) && !similar) {
							similar = true;
							dictionary[j].method = 'i';
							data.push_back(j);					// push index to deque
							dictionary[j].discovered = true;	// mark it as discovered
							++numDiscovered;
							dictionary[j].foundbyInd = curr_word_ind;	// remember which index found it
						}

						// Check if end was discovered
						if (dictionary[end_ind].discovered)
							return true;

						if (deleteM(curr_word_ind, j) && !similar) {
							similar = true;
							dictionary[j].method = 'd';
							data.push_back(j);					// push index to deque
							dictionary[j].discovered = true;	// mark it as discovered
							++numDiscovered;
							dictionary[j].foundbyInd = curr_word_ind;	// remember which index found it
						}

						// Check if end was discovered
						if (dictionary[end_ind].discovered)
							return true;
					}
				}
			}// for

		}// while

		return false;
	};

	void output() {

		if (begin == end) {
			cout << "Words in morph: " << 1 << '\n'
				<< end << '\n';
		}
		else if (search()) {		// If solution found
			deque<size_t> solution;
			solution.push_back(end_ind);
			size_t word_ind = dictionary[end_ind].foundbyInd;
			for (size_t i = 0; word_ind != begin_ind; ++i) {
				if (word_ind == i) {
					word_ind = dictionary[i].foundbyInd;
					solution.push_front(i);
					i = 0;
				}
			}

			solution.push_front(begin_ind);

			cout << "Words in morph: " << solution.size() << '\n';
			if (outputForm == 'W') {
				for (size_t i = 0; i < solution.size(); ++i) {
					cout << dictionary[ solution[i] ].word << '\n';
				}
			}
			else {	// outputForm == 'M'
				cout << dictionary[solution[0]].word << '\n';
				for (size_t i = 1; i < solution.size(); ++i) {
					cout << dictionary[solution[i]].method << ",";						// "c/i/d/s, "
					if (dictionary[solution[i]].method == 'c' || dictionary[solution[i]].method == 'i') {	// "<pos>, <letter>\n"
						cout << dictionary[solution[i]].pos << "," << dictionary[solution[i]].letter << '\n';
					}
					else if (dictionary[solution[i]].method == 'd' || dictionary[solution[i]].method == 's') {	// "<pos>\n"
						cout << dictionary[solution[i]].pos << '\n';
					}
				}
			}

		} 
		else {
			cout << "No solution, " << numDiscovered << " words discovered.\n";
		}
	};
};