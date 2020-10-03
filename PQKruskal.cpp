#include <list>
#include <chrono>
#include <fstream>
#include <sstream>
#include <map>
#include <cassert>
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

class PriorityQueue
{
private:
	list<Edge> edges;

public:
	PriorityQueue() {}

	void Push(const Edge& e)
	{
		auto it = edges.begin();
		for (auto ie = edges.end(); it != ie; ++it)
		{
			if (e > *it)
			{
				edges.insert(it, e);
				return;
			}
		}
		edges.push_back(e);
	}

	const Edge Top()
	{
		assert(!edges.empty());
		return *edges.begin();
	}

	void Pop()
	{
		edges.pop_front();
	}

	bool Empty()
	{
		return edges.empty();
	}
};

#define INFDISTANCE 1 << 30

 
void KruskalUsingPriorityQueue(const vector<Edge>& edges, int vsize, ofstream& ofs)
{
	// record start time 
	auto start = chrono::steady_clock::now();

	PriorityQueue pq;
	for (const auto& tmpEdge : edges)
		pq.Push(tmpEdge);
	map<int, int> nodeSets;
	for (int i = 0; i < vsize; i++)
		nodeSets[i] = i;

	int cost = 0;
	while (!pq.Empty())
	{
		// always try to remove the least cost edge 
		const Edge firstEdge = pq.Top();
		pq.Pop();
		int minNode = (firstEdge.node1 < firstEdge.node2) ? firstEdge.node1 : firstEdge.node2;
		int maxNode = (firstEdge.node1 > firstEdge.node2) ? firstEdge.node1 : firstEdge.node2;
		int minSets = nodeSets[minNode];
		int maxSets = nodeSets[maxNode];
		if (minSets == maxSets)	// already in the same sets 
			continue;
		ofs << firstEdge.node1 << " " << firstEdge.node2 << ":" << firstEdge.weight << endl;
		cost += firstEdge.weight;

		// change all the node2 set to node1 
		for (auto it = nodeSets.begin(), ie = nodeSets.end(); it != ie; ++it)
		{
			if (it->second == maxSets)
				it->second = minSets;
		}
	}

	auto end = std::chrono::steady_clock::now();
	double dr_ms = std::chrono::duration<double, std::milli>(end - start).count();
	ofs << "[Priority-Queue -based Kruskal'a algorithm] MST cost: " << cost << ", time consumed: " << dr_ms << " ms";
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
	KruskalUsingPriorityQueue(edges, maxNode + 1, ofs);
	return 0;
}
