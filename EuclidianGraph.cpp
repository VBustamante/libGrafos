//
// Created by vbustamante on 16-Nov-18.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

#include "EuclidianGraph.h"
#include "GetTimeMs64.h"

using namespace std;

EuclidianGraph::EuclidianGraph(std::string fileName) {
  auto start = GetTimeMs64();
  ifstream input(fileName);

  vCount = 0;
  if(!(input >> vCount) || vCount < 1){
    input.close();
    throw "File doesn't start with the vertex count (or it is less than 1)";
  }
  nodes.reserve(vCount);

  unsigned int x, y;

  //a and b are vertices, w is the edge's weight. only works with weighted graphs
  for(unsigned int vIndex = 0; input >> x >> y; vIndex++){
    if(vIndex >= vCount){
      input.close();
      throw "Wrong vertex count (too many nodes)";
    }

    if(x > nodes[max.first].first) max.first = vIndex;
    else if(x < nodes[min.first].first) min.first = vIndex;
    if(y > nodes[max.second].second) max.second = vIndex;
    else if(y < nodes[min.second].second) min.second = vIndex;

    nodes[vIndex].first = x;
    nodes[vIndex].second = y;
  }

  cout << "Created Euclidian Graph from " << fileName << " ("<< (GetTimeMs64() - start) << "ms)"<< endl;

  input.close();

}

void EuclidianGraph::dump() {
  cout << "Vertices: " << vCount << endl;
  cout << "X Range: " << nodes[min.first].first << " to " << nodes[max.first].first << endl;
  cout << "Y Range: " << nodes[min.second].second << " to " << nodes[max.second].second << endl;
}



EuclidianGraph::~EuclidianGraph() = default;


double EuclidianGraph::getDistance(int a, int b) {
  if(a >= vCount || b>= vCount) return -1;

  int dx = nodes[a].first - nodes[b].first;
  int dy = nodes[a].second - nodes[b].second;

  return sqrt(pow(dx, 2) + pow(dy, 2));

}

void EuclidianGraph::solveTsp() {

  sort(nodes.begin(), nodes.begin() + vCount, [](pair<unsigned int,  unsigned int> a, pair<unsigned int,  unsigned int> b){
    return a.first < b.first;
  });

  for(int i = 0; i < vCount; i++){
    cout << i << ": " << nodes[i].first << " - " << nodes[i].second << endl;
  }

  auto **b = new double*[vCount];
  for(int i = 0; i < vCount; i++) {
    b[i] = new double[vCount];
  }

  auto **r = new int*[vCount-2];
  for(int i = 0; i < vCount-2; i++) {
    r[i] = new int[vCount];
  }

  b[0][1] = getDistance(0, 1);
  for(int j = 2; j < vCount; j++){
    for(int i = 0; i < j-2; i++){
      b[i][j] = b[i][j-1] +  getDistance(j-1, j);
      r[i][j] = j - 1;
    }

    b[j-1][j] = numeric_limits<double>::max();

    for(int k = 0; k < j-2; k++) {
      double q = b[k][j - 1] + getDistance(k, j);
      if(q < b[j - 1][j]){
        b[j - 1][j] = q;
        r[j - 1][j] = k;
      }
    }
  };
  b[vCount - 1][vCount - 1] = b[vCount-2][vCount - 1] + getDistance(vCount - 2, vCount - 1);

  cout << (vCount - 1) << " - ";
  cout << (vCount - 2) << " - ";
  int k = r[vCount - 2][vCount - 1];
  printPath(r, k, vCount - 2);
  cout << k << endl;


  // Cleanup
  for(int i = 0; i < vCount; i++) {
    delete b[i];
  }
  delete[] b;

  for(int i = 0; i < vCount-2; i++) {
    delete r[i];
  }
  delete[] r;
}

void EuclidianGraph::printPath(int **r, int i, int j) {
  int k;
  if(i < j){
    k = r[i][j];
    if(k != i) cout << k << " - ";
    if(k > 0) printPath(r, i, k);
  }else{
    k = r[j][i];
    if(k > 0){
      printPath(r, k, j);
      cout << k << " - ";
    }
  }
}
