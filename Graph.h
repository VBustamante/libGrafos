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
  void dump();
  void REPL();
  ~Graph();
protected:
  // Internal Classes
  class Representation{
  public:
    virtual bool getAdjacency(int v1, int v2)= 0;
    virtual void addAdjacency(int v1, int v2)= 0;
    virtual unsigned int getDegree(int vertex) = 0;
    virtual void getNeighbours(int vertex, list<int> &neighbours)= 0;
    unsigned int getVertexCount(){return this->vertexCount;};
    unsigned int setEdgeCount(unsigned int edgeCount){this->edgeCount = edgeCount;};
    unsigned int getEdgeCount(){return this->edgeCount;};
    virtual void getConnectedComponents(list < list<int> *> &connectedComponents);
    virtual ~Representation() = default;;

  protected:
    Representation() = default; //protected constructor makes the class abstract
    unsigned int vertexCount=0;
    unsigned int edgeCount=0;

    void doDfs(int vertex, bool *visited, list<int> *vertexList);
  };

  class AdjacencyMatrix : public Representation{
  public:
    explicit AdjacencyMatrix(unsigned int vertexCount);
    ~AdjacencyMatrix() override;

    bool getAdjacency(int v1, int v2) override;
    void addAdjacency(int v1, int v2) override;
    unsigned int getDegree(int vertex) override;
    void getNeighbours(int vertex, list<int> &neighbours) override;

  private:
    int calc1DIndex(int v1, int v2);
    bool *adjacencies;
  };

  class AdjacencyList : public Representation{
  public:
    explicit AdjacencyList(unsigned int vertexCount);
    ~AdjacencyList() override;

    bool getAdjacency(int v1, int v2) override;
    void addAdjacency(int v1, int v2) override;
    unsigned int getDegree(int vertex) override;
    void getNeighbours(int vertex, list<int> &neighbours) override;

  private:
    list<int> **adjacencies;
  };

  // Data itself
  RepresentationType  representationType;
  Representation *representation;
};

#endif //LIBGRAFOS_GRAPH_H
