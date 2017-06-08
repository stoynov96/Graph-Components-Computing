#include <iostream>
#include "graph.h"

using namespace std;

void displayInfo(int,int,IMyGraph&);

int main() {
	int vertCount;
	int edgeCount;
	int compCount;
	IMyGraph graph;

	graph.readGraph(vertCount, edgeCount);

	// for(int i = 0; i < graph.verticeCount; i++) {
	// 	cout << graph.vertices[i]->index << ": ";
	// 	cout << graph.vertices[i]->findLeader()->index << "\n";
	// }

	displayInfo(vertCount, edgeCount, graph);

	cout << endl;

}

void displayInfo(int vertCount, int edgeCount, IMyGraph &graph) {
/*
TODO: Desc
*/
	cout << "There are " << vertCount << " vertices.\n";
	cout << "There are " << edgeCount << " edges.\n";
	graph.displayComponentInfo();

	return;
}