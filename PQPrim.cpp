#include <list>
#include <chrono>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <cassert>
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


void PrimUsingPriorityQueue(const vector<vector<int> >& distance, ofstream& ofs)
{
	// record start time 
	auto start = chrono::steady_clock::now();

	PriorityQueue pq;
	// start from node 0
	for (int i = 1; i < (int)distance.size(); i++)
		pq.Push(Edge(0, i, distance[0][i]));
	int cost = 0;
	while (true)
	{
		PriorityQueue tmpPQ;
		const Edge firstEdge = pq.Top();
		pq.Pop();
		int curNode = firstEdge.node2;
		if (firstEdge.weight != INFDISTANCE)	// still reachable 
		{
			cost += firstEdge.weight;
			ofs << firstEdge.node1 << " " << firstEdge.node2 << ":" << firstEdge.weight << endl;
			while (!pq.Empty())
			{
				const Edge tmpEdge = pq.Top();
				if (distance[curNode][tmpEdge.node2] < tmpEdge.weight)
					tmpPQ.Push(Edge(curNode, tmpEdge.node2, distance[curNode][tmpEdge.node2]));
				else
					tmpPQ.Push(tmpEdge);
				pq.Pop();
			}
		}
		else	// treat it like another graph 
		{
			while (!pq.Empty())
			{
				const Edge tmpEdge = pq.Top();
				tmpPQ.Push(Edge(curNode, tmpEdge.node2, distance[curNode][tmpEdge.node2]));
				pq.Pop();
			}
		}
		pq = tmpPQ;
		if (pq.Empty())
			break;
	}

	auto end = std::chrono::steady_clock::now();
	double dr_ms = std::chrono::duration<double, std::milli>(end - start).count();
	ofs << "[Priority-Queue -based Prim'a algorithm] MST cost: " << cost << ", time consumed: " << dr_ms << " ms";
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
	PrimUsingPriorityQueue(distanceMatrix, ofs);
	return 0;
}
