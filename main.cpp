#include <iostream>

#include "Graph.h"
#include "GetTimeMs64.h"

using namespace std;

int main() {
  cout << "Startup!" << endl;
  Graph *g;
  try {
    g = new Graph("graphs/simple.txt", Graph::RepresentationType::ADJ_MATRIX);
    g->dump();
  }catch (const char* msg){
    cout << msg<<endl;
    cout << "Halting" <<endl;
    exit(1);
  }


  // REPL
  string cmd;
  do{
    cout<< "LibGraph REPL >>";
    cin >> cmd;

    auto start = GetTimeMs64();
    if(cmd == "end") break;
    else if(cmd == "edg") {
      int v1, v2;
      if((cin >> v1) && (cin >> v2)){
        cout << "Edge "<<v1<<"<->"<<v2<< " = " << g->getAdjacency(v1, v2);
      }else cout << "Attr error";
    }
    else if(cmd == "deg") {
      int v1;
      if((cin >> v1)){
        cout << "Degree "<<v1<< " = " << g->getDegree(v1);
      }else cout << "Attr error";
    }
    else if(cmd == "bfs") {
      int v1;
      if((cin >> v1)){
        g->generateSearchTree(v1, Graph::SearchType::BFS);
      }else cout << "Attr error";
    }
    else if(cmd == "dfs") {
      int v1;
      if((cin >> v1)){
        g->generateSearchTree(v1, Graph::SearchType::DFS);
      }else cout << "Attr error";
    }else{
      cout << "Unknown command";
    }

    cout<<" (" << (GetTimeMs64() - start) << "ms)" << endl;
    cin.clear();
    fflush(stdin);
  }while(cmd != "end");


  delete g;
  return 0;
}