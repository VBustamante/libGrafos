#include <iostream>
#include <fstream>

#include "Graph.h"

using namespace std;

int main() {
  cout << "Startup!" << endl;
  Graph *g = new Graph("graphs/as_graph.txt", Graph::RepresentationType::ADJ_MATRIX);
  g->dump();
  cout << "Cleanup!" << endl;
  delete g;
  return 0;
}