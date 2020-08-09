// O.Kürşat Karayılan
// 150140011


#include <fstream>
#include <iostream> 
#include <list> 
#include <vector>
#include <string>
#include <ctime>
#include <map>
using namespace std; 
int nodeid = -1;    //total node count
int leafcheck;      //tree level number

struct Pokemon
{
    string specs[4][5];
    int hp;
    int pp;
};

struct Node
{

    int p_hp;
    int p_pp;
    int b_hp;   
    int b_pp;
    string turn;
    float prob;
    int level;
    bool isleaf;
    string action;  //action string
    int id;         //own id
    int mother;     //id of the node created from. motherid
    Node()
    {
        nodeid = nodeid + 1;    //struct constructor. Each new node gets id sequently
        id = nodeid;
    }

};

class Graph 
{ 

public: 

    map<int, vector<Node> > adjList; 
    void addEdge(int id, vector<Node> v);                            //add edges
    void createnode(Node node, Pokemon pikachu, Pokemon blastoise);  //create node
    void bfs(int s);                                                 //bfs algo 
    void dfs(int s);                                                 //dfs algo
    void dfs_helper(int s, bool *visited);
    void path(int s, string pokemon);                                //finding node where pikachu or blastoise hp=0 with lowest level then printing actions.

}; 

void Graph::addEdge(int id, vector<Node> v)
{
	adjList[id] = v;
}

void Graph::createnode(Node node, Pokemon pikachu, Pokemon blastoise)
{

    if((node.p_hp <=0) or (node.b_hp <=0))
    {
        //dont create node, do nothing if pokemons' hp <= 0
    }
	else if(node.turn == "P")  //pikachu's turn
	{
        vector<Node> tempV;
        if(node.id == 0)
        {
            tempV.push_back(node);
        } 
        for(int i=0; i<3; i++)
        {
            if((node.p_pp + stoi(pikachu.specs[i][1])) > 0)       // 0:Attack name
            {                                                     // 1:PP
                Node temp;                                        // 2:Accuracy
                temp.mother = node.id;                            // 3:Damage
                temp.p_hp = node.p_hp;                            
                temp.p_pp = node.p_pp + stoi(pikachu.specs[i][1]);
                temp.b_hp = node.b_hp - stoi(pikachu.specs[i][3]);
                temp.b_pp = node.b_pp;
                temp.turn = "B";
                temp.level = node.level + 1;
                if(temp.level == leafcheck)
                {
                    temp.isleaf = 1;
                    temp.prob = (1.0/3.0) * (stoi(pikachu.specs[i][2])/100.0) * node.prob;
                    temp.action = "Pikachu used " + pikachu.specs[i][0] + ". It's effective.";
                    tempV.push_back(temp);
                }
                else
                {
                    temp.isleaf = 0;
                    temp.prob = (1.0/3.0)* (stoi(pikachu.specs[i][2])/100.0) * node.prob;
                    temp.action = "Pikachu used " +  pikachu.specs[i][0] + ". It's effective.";
                    tempV.push_back(temp);
                    createnode(temp, pikachu, blastoise);
                }
                if(stoi(pikachu.specs[i][2]) != 100)    //Not effective skills
                {
                    Node temp2;
                    temp2.mother = node.id;                          
                    temp2.p_hp = node.p_hp;                            
                    temp2.p_pp = node.p_pp + stoi(pikachu.specs[i][1]);
                    temp2.b_hp = node.b_hp;
                    temp2.b_pp = node.b_pp;
                    temp2.turn = "B";
                    temp2.level = node.level + 1;
                    if(temp2.level == leafcheck)
                    {
                        temp2.isleaf = 1;
                        temp2.prob = (1.0/3.0)*(100 - stoi(pikachu.specs[i][2]))/100.0 * node.prob;
                        temp2.action = "Pikachu used " +  pikachu.specs[i][0] + ". It's not effective.";
                        tempV.push_back(temp2);
                    }
                    else
                    {
                        temp2.isleaf = 0;
                        temp2.prob = (1.0/3.0)*(100 - stoi(pikachu.specs[i][2]))/100.0 * node.prob;
                        temp2.action = "Pikachu used " +  pikachu.specs[i][0] + ". It's not effective.";
                        tempV.push_back(temp2);
                        createnode(temp2, pikachu, blastoise);
                    } 
                }
            }
        }
        addEdge(node.id, tempV);
	}
    else if(node.turn == "B")   //Blastoise's turn
    {
        vector<Node> tempV;
        for(int i=0; i<3; i++)
        {
            if((node.b_pp + stoi(blastoise.specs[i][1])) > 0)     // 0:Attack name
            {                                                     // 1:PP
                Node temp;                                        // 2:Accuracy
                temp.mother = node.id;                            // 3:Damage
                temp.b_hp = node.b_hp;                            
                temp.b_pp = node.b_pp + stoi(blastoise.specs[i][1]);
                temp.p_hp = node.p_hp - stoi(blastoise.specs[i][3]);
                temp.p_pp = node.p_pp;
                temp.turn = "P";
                temp.level = node.level + 1;
                if(temp.level == leafcheck)
                {
                    temp.isleaf = 1;
                    temp.prob = (1.0/3.0) * (stoi(blastoise.specs[i][2])/100.0) * node.prob;
                    temp.action = "Blastoise used " +  blastoise.specs[i][0] + ". It's effective.";
                    tempV.push_back(temp);
                }
                else
                {
                    temp.isleaf = 0;
                    temp.prob = (1.0/3.0)*stoi(blastoise.specs[i][2])/100.0 * node.prob;
                    temp.action = "Blastoise used " +  blastoise.specs[i][0] + ". It's effective.";
                    tempV.push_back(temp);
                    createnode(temp, pikachu, blastoise);
                }
            }
        }
        addEdge(node.id, tempV);
    }
}

void Graph::bfs(int s)
{
    bool *visited = new bool[nodeid];
    for(int i = 0; i<nodeid; i++) visited[i]=false;
    list<int> queue;
    visited[s] = true;
    queue.push_back(s);
    int counter = 0;
    while(!queue.empty())
    {
        s = queue.front();
        queue.pop_front();
        for(int i=0; i<adjList[s].size(); i++)
        {
            if(!visited[adjList[s][i].id])
            {
                visited[adjList[s][i].id] = true;
                queue.push_back(adjList[s][i].id);
            }
        }
    
    } 
}
void Graph::dfs_helper(int s, bool *visited)
{
    visited[s] = true;

    for(int i=0; i<adjList[s].size(); i++)
    {
        if(!visited[adjList[s][i].id])
        {
            dfs_helper(adjList[s][i].id, visited);
        }
    }
}

void Graph::dfs(int s)
{
    bool *visited = new bool[nodeid];
    for(int i=0; i<nodeid; i++) visited[i] = false;
    dfs_helper(s, visited);
}

void Graph::path(int s, string pokemon)
{
    bool *visited = new bool[nodeid];
    for(int i = 0; i<nodeid; i++) visited[i]=false;
    list<int> queue;
    visited[s] = true;
    queue.push_back(s);
    vector<Node> zerolist;
    while(!queue.empty())      //using bfs to find hp=0 nodes for each pokemon. find and add to zerolist vector.
    {
        s = queue.front();
        queue.pop_front();
        for(int i=0; i<adjList[s].size(); i++)
        {   	
            if(!visited[adjList[s][i].id])
            {
                visited[adjList[s][i].id] = true;
                if(pokemon == "blastoise")
                {
                    if(adjList[s][i].p_hp <= 0)
                    {    
                        zerolist.push_back(adjList[s][i]);
                    }
                }
                else
                {
                    if(adjList[s][i].b_hp <= 0)
                    {
                      	zerolist.push_back(adjList[s][i]);
                    }
                }
                queue.push_back(adjList[s][i].id);
            }   
            
        }
    }
    Node lowest;
    lowest.level = zerolist[0].level;
    for(int i=1; i<zerolist.size(); i++)
    {
    	if(zerolist[i].level <= lowest.level)
    	{
            lowest = zerolist[i];
    	}
    }

    list<Node> actionList;                  //Creating actionlist to store necessary nodes through found node.
    if(pokemon=="blastoise")    //checking pokemon name and if 0hp nodes found or not
    {
        int id=lowest.id;
        int mother = lowest.mother;
        actionList.push_front(lowest);
        int flag2 = 0;
        for(int i=0; i<adjList.size(); i++) //Traversing adjList to find action sequence. Make use motherid. 
        {                           //Each node has motherid stores upper node's id in tree.
            for(int j=0; j<adjList[i].size(); j++)
            {
                if(adjList[i][j].id == mother)
                {
                	if(id == 0)
                	{
            			flag2 = 1; //this means we traversed through head node. So we can break for loop.
                	}
                    else
                    {
                    	actionList.push_front(adjList[i][j]);    //found motherid. then push front to list. so we can pop front later on.
                   		id = adjList[i][j].id;
                    	mother = adjList[i][j].mother;
            			i = -1;                        //resetting i so it traverses from beginning again.
                    	break;
                    }
                    
                }
            }
            if(flag2) break;

        }
        cout << "-------------------------" << endl; 
        while(!actionList.empty())      //Printing actions
        {
            Node out = actionList.front();
            actionList.pop_front();
            cout << out.action << "  -  (P:" << out.p_hp << " B:" << out.b_hp << ")" << endl;
        }
        cout << "Level count: " << lowest.level << endl;
        cout << "Probability: " << lowest.prob << endl;
        cout << "-------------------------" << endl; 
    }
    if(pokemon=="pikachu")  //same stuff for the case pikachu win
    {
        int id=lowest.id;
        int mother = lowest.mother;

        actionList.push_front(lowest);
        int flag2 = 0;
        for(int i=0; i<adjList.size(); i++)
        {
            for(int j=0; j<adjList[i].size(); j++)
            {
                if(adjList[i][j].id == mother)
                {
                    if(id == 0)
                    {
                        flag2 = 1;
                    }
                    else
                    {
                        actionList.push_front(adjList[i][j]);
                        id = adjList[i][j].id;
                        mother = adjList[i][j].mother;
                        i = -1;
                        break;
                    }
                }
            }
            if(flag2) break;
        }
        cout << "-------------------------" << endl; 
        while(!actionList.empty())
        {
            Node out = actionList.front();
            actionList.pop_front();
            cout << out.action << "  -  (P:" << out.p_hp << " B:" << out.b_hp << ")" << endl;
        }
        cout << "Level count: " << lowest.level << endl;
        cout << "Probability: " << lowest.prob << endl;
        cout << "-------------------------" << endl; 
    }
    

}

void printlastlayer(Node node)
{
    cout << "P_HP:" << node.p_hp << " P_PP:" << node.p_pp << " B_HP:" << node.b_hp << " B_PP:" << node.b_pp << " PROB:" << node.prob << endl;
}

int main(int argc, const char * argv[]) 
{   
    string part = argv[1]; 
    if(strcmp(argv[1],"part3") == 0){
      	leafcheck = 10; 
    } 
    else 
    {
    	leafcheck = stoi(argv[2]);
    }
    string mode="";
    if(argv[3]!=NULL) mode = argv[3];

    clock_t begin=0, end=0;
    string blank;
    ifstream inFile;
    Pokemon pikachu, blastoise;
    pikachu.hp = 200;
    blastoise.hp = 200;
    pikachu.pp = 100;
    blastoise.pp = 100;
    inFile.open("pikachu.txt");
    while(inFile){
        getline(inFile, blank);
        for(int i=0; i<4; i++){
            for(int j=0;j<4;j++){
                getline(inFile, pikachu.specs[i][j], ',');
            }
            getline(inFile, pikachu.specs[i][4]);
        }
    }
    inFile.close();

    inFile.open("blastoise.txt");
    while(inFile){
        getline(inFile, blank);
        for(int i=0; i<4; i++){
            for(int j=0;j<4;j++){
                getline(inFile, blastoise.specs[i][j], ',');
            }
            getline(inFile, blastoise.specs[i][4]);
        }
    }
    inFile.close();


    Graph game;
    Node head;
    head.p_hp = pikachu.hp;
    head.p_pp = pikachu.pp;
    head.b_hp = blastoise.hp;       //head node init
    head.b_pp = blastoise.pp;
    head.turn = "P";
    head.prob = 1;
    head.level=0;
    head.isleaf = 0;
    head.mother = 0;
    head.action = "Game started";
    game.createnode(head, pikachu, blastoise);
    cout << "Node count:" << nodeid+1 << endl;

    if(part=="part1")
    {
        for(int i=0; i<game.adjList.size(); i++)
        {
            for(int j=0; j<game.adjList[i].size(); j++)
            {
                if(game.adjList[i][j].isleaf == 1)
                {
                    printlastlayer(game.adjList[i][j]);
                }
            }
        }
    }

    if(part=="part2")
    {
        if(mode == "bfs")
        {
            begin = clock();
            game.bfs(0);
            end = clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            cout << "BFS Running time: " << elapsed_secs << " seconds" << endl;   
        }  

        if(mode == "dfs")
        {
            begin = clock();
            game.dfs(0);
            end = clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            cout << "DFS Running time: " << elapsed_secs << " seconds" << endl;   
        }
    }

    if(part=="part3")
    {
    	begin = clock();
    	game.path(0, argv[2]);
    	end = clock();
    	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << "Running time: " << elapsed_secs << " seconds" << endl;  
    }
    

    return 0; 
} 
