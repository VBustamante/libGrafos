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
      rString = "List (NOT IMPLEMENTED)";
      break;
  }

  cout << "Creating " << rString << " from " << fileName << endl;

  input.close();
}

Graph::~Graph() {
  delete(representation);
}

int Graph::Representation::getDegree(int vertex){
  int degree = 0;
  for(int i =1; i<= vertexCount; i++){
    if(getAdjacency(vertex, i)) degree++;
  }
  return degree;
}

void Graph::Representation::getNeighbours(int vertex, list<int> &neighbours) {
  for(int i =1; i<= vertexCount; i++){
    if(getAdjacency(vertex, i)) neighbours.push_back(i);
  }
}

Graph::AdjacencyMatrix::AdjacencyMatrix(ifstream &file) {
  file >> vertexCount;
  cout << "Vertex count: " << vertexCount << endl;

  // We store the matrix on a flat array to optimize memory access.
  // Matrices  imply more mallocs which imply heap fragmentation.
  adjacencies = new bool[vertexCount * vertexCount];

  for (int i=0; i<vertexCount * vertexCount; i++) adjacencies[i] = false;

  int a, b;
  while(file >> a >> b){
    setAdjacency(a, b, true);
    setAdjacency(b, a, true);
    edgeCount++;
  }
}

Graph::AdjacencyMatrix::~AdjacencyMatrix() {
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

void Graph::AdjacencyMatrix::setAdjacency(int v1, int v2, bool value) {
  adjacencies[calc1DIndex(v1, v2)] = value;
}

