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

}

Graph::AdjacencyMatrix::AdjacencyMatrix(ifstream &file) {
  string line;
  getline(file, line);
  cout << "Vertex count: " << line << endl;

  int vcount = stoi(line);
  adjacencies = new bool[vcount * vcount];

  // TODO fill adjacencies from file
}

Graph::AdjacencyMatrix::~AdjacencyMatrix() {
  delete[] adjacencies;
}


int *Graph::AdjacencyMatrix::getNeighbours(int vertex) {
  return GraphRepresentation::getNeighbours(vertex);
}

