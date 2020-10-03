#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

struct Edge
{
	int node1;
	int node2;
	int weight;
	Edge() : node1(-1), node2(-1), weight(0)
	{}
	Edge(int n1, int n2, int w) : node1(n1), node2(n2), weight(w)
	{}

	bool operator < (const Edge& e) const { return weight > e.weight; }
	bool operator == (const Edge& e) const { return weight == e.weight; }
	bool operator > (const Edge& e) const { return weight < e.weight; }
};

inline bool compEdge(const Edge& e1, const Edge& e2)
{
	return e1.weight < e2.weight;
}

class UnionFind
{
private:
	std::vector<int> idArray;
	int size;

public:
	UnionFind(int _size) : idArray(_size)
	{
		size = _size;
		for (int j = 0; j < size; j++) 
			idArray[j] = j;
	}
	
	bool IsConnected(int p, int q) 
	{
		return idArray[p] == idArray[q];
	}

	void Union(int p, int q) 
	{
		int pid = idArray[p];
		int qid = idArray[q];
		for (int i = 0; i < size; i++)
		{
			if (idArray[i] == pid)
				idArray[i] = qid;
		}
	}
};

#define INFDISTANCE 1 << 30

void KruskalUsingUnionFind(const vector<Edge>& edges, int vsize, ofstream& ofs)
{
	// record start time 
	auto start = chrono::steady_clock::now();
	
	vector<Edge> allEdges = edges;
	UnionFind uf(vsize);
	sort(allEdges.begin(), allEdges.end(), compEdge); 

	int cost = 0;
	for (const auto& tmpEdge : allEdges)
	{
		if (uf.IsConnected(tmpEdge.node1, tmpEdge.node2))
			continue;
		uf.Union(tmpEdge.node1, tmpEdge.node2);
		ofs << tmpEdge.node1 << " " << tmpEdge.node2 << ":" << tmpEdge.weight << endl;
		cost += tmpEdge.weight;
	}

	auto end = std::chrono::steady_clock::now();
	double dr_ms = std::chrono::duration<double, std::milli>(end - start).count();
	ofs << "[Union-Find -based Kruskal'a algorithm] MST cost: " << cost << ", time consumed: " << dr_ms << " ms";
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "Usage: " << argv[0] << " input output" << endl;
		exit(0);
	}

	ifstream ifs(argv[1]);
	ofstream ofs(argv[2]);
	if (!ifs || !ofs)
	{
		cout << "Can not open file: " << argv[1] << " or " << argv[2] << endl;
		exit(0);
	}

	int startNode, endNode, weight;
	char ch;
	int maxNode = -1;
	vector<Edge> edges;
	string line;

	while (getline(ifs, line))
	{
		stringstream strLine(line);
		if (strLine >> startNode)
		{
			if (startNode > maxNode)
				maxNode = startNode;
			while (strLine >> endNode >> ch >> weight)
			{
				if (endNode > maxNode)
					maxNode = endNode;
				edges.push_back(Edge(startNode, endNode, weight));
			}
		}
	}

	// make the distance matrix 
	vector<vector<int> > distanceMatrix;
	vector<int> tmpDistance(maxNode + 1, INFDISTANCE);
	for (int i = 0; i < maxNode + 1; i++)
	{
		distanceMatrix.push_back(tmpDistance);
		distanceMatrix[i][i] = 0;
	}
	for (const auto& tmpEdge : edges)
	{
		distanceMatrix[tmpEdge.node1][tmpEdge.node2] = tmpEdge.weight;
		distanceMatrix[tmpEdge.node2][tmpEdge.node1] = tmpEdge.weight;
	}
	
	// test the MST using all kinds of ways 
	KruskalUsingUnionFind(edges, maxNode + 1, ofs);
	return 0;
}
