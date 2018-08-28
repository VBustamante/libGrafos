//
// Created by vbustamante on 27/08/2018.
//

#ifndef LIBGRAFOS_GRAPH_H
#define LIBGRAFOS_GRAPH_H

#include <string>
#include <iostream>
#include <fstream>
using namespace std;


class Graph {
public:
  enum class RepresentationType { ADJ_MATRIX, ADJ_LIST};
  Graph(std::string fileName, RepresentationType representationType);  // This is the constructor

protected:
  RepresentationType  representationType;
  class GraphRepresentation{
  public:
    virtual int *getNeighbours(int vertex) {cout << "Call to pure virtual method GraphRepresentation::getNeighbours" << endl;};
  };

  class AdjacencyMatrix : public GraphRepresentation{
  public:
    explicit AdjacencyMatrix(ifstream &file);
    ~AdjacencyMatrix();
    int *getNeighbours(int vertex) override;

  private:
    bool *adjacencies;
  };
  GraphRepresentation *representation;
};

#endif //LIBGRAFOS_GRAPH_H
