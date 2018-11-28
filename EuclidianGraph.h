//
// Created by vbustamante on 16-Nov-18.
//

#ifndef LIBGRAFOS_EUCLIDIANGRAPH_H
#define LIBGRAFOS_EUCLIDIANGRAPH_H


#include <string>
#include <vector>


using namespace std;

class EuclidianGraph {
public:
  struct Node{
    unsigned int x, y, id;
  };

  explicit EuclidianGraph(std::string fileName);
  ~EuclidianGraph();
  void dump();
  double getDistance(int a, int b);
  void solveTsp();
  vector<EuclidianGraph::Node> *getNodes();
  vector<EuclidianGraph::Node> *getPath();

  unsigned int getMaxX();
  unsigned int getMaxY();
  unsigned int getMinX();
  unsigned int getMinY();
  unsigned int getN();

private:
  unsigned int vCount;
  unsigned int maxX;
  unsigned int maxY;
  unsigned int minX;
  unsigned int minY;
  vector<Node> nodes;
  bool pathStatus = false;
  vector<Node> path;
};


#endif //LIBGRAFOS_EUCLIDIANGRAPH_H
