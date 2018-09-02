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
  }

  int a, b;
  unsigned int lineCount = 1;
  unsigned int edgeCount = 0;
#define LIBGRAPH_BADLINE_THRESHOLD 10
  unsigned badLines = 0;


  while(input >> a >> b){
    lineCount++;
    if(a < 1 || a > vCount || b < 1 || b > vCount ){
      badLines++;
      if(badLines < LIBGRAPH_BADLINE_THRESHOLD) cout << "Bad edge at line "<<lineCount<<". Skipping."<<endl;
      if(badLines == LIBGRAPH_BADLINE_THRESHOLD) cout << "More than "<<LIBGRAPH_BADLINE_THRESHOLD<<" bad lines. Will stop reporting but keep skipping."<<endl;
      continue;
    }
    representation->addAdjacency(a, b);
    representation->addAdjacency(b, a);
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

    int vCount = 0;
    for(auto v: *component){
      if(vCount % LIBGRAPH_VERTS_PER_LINE == 0) *out << endl;
      vCount += 1;
      *out << setw(7) << left << v <<" ";
    }

    *out << endl;

    delete component;
  }



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

void Graph::AdjacencyMatrix::addAdjacency(int v1, int v2) {
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

void Graph::AdjacencyList::addAdjacency(int v1, int v2) {
  if(!isValidVertex(v1) || !isValidVertex(v2)) return;
  list<int> *v1Neighbours = adjacencies[ v1 - 1];
  v1Neighbours->push_front(v2);
}

unsigned int Graph::AdjacencyList::getDegree(int vertex) {
  return isValidVertex(vertex)?adjacencies[vertex - 1]->size():0;
}

void Graph::AdjacencyList::getNeighbours(int vertex, list<int> &neighbours) {
  if(!isValidVertex(vertex)) return;
  list<int> *a = adjacencies[vertex - 1];

  for(int n: *a){
    neighbours.push_front(n);
  }
}
