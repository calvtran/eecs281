// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <limits>
#include <getopt.h>

using namespace std;

const double INFNTY = numeric_limits<double>::infinity();

struct Edge {
	double edge_dist = -1;
	int parent_index = -1;
	bool visited = false;
};

enum class RoomType { outer, lab, decon };

class Room {
public:
	int x;
	int y;
	RoomType room = RoomType::outer;
	Room() : x(0), y(0), room(RoomType::outer) {};
	Room(int x_in, int y_in) : x(x_in), y(y_in) {
		if ((x_in == 0 && y_in <= 0) || (x_in <= 0 && y_in == 0))
			room = RoomType::decon;
		else if (x_in < 0 && y_in < 0)
			room = RoomType::lab;
	};
};

class DistFunct {
public:
	vector<vector<double>> dist;
//public:
	DistFunct(size_t n, vector<Room> &rooms) {
		dist.resize(n);
		for (size_t i = 0; i < n; i++) {
			dist[i].resize(n);
		}

		for (size_t i = 0; i < n; i++) {
			for (size_t j = i; j < n; j++) {
				if (i == j) {
					dist[i][j] = 0;
				}
				else {
					dist[i][j] = sqrt((((double)rooms[i].x - (double)rooms[j].x) * ((double)rooms[i].x - (double)rooms[j].x)) +
						(((double)rooms[i].y - (double)rooms[j].y) * ((double)rooms[i].y - (double)rooms[j].y)));
					dist[j][i] = dist[i][j];
				}
			}
		}
	}

	//double& at(const size_t from, const size_t to) {
	//	return dist[from][to];
	//}
};

class OPT {
private:
	vector<size_t> curr_path;
	vector<int> visited;
	//struct Node {
	//	//Room room;
	//	bool visited = false;
	//};
	//vector<Node> rooms;
	vector<size_t> best_path;
	DistFunct distances;
	size_t num_rooms;
	double best_length = 0;
	double curr_length = 0;

public:

	OPT(size_t n, vector<Room>& rooms_in, vector<size_t>& path, double best) : distances(n, rooms_in), num_rooms(n), best_length(best) {
		curr_path.reserve(n);
		best_path.reserve(n);
		visited.resize(n, 0);
		//rooms.reserve(n);
		//for (size_t i = 0; i < n; i++) {
		//	rooms.push_back({ /*rooms_in[i],*/ false });
		//}
		curr_path = path;
		best_path = path;
		//rooms[0].visited = true;
		visited[0] = 1;
	}

	double getLength() {
		return best_length;
	}

	vector<size_t> getPath() {
		return best_path;
	}

	bool promising(size_t permLength) {
		return ((curr_length + genRemainMST(permLength)) < best_length);
	}

	void genPerms(size_t permLength) {
		if (permLength == curr_path.size()) {
			curr_length += distances.dist[0][curr_path.back()];
			if (curr_length < best_length) {
				best_length = curr_length;
				best_path = curr_path;
			}
			curr_length -= distances.dist[0][curr_path.back()];
			return;
		} // if
		if (curr_path.size() - permLength > 4)
			if (!promising(permLength))
				return;
		for (size_t i = permLength; i < curr_path.size(); ++i) {
			swap(curr_path[permLength], curr_path[i]);
			//rooms[curr_path[permLength]].visited = true;
			visited[curr_path[permLength]] = 1;
			curr_length += distances.dist[curr_path[permLength - 1]][curr_path[permLength]];
			genPerms(permLength + 1);
			curr_length -= distances.dist[curr_path[permLength - 1]][curr_path[permLength]];
			visited[curr_path[permLength]] = 0;
			//rooms[curr_path[permLength]].visited = false;
			swap(curr_path[permLength], curr_path[i]);
		} // for
	} // genPerms()

	double genRemainMST(size_t permLength) {
		double total = 0;
		vector<int> mst_visited(num_rooms, 0);
		vector<double> mst_dist(num_rooms, INFNTY);

		// connect closest unvisited to recent node in curr_path
		double min = INFNTY;
		size_t start = -1;
		for (size_t i = 0; i < num_rooms; ++i) {
			if (!visited[i]) {
				if (distances.dist[curr_path[permLength - 1]][i] < min) {
					min = distances.dist[curr_path[permLength - 1]][i];
					start = i;
				}
			}
		}
		total += min;

		mst_dist[start] = 0;
		//edges[start].edge_dist = 0;
		//edges[start].parent_index = -1;	

		// Build MST from unvisited
		for (size_t i = 0; i < num_rooms; i++) {
			if (!visited[i]) {
				// Loop over all vertices: find closest unvisited false k_v
				size_t v = 0;
				double min = INFNTY;
				for (size_t j = 0; j < num_rooms; ++j) {
					if (!visited[j]) {
						if (!mst_visited[j] && mst_dist[j] < min) {
							min = mst_dist[j];
							v = j;
						}
					}
				}

				// Mark k_v as true
				mst_visited[v] = 1;

				// Loop over all vertices: update false unvisited neighbors of k_v
				for (size_t w = 0; w < num_rooms; ++w) {
					if (!visited[w]) {
						if (!mst_visited[w]) {
							double dub = distances.dist[v][w];
							if (dub < mst_dist[w]) {
								mst_dist[w] = dub;
								//edges[w].edge_dist = dub;
								//edges[w].parent_index = (int)v;
							}
						}
					}
				}
			}
		}

		// total up mst distances of unvisited nodes
		for (size_t i = 0; i < num_rooms; i++) {
			if (!visited[i])
				total += mst_dist[i];
		}

		// connect closest unvisited to 0
		min = INFNTY;
		for (size_t i = 0; i < num_rooms; ++i) {
			if (!visited[i]) {
				if (i != start && distances.dist[0][i] < min) {
					min = distances.dist[0][i];
				}
			}
		}
		total += min;

		return total;
	}
};

class Pathfinder {
private:
	vector<Room> room_vec;
	vector<size_t> path;
	size_t num_rooms;
	enum class ModeType { MST, FASTTSP, OPTTSP };
	ModeType mode = ModeType::MST;
	double weight = 0;
	double length = 0;

public:

	void getOptions(int argc, char* argv[]) {
		int gotopt = 0;
		int option_index = 0;
		opterr = false;
		struct option long_opts[] = {
			{"mode", required_argument, nullptr, 'm'},
			{"help", no_argument, nullptr, 'h'},
			{nullptr, 0, nullptr, '\0'}
		};
		
		while ((gotopt = getopt_long(argc, argv, "m:h", long_opts, &option_index)) != -1) {
			switch (gotopt) {
			case 'm':
				if (strcmp(optarg,"MST") == 0) {
					mode = ModeType::MST;
				}
				else if (strcmp(optarg,"FASTTSP") == 0) {
					mode = ModeType::FASTTSP;
				}
				else if (strcmp(optarg,"OPTTSP") == 0) {
					mode = ModeType::OPTTSP;
				}
				else {
					if (optarg != NULL && optarg[0] == '\0')
						cerr << "Error: No mode specified\n";
					else
						cerr << "Error: Invalid mode\n";

					exit(1);
				}
				break;
			case 'h':
				cout << "help\n";
				exit(0);
				break;
			default:
				cerr << "Error: Invalid command line option\n";
				exit(1);
			}
		}
	}

	void readMap() {
		bool decon = true;
		bool lab = true;
		bool outer = true;
		cin >> num_rooms;

		room_vec.reserve(num_rooms);

		if (mode == ModeType::MST) {
			decon = false;
			lab = false;
			outer = false;
		}

		for (size_t i = 0; i < num_rooms; ++i) {
			int x, y;
			cin >> x >> y;
			room_vec.emplace_back(x, y);

			if (mode == ModeType::MST) {
				if (room_vec[i].room == RoomType::decon)	// If decontamination exists
					decon = true;
				if (room_vec[i].room == RoomType::lab)	// If lab exists
					lab = true;
				if (room_vec[i].room == RoomType::outer)	// If lab exists
					outer = true;
			}
		}

		

		if (mode == ModeType::MST) {
			if (outer && lab && !decon) {	// If there are both outer and lab rooms, but no decon...
				cerr << "Cannot construct MST\n";
				exit(1);
			}
			else {
				pathA();
			}

		}
		else if (mode == ModeType::FASTTSP) {
			pathB();
		}
		else {
			pathC();
		}
	}

	// MST
	void pathA() {
		// Nested-loop Prim's
		vector<Edge> edges(num_rooms);
		vector<double> dist(num_rooms, INFNTY);
		edges[0].edge_dist = 0;
		edges[0].parent_index = -1;
		dist[0] = 0;

		for (size_t i = 0; i < num_rooms; i++) {

			// Loop over all vertices: find closest false k_v
			int v = 0;
			double min = INFNTY;
			for (size_t j = 0; j < num_rooms; ++j) {
				if (!edges[j].visited && dist[j] < min) {
					min = dist[j];
					v = (int)j;
				}
			}

			// Mark k_v as true
			edges[v].visited = true;

			// Loop over all vertices: update false neighbors of k_v
			for (size_t w = 0; w < num_rooms; ++w) {
				if (!edges[w].visited) {
					double dub = getDist(room_vec[v], room_vec[w]);
					if (dub < dist[w]) {
						dist[w] = dub;
						edges[w].edge_dist = dub;
						edges[w].parent_index = v;
					}
				}
			}
			
		}

		if (mode == ModeType::OPTTSP)
			return;

		printMST(edges);
	}

	double getDist(const Room& A, const Room& B) {
		if ((A.room == RoomType::outer && B.room == RoomType::lab) || (A.room == RoomType::lab && B.room == RoomType::outer))
			return INFNTY;
		else
			return (((double)A.x - (double)B.x) * ((double)A.x - (double)B.x)) + (((double)A.y - (double)B.y) * ((double)A.y - (double)B.y));
	}

	double getDistBC(const Room& A, const Room& B) {
		return (((double)A.x - (double)B.x) * ((double)A.x - (double)B.x)) + (((double)A.y - (double)B.y) * ((double)A.y - (double)B.y));
	}

	// FASTTSP
	void pathB() {
		//double length = 0;
		
		// Build partial path 0-1-2
		path.reserve(num_rooms);
		for (size_t i = 0; i < 3; i++) {
			path.push_back(i);
			if (i > 0)
				length += sqrt(getDistBC(room_vec[i], room_vec[i - 1]));
		}
		length += sqrt(getDistBC(room_vec[0], room_vec[2]));
		
		// Given a partial tour, arbitrary select a city k that is not yet in the partial path.		
		for (size_t k = 3; k < num_rooms; k++) {

			double min = INFNTY;
			size_t index = 0;
			for (size_t i = 0; i < path.size(); i++) {	// Find the edge{ i, j } in partial path
				size_t j = (i + 1) % path.size();

				double d = length;
				d += sqrt(getDistBC(room_vec[path[i]], room_vec[k])) + sqrt(getDistBC(room_vec[path[j]], room_vec[k]));
				d -= sqrt(getDistBC(room_vec[path[i]], room_vec[path[j]]));

				// minimizes cik + ckj − cij
				if (d < min) {
					min = d;
					index = i;
				}
			}

			length -= sqrt(getDistBC(room_vec[path[index]], room_vec[path[(index + 1) % path.size()]]));
			length += sqrt(getDistBC(room_vec[path[index]], room_vec[k]));
			length += sqrt(getDistBC(room_vec[path[(index + 1) % path.size()]], room_vec[k]));

			path.insert(path.begin() + index + 1, k);	// Insert k between i and j.

		}	// If all cities are inserted then STOP, else loop.
		
		if (mode == ModeType::FASTTSP)
			printTSP(length);
		else
			return;
	}

	// OPTTSP
	void pathC() {
		pathB();
		OPT OPTpath(num_rooms, room_vec, path, length);
		OPTpath.genPerms(1);
		length = OPTpath.getLength();
		path = OPTpath.getPath();

		printTSP(length);
	}

	void printMST(const vector<Edge> &edges) {
		double weight = 0;
		for (size_t i = 0; i < edges.size(); ++i) {
			if (edges[i].parent_index >= 0)
				weight += sqrt(edges[i].edge_dist);
			
		}
		cout << fixed << setprecision(2) << weight << "\n";

		for (size_t i = 0; i < edges.size(); ++i) {
			if (edges[i].parent_index >= 0) {
				cout << edges[i].parent_index << " " << i << "\n";
			}
		}
	}

	void printTSP(double length) {
		cout << fixed << setprecision(2) << length << "\n";

		for (size_t i = 0; i < path.size(); ++i) {
			cout << path[i] << " ";
		}
		cout << "\n";
	}
};