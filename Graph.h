//
// Created by vbustamante on 27/08/2018.
//

#ifndef LIBGRAFOS_GRAPH_H
#define LIBGRAFOS_GRAPH_H

#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <functional>
#include <vector>
#include <utility>

using namespace std;

class Graph {
public:
  enum class RepresentationType { ADJ_MATRIX, ADJ_LIST, WEIGHTED_ADJ_LIST};
  enum class SearchType { BFS, DFS};
  Graph(std::string fileName, RepresentationType representationType);
  unsigned int getVertexCount(){return representation->getVertexCount();};
  unsigned int getEdgeCount(){return representation->getEdgeCount();};
  bool getAdjacency(int v1, int v2){representation->getAdjacency(v1, v2);};
  virtual unsigned int getDegree(int vertex){ return representation->getDegree(vertex);};
  void generateSearchTree(int vertex, SearchType searchType);
  int doSearch(int root, int target, SearchType type);
  int getDiameter();
  void generateMinimunSpanningTree(int v);
  int getEccentricity(int v);

  void dump();
  ~Graph();
protected:
  // Internal Classes
  class Representation{
  public:
    virtual bool getAdjacency(int v1, int v2)= 0;
    virtual void addAdjacency(int v1, int v2, float w)= 0;
    virtual unsigned int getDegree(int vertex) = 0;
    virtual void getNeighbours(int vertex, list<int> &neighbours)= 0;
    unsigned int getVertexCount(){return this->vertexCount;};
    unsigned int setEdgeCount(unsigned int edgeCount){this->edgeCount = edgeCount;};
    unsigned int getEdgeCount(){return this->edgeCount;};
    bool isValidVertex(int v);
    void doDfs(int root, vector<bool> &visited, function<void (int, int)> hook);
    void doBfs(int root, vector<bool> &visited, function<void(int, int)> hook);
    void getConnectedComponents(list < list<int> *> &connectedComponents);
    //acochambracao
    virtual bool doDijkstra(int v, float *&distList, int *&daddy, float &eccentricity)=0;
    virtual void doPrim(int v, float *&costList, int *&daddy)=0;

    virtual ~Representation() = default;;

  protected:
    Representation() = default; //protected constructor makes the class abstract
    unsigned int vertexCount=0;
    unsigned int edgeCount=0;
  };

  class AdjacencyMatrix : public Representation{
  public:
    explicit AdjacencyMatrix(unsigned int vertexCount);
    ~AdjacencyMatrix() override;

    bool getAdjacency(int v1, int v2) override;
    void addAdjacency(int v1, int v2, float w) override;
    unsigned int getDegree(int vertex) override;
    void getNeighbours(int vertex, list<int> &neighbours) override;

    bool doDijkstra(int v, float *&distList, int *&daddy, float &eccentricity) override {return false;};
    void doPrim(int v, float *&costList, int *&daddy) override {};
  private:
    int calc1DIndex(int v1, int v2);
    vector<bool> adjacencies;
  };

  class AdjacencyList : public Representation{
  public:
    explicit AdjacencyList(unsigned int vertexCount);
    ~AdjacencyList() override;

    bool getAdjacency(int v1, int v2) override;
    void addAdjacency(int v1, int v2, float w) override;
    unsigned int getDegree(int vertex) override;
    void getNeighbours(int vertex, list<int> &neighbours) override;

    bool doDijkstra(int v, float *&distList, int *&daddy, float &eccentricity) override {return false;};
    void doPrim(int v, float *&costList, int *&daddy) override {};

  private:
    list<int> **adjacencies;
  };

  class WeightedAdjacencyList : public Representation {
  public:
      explicit WeightedAdjacencyList(unsigned int vertexCount);
      ~WeightedAdjacencyList() override;

      bool getAdjacency(int v1, int v2) override;
      //returns float equal to the edge's weight; 0 if no adjacency
      float getAdjacencyWeight(int v1, int v2);
      void addAdjacency(int v1, int v2, float w) override;
      unsigned int getDegree(int vertex) override;
      void getNeighbours(int vertex, list<int> &neighbours) override;
      void getWeightedNeighbours(int vertex, list<pair<int,float>> &neighbours);
      bool doDijkstra(int v, float *&distList, int *&daddy, float &eccentricity) override;
      void doPrim(int v, float *&costList, int *&daddy) override;

  private:
      list<pair<int,float>> **adjacencies;
  };

  // Data itself
  RepresentationType  representationType;
  Representation *representation;

};

#endif //LIBGRAFOS_GRAPH_H
