//
// Created by vbustamante on 27/08/2018.
//
#include "Graph.h"

Graph::Graph(const std::string fileName, Graph::RepresentationType representationType) {
  ifstream input(fileName);
  this->representationType = representationType;

  string rString;
  switch (representationType){
    case Graph::RepresentationType::ADJ_MATRIX:
      rString = "Matrix";
      representation = new Graph::AdjacencyMatrix(input);
      break;
    case Graph::RepresentationType::ADJ_LIST:
      rString = "List";
      representation = new Graph::AdjacencyList(input);
      break;
  }

  cout << "Creating " << rString << " from " << fileName << endl;

  input.close();
}

Graph::~Graph() {
  cout << "~Graph"<<endl;
  delete representation;
}

void Graph::REPL() {
  string cmd;
  do{
    cout<< "LibGraph REPL >>";
    cin >> cmd;

    if(cmd == "end") break;
    else if(cmd == "edg") {
      int v1, v2;
      cin >> v1 >> v2;
      cout << "Edge "<<v1<<"<->"<<v2<< " = " << representation->getAdjacency(v1, v2) <<endl;
    }
    else if(cmd == "deg") {
      int v1;
      cin >> v1;
      cout << "Degree "<<v1<< " = " << getDegree(v1) <<endl;
    }else{
      cout << "Unknown command"<<endl;

    }
    cin.clear();
    fflush(stdin);
  }while(cmd != "end");
}

void Graph::dump() {
  cout << "Vertex Count " << representation->getVertexCount() << endl;
  cout << "Edge Count " << representation->getEdgeCount() << endl;

  int maxDegree, minDegree;

  maxDegree = minDegree = getDegree(1);

  for(int i=2; i<=representation->getVertexCount(); i++){
    int d = getDegree(i);
    if(d>maxDegree) maxDegree = d;
    if(d<minDegree) minDegree = d;
  }

  cout << "Min Degree " << minDegree << endl;
  cout << "Max Degree " << maxDegree << endl;

  int avgDegree = (representation->getEdgeCount()*2)/representation->getVertexCount();
  cout << "Avg Degree " << avgDegree << endl;

  // mediana de grau
}

// Internal Classes
int Graph::getDegree(int vertex){
  int degree = 0;
  for(int i =1; i<= representation->getVertexCount(); i++){
    if(representation->getAdjacency(vertex, i)) degree++;
  }
  return degree;
}

void Graph::getNeighbours(int vertex, list<int> &neighbours) {
  for(int i =1; i<= representation->getVertexCount(); i++){
    if(representation->getAdjacency(vertex, i)) neighbours.push_front(i);
  }
}

// Matrix
Graph::AdjacencyMatrix::AdjacencyMatrix(ifstream &file) {
  file >> vertexCount;
  cout << "Vertex count: " << vertexCount << endl;

  // We store the matrix on a flat array to optimize memory access.
  // Matrices  imply more mallocs which imply heap fragmentation.
  adjacencies = new bool[vertexCount * vertexCount];

  for (int i=0; i<vertexCount * vertexCount; i++) adjacencies[i] = false;

  int a, b;
  while(file >> a >> b){
    addAdjacency(a, b);
    addAdjacency(b, a);
    edgeCount++;
  }
}

Graph::AdjacencyMatrix::~AdjacencyMatrix() {
  cout << "~AdjacencyMatrix"<<endl;
  delete[] adjacencies;
}

// We need this to access our flat matrix
// Also for some reason the indexes start at one so we have to deal with that
int Graph::AdjacencyMatrix::calc1DIndex(int v1, int v2) {
  return (v1 - 1)*vertexCount + (v2 - 1);
}

bool Graph::AdjacencyMatrix::getAdjacency(int v1, int v2) {
  return adjacencies[calc1DIndex(v1, v2)];
}

void Graph::AdjacencyMatrix::addAdjacency(int v1, int v2) {
  adjacencies[calc1DIndex(v1, v2)] = true;
}

// List
Graph::AdjacencyList::AdjacencyList(ifstream &file) {
  file >> vertexCount;
  cout << "Vertex count: " << vertexCount << endl;

  adjacencies = new list<int> *[vertexCount];

  for (int i=0; i<vertexCount; i++) adjacencies[i] = new list<int>;

  int a, b;
  while(file >> a >> b){
    addAdjacency(a, b);
    addAdjacency(b, a);
    edgeCount++;
  }
}

Graph::AdjacencyList::~AdjacencyList() {
  cout << "~AdjacencyList"<<endl;
  for (int i=0; i<vertexCount; i++) delete adjacencies[i];
  delete[] adjacencies;
}


bool Graph::AdjacencyList::getAdjacency(int v1, int v2) {
  list<int> *v1Neighbours = adjacencies[v1 - 1];
  bool found = false;

  for (int &v1Neighbour : *v1Neighbours) {
    if(v1Neighbour == v2 - 1){
      found = true;
      break;
    }
  }

  return found;
}

void Graph::AdjacencyList::addAdjacency(int v1, int v2) {
  list<int> *v1Neighbours = adjacencies[v1 - 1];
  v1Neighbours->push_front(v2 - 1);
}

