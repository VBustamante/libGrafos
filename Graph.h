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
  // Internal Classes
  class Representation{
  public:
    virtual bool getAdjacency(int v1, int v2)= 0;
    virtual void addAdjacency(int v1, int v2)= 0;
    int getDegree(int vertex);
    void getNeighbours(int vertex, list<int> &neighbours);
    virtual ~Representation() = default;;

  protected:
    Representation() = default; //protected constructor makes the class abstract
    int vertexCount=0;
    int edgeCount=0;
  };

  class AdjacencyMatrix : public Representation{
  public:
    explicit AdjacencyMatrix(ifstream &file);

    ~AdjacencyMatrix() override;
    bool getAdjacency(int v1, int v2) override;
    void addAdjacency(int v1, int v2) override;

  private:
    int calc1DIndex(int v1, int v2);
    bool *adjacencies;
  };

  class AdjacencyList : public Representation{
  public:
    explicit AdjacencyList(ifstream &file);

    ~AdjacencyList() override;
    bool getAdjacency(int v1, int v2) override;
    void addAdjacency(int v1, int v2) override;

  private:
    list<int> **adjacencies;
  };

  // Data itself
  RepresentationType  representationType;
  Representation *representation;
};

#endif //LIBGRAFOS_GRAPH_H
