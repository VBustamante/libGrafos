#include <iostream>
#include <fstream>

#include "Graph.h"

using namespace std;

int main() {
  Graph *g = new Graph("graphs/as_graph.txt", Graph::RepresentationType::ADJ_MATRIX);
  cout << "Hello!" << endl;
  return 0;
}