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
  explicit EuclidianGraph(std::string fileName);
  ~EuclidianGraph();
  void dump();
  double getDistance(unsigned int a, unsigned int b);
  void solveTsp();

private:
  unsigned int vCount;
  pair<unsigned int, unsigned int> max; // max(x, y)
  pair<unsigned int, unsigned int> min; // min(x, y)
  vector<pair<unsigned int, unsigned int>> nodes;
};


#endif //LIBGRAFOS_EUCLIDIANGRAPH_H
