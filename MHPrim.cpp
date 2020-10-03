#include <chrono>
#include <fstream>
#include <sstream>
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

class MinHeap
{
private:
	std::vector<Edge> _minHeap;
	int _index;
	int _maxSize;

public:
	MinHeap(int maxSize) : _minHeap(maxSize)
	{
		_maxSize = maxSize;
		_index = -1;
	}

	bool Empty() 
	{
		return _index == -1;
	}
	
	void Add(Edge e) 
	{
		if (Full()) 
			return;
		_index++;
		_minHeap[_index] = e;
	}
	
	bool Top(Edge& e)
	{
		if (Empty())
			return false;
		e = _minHeap[0];
		return true;
	}

	bool Pop()
	{
		if (Empty())
			return false;
		_minHeap[0] = _minHeap[_index];
		_index --;
		AdjustDown(0);
		return true;
	}

	// adjust downward 
	void AdjustDown(int index)
	{
		if (Empty())
			return;
		while (index < _index)
		{
			Edge tempEdge = _minHeap[index];
			int oneC = 2 * index + 1;
			int twoC = 2 * index + 2;
			if (oneC == _index) // if one child is the last element of the heap 
			{
				if (_minHeap[oneC] > tempEdge)
				{
					_minHeap[index] = _minHeap[oneC];
					_minHeap[oneC] = tempEdge;
				}
				return;
			}
			if (twoC >_index)
				return;
			if (_minHeap[oneC] > _minHeap[twoC] || _minHeap[oneC] == _minHeap[twoC]) 
			{
				if (_minHeap[oneC] > tempEdge)
				{
					_minHeap[index] = _minHeap[oneC];
					_minHeap[oneC] = tempEdge;
					index = oneC;
				}
				else
					index = _index;
			}
			else
			{
				if (_minHeap[twoC] > tempEdge)
				{
					_minHeap[index] = _minHeap[twoC];
					_minHeap[twoC] = tempEdge;
					index = twoC;
				}
				else
					index = _index;
			}
		}
	}

	// adjust upward 
	void AdjustUp(int index)
	{
		if (index > _index)
			return;
		while (index>-1)
		{
			Edge tempEdge = _minHeap[index];
			int father = (index - 1) / 2;
			if (father >= 0) 
			{
				if (tempEdge > _minHeap[father])
				{
					_minHeap[index] = _minHeap[father];
					_minHeap[father] = tempEdge;
					index = father;
				}
				else
					index = -1;
			}
			else 
				index = -1;
		}
	}

	// build the min heap 
	void CreateMinHeap() 
	{
		if (Empty()) 
			return;
		for (int i = (_index - 1) / 2; i > -1; i--)
			AdjustDown(i);
	}

private:
	bool Full()
	{
		return _index >= _maxSize;
	}
};

#define INFDISTANCE 1 << 30

void PrimUsingMinHeap(const vector<vector<int> >& distance, ofstream& ofs)
{
	// record start time 
	auto start = chrono::steady_clock::now();

	int vSize = (int)distance.size();
	MinHeap heap(vSize);

	// start from node 0
	for (int i = 1; i < (int)distance.size(); i++)
		heap.Add(Edge(0, i, distance[0][i]));
	heap.CreateMinHeap();
	int cost = 0;
	while (true)
	{
		MinHeap tmpHeap(vSize);
		Edge firstEdge;
		assert(heap.Top(firstEdge));
		assert(heap.Pop());
		int curNode = firstEdge.node2;
		if (firstEdge.weight != INFDISTANCE)	// still reachable 
		{
			cost += firstEdge.weight;
			ofs << firstEdge.node1 << " " << firstEdge.node2 << ":" << firstEdge.weight << endl;
			while (!heap.Empty())
			{
				Edge tmpEdge;
				assert(heap.Top(tmpEdge));
				if (distance[curNode][tmpEdge.node2] < tmpEdge.weight)
					tmpHeap.Add(Edge(curNode, tmpEdge.node2, distance[curNode][tmpEdge.node2]));
				else
					tmpHeap.Add(tmpEdge);
				assert(heap.Pop());
			}
		}
		else	// treat it like another graph 
		{
			while (!heap.Empty())
			{
				Edge tmpEdge;
				assert(heap.Top(tmpEdge));
				tmpHeap.Add(Edge(curNode, tmpEdge.node2, distance[curNode][tmpEdge.node2]));
				assert(heap.Pop());
			}
		}
		tmpHeap.CreateMinHeap();
		heap = tmpHeap;
		if (heap.Empty())
			break;
	}

	auto end = std::chrono::steady_clock::now();
	double dr_ms = std::chrono::duration<double, std::milli>(end - start).count();
	ofs << "[Min-Heap -based Prim'a algorithm] MST cost: " << cost << ", time consumed: " << dr_ms << " ms";
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
	PrimUsingMinHeap(distanceMatrix, ofs);
	return 0;
}
