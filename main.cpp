#include <iostream>
#include <stdlib.h>

#include "Graph.h"
#include "GetTimeMs64.h"
#include "tinyfiledialogs.h"

using namespace std;

int main() {
  cout << "Startup!" << endl;
  Graph *g;
  try {
    //char const * fileFilterPatterns[] = { "*.txt"};
    //const char* filename = tinyfd_openFileDialog("Selecione seu Grafo", "./graphs/.", 1, fileFilterPatterns, NULL, 0);

    g = new Graph("graphs/grafo_3.txt", Graph::RepresentationType::WEIGHTED_ADJ_LIST);
    g->dump();
  }catch (const char* msg){
    cout << msg<<endl;
    cout << "Halting" <<endl;
    exit(1);
  }

//  auto searchStart = GetTimeMs64();
//  const int loops = 10;
//  for(int i = 1; i <= loops; i++){
//      cout<<i<<" ";
//    g->doSearch((i%g->getVertexCount())+1, ((i+1)%g->getVertexCount())+1, Graph::SearchType::DFS);
//  }
//  cout << endl << loops << " searches in "<<GetTimeMs64() - searchStart << "ms" << endl;

  auto searchStart = GetTimeMs64();

//  cout << "Diameter: " << g->getDiameter() << " ("<<GetTimeMs64() - searchStart << "ms)"<< endl;

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
    }
    else if(cmd == "ecc") {
        int v1;
        if((cin >> v1)){
            float p = g->getEccentricity(v1);
            if(p == -1) cout<<"Invalid vertex" << endl;
            else cout << "Found eccentricity "<< p << " from vertex " << v1 <<endl;
        }else cout << "Attr error";
    }
    else if(cmd == "path") {
      int v1, v2;
      float *distList;
      int *daddy;
      float distance;
      if((cin >> v1) && (cin >> v2)){
        int p = g->getPaths(v1, v2, distList, daddy);
        if(p == -1) cout<<"Couldn't find "<<v2<<" from "<<v1<<endl;
        else {
          cout << "Found path to " << v2 << " from vertex " << v1 << endl;
          distance = distList[v2-1];
          while (v2 != v1) {
            cout << v2 << " ";
            v2 = daddy[v2 - 1];
          }
          cout << v1 << endl << "With distance " << distance << endl;
        }
      }else cout << "Attr error";
    }
    else if(cmd == "mst") {
      int v1;
      if((cin >> v1)){
        int p = g->generateMinimumSpanningTree(v1);
        if(p == -1) cout<<"Invalid vertex" << endl;
        else cout << "Generated Minimum Spanning Tree from vertex " << v1 <<endl;
      }else cout << "Attr error";
    }
    else if(cmd == "avgdist") {
      float p = g->getAverageDistance();
      if(p == -1) cout<<"Invalid vertex" << endl;
      else cout << "Average distance is " << p <<endl;
    }
    else if(cmd == "ecctime") {
      int vertexCount = g->getVertexCount();
      for (int i = 0; i<100; i++){
        g->getEccentricity(rand() % vertexCount + 1);
      }
    }
    else if(cmd == "nb") {
        int v1;
        list<int> neighbours;
        if((cin >> v1)){
            g->getNeighbours(v1, neighbours);
            for(auto n: neighbours) cout << n << " ";
        }else cout << "Attr error";
        cout << endl;
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