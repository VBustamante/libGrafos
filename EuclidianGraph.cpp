//
// Created by vbustamante on 16-Nov-18.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <sstream>

#include "EuclidianGraph.h"
#include "GetTimeMs64.h"

using namespace std;

EuclidianGraph::EuclidianGraph(std::string fileName) {
  auto start = GetTimeMs64();
  ifstream input(fileName);

  vCount = 0;
  if(!(input >> vCount) || vCount < 1){
    input.close();
    cout << "Erro reading " << fileName <<  endl;
    throw "File doesn't start with the vertex count or it is less than 1. (also go check if it exists)";
  }
  nodes.reserve(vCount);

  unsigned int x, y;

  maxX = maxY = 0;
  minX = minY = numeric_limits<unsigned int>::max();

  for(unsigned int vIndex = 0; input >> x >> y; vIndex++){
    if(vIndex >= vCount){
      input.close();
      cout << "Erro reading " << fileName <<  endl;
      throw "Wrong vertex count (too many nodes)";
    }

    if(x > maxX) maxX = x;
    else if(x < minX) minX = x;
    if(y > maxY) maxY = y;
    else if(y < minY) minY = y;


    nodes[vIndex].x = x;
    nodes[vIndex].y = y;
    nodes[vIndex].id = vIndex + 1;
  }

  cout << "Created Euclidian Graph from " << fileName << " ("<< (GetTimeMs64() - start) << "ms)"<< endl;

  input.close();

}

void EuclidianGraph::dump() {
  cout << "Vertices: " << vCount << endl;
  cout << "X Range: " << minX << " to " << maxX << endl;
  cout << "Y Range: " << minY << " to " << maxY << endl;
}



EuclidianGraph::~EuclidianGraph() = default;


double EuclidianGraph::getDistance(int a, int b) {
  if(a >= vCount || b>= vCount) return -1;

  int dx = nodes[a].x - nodes[b].x;
  int dy = nodes[a].y - nodes[b].y;

  return sqrt(pow(dx, 2) + pow(dy, 2));

}

void EuclidianGraph::solveTsp() {

  sort(nodes.begin(), nodes.begin() + vCount, [](Node a, Node b){
    return a.x < b.x;
  });

  std::vector<std::vector<double>> b; // Stores the cost of shortest bitonic path [i][j]

  vector<vector<int>> p(vCount, vector<int>(vCount, vCount)); // Stores the parent of J of shortest bitonic path [i][j]


  b.resize(vCount);
  for(unsigned int i=0; i<vCount; i++){
    b.at(i).resize(vCount);
  }

  b[0][0] = 0;

  for(unsigned int j=1; j<vCount; j++) {
    b[0][j] = b[0][j-1] + getDistance(j-1, j);
    p[0][j] = j-1;
  }

  for(int i = 1; i < vCount; i++) {
    for(int j = i; j < vCount; j++) {
      b[i][j] = -1;
      if( i == j || i == j-1 ) {
        auto min = std::numeric_limits<double>::max();
        int parent = 9;
        for(int k = 0; k < i; k++){
          auto t = b[k][i] + getDistance(k, j);
          if(t < min){
            min=t;
            parent = k;
          }
        }
        b[i][j] = min;
        p[i][j] = parent;
      } else {
        b[i][j] = b[i][j-1] + getDistance(j-1, j);
        p[i][j] = j-1;
      }
    }
  }
  cout << "The cost of the minimal bitonic tour is " << b[vCount -1][vCount -1] << endl;

  path.resize(vCount);
  {
    auto path1 = stack<int>();
    auto path2 = stack<int>();
    auto activePath = &path1;

    int i = vCount - 2;
    int j = vCount - 1;

    while(j>0){
      activePath->push(j);
      j = p[i][j];
      if(j < i){
        auto temp = i;
        i = j;
        j = temp;
        activePath = activePath == &path1? &path2 : &path1;
      }
    }
    path1.push(0);

    cout << path1.size() << " + "<< path2.size() <<" should be " << vCount << endl;

    auto carriage = 0;
    while(!path1.empty()){
      path[carriage++] = nodes[path1.top()];
      path1.pop();
    }
    carriage = vCount - 1;
    while(!path2.empty()){
      path[carriage--] = nodes[path2.top()];
      path2.pop();
    }
  }
  for(int i = 0; i< vCount; i++) cout << path[i].id << " ";
  cout << endl;
  pathStatus = true;
}

vector<EuclidianGraph::Node> *EuclidianGraph::getNodes() {
  return &nodes;
}

vector<EuclidianGraph::Node> *EuclidianGraph::getPath() {
  return pathStatus? &path: nullptr;
}

unsigned int EuclidianGraph::getMaxX(){return maxX;};
unsigned int EuclidianGraph::getMaxY(){return maxY;};
unsigned int EuclidianGraph::getMinX(){return minX;};
unsigned int EuclidianGraph::getMinY(){return minY;}

unsigned int EuclidianGraph::getN() {
  return vCount;
};
