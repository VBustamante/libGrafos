#include <iostream>

#include "Graph.h"
#include "GetTimeMs64.h"

using namespace std;

int main() {
  cout << "Startup!" << endl;
  Graph *g;
  try {
    g = new Graph("graphs/as_graph.txt", Graph::RepresentationType::ADJ_LIST);
//    g->dump();
  }catch (const char* msg){
    cout << msg<<endl;
    cout << "Halting" <<endl;
    exit(1);
  }

//  auto searchStart = GetTimeMs64();
//  for(int i = 1; i <= 1000; i++){
//    if(i%50 == 0) cout<<i<<endl;
//    g->doSearch((i%g->getVertexCount())+1, ((i+1)%g->getVertexCount())+1, Graph::SearchType::BFS);
//  }
//  cout << "1000 searches in "<<GetTimeMs64() - searchStart << "ms" << endl;
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
    else if(cmd == "bft") {
      int v1;
      if((cin >> v1)){
        g->generateSearchTree(v1, Graph::SearchType::BFS);
      }else cout << "Attr error";
    }
    else if(cmd == "dft") {
      int v1;
      if((cin >> v1)){
        g->generateSearchTree(v1, Graph::SearchType::DFS);
      }else cout << "Attr error";
    }
    else if(cmd == "bfs") {
      int v1, v2;
      if((cin >> v1) && (cin >> v2)){
        int p = g->doSearch(v1, v2, Graph::SearchType::DFS);
        if(p == -1) cout<<"Couldn't find "<<v2<<" from "<<v1<<endl;
        else cout << "Found "<<v2<< " from " << v1 << " via " << p;
      }else cout << "Attr error";
    }
    else if(cmd == "dfs") {
      int v1, v2;
      if((cin >> v1) && (cin >> v2)){
        int p = g->doSearch(v1, v2, Graph::SearchType::DFS);
        if(p == -1) cout<<"Couldn't find "<<v2<<" from "<<v1<<endl;
        else cout << "Found "<<v2<< " from " << v1 << " via " << p;
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