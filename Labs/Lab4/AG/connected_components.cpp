/*  Identifier = 15C1680EE94C640EC35E1694295A3625C3254CBA

    EECS 281 Lab 4: Connected Components

    For this lab, you will write a program that calculates the
    number of connected components in an undirected graph.

    Your program will receive an input file in the following format:
    1) The first line of input contains the number of vertices V and number of edges E in the graph.
    2) The next E lines list out the connections between vertices in the graph in the format of "u v",
    which indicates the existence of an edge between u and v.

    For example, the following input file would be read as follows:
    4 2  (the graph has 4 vertices (numbered from 0-3), and two edges exist in the graph)
    0 1  (the first of these edges connects vertex 0 with vertex 1)
    0 2  (the second of these edges connects vertex 0 with vertex 2)

    In this case, vertices {0, 1, 2} form a connected component, and vertex {3} forms a connected component
    (since 3 is not connected to anything). Thus, your program should print out 2.

    You will be using union-find to complete this lab. The following starter code has been provided. Feel
    free to modify this code in your implementation. Good luck!
*/

#include <stdio.h>
#include <vector>
#include <iostream>
using std::vector;
using std::cin;
using std::cout;
using std::endl;

class Graph {
    unsigned int V;
    unsigned int E;
    // TODO: add any additional member variables, as needed
    vector<unsigned int> representatives;
public:
    // Graph constructor that initializes the graph and its member variables
    Graph(unsigned int V, unsigned int E) {
        this->V = V;
        this->E = E;
        // TODO: initialize member variables that you added above
        for (unsigned int i = 0; i < V; ++i) {
            representatives.push_back(i);
        }

        for (unsigned int j = 0; j < E; ++j) {
            unsigned int a, b;
            cin >> a; 
            cin >> b;
            set_union(a, b);
        }
    } // Graph()

      // TODO: implement the find function
    unsigned int find(unsigned int v) {
        unsigned int start = v;
        while (representatives.at(v) != v)
            v = representatives.at(v);

        while (representatives.at(start) != v) {
            unsigned int temp = representatives.at(start);
            representatives.at(start) = v;
            start = temp;
        }
        return v;
    } // find()

      // TODO: implement the union function
    void set_union(unsigned int a, unsigned int b) {
        representatives.at(find(b)) = find(a);
    } // set_union()

      // TODO: implement the count function
    unsigned int count_components() {
        unsigned int result = 0;
        for (unsigned int i = 0; i < representatives.size(); ++i) {
            if (find(i) == i)
                ++result;
        }
        return result;
    } // count_components()

};

int main() {
    //  You do not need to modify main.
    std::ios_base::sync_with_stdio(false);
    unsigned int vertex_count, edge_count = 0;
    cin >> vertex_count;
    cin >> edge_count;
    Graph g(vertex_count, edge_count);
    cout << g.count_components() << endl;
    return 0;
}
