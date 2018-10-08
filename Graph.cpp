//
// Created by vbustamante on 27/08/2018.
//
#include <algorithm>
#include <stack>
#include <iomanip>
#include <queue>
#include "Graph.h"
#include "GetTimeMs64.h"

Graph::Graph(const std::string fileName, Graph::RepresentationType representationType) {
  this->representationType = representationType;

  auto start = GetTimeMs64();
  ifstream input(fileName);
  unsigned int vCount = 0;
  if(!(input >> vCount) || vCount < 1){
    input.close();
    throw "File doesn't start with the vertex count (or it is less than 1)";
  }

  string rString;
  switch (representationType){
    case Graph::RepresentationType::ADJ_MATRIX:
      rString = "Matrix";
      representation = new Graph::AdjacencyMatrix(vCount);
      break;
    case Graph::RepresentationType::ADJ_LIST:
      rString = "List";
      representation = new Graph::AdjacencyList(vCount);
      break;

    case Graph::RepresentationType::WEIGHTED_ADJ_LIST:
      rString = "Weighted List";
      representation = new Graph::WeightedAdjacencyList(vCount);
      break;
  }

  int a, b;
  float w = 1;
  unsigned int lineCount = 1;
  unsigned int edgeCount = 0;
#define LIBGRAPH_BADLINE_THRESHOLD 10
  unsigned badLines = 0;


  //a and b are vertices, w is the edge's weight. only works with weighted graphs
  while(input >> a >> b >> w){
    lineCount++;
    if(a < 1 || a > vCount || b < 1 || b > vCount ){
      badLines++;
      if(badLines < LIBGRAPH_BADLINE_THRESHOLD) cout << "Bad edge at line "<<lineCount<<". Skipping."<<endl;
      if(badLines == LIBGRAPH_BADLINE_THRESHOLD) cout << "More than "<<LIBGRAPH_BADLINE_THRESHOLD<<" bad lines. Will stop reporting but keep skipping."<<endl;
      continue;
    }
    representation->addAdjacency(a, b, w);
    representation->addAdjacency(b, a, w);

    edgeCount++;
  }

  if(badLines >= LIBGRAPH_BADLINE_THRESHOLD) cout << "Skipped "<<lineCount<<" bad edges."<<endl;

  representation->setEdgeCount(edgeCount);

  cout << "Created " << rString << " from " << fileName << " ("<< (GetTimeMs64() - start) << "ms)"<< endl;

  input.close();
}

Graph::~Graph() {
  delete representation;
}

#define LIBGRAPH_FILE_OUTPUT false
void Graph::dump() {
  #if LIBGRAPH_FILE_OUTPUT
    ofstream file;
    file.open("dump.txt");
    ofstream *out = &file;
  #else
    ostream *out = &cout;
  #endif

  auto start = GetTimeMs64();
  *out << "Vertex Count " << representation->getVertexCount() << endl;
  *out << "Edge Count " << representation->getEdgeCount() << endl;

  vector<int> degrees(representation->getVertexCount());
  int maxDegree=0, minDegree=representation->getVertexCount();

  int a = 0, b = 0, c = 0;
  for(int i=1; i<=representation->getVertexCount(); i++){
    int d = representation->getDegree(i);
    if(d>maxDegree) maxDegree = d;
    if(d<minDegree) minDegree = d;
    degrees[i-1] = d;
  }

  sort(degrees.begin(), degrees.end());

  *out << "Min Degree " << minDegree << endl;
  *out << "Max Degree " << maxDegree << endl;

  int avgDegree = (representation->getEdgeCount()*2)/representation->getVertexCount();
  *out << "Avg Degree " << avgDegree << endl;

  int midDegree;
  if(representation->getVertexCount()%2){ //ODD
    midDegree = degrees[(representation->getVertexCount()/2) + 1];
  }else{ //EVEN
    int i = representation->getVertexCount()/2;
    midDegree = (degrees[i] + degrees[i+1])/2;
  }

  *out << "Med Degree " << midDegree << endl;

  list<list<int>*> cc;
  representation->getConnectedComponents(cc);

  cc.sort( []( list<int> *a, list<int> *b ) { return a->size() > b->size(); } );

  *out << "Connected Components: " << cc.size() << endl;

  unsigned int cIndex = 0;
  for(auto component: cc){
    *out << "Component " << ++cIndex << ", size: " << component->size()<< ", vertices:";

    #define LIBGRAPH_VERTS_PER_LINE 10

//    int vCount = 0;
//    for(auto v: *component){
//      if(vCount % LIBGRAPH_VERTS_PER_LINE == 0) *out << endl;
//      vCount += 1;
//      *out << setw(7) << left << v <<" ";
//    }

    *out << endl;

    delete component;
  }

//  generateMinimunSpanningTree(0);

  *out << "Dumped (" << (GetTimeMs64() - start) << "ms)" << endl;
  #if LIBGRAPH_FILE_OUTPUT
    out->close();
  #endif
}

void Graph::generateSearchTree(int vertex, Graph::SearchType searchType) {
  #if LIBGRAPH_FILE_OUTPUT
    ofstream file;
      file.open("searchTree.txt");
      ofstream *out = &file;
  #else
    ostream *out = &cout;
  #endif

  if(!representation->isValidVertex(vertex)){
    *out<<"Invalid Vertex"<<endl;
    return;
  }

  vector<int> levels(getVertexCount());

  for(int i = 0; i< getVertexCount(); i++) levels[i] = 0;

  auto addLeaf = [this, &levels, out](int child, int parent){
    if(parent == 0) *out << child << " " << parent << " 0" << endl;
    if(!representation->isValidVertex(child) || !representation->isValidVertex(parent)) return;
    *out << child << " " << parent << " ";
    parent -= 1; child -= 1;

    levels[child] = levels[parent] + 1;

    *out << levels[child] << endl;
  };

  vector<bool> visited(getVertexCount());

  switch(searchType){
    case SearchType::BFS:
      representation->doBfs(vertex, visited, addLeaf);
      break;
    case SearchType::DFS:
      representation->doDfs(vertex, visited, addLeaf);
      break;
  }
 #if LIBGRAPH_FILE_OUTPUT
    out->close();
  #endif

}

int Graph::doSearch(int root, int target, SearchType type){
  int tParent = -1;
  auto checkResult = [target, &tParent](int child, int parent){
    if(child == target) tParent = parent;
  };

  vector<bool> visited(getVertexCount());

  switch(type){
    case SearchType::BFS:
      representation->doBfs(root, visited, checkResult);
      break;
    case SearchType::DFS:
      representation->doDfs(root, visited, checkResult);
      break;
  }

  return tParent;
};

int Graph::getDiameter(){

  int maxLevel = 0;
  pair<int, int> estrangedPair;
  vector<int> levels(getVertexCount());
  vector<bool> visited(getVertexCount());

  for(int root = 1; root <= getVertexCount(); root++){
    for(int j = 0; j< getVertexCount(); j++) levels[j] = 0;
    for(int j = 0; j< getVertexCount(); j++) visited[j] = false;

    auto getLevel = [this, root, &levels, &maxLevel, &estrangedPair](int child, int parent){
      if(!representation->isValidVertex(child) || !representation->isValidVertex(parent)) return;
      int lvl = levels[parent-1] + 1;
      levels[child-1] = lvl;
      if(lvl>maxLevel){
        maxLevel = lvl;
        estrangedPair.first = root;
        estrangedPair.second = child;
      }
    };

    representation->doBfs(root, visited, getLevel);
    if(root == 1) for(int j = 0;j<getVertexCount();j++) if(!visited[j]) return -1;

  }
  cout << "from "<<estrangedPair.first<<" to "<<estrangedPair.second<<endl;
  return maxLevel;
};

int Graph::generateMinimumSpanningTree(int v) {
  #if LIBGRAPH_FILE_OUTPUT
    ofstream file;
        file.open("minimumSpanningTree.txt");
        ofstream *out = &file;
  #else
    ostream *out = &cout;
  #endif

   int *daddy;
   float *weightList;
   float totalWeight = 0;

   if(!representation->isValidVertex(v)){
      #if LIBGRAPH_FILE_OUTPUT
           out->close();
      #endif
     return -1;
   }

   representation->doPrim(v, weightList, daddy);
   *out << representation->getVertexCount() << endl;

   for(int i=0; i < representation->getVertexCount(); i++){
     if(i+1 != daddy[i]){
       *out << i+1 << " " << daddy[i] << " " << weightList[i] << endl;
     }
     totalWeight += weightList[i];
   }
  cout << "total weight " << totalWeight << endl;
  #if LIBGRAPH_FILE_OUTPUT
  out->close();
  #endif
  return 0;
}

int Graph::getEccentricity(int v) {
  float* distList;
  int* daddy;
  float ecc;

  if(!representation->isValidVertex(v))
    return -1;

  representation->doDijkstra(v, distList, daddy, ecc);
  return ecc;
}

int Graph::getPaths(int v1, int v2, float *&distList, int *&daddy) {
  float dummy;

  if(!representation->isValidVertex(v1) || !representation->isValidVertex(v2))
    return -1;

  representation->doDijkstra(v1, distList, daddy, dummy);
  return 0;
}


// Internal Classes
bool Graph::Representation::isValidVertex(int v) {
  bool c = (v>0 && v<= vertexCount);
  return c;
}

void Graph::Representation::doDfs(int root, vector<bool> &visited, function<void (int child, int parent)> hook) {
  stack<pair<int, int>> s;
  s.push(pair<int,int>(root, 0));

  while(!s.empty()){
    pair<int, int> v = s.top();
    s.pop();
    if(!visited[v.first-1]){
      visited[v.first - 1] = true;
      hook(v.first , v.second);

      list<int> neighbours;
      getNeighbours(v.first, neighbours);

      for(auto n: neighbours) s.push(pair<int, int>(n, v.first));
    }
  }
}

void Graph::Representation::doBfs(int root, vector<bool> &visited, function<void (int child, int parent)> hook) {
  queue<int> s;
  s.push(root);
  visited[root-1] = true;
  hook(root, 0);

  while(!s.empty()){
    int v = s.front();
    s.pop();

    list<int> neighbours;
    getNeighbours(v, neighbours);

    for(auto n: neighbours){
      if(!visited[n-1]){
        visited[n-1] = true;
        s.push(n);
        hook(n, v);
      }
    }
  }
}

void Graph::Representation::getConnectedComponents(list<list<int>*> &connectedComponents) {
  vector<bool> visited(getVertexCount());

  for(int i = 0; i<getVertexCount(); i++) visited[i] = false;

  for(int i = 1; i<=getVertexCount(); i++){
    if(!visited[i-1]){
      auto *l = new list<int>;
      connectedComponents.push_front(l);
      doDfs(i, visited, [l](int c, int p){l->push_front(c);});
    }
  }
}

// Matrix
Graph::AdjacencyMatrix::AdjacencyMatrix(unsigned int vertexCount) {
  this->vertexCount = vertexCount;

  // We store the matrix on a flat array to optimize memory access.
  // Matrices imply more mallocs which imply heap fragmentation.
  adjacencies.reserve(vertexCount * vertexCount);

  for (int i=0; i<vertexCount * vertexCount; i++) adjacencies[i] = false;

}

Graph::AdjacencyMatrix::~AdjacencyMatrix() {
}

// We need this to access our flat matrix
// Also for some reason the indexes start at one so we have to deal with that
int Graph::AdjacencyMatrix::calc1DIndex(int v1, int v2) {
  return (v1 - 1)*vertexCount + (v2 - 1);
}

bool Graph::AdjacencyMatrix::getAdjacency(int v1, int v2) {
  if(!isValidVertex(v1) || !isValidVertex(v2)) return false;

  return adjacencies[calc1DIndex(v1, v2)];
}

void Graph::AdjacencyMatrix::addAdjacency(int v1, int v2, float w) {
  if(!isValidVertex(v1) || !isValidVertex(v2)) return;

  adjacencies[calc1DIndex(v1, v2)] = true;
}

unsigned int Graph::AdjacencyMatrix::getDegree(int vertex){
  if(!isValidVertex(vertex))return 0;

  unsigned int degree = 0;
  for(int i =1; i<= getVertexCount(); i++){
    if(getAdjacency(vertex, i)) degree++;
  }
  return degree;
}

void Graph::AdjacencyMatrix::getNeighbours(int vertex, list<int> &neighbours) {
  if(!isValidVertex(vertex))return;

  for(int i =1; i<= getVertexCount(); i++){
    if(getAdjacency(vertex, i)) neighbours.push_front(i);
  }
}


// List
Graph::AdjacencyList::AdjacencyList(unsigned int vertexCount) {
  this->vertexCount = vertexCount;
  adjacencies = new list<int> *[vertexCount];

  for (int i=0; i<vertexCount; i++) adjacencies[i] = new list<int>;
}

Graph::AdjacencyList::~AdjacencyList() {
  for (int i=0; i<vertexCount; i++) delete adjacencies[i];
  delete[] adjacencies;
}


bool Graph::AdjacencyList::getAdjacency(int v1, int v2) {
  if(!isValidVertex(v1) || !isValidVertex(v2)) return false;

  list<int> *v1Neighbours = adjacencies[v1 - 1];
  bool found = false;

  for (int &v1Neighbour : *v1Neighbours) {
    if(v1Neighbour == v2){
      found = true;
      break;
    }
  }

  return found;
}

void Graph::AdjacencyList::addAdjacency(int v1, int v2, float w) {
  if(!isValidVertex(v1) || !isValidVertex(v2)) return;
  list<int> *v1Neighbours = adjacencies[ v1 - 1];
  v1Neighbours->push_front(v2);
}

unsigned int Graph::AdjacencyList::getDegree(int vertex) {
  return isValidVertex(vertex)?adjacencies[vertex - 1]->size():0;
}

void Graph::AdjacencyList::getNeighbours(int vertex, list<int> &neighbours) {
    if (!isValidVertex(vertex)) return;
    list<int> *a = adjacencies[vertex - 1];

    for (int n: *a) {
        neighbours.push_front(n);
    }
}


// Weighted List
Graph::WeightedAdjacencyList::WeightedAdjacencyList(unsigned int vertexCount) {
    this->vertexCount = vertexCount;
    adjacencies = new list<pair<int,float>> *[vertexCount];

    for (int i=0; i<vertexCount; i++) adjacencies[i] = new list<pair<int,float>>;
}

Graph::WeightedAdjacencyList::~WeightedAdjacencyList() {
    for (int i=0; i<vertexCount; i++) delete adjacencies[i];
    delete[] adjacencies;
}

bool Graph::WeightedAdjacencyList::getAdjacency(int v1, int v2) {
    if(!isValidVertex(v1) || !isValidVertex(v2)) return false;

    list<pair<int,float>> *v1Neighbours = adjacencies[v1 - 1];
    bool found = false;

    for (pair<int,float> &v1Neighbour : *v1Neighbours) {
        if(v1Neighbour.first == v2){
            found = true;
            break;
        }
    }

    return found;
}

float Graph::WeightedAdjacencyList::getAdjacencyWeight(int v1, int v2) {
    if(!isValidVertex(v1) || !isValidVertex(v2)) return false;

    list<pair<int,float>> *v1Neighbours = adjacencies[v1 - 1];
    int found = 0;

    for (pair<int,float> &v1Neighbour : *v1Neighbours) {
        if(v1Neighbour.first == v2){
            found = v1Neighbour.second;
            break;
        }
    }

    return found;
}

void Graph::WeightedAdjacencyList::addAdjacency(int v1, int v2, float w) {
    if(!isValidVertex(v1) || !isValidVertex(v2)) return;
    list<pair<int,float>> *v1Neighbours = adjacencies[ v1 - 1];
    v1Neighbours->push_front(make_pair(v2, w));
}

unsigned int Graph::WeightedAdjacencyList::getDegree(int vertex) {
    return isValidVertex(vertex)?adjacencies[vertex - 1]->size():0;
}

void Graph::WeightedAdjacencyList::getNeighbours(int vertex, list<int> &neighbours) {
    if (!isValidVertex(vertex)) return;
    list<pair<int,float>> *a = adjacencies[vertex - 1];

    for (pair<int,float> n: *a) {
        neighbours.push_front(n.first);
    }
}

void Graph::WeightedAdjacencyList::getWeightedNeighbours(int vertex, list<pair<int,float>> &neighbours) {
  if (!isValidVertex(vertex)) return;
  list<pair<int,float>> *a = adjacencies[vertex - 1];

  for (pair<int,float> n: *a) {
    neighbours.push_front(n);
  }
}

//Receives v as the starting point. Returns a list of distances from i (list index) to v, and a list of i's parent
//TODO possivelmente da p juntar dijkstra e prim em uma func só
bool Graph::WeightedAdjacencyList::doDijkstra(int v, float *&distList, int *&daddy, float &eccentricity){
  priority_queue <pair<float, int>, vector<pair<float, int>>, greater<pair<float,int>>> distHeap;
  distList = new float[vertexCount];
  daddy = new int[vertexCount];
  bool explored[vertexCount];
  float ecc = 0;

  for(int i=0; i < vertexCount; i++){
    distList[i] = 2147483647;
    daddy[i] = -1;
    explored[i] = false;
  }
  distList[v-1] = 0;
  daddy[v-1] = v;
  distHeap.push(make_pair(0, v));

  //TODO não ótimo, acaba antes da heap ficar vazia?
  while(!distHeap.empty()){
    int u = distHeap.top().second;

    if(ecc < distHeap.top().first && !explored[u-1]){
      ecc = distHeap.top().first;
    }

    distHeap.pop();
    explored[u-1] = true;

    list<pair<int,float>> *neighbours = adjacencies[u-1];
    for(pair<int,float> w: *neighbours){
      //w.first is the vertex, w.second is the weight from u to w
      if(!explored[w.first-1]) {
        if (w.second < 0) {
          cout << "Negative weight not supported by Dijkstra algorithmn" << endl;
          return false;
        }
        if (distList[w.first - 1] > distList[u - 1] + w.second) {
          distList[w.first - 1] = distList[u - 1] + w.second;
          distHeap.push(make_pair(distList[w.first - 1], w.first));
          daddy[w.first - 1] = u;

        }
      }
    }
  }
  eccentricity = ecc;
  return true;
}

void Graph::WeightedAdjacencyList::doPrim(int v, float *&costList, int *&daddy) {
  priority_queue <pair<float, int>, vector<pair<float, int>>, greater<pair<float,int>>> costHeap;
  costList = new float[vertexCount];
  daddy = new int [vertexCount];
  bool explored[vertexCount];

  for(int i=0; i < vertexCount; i++){
    costList[i] = 2147483647;
    daddy[i] = -1;
    explored[i] = false;
  }

  costList[v-1] = 0;
  costHeap.push(make_pair(0, v));
  daddy[v-1] = v;

  //TODO não ótimo, acaba antes da heap ficar vazia?
  while(!costHeap.empty()){
    int u = costHeap.top().second;
    costHeap.pop();
    explored[u-1] = true;

    list<pair<int,float>> *neighbours = adjacencies[u-1];
    for(pair<int,float> w: *neighbours) {
      if(!explored[w.first-1]) {
        if (costList[w.first - 1] > w.second) {
          costList[w.first - 1] = w.second;
          costHeap.push(make_pair(w.second, w.first));
          daddy[w.first - 1] = u;
        }
      }
    }
  }

//  cout << "MST" << endl;
//  for(int i=0; i < vertexCount; i++){
//    cout << i+1 << " - " << daddy[i] << endl;
//  }
}


