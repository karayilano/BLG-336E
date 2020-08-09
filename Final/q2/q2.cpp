#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

class Graph{
    vector <vector<int> > capacity;
    vector <vector<int> > flow;
    int nodeCount;
    vector <int> excess;
    vector <int> height; 
    public:
    void addCapacity(int, int, int);
    void push(int, int);
    void relabel(int);
    void initializePreflow(int);
    int overflowingVertex();
    int findMaxFlow();
    void printCap();
    Graph(int);
    ~Graph();
};

Graph::Graph(int n){
    nodeCount = n;
    capacity.assign(nodeCount, vector<int>(nodeCount, 0));
}

void Graph::addCapacity(int from, int to, int cap){
    capacity[from][to] = cap;
}

int Graph::overflowingVertex(){
    // IMPLEMENT HERE
}

void Graph::initializePreflow(int source){
    height.assign(nodeCount, 0);
    excess.assign(nodeCount, 0);
    height[source] = nodeCount;
    for(int i=0; i<nodeCount; i++)
    {
        for(int j=0; j<nodeCount; j++)
        {
            if(capacity[i][j]!=0)
            {
                flow[i][j] = 0;
            }
        }
    }

    for(int i=0; i<nodeCount; i++)
    {
        if(capacity[source][i]!=0)
        {
            flow[source][i] = capacity[source][i];
            flow[i][source] = -capacity[source][i];
            excess[i] = capacity[source][i];
            excess[source] = excess[source] - capacity[source][i];
        }
    }

}

void Graph::push(int u, int v){
    int excessFlow = excess[u];
    int residualCap = capacity[u][v] - flow[u][v];
    int delta = min(excessFlow, residualCap);
    flow[u][v] = flow[u][v] + delta;
    flow[v][u] = flow[u][v] - delta; 
    excess[u] = excess[u] - delta;
    excess[v] = excess[v] + delta; 
}

void Graph::relabel(int u){
    int minH = -1;
    for(int i=0; i<nodeCount; i++)
    {
        int residualCap = capacity[u][i] - flow[u][i];
        if(capacity[u][i]!=0 and residualCap>0)
        {
            if(min<height[i])
            {
                min = height[i];
            }
        }  
    }
    height[u] = min + 1;
}

int Graph::findMaxFlow(){
    // IMPLEMENT HERE
}

Graph::~Graph(){

}

int main(int argumentNumber, char *arguments[]){
    int nodeCount;
    string filename = arguments[1];
    string tempString;
    int temp, from, to, flow;
    
    ifstream file;
    file.open(filename);
    file >> nodeCount;
    Graph graph(nodeCount);
    file >> tempString >> tempString >> tempString;
    while(file.good()){
        file >> from >> to >> flow;
        graph.addCapacity(from, to, flow);
    }
    graph.initializePreflow(0); // 0th node is source
    cout << "Maximum flow: " << graph.findMaxFlow() << endl; 
    return 0;
}