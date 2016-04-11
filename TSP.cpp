#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>
#include "Graph.cpp"

using namespace std;

vector<string> split(const string &s) {
    string buf; // Have a buffer string
    stringstream ss(s); // Insert the string into a stream

    vector<string> tokens; // Create vector to hold our words

    while (ss >> buf) {
        tokens.push_back(buf);
	}
	
	return tokens;
}

int main() {
	clock_t startTime, endTime, elapsed;
    double duration;

    startTime = clock();
	
	
	ifstream file("tsp.txt");
	if (!file) {
		cerr << "Could not open the file!" << endl;
		return 1;
	}
	
	//Build the graph from the input file
	Graph graph;
	string str;
	int index = 0;
	while(getline(file, str)) {
		vector<string> splitLine = split(str);
		if(index == 0) {
			for(int i = 0; i < splitLine.size(); i++) {
				graph.addVertex(to_string(i));
			}
		}
		
		for(int i = 1; i < splitLine.size(); i++) {
			graph.addEdge(splitLine.at(0), to_string(i + index), stod(splitLine.at(i))); 
		}
		index++;
	}
	
	Graph minGraph = graph.makeMinSpanningTree();
	
	vector<Vertex*> verts = minGraph.getAllVerticies();
	vector<pair<double, Vertex*> > edges;
	vector<string> oddDegree;
	for(int i = 0; i < verts.size(); i++) {
		edges = verts.at(i)->adj;
		if(edges.size() % 2 != 0) {
			oddDegree.push_back(verts.at(i)->name);
		}
	}
	
	Graph nextGraph = graph.perfectMatching(oddDegree);
	
	minGraph.combineGraph(nextGraph);
	
	minGraph.traverse(true);
	
	endTime = clock();
	elapsed = endTime - startTime;
	duration = ((double)elapsed / (double) CLOCKS_PER_SEC);
	return 0;
}
