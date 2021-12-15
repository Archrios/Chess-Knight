#ifndef GRAPH
#define GRAPH
using namespace std;
#include<iostream>
#include<vector>
#include<queue>
#include <algorithm>
template <typename T>
class Graph;


//class Node {
//
//public:
//	Node(T value, Node<T>* link = nullptr) {
//		elm = value;
//		next = link;
//	}
//	friend class Graph;
//private:
//	T elm;
//	Node* next;
//};
template <typename T>
class Edge {
public:
	Edge(T vertex1, T vertex2, int w = NULL) {
		v1 = vertex1;
		v2 = vertex2;
		weight = w;
	}

	/*
	utility function to return v1
	*/
	int getV1() {
		return v1;
	}

	/*
	utility function to return v2
	*/
	int getV2() {
		return v2;
	}

	/*
	utility function to return weight
	*/
	int getWeight() {
		return weight;
	}
	friend class Graph<T>;
private:
	T v1;
	T v2;
	int weight;
};

template <typename T>
class Graph {
public:
	Graph() {
		set = {};
		edges = {};
	}

	/*
	checks if vertex exists within the set of vertexes.
	checks for one if given one vertex, checks for two if two vertexes given
	*/
	bool CheckVertex(T v1, T v2 = NULL) {
		bool check1 = true;
		bool check2 = (v2 == NULL) ? NULL : true;
		if (check2 == NULL) {
			/*for (it = set.begin(); it != set.end(); ++it) {
				if (it[0][0] == v1) {
					check1 = false;
				}
			}*/
			for (int i = 0; i < set.size(); i++) {
				if (set[i][0] == v1) {
					check1 = false;
				}
			}
			return !check1;
		}
		else {
			/*for (it = set.begin(); it != set.end(); ++it) {
				if (it[0][0] == v1) {
					check1 = false;
				}
				if (it[0][0] == v2) {
					check2 = false;
				}
			}*/
			for (int i = 0; i < set.size(); i++) {
				if (set[i][0] == v1) {
					check1 = false;
				}
				if (set[i][0] == v2) {
					check2 = false;
				}
			}
			return  !check1 && !check2;
		}
	}

	/*
	checks if an undirected edge from v1 to v2  or vice versa already exists. return true if it does
	false otherwise
	*/
	bool checkUDEdge(T v1, T v2) {
		for (int i = 0; i < edges.size(); i++) {
			if ((edges[i].v1 == v1 && edges[i].v2 == v2)||(edges[i].v2 == v1 && edges[i].v1 == v2)) { return true; }
		}
		return false;
	}

	/*
	checks if a directed edge from v1 to v2 already exists. return true if it does
	false otherwise
	*/
	bool checkDEdge(T v1, T v2) {
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i].v1 == v1 && edges[i].v2 == v2) { return true; }
		}
		return false;
	}

	/*
	checks if vertex already exists in the set, prints out error message if it does
	inserts the vertex into the set otherwise
	*/
	void InsertVertex(T vertex) {
		if (CheckVertex(vertex)) {
			cout << "Node already exists" << endl;
		}
		else {
			vector<T> node; node.push_back(vertex);
			set.push_back(node);
		}
	}

	/*
	checks if vertexes exist and if they do, inserts a directed edge from v1 to v2
	then goes down vector of node to attach the connected nodes to create an adjacency vector

	NOTE: checking if edge already exists is done in the function that generates the graphs
	in that it only calls this function if the edge doesnt exist already
	*/
	void InsertDEdge(T v1, T v2, int weight = NULL) {
		if (CheckVertex(v1, v2)) {
			edges.push_back(Edge<T>(v1, v2, weight));
			for (auto i = set.begin(); i != set.end(); i++) {
				if (i[0][0] == v1) {
					i[0].push_back(v2);
				}
			}
		}
	}

	/*
	checks if vertexes exist and if they do, inserts an undirected edge from v1 to v2
	then goes down vector of node to attach the connected nodes to create an adjacency vector

	NOTE: checking if edge already exists is done in the function that generates the graphs
	in that it only calls this function if the edge doesnt exist already
	*/
	void InsertUDEdge(T v1, T v2, int weight = NULL) {
		if (CheckVertex(v1, v2)) {
			edges.push_back(Edge<T>(v1, v2, weight));
			for (auto i = set.begin(); i != set.end(); i++) {
				if (i[0][0] == v1) {
					i[0].push_back(v2);
				}
				if (i[0][0] == v2) {
					i[0].push_back(v1);
				}
			}
		}
		else {
			cout << "Edge cannot connect one or more vertexes that do not exist" << endl;
			cout << "vertexes in question are: " << v1 << " and " << v2 << endl;
		}
	}


	void DeleteVertex(T v) {
		for (int i = 0; i < set.size(); i++) {
			if (set[i][0] == v) {
				set.erase(set.begin() + i);
			}
		}
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i].v1 == v || edges[i].v2 == v) {
				edges.erase(edges.begin() + i);
			}
		}
	}

	void DeleteDEdge(T v1, T v2) {
		for (int i = 0; i < set.size(); i++) {
			if (set[i][0] == v1) {
				for (int j = 0; j < set[i].size(); j++) {
					if (set[i][j] == v2) {
						set[i].erase(set[i].begin() + j);
					}
				}
			}
		}
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i].v1 == v1 && edges[i].v2 == v2)  {
				edges.erase(edges.begin() + i);
			}
		}
	}

	void DeleteUDEdge(T v1, T v2) {
		for (int i = 0; i < set.size(); i++) {
			if (set[i][0] == v1) {
				for (int j = 0; j < set[i].size(); j++) {
					if (set[i][j] == v2) {
						set[i].erase(set[i].begin() + j);
					}
				}
			}
			if (set[i][0] == v2) {
				for (int j = 0; j < set[i].size(); j++) {
					if (set[i][j] == v1) {
						set[i].erase(set[i].begin() + j);
					}
				}
			}
		}
		for (int i = 0; i < edges.size(); i++) {
			if ((edges[i].v1 == v1 && edges[i].v2 == v2) || (edges[i].v2 == v1 && edges[i].v1 == v2)) {
				edges.erase(edges.begin() + i);
			}
		}
	}

	vector<int> Adjacent(int v) {};

	/*
	checks if the graph is empty. return true if it is, false otherwise
	*/
	bool IsEmpty(Graph g) {
		if (g.set.size()) {
			return false;
		}
		return true;
	}

	/*
	unused utility function for printing out the vector of vertexes
	*/
	void printSet() {
		for (int i = 0; i < set.size(); i++) {
			cout << set[i][0] << endl;
		}
	}

	vector<T> getVertexes() {
		vector<T> list;
		for (int i = 0; i < set.size(); i++) {
			list.push_back(set[i][0]);
		}
		return list;
	}

	void printADJlist() {
		cout << "Adjacency List: " << endl;
		for (int i = 0;i<  set.size(); i++) {
			for (int j = 0; j < set[i].size(); j++) {
				cout << set[i][j] << " ";
			}
			cout << endl;
		}
	}

	void printEdges() {
		cout << "Edges: " << endl;
		for (auto i = edges.begin(); i != edges.end(); i++) {
			cout << i[0].v1 << " " << i[0].v2 << " " << i[0].weight << endl;
		}
	}

	/*
	utility function to return vector of edges
	*/
	vector<Edge<T>> getEdges() {
		return edges;
	}

	/*
	utility function to return vector of vertex nodes
	*/
	vector<vector<T>> getSet() {
		return set;
	}
	
	bool hasMore(vector<int> list) {
		for (int i = 0; i < list.size(); i++) {
			if (list[i] == 0) { return true; }
		}
		return false;
	}

	int findPos(T value) {
		for (int i = 0; i < set.size(); i++) {
			if (set[i][0] == value) { return i; }
		}
		return 0;
	}

	void DFS(T v)
	{
		int pos = findPos(v);
		num[pos] = number++;
		T value;
		for (int i = 1; i < set[pos].size(); i++) {
			value = set[pos][i];
			if (num[findPos(value)] == 0) {
				edges.push_back(Edge<T>(v, value));
				DFS(value);
			}
		}
	}
	void depthFirstSearch(T v)
	{
		int size = set.size();
		num.clear();
		num.assign(size, 0);
		edges.clear();
		number = 1;
		bool cont = true;
		while (cont) {
			DFS(v);
			if (hasMore(num)) {
				for (int i = 0; i < size; i++) {
					if (num[i] == 0) {
						v = set[i][0];
						break;
					}
				}
			}
			else {
				cont = false;
			}
		}
		this->printEdges();
		/*for (int i = 0; i < num.size(); i++) {
			cout << num[i] << endl;
		}*/
	}

	void breadthFirstTraversal(T v) {
		int size = set.size();
		num.clear();
		num.assign(size, 0);
		edges.clear();
		number = 1; int value;
		while (!queueE.empty()) {
			queueE.pop();
		}
		bool cont = true;
		int pos = 0;
		while (cont) {
			pos = findPos(v);
			queueE.push(v);
			num[pos] = number++;
			while (!queueE.empty()) {
				v = queueE.front(); queueE.pop();
				pos = findPos(v);
				for (int i = 1; i < set[pos].size(); i++) {
					value = set[pos][i];
					if (num[findPos(value)] == 0) {
						queueE.push(value);
						edges.push_back(Edge<T>(v, value));
						num[findPos(value)] = number++;
					}
				}
			}
			if (hasMore(num)) {
				for (int i = 0; i < size; i++) {
					if (num[i] == 0) { 
						v = set[i][0];
						break;
					}
				}
			}
			else {
				cont = false;
			}
		}
		this->printEdges();

	}

private:
	vector<vector<T>> set;
	vector<Edge<T>> edges;
	int size = set.size();
	vector<int>num;
	queue<T> queueE;
	int number;
};
#endif
