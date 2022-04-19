// Testing file provided for EECS 281 Lab 7

#include "hashtable.h"
#include <string>
#include <iostream>

int main() {

    // VERY BASIC EXAMPLE (will likely not compile with default/empty hashtable.h file)
    HashTable<std::string, int> midterm;
    std::cout << midterm.insert("sam", 50) << " ";
    std::cout << midterm.insert("fee", 100) << " ";
    std::cout << midterm.insert("milo", 95) << " ";
    std::cout << midterm.insert("gabe", 88) << " \n";
    std::cout << midterm["sam"] << " ";
    std::cout << midterm["fee"] << " ";
    std::cout << midterm["milo"] << " ";
    std::cout << midterm["gabe"] << " \n";
    std::cout << midterm.erase("sam") << " ";
    std::cout << midterm["sam"] << "\n";
    // ADD MORE TESTS OF YOUR OWN
    std::cout << midterm.erase("fee") << " ";
    std::cout << midterm["fee"] << "\n";
    std::cout << midterm.erase("milo") << " ";
    std::cout << midterm["milo"] << "\n";
    
    std::cout << midterm.insert("gary", 24) << " ";
    std::cout << midterm["gary"] << " \n";
    std::cout << midterm.insert("gary", 25) << " ";
    std::cout << midterm["gary"] << " \n";

    HashTable<int, int> table;

    /*std::cout << "\ninserting again\n";
    for (int i = 0; i < 20; ++i) {
        std::cout << "t[" << i << "] " << table.insert(i, i) << "\n";
    }*/

    std::cout << "insert t[0] " << table.insert(0, 111) << "\n";
    std::cout << "insert t[4] " << table.insert(4, 222) << "\n";
    std::cout << "insert t[8] " << table.insert(8, 333) << "\n";
    std::cout << "insert t[12] " << table.insert(12, 444) << "\n";

    std::cout << "\ntable\n";
    for (int i = 0; i < 13; i += 4) {
        std::cout << "t[" << i << "] = " << table[i] << "\n";
    }

    std::cout << "\nerase t[0] " << table.erase(0) << "\n";
    std::cout << "erase t[12] " << table.erase(12) << "\n";
    std::cout << "insert t[12] " << table.insert(12, 111) << "\n";
    std::cout << "insert t[0] " << table.insert(0, 444) << "\n";

    //std::cout << "\ninserting\n";
    //for (int i = 0; i < 20; ++i) {
    //    std::cout << "t[" << i << "] " << table.insert(i, i) << "\n";
    //}

    std::cout << "\ntable\n";
    for (int i = 0; i < 20; ++i) {
        std::cout << "t[" << i << "] = " << table[i] << "\n";
    }

    std::cout << "\ntable again\n";
    for (int i = 0; i < 20; ++i) {
        std::cout << "t[" << i << "] = " << table[i] << "\n";
    }

    std::cout << "\ninserting some\n";
    table.insert(20, 123);
    table[15] = 404;
    table[10] = 43110;
    table[5] = 8008135;

    std::cout << "\ntable\n";
    for (int i = 0; i < 21; ++i) {
        std::cout << "t[" << i << "] = " << table[i] << "\n";
    }

    std::cout << "\ntable again\n";
    for (int i = 0; i < 21; ++i) {
        std::cout << "t[" << i << "] = " << table[i] << "\n";
    }

    std::cout << "\nerasing some\n";
    std::cout << "t[11] " << table.erase(11) << "\n";
    std::cout << "t[12] " << table.erase(12) << "\n";
    std::cout << "t[13] " << table.erase(13) << "\n";
    std::cout << "t[14] " << table.erase(14) << "\n";

    std::cout << "\nerasing some again\n";
    std::cout << "t[11] " << table.erase(11) << "\n";
    std::cout << "t[12] " << table.erase(12) << "\n";
    std::cout << "t[13] " << table.erase(13) << "\n";
    std::cout << "t[14] " << table.erase(14) << "\n";

    std::cout << "\ninserting some back\n";
    table[11] = 9999;
    table[12] = 8888;
    table[13] = 7777;
    table[14] = 6666;

    std::cout << "\ntable\n";
    for (size_t i = 0; i < table.size(); ++i) {
        std::cout << "t[" << i << "] = " << table[i] << "\n";
    }

    std::cout << "\nerasing all\n";
    for (int i = 0; i < 21; ++i) {
        std::cout << "t[" << i << "] " << table.erase(i) << "\n";
    }

    std::cout << "\ntable\n";
    for (size_t i = 0; i < 40; ++i) {
        table[i] = 404;
        std::cout << "t[" << i << "] = " << table[i] << "\n";
    }

    std::cout << "\nerasing half\n";
    for (int i = 0; i < 40; ++i) {
        if (i % 2)
            std::cout << "t[" << i << "] " << table.erase(i) << "\n";
    }

    std::cout << "\ninserting some\n";
    for (int i = 40; i < 60; ++i) {
        std::cout << "t[" << i << "] " << table.insert(i, 6969) << "\n";
    }

    std::cout << "\ninserting one more\n";
    std::cout << "t[1] = " << table[1] << "\n";

    std::cout << "\nerasing half again\n";
    for (int i = 0; i < 40; ++i) {
        if (i % 2)
            std::cout << "t[" << i << "] " << table.erase(i) << "\n";
    }

    std::cout << "\ntable\n";
    for (size_t i = 0; i < table.size(); ++i) {
        std::cout << "t[" << i << "] = " << table[i] << "\n";
    }

    HashTable<int, int> table2;
    
    // All deleted
    {
        for (int i = 0; i < 40; i += 4) {
            table2.insert(i, 12345);
        }

        for (int i = 0; i < 40; i += 4) {
            table2.erase(i);
        }

        for (int i = 3; i < 30; i += 3) {
            table2.insert(i, 12345);
        }

        for (int i = 3; i < 30; i += 3) {
            table2.erase(i);
        }

        for (int i = 5; i < 50; i += 5) {
            table2.insert(i, 12345);
        }

        for (int i = 5; i < 50; i += 5) {
            table2.erase(i);
        }

        for (int i = 23; i < 230; i += 23) {
            table2.insert(i, 12345);
        }

        for (int i = 23; i < 230; i += 23) {
            table2.erase(i);
        }

        for (int i = -10; i < 0; ++i) {
            table2.insert(i, 12345);
        }

        for (int i = -10; i < 0; ++i) {
            table2.erase(i);
        }

        for (int i = -20; i < -10; ++i) {
            table2.insert(i, 12345);
        }

        for (int i = -20; i < -10; ++i) {
            table2.erase(i);
        }
    }

    std::cout << "Insert t2[-18] = 121212 " << table2.insert(-18, 121212) << "\n";
    std::cout << "t2[0] = 252525 \n";
    table2[0] = 252525;
    std::cout << "Erasing t2[0] " << table2.erase(0) << "\n";
    std::cout << "Erasing t2[0] again " << table2.erase(0) << "\n";
    std::cout << "Insert t2[0] = 252525 " << table2.insert(0, 404040) << "\n";

    return 0;
}
