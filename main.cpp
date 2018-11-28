#include <iostream>
#include <stdlib.h>
#include <sstream>

#include "Graph.h"
#include "GetTimeMs64.h"
#include "EuclidianGraph.h"
#include "Screen.h"

using namespace std;

int main() {
  cout << "Startup!" << endl;

  string cmd;
  do{
    EuclidianGraph *g;
    try {
      stringstream ss;
      cout << "Enter file index: ";
      int fileId;
      cin >> fileId;
      ss << "./graphs/points-" << fileId << ".txt";
      g = new EuclidianGraph(ss.str());
      g->dump();
    }catch (const char* msg){
      cout << msg << endl;
      continue;
    }



    // REPL
    do{
      cout<< "LibGraph REPL >>";
      cin >> cmd;

      auto start = GetTimeMs64();
      if(cmd == "end") break;
      if(cmd == "clr") break;
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
        auto screen = new Screen(g);
        delete screen;
      }
      else{
        cout << "Unknown command";
      }

      cout<<" (" << (GetTimeMs64() - start) << "ms)" << endl;
      cin.clear();
      fflush(stdin);
    }while(cmd != "clr");

    delete g;

  }while(cmd != "end");
  return 0;
}