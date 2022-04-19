//	Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "TableEntry.h"

using namespace std;

class Table {
private:
	class Compare {
	private:
		TableEntry value;
		int OP = -1;
		int col = -1;
	public:
		Compare(TableEntry val, int OP_in, int col_in) : value(val), OP(OP_in), col(col_in) {}

		bool operator()(vector<TableEntry>& entry) const {
			if (OP == 0) {
				return (entry[col] < value);
			}
			else if (OP == 1) {
				return (entry[col] > value);
			}
			else {
				return (value == entry[col]);
			}
		}
	};

	struct TableEntryLess {
		bool operator() (const TableEntry& lhs, const TableEntry& rhs) const
		{
			return (lhs < rhs);
		}
	};

	map<TableEntry, vector<size_t>, TableEntryLess> bst;
	unordered_map<TableEntry, vector<size_t>> hash;
	vector<vector<TableEntry>> values;
	vector<string> col_names;
	vector<EntryType> col_types;
	string tablename;
	string indexON;
	size_t num_rows = 0;
	size_t num_col = 0;
	int indexCOL = -1;

public:
	Table() {}

	Table(string name, vector<string> names, vector<EntryType> types) {
		tablename = name;
		col_names = names;
		col_types = types;
		num_rows = 0;
		num_col = col_names.size();
	}

	void add() {
		vector<TableEntry> entry;
		entry.reserve(num_col);						// reserve num of cols in row
		for (size_t c = 0; c < num_col; ++c) {			// Read num_col times
			switch (col_types[c]) {
			case EntryType::String:
			{
				string str;
				cin >> str;
				entry.emplace_back(str);
				break;
			}
			case EntryType::Double:
			{
				double dub;
				cin >> dub;
				entry.emplace_back(dub);
				break;
			}
			case EntryType::Int:
			{
				int num;
				cin >> num;
				entry.emplace_back(num);
				break;
			}
			default:
			{
				string flag;
				cin >> flag;
				entry.emplace_back(flag == "true");
				break;
			}
			}
		}

		values.emplace_back(entry);
		// Add to BST or HASH
		if (!hash.empty()) {
			auto it = hash.find(values[num_rows][indexCOL]);
			if (it != hash.end()) {	// If entry already exists..
				it->second.reserve(it->second.size() + 1);
				it->second.emplace_back(num_rows);			// Insert index
			}
			else {	// If new entry...
				hash.reserve(hash.size() + 1);
				vector<size_t> v = { num_rows };					// Create vector of indices
				hash.emplace(values[num_rows][indexCOL], v);	// insert into hash
			}
		}

		if (!bst.empty()) {
			auto it = bst.find(values[num_rows][indexCOL]);
			if (it != bst.end()) {	// If entry already exists..
				it->second.reserve(it->second.size() + 1);
				it->second.emplace_back(num_rows);			// Insert index
			}
			else {	// If new entry...
				vector<size_t> v = { num_rows };					// Create vector of indices
				bst.emplace(values[num_rows][indexCOL], v);	// insert into bst
			}
		}

		++num_rows;
	}

	int delete_where(const string& name, char OP) {
		int nDel = 0;
		auto it = find(col_names.begin(), col_names.end(), name);

		if (it != col_names.end()) {
			int col = (int)(it - col_names.begin());			// index of col we are comparing
			auto end_it = values.end();
			auto it_remove = values.end();
			switch (col_types[col]) {
			case EntryType::String:
			{
				string str;
				cin >> str;
				it_remove = remove_if(values.begin(), end_it, Compare(TableEntry(str), getOP(OP), col));
				break;
			}
			case EntryType::Double:
			{
				double dub;
				cin >> dub;
				it_remove = remove_if(values.begin(), end_it, Compare(TableEntry(dub), getOP(OP), col));
				break;
			}
			case EntryType::Int:
			{
				int num;
				cin >> num;
				it_remove = remove_if(values.begin(), end_it, Compare(TableEntry(num), getOP(OP), col));
				break;
			}
			default:
			{
				string flag;
				cin >> flag;
				it_remove = remove_if(values.begin(), end_it, Compare(TableEntry(flag == "true"), getOP(OP), col));
				break;
			}
			}

			nDel = (int)(values.size() - (it_remove - values.begin()));
		}

		if (nDel > 0) {
			// Pop back each col nDel times
			for (int i = 0; i < nDel; ++i) {
				values.pop_back();
			}

			num_rows -= nDel;

			if (!hash.empty()) {
				genHash(indexON);
			}

			if (!bst.empty()) {
				genBST(indexON);
			}
		}

		return nDel;
	}

	TableEntry getEntry(int col) const {
		switch (col_types[col]) {
		case EntryType::String:
		{
			string str;
			cin >> str;
			return TableEntry(str);
		}
		case EntryType::Double:
		{
			double dub;
			cin >> dub;
			return TableEntry(dub);
		}
		case EntryType::Int:
		{
			int num;
			cin >> num;
			return TableEntry(num);
		}
		default:
		{
			string flag;
			cin >> flag;
			return TableEntry(flag == "true");
		}
		}
	}

	void print_where(const vector<string>& columns, bool quiet) {
		string col_name;
		char OP;
		cin >> col_name >> OP;
		if (find(col_names.begin(), col_names.end(), col_name) == col_names.end()) {
			cout << "Error: " << col_name << " does not name a column in " << tablename << "\n";
			return;
		}
		auto it = find(col_names.begin(), col_names.end(), col_name);
		int col = (int)(it - col_names.begin());
		{
			//switch (getColType(col_name)) {
			//case EntryType::String: 
			//{
			//	string str;
			//	cin >> str;
			//	if (indexON == col_name) {	// If col_name was indexed...
			//		if (!hash.empty() && OP == '=') {
			//			print_Hashed(TableEntry(str), columns, quiet);
			//		}
			//		else if (!bst.empty() && (OP == '<' || OP == '>')) {
			//			print_BST(TableEntry(str), columns, getOP(OP), quiet);
			//		}
			//		else {
			//			Compare* pred = new Compare(TableEntry(str), getOP(OP), col);
			//			print_where_helper(columns, pred, quiet);
			//			delete pred;
			//		}
			//	}
			//	else {	// Hash/BST in use...
			//		Compare* pred = new Compare(TableEntry(str), getOP(OP), col);
			//		print_where_helper(columns, pred, quiet);
			//		delete pred;
			//	}
			//	break;
			//}
			//case EntryType::Double:
			//{
			//	double dub;
			//	cin >> dub;
			//	if (indexON == col_name) {	// If col_name was indexed...
			//		if (!hash.empty() && OP == '=') {
			//			print_Hashed(TableEntry(dub), columns, quiet);
			//		}
			//		else if (!bst.empty() && (OP == '<' || OP == '>')) {
			//			print_BST(TableEntry(dub), columns, getOP(OP), quiet);
			//		}
			//		else {
			//			Compare* pred = new Compare(TableEntry(dub), getOP(OP), col);
			//			print_where_helper(columns, pred, quiet);
			//			delete pred;
			//		}
			//	}
			//	else {
			//		Compare* pred = new Compare(TableEntry(dub), getOP(OP), col);
			//		print_where_helper(columns, pred, quiet);
			//		delete pred;
			//	}
			//	break;
			//}
			//case EntryType::Int:
			//{
			//	int num;
			//	cin >> num;

			//	if (indexON == col_name) {	// If col_name was indexed...
			//		if (!hash.empty() && OP == '=') {
			//			print_Hashed(TableEntry(num), columns, quiet);
			//		}
			//		else if (!bst.empty() && (OP == '<' || OP == '>')) {
			//			print_BST(TableEntry(num), columns, getOP(OP), quiet);
			//		}
			//		else {
			//			Compare* pred = new Compare(TableEntry(num), getOP(OP), col);
			//			print_where_helper(columns, pred, quiet);
			//			delete pred;
			//		}
			//	}
			//	else {
			//		Compare* pred = new Compare(TableEntry(num), getOP(OP), col);
			//		print_where_helper(columns, pred, quiet);
			//		delete pred;
			//	}
			//	break;
			//}
			//case EntryType::Bool:
			//{
			//	string flag;
			//	cin >> flag;

			//	if (indexON == col_name) {	// If col_name was indexed...
			//		if (!hash.empty() && OP == '=') {
			//			print_Hashed(TableEntry(flag == "true"), columns, quiet);
			//			
			//		}
			//		else if (!bst.empty() && (OP == '<' || OP == '>')) {
			//			print_BST(TableEntry(flag == "true"), columns, getOP(OP), quiet);
			//		}
			//		else {
			//			Compare* pred = new Compare(TableEntry(flag == "true"), getOP(OP), col);
			//			print_where_helper(columns, pred, quiet);
			//			delete pred;
			//		}
			//	}
			//	else {
			//		Compare* pred = new Compare(TableEntry(flag == "true"), getOP(OP), col);
			//		print_where_helper(columns, pred, quiet);
			//		delete pred;
			//	}
			//	break;
			//}
			//default: 
			//	break;
			//}
		}
		if (indexON == col_name) {	// If col_name was indexed...
			if (!hash.empty() && OP == '=') {
				print_Hashed(getEntry(col), columns, quiet);
			}
			else if (!bst.empty() && (OP == '<' || OP == '>')) {
				print_BST(getEntry(col), columns, getOP(OP), quiet);
			}
			else {
				Compare* pred = new Compare(getEntry(col), getOP(OP), col);
				print_where_helper(columns, pred, quiet);
				delete pred;
			}
		}
		else {	// Hash/BST in use...
			Compare* pred = new Compare(getEntry(col), getOP(OP), col);
			print_where_helper(columns, pred, quiet);
			delete pred;
		}
	}

	void print_Hashed(const TableEntry& entry, const vector<string>& columns, bool quiet) const {
		int num_printed = 0;
		if (!quiet) {
			for (size_t i = 0; i < columns.size(); ++i) {	// print col names
				cout << columns[i] << " ";
			}
			cout << "\n";

			if (hash.find(entry) != hash.end()) {
				for (size_t i = 0; i < hash.at(entry).size(); ++i) {
					for (size_t j = 0; j < columns.size(); ++j) {
						int c = (int)(find(col_names.begin(), col_names.end(), columns[j]) - col_names.begin());
						if (col_types[c] == EntryType::Bool) {
							cout << boolalpha;
						}

						cout << values[hash.at(entry)[i]][c] << " ";
					}
					++num_printed;
					cout << "\n";
				}
			}
		}
		else {	// quiet mode
			if (hash.find(entry) != hash.end()) {
				num_printed += (int)hash.at(entry).size();
			}
		}

		cout << "Printed " << num_printed << " matching rows from " << tablename << "\n";
	}

	void print_BST(const TableEntry& entry, const vector<string>& columns, int OP, bool quiet) const {
		int num_printed = 0;
		if (!quiet) {
			for (size_t i = 0; i < columns.size(); ++i) {	// print col names
				cout << columns[i] << " ";
			}
			cout << "\n";
		}

		auto it = bst.find(entry);
		auto end = bst.end();
		if (OP == 0) {
			it = bst.begin();					// <	start at least element
			end = bst.lower_bound(entry);
		}
		else {
			it = bst.upper_bound(entry);		// >	start at one greater
		}

		if (!quiet) {
			while (it != end) {	// print all entries < or > value
				for (size_t i = 0; i < it->second.size(); ++i) {	// print all indices
					for (size_t j = 0; j < columns.size(); ++j) {
						int c = (int)(find(col_names.begin(), col_names.end(), columns[j]) - col_names.begin());

						if (col_types[c] == EntryType::Bool) {
							cout << boolalpha;
						}

						cout << values[it->second[i]][c] << " ";

					}
					++num_printed;
					cout << "\n";
				}

				it++;
			}
		}
		else {	// Quiet mode
			while (it != end) {
				num_printed += (int)it->second.size();
				it++;
			}
		}

		cout << "Printed " << num_printed << " matching rows from " << tablename << "\n";
	}

	void print_where_helper(const vector<string>& columns, Compare* pred, bool quiet) {
		int num_printed = 0;
		if (!quiet) {
			for (size_t i = 0; i < columns.size(); ++i) {	// print col names
				cout << columns[i] << " ";
			}
			cout << "\n";

			auto it_print = find_if(values.begin(), values.end(), (*pred));	// Find first row to print
			while (it_print != values.end()) {
				int r = (int)(it_print - values.begin());			// row offset
				for (size_t i = 0; i < columns.size(); ++i) {
					int c = (int)(find(col_names.begin(), col_names.end(), columns[i]) - col_names.begin());

					cout << values[r][c] << " ";
				}
				++num_printed;
				cout << "\n";

				it_print = find_if(it_print + 1, values.end(), (*pred));	// Find next row to print
			}
		}
		else {	// Quiet mode
			num_printed = (int)count_if(values.begin(), values.end(), (*pred));
		}

		cout << "Printed " << num_printed << " matching rows from " << tablename << "\n";
	}

	void print_all(const vector<string>& columns, bool quiet) {
		if (!quiet) {
			for (size_t i = 0; i < columns.size(); ++i) {	// print col names
				cout << columns[i] << " ";
			}
			cout << "\n";


			for (size_t r = 0; r < values.size(); ++r) {		// Print each row
				for (size_t i = 0; i < columns.size(); ++i) {
					int c = (int)(find(col_names.begin(), col_names.end(), columns[i]) - col_names.begin());
					cout << values[r][c] << " ";
				}
				cout << "\n";
			}
		}

		cout << "Printed " << num_rows << " matching rows from " << tablename << "\n";
	}

	void genHash(const string& name) {
		// Clear any user-generated index
		if (!hash.empty()) {
			hash.clear();
		}
		if (!bst.empty()) {
			bst.clear();
		}

		vector<size_t> indices;
		indexON = name;
		indexCOL = (int)(find(col_names.begin(), col_names.end(), name) - col_names.begin());

		for (size_t r = 0; r < values.size(); ++r) {		// Hash each row at col c
			if (hash.find(values[r][indexCOL]) == hash.end()) {	// If key doesnt already exist
				hash.reserve(hash.size() + 1);
				indices.reserve(1);
				hash.emplace(values[r][indexCOL], indices);
			}

			hash[values[r][indexCOL]].reserve(hash[values[r][indexCOL]].size() + 1);
			hash[values[r][indexCOL]].emplace_back(r);
		}
	}

	void genBST(const string& name) {
		// Clear any user-generated index
		if (!hash.empty()) {
			hash.clear();
		}
		if (!bst.empty()) {
			bst.clear();
		}
		indexON = name;
		vector<size_t> indices;

		indexCOL = (int)(find(col_names.begin(), col_names.end(), name) - col_names.begin());
		for (size_t r = 0; r < values.size(); ++r) {	// Insert value at col c for all rows
			if (bst.find(values[r][indexCOL]) == bst.end()) {	// If key hasnt already been mapped
				indices.reserve(1);
				bst.emplace(values[r][indexCOL], indices);
			}

			bst[values[r][indexCOL]].reserve(bst[values[r][indexCOL]].size() + 1);
			bst[values[r][indexCOL]].emplace_back(r);
		}
	}

	size_t size() const {
		return values.size();
	}

	void reserve(int N) {
		values.reserve(values.size() + N);
		//for (size_t i = values.size(); i < values.size() + N; ++i) {	// Reserve num_col of each new row
		//	values[i].reserve(num_col);
		//}
	}

	int getOP(char OP) const {
		if (OP == '<')
			return 0;
		else if (OP == '>')
			return 1;
		else
			return 2;
	}

	size_t getNumRows() const {
		return values.size();
	}

	bool findColName(const string& name) const {
		if (find(col_names.begin(), col_names.end(), name) != col_names.end()) {
			return true;
		}
		else
			return false;
	}

	int findCol(const string& name) const {
		return (int)(find(col_names.begin(), col_names.end(), name) - col_names.begin());
	}

	EntryType getColType(const string& name) const {
		int i = (int)(find(col_names.begin(), col_names.end(), name) - col_names.begin());
		return col_types[i];
	}

	TableEntry getValue(size_t row, size_t col) {
		return values[row][col];
	}

	TableEntry getValue(size_t row, const string& col) {
		return values[row][(int)(find(col_names.begin(), col_names.end(), col) - col_names.begin())];
	}
};
