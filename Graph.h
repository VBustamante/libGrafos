//
// Created by vbustamante on 27/08/2018.
//

#ifndef LIBGRAFOS_GRAPH_H
#define LIBGRAFOS_GRAPH_H

#include <string>
#include <iostream>
#include <fstream>
#include <list>

using namespace std;

class Graph {
public:
  enum class RepresentationType { ADJ_MATRIX, ADJ_LIST};
  Graph(std::string fileName, RepresentationType representationType);
  ~Graph();
protected:
  class Representation{
  public:
    virtual bool getAdjacency(int v1, int v2)= 0;
    virtual void setAdjacency(int v1, int v2, bool value)= 0;
    int getDegree(int vertex);
    void getNeighbours(int vertex, list<int> &neighbours);

  protected:
    Representation() = default; //protected constructor makes the class abstract
    int vertexCount=0;
    int edgeCount=0;
  };

  class AdjacencyMatrix : public Representation{
  public:
    explicit AdjacencyMatrix(ifstream &file);
    ~AdjacencyMatrix();
    bool getAdjacency(int v1, int v2) override;
    void setAdjacency(int v1, int v2, bool value) override;

  private:
    int calc1DIndex(int v1, int v2);
    bool *adjacencies;
  };

  RepresentationType  representationType;
  Representation *representation;
};

#endif //LIBGRAFOS_GRAPH_H
