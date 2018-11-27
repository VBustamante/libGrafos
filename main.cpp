#include <iostream>
#include <stdlib.h>

#include "Graph.h"
#include "GetTimeMs64.h"
#include "tinyfiledialogs.h"
#include "EuclidianGraph.h"
#include "Screen.h"

using namespace std;

int main() {
  cout << "Startup!" << endl;
  EuclidianGraph *g;
  try {
    char const * fileFilterPatterns[] = { "*.txt"};
    const char* filename = tinyfd_openFileDialog("Selecione seu Grafo", "./graphs/.", 1, fileFilterPatterns, NULL, 0);
    g = new EuclidianGraph(filename);
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
    else if(cmd == "dst") {
      unsigned int v1, v2;
      if((cin >> v1) && (cin >> v2)){
        cout << "Dist "<<v1<<"<->"<<v2<< " = " << g->getDistance(v1, v2) << endl;
      }else cout << "Attr error";
    }
    else if(cmd == "tsp"){
      g->solveTsp();
    }
    else if(cmd == "see"){
      auto s = new Screen();

      delete s;
    }
    else{
      cout << "Unknown command";
    }

    cout<<" (" << (GetTimeMs64() - start) << "ms)" << endl;
    cin.clear();
    fflush(stdin);
  }while(cmd != "end");


  delete g;
  return 0;
}