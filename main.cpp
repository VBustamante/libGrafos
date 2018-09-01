#include <iostream>
#include <fstream>

#include "Graph.h"

using namespace std;

int main() {
  cout << "Startup!" << endl;
  Graph *g;
  g = new Graph("graphs/as_graph.txt", Graph::RepresentationType::ADJ_LIST);
  g->dump();
  g->REPL();
  delete g;
  return 0;
}