//O.Kürşat Karayılan
//150140011

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <climits>
using namespace std;

struct Edge{
    int sourceNode;
    int destinationNode;
    int weight;
};

class Graph{
    int numberOfNodes;
    int numberOfEdges;
    vector<Edge> edges;
    
public:
    void setNumberOfNodes(int n){
        numberOfNodes = n;
    }
    int getNumberOfNodes(){
        return numberOfNodes;
    }
    void setNumberOfEdges(int n){
        numberOfEdges = n;
    }
    int getNumberOfEdges(){
        return numberOfEdges;
    }
    
    void AddEdge(int sourceNode, int destinationNode, int weight){
        Edge e = Edge();
        e.sourceNode = sourceNode;
        e.destinationNode = destinationNode;
        e.weight = weight;
        
        edges.push_back(e);
    }
    vector<Edge> getAllEdges(){
        return edges;
    }
    
    vector<Edge> getOutgoingEdges(int src){
        vector<Edge> outgoingEdges;
        
        for (Edge e : edges){
            if(e.sourceNode == src){
                outgoingEdges.push_back(e);
            }
        }
        
        return outgoingEdges;
    }
    
    int getWeightOfEdge(int src, int dst){
       int weight = 0;
       for (Edge e : edges){
           if(e.sourceNode == src && e.destinationNode == dst){
               weight = e.weight;
           }
        }
       
       return weight;
    }
    
    void printGraph(){
        cout << "************* Graph data *************" << endl;
        cout << "Number of total edges: " << numberOfEdges << endl;
        cout << "Number of total nodes: " << numberOfNodes << endl << endl;
        
        cout << "Source node --> Destination node : Edge weigth" << endl;
        for (std::vector<Edge>::const_iterator i = edges.begin(); i != edges.end(); ++i){
            std::cout << i->sourceNode << " --> " ;
            std::cout << i->destinationNode << " : " ;
            std::cout << setw(2) << i->weight << endl ;
        }
        cout << "**************************************" << endl << endl;
    }
};

class BellmanFord{
    Graph G;
    int sourceNode;
    int destinationNode;
    int** M;
    bool graphContainsNegativeWeightCycle;
        
public:
    void setGraph(Graph g){
        G = g;
    }
    Graph getGraph(){
        return G;
    }
    void setSourceNode(int s){
        sourceNode = s;
    }
    int getSourceNode(){
        return sourceNode;
    }
    void setDestinationNode(int t){
        destinationNode = t;
    }
    int getDestinationNode(){
        return destinationNode;
    }
    bool buildDPTable(){
        int n = G.getNumberOfNodes();
        M = new int*[n];
        for(int i=0; i<n; i++)
        {
            M[i] = new int[n];
        }
        for(int i=0; i<n; i++)
        {
            M[i][destinationNode] = 0;
        }
        //Setting INF
        for(int i=0; i<n; i++)
        {
        	if(i==destinationNode)
        	{
        		;
        	}
            else
            {
            	M[0][i] = INT_MAX;
            }
        }
        //First iteration
        for(int j=0; j<n; j++)
        {
        	if(j==destinationNode)
        	{
        		;
        	}	
            else if(G.getWeightOfEdge(j, destinationNode)!=0)
            {
                M[1][j] = G.getWeightOfEdge(j, destinationNode);
            }
            else
            {
                M[1][j] = INT_MAX;
            }
        }
        //Other iterations 
        for(int i=2; i<=n-1; i++)
        {
            for(int j=0; j<n; j++)
            {
            	if(j==destinationNode)
            	{
            		;
            	}
            	else
            	{
            		vector<Edge> outgoing = G.getOutgoingEdges(j);
                	int min = INT_MAX;
                	for(Edge e: outgoing)
                	{
	                	if(M[i-1][e.destinationNode] == INT_MAX)
	                	{
	                		M[i][j] = M[i-1][j];
	                	}
	                    else if(e.weight + M[i-1][e.destinationNode] < min)
	                    {
	                        min = e.weight + M[i-1][e.destinationNode];
	                    }
                	}
                	M[i][j] = min;
            	}      
            }
        }

        //Doing one more iteration to check negative cycle
      	int temp[n];
        for(int j=0; j<n; j++)
        {
        	if(j==destinationNode)
        	{
        		temp[j] = 0;
        	}
        	else
        	{
	        	vector<Edge> outgoing = G.getOutgoingEdges(j);
	            int min = INT_MAX;
	            for(Edge e: outgoing)
	            {
	            	if(M[n-1][e.destinationNode] == INT_MAX)
	            	{
	            		temp[j] = M[n-1][j];
	            	}
	                else if(e.weight + M[n-1][e.destinationNode] < min)
	                {
	                    min = e.weight + M[n-1][e.destinationNode];
	                }
	            }
	            temp[j] = min;
	        }
                  
        }
        //Comparing one more iterated one with old one to check negative cycle. We should get same result if not negative cycle
        for(int i=0; i<n; i++)
        {
        	if(temp[i]!=M[n-1][i])
        	{
        		graphContainsNegativeWeightCycle=false;
        		return false;
        	}
        	
        }    
        return true;
        
    }
    
    void printDPTable(){
        if(!graphContainsNegativeWeightCycle){
            cout << endl << "Dynamic programming table: (shortest paths from each node to destination node): " << endl << endl;
            
            int numberOfNodes = G.getNumberOfNodes();
            
            for(int i = 0; i <= numberOfNodes; i++){
                for(int j = 0; j <= numberOfNodes; j++){
                    if(i == 0 && j == 0)
                        cout << "\t";
                    else if(i != 0 && j == 0)
                        cout << setw(10) << "Iteration-" << i - 1;
                    else if(i == 0 && j != 0)
                        cout << "\t" << "Node-" << j - 1 << " ";
                    else{
                        if(M[i - 1][j - 1] == INT_MAX)
                            cout << "\t " << "\u221E";
                        else
                            cout << "\t" << setw(2) << M[i - 1][j - 1];
                    }
                }
                cout << endl;
            }
            cout << endl;
        }
    }
    
    void printShortestPathBetweenSourceAndDestination(){
        if(M[G.getNumberOfNodes()-1][sourceNode] == INT_MAX)
        {
        	cout << "No path found from " << sourceNode << " to " << destinationNode << endl;
        }

    	else
    	{
        	cout << "Shortest path value from " << sourceNode << " to " << destinationNode << ": " << M[G.getNumberOfNodes()-1][sourceNode] << endl;
    		cout << "path: ";

    		vector<int> path;
    		path.push_back(sourceNode);
    		vector<int> visited(G.getNumberOfNodes(), 0);
        	int path_node = sourceNode;
        	visited[sourceNode] = 1;
        	visited[destinationNode] = 1;
        	int exitwhile = 1;
        	int totalpath = M[G.getNumberOfNodes()-1][sourceNode];
        	int sumpath = 0;
        	while(path_node!=destinationNode)
        	{
        		int min = INT_MAX;
	        	vector<Edge> outgoing = G.getOutgoingEdges(path_node);
	        	Edge eklenecek;
	        	for(Edge e: outgoing)
	        	{
	        		if(e.destinationNode == destinationNode)
	        		{
	        			if(sumpath+e.weight==totalpath)
	        			{
		        			path_node = destinationNode;
		        			path.push_back(destinationNode);
		        			exitwhile = 0;	
	        			}
	        			else
	        			{
		        			vector<Edge> temp = G.getOutgoingEdges(e.sourceNode); 
		        			int flag=0;
		        			for(Edge k: temp)
		        			{
		        				if(k.destinationNode == destinationNode)
		        				{
		        					; //pass
		        				}
		        				else if(!visited[k.destinationNode])
		        				{
		        					flag=1;
		        				}
		  	      			}
		  	      			if(!flag)
		  	      			{
		  	      				if(totalpath!=sumpath+e.weight)
		  	      				{
		  	      					cout<<"this is wrong path please try different node :(" << endl;
                                    path.push_back(destinationNode);
                                    path_node = destinationNode;
                                    exitwhile = 0;  
		  	      				}
		  	      				else
		  	      				{
			  	      				path.push_back(destinationNode);
			  	      				path_node = destinationNode;
			  	      				exitwhile = 0;	
		  	      				}
		  	      			}
	        			}	
	        			
	        		}
	            	else if(M[G.getNumberOfNodes()-1][e.destinationNode] == INT_MAX)
	            	{
	            		; //pass
	            	}
	            	else if(visited[e.destinationNode])
	            	{
	            		; //pass
	            	}
	                else if(M[G.getNumberOfNodes()-1][e.destinationNode] < min)
	                {	 
	                    min = M[G.getNumberOfNodes()-1][e.destinationNode];
	                	eklenecek = e;
	                }
	        	}

	        	if(exitwhile)
	        	{
		        	visited[eklenecek.destinationNode] = 1;
		            path.push_back(eklenecek.destinationNode);
		            sumpath = eklenecek.weight + sumpath;
		        	path_node = eklenecek.destinationNode; 
	        	}
	        	if(sumpath == totalpath)
        		{
        			path.push_back(destinationNode);	
        			break;
        		}
        	}
    		
        	
	        //Printing path
	        cout << path[0];
	        for(int i=1; i<path.size(); i++)
	        {
	        	cout << "->" << path[i];
	        }
	        cout << endl;
    	}
    	
    }
};

int main(int argc, char* argv[])
{
    if(argc > 1){
        Graph graph;
        
        //read graph data from file
        string inputFileName = argv[1];
        string line;
        ifstream infile(inputFileName);
        
        getline(infile, line);
        graph.setNumberOfNodes(stoi(line));
        getline(infile, line);
        graph.setNumberOfEdges(stoi(line));

        int src, dst, w;
        while (infile >> src >> dst >> w) {
           graph.AddEdge(src, dst, w);
        }
        
        graph.printGraph();
        
        BellmanFord bf;
        bf.setGraph(graph);
        
        int dstNode;
        int srcNode;
        
        cout << "Enter the destination node: ";
        cin >> dstNode;
        
        bf.setDestinationNode(dstNode);
    
        if(bf.buildDPTable()){
            bf.printDPTable();
            
            cout << "Enter a source node: ";
            cin >> srcNode;
            bf.setSourceNode(srcNode);
            
            bf.printShortestPathBetweenSourceAndDestination();
        }
        else
        {
        	cout << "Graph contaions negative cycle!" << endl;
        }
    }
    else{
        cout << "Error: please provide a test file!" << endl;
    }
    
    return 0;
}
