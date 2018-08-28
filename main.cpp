#include <iostream>
#include <fstream>

#include "Graph.h"

using namespace std;

int main() {
  cout << "Startup!" << endl;
  Graph *matrix = new Graph("graphs/as_graph.txt", Graph::RepresentationType::ADJ_LIST);

  cout << "Cleanup!" << endl;
  delete matrix;
  return 0;
}