#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>
#include <limits>

using namespace std;

struct Vertex{
	vector<pair<double,Vertex*> > adj; //cost of edge, destination vertex
	string name;
	bool visited;
	Vertex(string s)
	{
		name = s;
		visited = false;
	}
};

class Graph
{
	public:
		typedef map<string, Vertex*> vmap;
		vmap work;
		void addVertex(const string&);
		void addEdge(const string& from, const string& to, double cost);
		void removeEdge(const string& from, const string& to);
		vector<pair<double, Vertex*> > getEdges(const string& name);
		Vertex* getVertex(const string& name);
		vector<Vertex*> getAllVerticies();
		double getDistance(const string& from, const string& to);
		Graph makeMinSpanningTree();
		vector<Vertex*> getOdds(vector<string> odds);
		Graph perfectMatching(vector<string> odds);
		void combineGraph(Graph g);
		double traverse(bool print);
		void doSwapping(clock_t startTime, Graph orig);
};

void Graph::addVertex(const string &name)
{
	vmap::iterator itr=work.begin();
	itr=work.find(name);
	if(itr==work.end())
	{
		Vertex *v;
		v = new Vertex(name);
		work[name] = v;
		return;
	}
}

void Graph::addEdge(const string& from, const string& to, double cost)
{
	Vertex *f=(work.find(from)->second);
	Vertex *t=(work.find(to)->second);
	
	bool alreadyAdded1 = false, alreadyAdded2 = false;
	for(int i = 0; i < f->adj.size(); i++) {
		if(f->adj.at(i).second->name == to) {
			alreadyAdded1 = true;
			break;
		}
	}
	
	for(int i = 0; i < t->adj.size(); i++) {
		if(t->adj.at(i).second->name == from) {
			alreadyAdded2 = true;
			break;
		}
	}
	
	if(!(alreadyAdded1 || alreadyAdded2)) {
		pair<double,Vertex *> edge = make_pair(cost,t);
		pair<double,Vertex*> e2 = make_pair(cost, f);
		f->adj.push_back(edge);
		t->adj.push_back(e2);
	}
}

void Graph::removeEdge(const string& from, const string& to) {
	Vertex *f=(work.find(from)->second);
	Vertex *t=(work.find(to)->second);
	
	for(int i = 0; i < f->adj.size(); i++) {
		if(f->adj.at(0).second->name == t->name) {
			f->adj.erase(f->adj.begin() + i - 1);
		}
	}
	
	for(int i = 0; i < t->adj.size(); i++) {
		if(t->adj.at(0).second->name == f->name) {
			t->adj.erase(t->adj.begin() + i - 1);
		}
	}
}

vector<pair<double, Vertex*> > Graph::getEdges(const string& name)
{
	return work.at(name)->adj;
}

Vertex* Graph::getVertex(const string& name)
{
	return work.at(name);
}

vector<Vertex*> Graph::getAllVerticies()
{
	vector<Vertex*> verticies;
	vmap::iterator itr;
	for(itr = work.begin(); itr != work.end(); itr++) {
		verticies.push_back(itr->second);
	}
	
	return verticies;
}

double Graph::getDistance(const string& from, const string& to)
{	
	double result = 0;
	vector<pair<double, Vertex*> > edges = this->getEdges(from);
	for(int i = 0; i < edges.size(); i++) {
		if(edges.at(i).second->name == to) {
			result = edges.at(i).first;
			break;
		}
	}
	
	return result;
}

vector<Vertex*> Graph::getOdds(vector<string> odds)
{
	vector<Vertex*> verticies;
	vmap::iterator itr;
	for(itr = work.begin(); itr != work.end(); itr++) {
		for(int i = 0; i < odds.size(); i++) {
			if(odds.at(i) == itr->second->name) {
				verticies.push_back(itr->second);
			}
		}
	}
	
	return verticies;
}

Graph Graph::makeMinSpanningTree()
{
	Graph g;
	
	vector<Vertex*> visited;
	vector<Vertex*> verts = this->getAllVerticies();
	for(int i = 0; i < verts.size(); i++) {
		verts.at(i)->visited = false;
		g.addVertex(verts.at(i)->name);
	}
	
	verts.at(0)->visited = true;
	visited.push_back(verts.at(0));
	
	vector<pair<double, Vertex*> > edges;
	string x;
	while(visited.size() != verts.size()) {
		Vertex *f = visited.at(visited.size() - 1);
		double min = numeric_limits<double>::max();
		int index = 0;
		for(int i = 0; i < verts.size(); i++) {
			edges = this->getEdges(f->name);
			for(int c = 0; c < edges.size(); c++) {
				if(!edges.at(c).second->visited) {
					if(edges.at(c).first < min) {
						index = c;
						min = edges.at(c).first;
						x = edges.at(c).second->name;
					}
				}
			}
		}
		
		edges.at(index).second->visited = true;
		visited.push_back(edges.at(index).second);
		g.addEdge(f->name, x, min);
	}
	
	return g;
}

Graph Graph::perfectMatching(vector<string> oddList)
{
	Graph g;
	vector<Vertex*> odds = this->getOdds(oddList);
	for(int i = 0; i < odds.size(); i++) {
		g.addVertex(odds.at(i)->name);
	}
	
	Vertex* closest = NULL;
	double length;
	vector<Vertex*>::iterator tmp, first;
	double dist = -1;

	// for each odd node
	while (!odds.empty()) {
		first = odds.begin();
		vector<Vertex*>::iterator it = odds.begin() + 1;
		vector<Vertex*>::iterator end = odds.end();
		length = numeric_limits<double>::max();
		for (; it != end; ++it) {
			// if this node is closer than the current closest, update closest and length
			
			dist = this->getDistance((*first)->name, (*it)->name);
			if (dist < length) {
				length = dist;
				closest = *it;
				tmp = it;
			}
		}	// two nodes are matched, end of list reached
		g.addEdge((*first)->name, closest->name, length);
		odds.erase(tmp);
		odds.erase(first);
	}
	
	return g;
}

void Graph::combineGraph(Graph g)
{
	vector<Vertex*> verts = g.getAllVerticies();
	for(int i = 0; i < verts.size(); i++) {
		vector<pair<double, Vertex*> > edges = g.getEdges(verts.at(i)->name);
		for(int c = 0; c < edges.size(); c++) {
			this->addEdge(verts.at(i)->name, edges.at(c).second->name, edges.at(c).first);
		}
	}
}

double Graph::traverse(bool print)
{
	vector<Vertex*> verts = this->getAllVerticies();
	
	for(int i = 0; i < verts.size(); i++) {
		verts.at(i)->visited = false;
	}
	
	double cost = 0;
	Vertex* v = verts.at(0);
	v->visited = true;
	string start = v->name;
	cost += v->adj.at(0).first;
	v = v->adj.at(0).second;
	bool isNotAtStart = false;
	
	if(print) {
		cout << start << " ";
	}
	while(true) {
		v->visited = true;
		
		if(print) {
			cout << v->name << " ";
		}
		
		for(int p = 0; p < v->adj.size(); p++) {
			if(!v->adj.at(p).second->visited) {
				cost += v->adj.at(p).first;
				v = v->adj.at(p).second;
				isNotAtStart = true;
			}
		}
		
		if(!isNotAtStart) {
			if(print) {
				cout << start;
			}
			for(int p = 0; p < v->adj.size(); p++) {
				if(v->adj.at(p).second->name.compare(start) == 0) {
					cost += v->adj.at(p).first;
				}
			}
			
			break;
		} else {
			isNotAtStart = false;
		}
	}
	
	return cost;
}
