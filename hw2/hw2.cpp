// O. Kürşat Karayılan
// 150140011
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

#define INF 999
int backsum;

class Graph
{
private:
	int **adjMatrix;
	int numVertices;
	bool *visited;		//visited and unvisited list for dijkstra 
	int *distance;		//stores distance values from src node
	int *prevVer;		//stores previous vertex
	int jh, jd, lh, ld;	// hotels and destinations
	int sum; 		// total time spent
	

public:
	vector<int> pathVec;
	vector<int> distVec;
	//Creating adjMatrix and setting all edges to INF initially. 
	Graph(int numVertices)		
	{
		this->numVertices = numVertices;
		adjMatrix = new int *[numVertices];
		for(int i=0; i<numVertices; i++)
		{
			adjMatrix[i] = new int[numVertices];
			for(int j=0; j<numVertices; j++)
			{
				adjMatrix[i][j] = INF;
			}
		}
		visited = new bool[numVertices];
		distance = new int[numVertices];
		prevVer = new int[numVertices];
		sum = 0;
	}
	//Adding edges i to j with weight.
	void addEdge(int i, int j, int weight)		
	{
		adjMatrix[i][j] = weight;
	}

	void set(int jh, int jd, int lh, int ld)
	{
		this->jh = jh;
		this->jd = jd;
		this->lh = lh;
		this->ld = ld;
	}

    void path(int src, char person, bool returning, bool correction=0)
	{
		if(returning)	//return back
		{
			cout<<endl;
			cout << "-- returning -- " << endl;
			if(!correction)			//correction works when looking for new paths. So no adding 30 min again.
			{
				sum = sum + 30;		//waited 30 min
				backsum = sum;
			}
			else{
				sum = backsum;
			}
			pathVec.clear(); 	//clearing pathVec and destVec vector to find path from destination to hotel
			distVec.clear();
			if(person == 'j')
			{
				int node = src;
				pathVec.insert(pathVec.begin(), src);
				while(node!=jd)
				{
					distVec.insert(distVec.begin(), adjMatrix[prevVer[node]][node]);
					node = prevVer[node];
					pathVec.insert(pathVec.begin(), node);
				}
				distVec.insert(distVec.begin(), 0);
				cout << "Joseph: " << endl;
			}
			else
			{
				int node = src;
				pathVec.insert(pathVec.begin(), src);
				while(node!=ld)
				{
					distVec.insert(distVec.begin(), adjMatrix[prevVer[node]][node]);
					node = prevVer[node];
					pathVec.insert(pathVec.begin(), node);
				}
				distVec.insert(distVec.begin(), 0);
				cout << "Lucy: " << endl;
			}
			for(int i=0; i<pathVec.size(); i++)
			{
				sum = sum + distVec[i];
				distVec[i] = sum;
				cout << "Node: " << pathVec[i] << " " << "Time: " << distVec[i] << endl;
			}
		}

		else if(person == 'j')
		{
			if(correction)
			{
				sum=0;
				pathVec.clear(); 	
				distVec.clear();
			}
			int node = src;
			pathVec.insert(pathVec.begin(), src);
			while(node!=jh)
			{
				distVec.insert(distVec.begin(), adjMatrix[prevVer[node]][node]);
				node = prevVer[node];
				pathVec.insert(pathVec.begin(), node);
			}

			distVec.insert(distVec.begin(), 0);
			cout << "Joseph: " << endl;
			for(int i=0; i<pathVec.size(); i++)
			{
				sum = sum + distVec[i];
				distVec[i] = sum;
				cout << "Node: " << pathVec[i] << " " << "Time: " << distVec[i] << endl;
			}
		}
		else
		{
			if(correction)
			{
				sum=0;
				pathVec.clear(); 	
				distVec.clear();
			}
			int node = src;
			pathVec.insert(pathVec.begin(), src);
			while(node!=lh)
			{
				distVec.insert(distVec.begin(), adjMatrix[prevVer[node]][node]);
				node = prevVer[node];
				pathVec.insert(pathVec.begin(), node);
			}

			distVec.insert(distVec.begin(), 0);
			cout << "Lucy: " << endl;
			for(int i=0; i<pathVec.size(); i++)
			{
				sum = sum + distVec[i];
				distVec[i] = sum;
				cout << "Node: " << pathVec[i] << " " << "Time: " << distVec[i] << endl;
			}
		}
	}

    int dijkstra(int src, int intersect=0)
    {
    	int tempMatrix[numVertices][numVertices];
    	for(int i=0; i<numVertices; i++)
    	{
    		for(int j=0; j<numVertices; j++)
    		{
    			tempMatrix[i][j] = adjMatrix[i][j];
    		}
    	}
    	if(intersect)
    	{		//If intersect found. I make that edge between prev node INF temporarily
    		tempMatrix[prevVer[intersect]][intersect] = INF;
    	}
    	//Initially no vertex visited
    	for(int i=0; i<numVertices; i++)
    	{
    		visited[i] = 0;
    	}

    	for(int i=0; i<numVertices; i++)
    		distance[i] = INF;
    	distance[src] = 0;


    	//visit unvisited smallest vertex
    	for(int t=0; t<numVertices; t++)
    	{

	    	int min=INF;
	    	int tovisit;
	    	for(int i=0; i<numVertices; i++)
	    	{
	    		if(distance[i]<min && visited[i]==0)
	    		{
	    			tovisit = i;
	    			min = distance[i];
	    		}

	    	}

	    	for(int i=0; i<numVertices; i++)
	    	{
	    		if(visited[i])
	    			continue;

	    		if((((distance[tovisit] + tempMatrix[tovisit][i]) < distance[i]) && (tempMatrix[tovisit][i] !=0)))
	    		{
	    			distance[i] = distance[tovisit] + tempMatrix[tovisit][i];
	    			prevVer[i] = tovisit;
	    		}
	    	}
	    	visited[tovisit] = 1;
    	}
    	if(distance[lh]==INF or distance[jh]==INF or distance[ld]==INF or distance[jd]==INF)	//check impossible cases
    	{
    		return 0;
    	}
    	else
    	{
    		return 1;
    	}		

    }
    //Delete operations
	~Graph() 
	{
	    for(int i=0; i<numVertices; i++)
	    {
	    	delete[] adjMatrix[i];
	    }
	    delete[] adjMatrix;
	    delete[] visited;
	    delete[] distance;
    }
};

int check_intersection(Graph &x1, Graph &x2)
{
	int smaller;
	vector<int>::iterator it;
    int sr; //Element to be searched
	if(x1.pathVec.size() <= x2.pathVec.size())		//checking which person's path is shorter
	{
		smaller = x1.pathVec.size();
		for(int i=0; i<smaller; i++)
		{
			sr = x1.pathVec[i];
			it = std::find(x2.pathVec.begin(), x2.pathVec.end(), sr); 
			if(it != x2.pathVec.end())			//element found 
			{
				if(x1.distVec[i] == x2.distVec[it-x2.pathVec.begin()])
				{
					cout << "There is intersection found at node:" << x1.pathVec[i] << endl;
					return x1.pathVec[i];
				}
			}
		}
	}
	else
	{
		smaller = x2.pathVec.size();
		for(int i=0; i<smaller; i++)
		{
			sr = x2.pathVec[i];
			it = std::find(x1.pathVec.begin(), x1.pathVec.end(), sr); 
			if(it != x1.pathVec.end())			//element found 
			{
				if(x2.distVec[i] == x1.distVec[it-x1.pathVec.begin()])
				{
					cout << "There is intersection found at node:" << x2.pathVec[i] << endl;
					return x2.pathVec[i];
				}
			}
		}
	}
	return false;
}

int main(int argc, char const *argv[])
{
	string file = argv[1]; 

	ifstream inFile;
	inFile.open(file);
    int vertices;
    inFile >> vertices;
    Graph g(vertices);
    Graph p(vertices);
    int jh,jd,lh,ld;
    inFile >> jh >> jd >> lh >> ld;		//Joseph and Lucy hotel, destination nodes.
    g.set(jh, jd, lh, ld);
    p.set(jh, jd, lh, ld);
    while(inFile)						//reading other nodes
    {
    	for(int i=0; i<3; i++)
    	{
    		int x1, x2, x3;
    		inFile >> x1 >> x2 >> x3;
    		g.addEdge(x1, x2, x3);
    		p.addEdge(x1, x2, x3);
    	}
    }
   	int control;
    inFile.close();
	g.dijkstra(jh);			//Find shortest path from j hotel to other nodes
	g.path(jd, 'j', 0);		//Print path J hotel to destination. 0 means no returning case
	p.dijkstra(lh);			//Shortest paths from L hotel
	p.path(ld, 'l', 0);		//Print path L hotel to destination.
	control = check_intersection(g, p);
	if(control != false)				//checking intersection while they are going to destinations
	{
		if(g.dijkstra(jh, control))
		{
			cout << "New path found for Joseph:" << endl;
			g.path(jd, 'j', 0, 1);
		}
		else if(p.dijkstra(lh, control))
		{
			cout << "New path found for Lucy:" << endl;
			p.path(ld, 'l', 0, 1);
		}
		else
		{
			cout << "No suitable path found!" << endl;
		}
	}

	g.dijkstra(jd);			//They are returning
	g.path(jh, 'j', 1);	
	p.dijkstra(ld);
	p.path(lh, 'l', 1);
	control = check_intersection(g, p);
	if(control != false)
	{
		if(p.dijkstra(ld, control))
		{
			cout << "New path found for Lucy:" << endl;
			p.path(lh, 'l', 1, 1);
		}
		else if(g.dijkstra(jd, control))
		{
			cout << "New path found for Joseph:" << endl;
			g.path(jh, 'j', 1, 1);
		}
		else
		{
			cout << "No suitable path found!" << endl;
		}
	}

	return 0;
}
