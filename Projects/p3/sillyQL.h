//	Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <iostream>
#include <unordered_map>
#include <getopt.h>
#include <vector>
#include "TableEntry.h"
#include "Table.h"

class SillyQL {
private:
	unordered_map<string, Table> database;
	int numTables = 0;
	bool quiet_mode = false;

public:

	SillyQL() {}

	void STFU() {
		quiet_mode = true;
	}

	void help() {
		cout << "[-h | --help] Prints guide for this program.\n"
			<< "[-q | --quiet] Enables quiet mode.\n\n"
			<< "COMMANDS:\n"
			<< "\tCREATE <tablename> <N> <coltype1> <coltype2> ... <coltypeN> <colname1> <colname2> ... <colnameN>\n"
			<< "==========\n"
			<< "\tINSERT INTO <tablename> <N> ROWS\n"
			<< "\t<value11> <value12> ... <value1M>\n"
			<< "\t<value21> <value22> ... <value2M>\n"
			<< "\t...\n"
			<< "\t<valueN1> <valueN2> ... < valueNM>\n"
			<< "==========\n"
			<< "\tDELETE FROM <tablename> WHERE <colname> <OP> <value>\n"
			<< "==========\n"
			<< "\tGENERATE FOR <tablename> <indextype> INDEX ON <colname>\n"
			<< "==========\n"
			<< "\tPRINT FROM <tablename> <N> <print_colname1> <print_colname2> ... <print_colnameN> [WHERE <colname> <OP> <value> | ALL]\n"
			<< "==========\n"
			<< "\tJOIN <tablename1> AND <tablename2> WHERE <colname1> = <colname2> AND PRINT <N> <print_colname1> <1|2> <print_colname2> <1|2> ... <print_colnameN> <1|2>\n";
		exit(0);
	}

	void createTable() {
		string name;
		cin >> name;

		if (database.empty() || database.find(name) == database.end()) {	// Find duplicate
			database.reserve(numTables + 1);
			vector<EntryType> entries;
			int N;
			cin >> N;					// Num columns

			string col_type;
			entries.reserve(N);
			for (int i = 0; i < N; ++i) {
				cin >> col_type;				// Col i's type
				
				if (col_type == "string")
					entries.emplace_back(EntryType::String);
				else if (col_type == "double")
					entries.emplace_back(EntryType::Double);
				else if (col_type == "int")
					entries.emplace_back(EntryType::Int);
				else if (col_type == "bool")
					entries.emplace_back(EntryType::Bool);
			}

			string col_name;
			vector<string> col_names;
			col_names.reserve(N);
			for (int i = 0; i < N; ++i) {
				cin >> col_name;				// Col i's name
				col_names.emplace_back(col_name);
			}
			
			database.emplace(name, Table(name, col_names, entries));
			++numTables;

			cout << "New table " << name << " with column(s) ";
			for (int i = 0; i < N; ++i) {
				cout << col_names[i] << " ";
			}
			cout << "created\n";

		} // if
		else {
			cout << "Error: Cannot create already existing table " << name << '\n';
		}
				
	} // createTable()

	void insert_into_table() {
		string junk, name;
		int N;
		cin >> junk >> name >> N >> junk;	// INTO <tablename> <N> ROWS

		if (database.find(name) != database.end()) {	// Find table
			size_t startN, endN;
				
			startN = database[name].getNumRows();	// startN = num_rows

			database[name].reserve(N);			// Num_rows in table = num_rows + N
			for (int i = 0; i < N; ++i) {
				database[name].add();			// reads line then adds values to table
			}

			endN = startN + (size_t)N - 1;		// endN

			cout << "Added " << N << " rows to " << name << " from position " << startN << " to " << endN << '\n';
		}
		else {
			cout << "Error: " << name << " does not name a table in the database\n";
			for (int i = 0; i < N; ++i) {	// Clear lines
				getline(cin, junk);
			}
		}
		
	}

	void removeTable() {
		string name;
		cin >> name;

		if (database.find(name) != database.end()) {	// If table exists...
			database.erase(name);

			cout << "Table " << name << " deleted\n";
		}
		else {
			cout << "Error: " << name << " does not name a table in the database\n";
		}
	}

	void delete_from_table() {
		string junk;
		cin >> junk;

		if (junk == "FROM") {
			string name;
			cin >> name;
			if (database.find(name) != database.end()) {	// Find table
				string col_name;
				
				cin >> junk >> col_name;		// WHERE <colname>

				if (database[name].findColName(col_name)) {
					char OP;
					cin >> OP;			// <OP>

					int numDeleted = database[name].delete_where(col_name, OP);
					cout << "Deleted " << numDeleted << " rows from " << name << "\n";
				}
				else {
					cout << "Error: " << col_name << " does not name a column in " << name << "\n";
				}

			}
			else {
				cout << "Error: " << name << " does not name a table in the database\n";
			}
		}
		else {
			cout << "Error: unrecognized command\n";
		}
	}

	void joinTables() {
		string table1, table2;
		cin >> table1 >> table2 >> table2;	// <tablename1> AND <tablename2>

		if (database.find(table1) != database.end()) {
			if (database.find(table2) != database.end()) {
				string junk, col1, col2;
				cin >> junk >> col1 >> junk >> col2;		// WHERE <colname1> = <colname2>
				int nPrinted = 0;
				if (database[table1].findColName(col1)) {
					if (database[table2].findColName(col2)) {
						if (!quiet_mode) {
							int N;
							cin >> junk >> junk >> N;		// AND PRINT <N>

							vector<pair<string, int>> printCOLs;
							printCOLs.reserve(N);
							string col_name;
							int fromTable;
							for (int i = 0; i < N; ++i) {
								cin >> col_name >> fromTable;	// <print_colnameN> <1/2>

								if (!database[table1].findColName(col_name) && fromTable == 1) {
									cout << "Error: " << col_name << " does not name a column in " << table1 << "\n";
									return;
								}
								else if (!database[table2].findColName(col_name) && fromTable == 2) {
									cout << "Error: " << col_name << " does not name a column in " << table2 << "\n";
									return;
								}

								printCOLs.emplace_back(make_pair(col_name, fromTable));
							}

							nPrinted = printJoined(database[table1], database[table2], printCOLs, col1, col2);
						}
						else
							nPrinted = joinQuiet(database[table1], database[table2], col1, col2);						
						
						cout << "Printed " << nPrinted << " rows from joining " << table1 << " to " << table2 << "\n";
					}
					else {
						cout << "Error: " << col2 << " does not name a column in " << table2 << "\n";
					}
				}
				else {
					cout << "Error: " << col1 << " does not name a column in " << table1 << "\n";
				}
			}
			else
				cout << "Error: " << table2 << " does not name a table in the database\n";
		}
		else {
			cout << "Error: " << table1 << " does not name a table in the database\n";
		}

		return;
	}

	int printJoined(Table& table1, Table& table2, const vector<pair<string, int>>& printCOLs, string& col1, string& col2) {
		int num_printed = 0;

		
		// Print col names
		for (size_t p = 0; p < printCOLs.size(); ++p) {		// Print each col
			cout << printCOLs[p].first << " ";
		}
		cout << "\n";
		
		
		for (size_t j = 0; j < table1.size(); ++j) {			// Table1's rows
			for (size_t i = 0; i < table2.size(); ++i) {				// Table2's rows
				if (table1.getValue(j, col1) == table2.getValue(i, col2)) {		// If the same...
					for (size_t p = 0; p < printCOLs.size(); ++p) {				// Print each col
						if (printCOLs[p].second == 1) {							// If col is in table1...
							cout << table1.getValue(j, printCOLs[p].first) << " ";
						}
						else {		// If col is in table2...
							cout << table2.getValue(i, printCOLs[p].first) << " ";
						}
					}	// row printed
					++num_printed;
					cout << "\n";
				}
			}
		}

		return num_printed;
	}

	int joinQuiet(Table& table1, Table& table2, string& col1, string& col2) {
		int c1, c2;
		c1 = table1.findCol(col1);
		c2 = table2.findCol(col2);

		int num_match = 0;
		map<TableEntry, vector<size_t>> data2;
		for (size_t j = 0; j < table2.size(); ++j) {
			data2[table2.getValue(j, c2)].emplace_back(j);	// Fill w/ table2 data
		}

		for (size_t i = 0; i < table1.size(); ++i) {
			auto it = data2.find(table1.getValue(i, c1));		// Compare w/ table1 data
			if (it != data2.end()) {
				num_match += (int)it->second.size();
			}
		}

		return num_match;
	}

	void print() {
		string word;
		string name;
		cin >> word >> name;	// FROM <tablename>

		if (database.find(name) != database.end()) {
			int N;
			cin >> N;	// <N>
			string col_name;
			vector<string> col_names;
			col_names.reserve(N);
			for (int i = 0; i < N; ++i) {
				cin >> col_name;
				if (database[name].findColName(col_name))
					col_names.emplace_back(col_name);
				else {
					cout << "Error: " << col_name << " does not name a column in " << name << "\n";
					return;
				}

			}

			cin >> word;
			if (word == "WHERE") {
				database[name].print_where(col_names, quiet_mode);
			}
			else {
				database[name].print_all(col_names, quiet_mode);
			}
		}
		else {
			cout << "Error: " << name << " does not name a table in the database\n";
		}
		return;
	}

	void genIndex() {
		string junk;
		cin >> junk;	// FOR

		string name;
		cin >> name;
		if (database.find(name) != database.end()) {	// Find table
			string indx_type;
			string col_name;
				
			cin >> indx_type >> junk >> junk >> col_name;		// <indextype> INDEX ON <colname>

			if (database[name].findColName(col_name)) {	// Find col name
				if (indx_type == "hash") {
					database[name].genHash(col_name);
				}
				else if (indx_type == "bst") {
					database[name].genBST(col_name);
				}
				else {
					cout << "Error: " << indx_type << " is not a valid index type\n";
					return;
				}

				cout << "Created " << indx_type << " index for table " << name << " on column " << col_name << "\n";
			}
			else {
				cout << "Error: " << col_name << " does not name a column in " << name << "\n";
			}
		}
		else {
			cout << "Error: " << name << " does not name a table in the database\n";
		}
	}
};