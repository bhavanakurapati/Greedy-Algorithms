# Program Execution:

A make file compiles all the four algorithms and to run the program we use:  
`./programname <inputfile> <outputfile>`  
Example:  
./Kruskal input.txt output.txt

# Motivation and Background:

Minimum Spanning Tree is a connected acyclic subgraph with the same set of vertices contained in the actual graph but a subset of edges and also having least overall edge cost. Here we use two greedy algorithms for generating minimum spanning tree, Kruskal’s and Prims algorithms and compare their perfromances.. The main motivation is to show that Kruskal’s algorithm using union find is performing better than using priority queue and also Prims algorithm is performing better using min heap than using priority queue. Finally, we also say that kruskals performs better for sparse graphs and prims performs better for dense graphs.

# Pseudocodes:

## Kruskal Using Priority Queue

```
Input: A weighted connected graph G= (V, E)

Step 1: Initialize keys of all vertices as
Step 2: Using Priority Queue implementation, sort all the edges in non-decreasing order of the edge weights.
Step 3:
While (Priority queue is not empty)
    →Take top of priority queue as the first edge and pop it.
    →Add nodes to the minsets and maxsets
    →If (minsets == maxstes)
    Continue;
    Print the nodes connecting these edges
    Print the weight of the edge
    Change all the node2 set to node1 set
Step 5: Also the printing the time taken to run the program and the cost
```

## Kruskal’s using Union Find:

```
Input : A weighted connected graph G=(V,E)

Step 1. A=∅
Step 2. for each v ∈ G.V:
Step 3. MAKE-SET (v)
Step 4.
For each (u, v) in G.E ordered by weight (u, v), increasing 5.
    If FIND-SET (u) ≠ FIND-SET (v):
        A = A ∪ {(u, v)}
        UNION (u, v)
    Return A
```

## Prims Using Priority Queue:

```
Input: A weighted connected graph G= (V, E)
Step 1: Implement a Priority Queue of size which is equal to number of vertices.
Step 2: Create a pair object for each vertex containing the information about vertex and key.
Step 3: Create a pair object for vertex 0 with key 0 and insert into priority queue
Step 4:
While(true)
    →Extract the first edge from taking the top of the priority queue and pop it from the priority queue
    If(weight of first edge is not equal to infdistance)
        Then print the nodes connecting these edges
        Print the weight of the edge
    Else
        Just pop the edge from the priority queue
Step 5: Also the printing the time taken to run the program and the cost
```

## Prims using Min Heap:

```
Step 1: Create min Heap of size = no of vertices.
Step 2: Create a heapNode for each vertex which will store information about vertex and key.
Step 3: Use minheap vector and key array to keep track of the vertices and respective key value which are currently in min heap.
Step 4: For each heapNode, Initialize key as MAX_VAL except the heapNode for the first vertex for which key will 0
Step 5:
While(true)
→Extract the first edge from taking the top of the priority queue and pop it from the priority queue
If(weight of first edge is not equal to infdistance)
Then print the nodes connecting these edges
Print the weight of the edge
Else
Just pop the edge from the priority queue
Step 5: Also the printing the time taken to run the program and the cost
```
