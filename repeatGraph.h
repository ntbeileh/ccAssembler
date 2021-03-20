//repeat graph things here!
#ifndef REPEATGRAPH_H
#define REPEATGRAPH_H
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <tuple>
#include <stack>
#include <cmath>
#include <cstring>

#include "node.h"
using namespace std;

class repeatGraph
{
  private:

    //suffix, prefix
    vector <Node *> allNodes; //the graph, may need to be changed to unordered_map
    vector <Node *> oneinoneout;   //
    vector <Node *> starts;       //nodes that have no parents
    vector <Node *> ends;
    list<Node *> assemblyNode; // list of best assembly nodes
    list<tuple<Node *, string, pair<int, int>, pair<int, int>>> assemblyEdges; 
  public: 
    repeatGraph();
    repeatGraph(string file, unordered_map<string, string>& reads);   // Create graph from paf file
    string cigarTQ(string target, string query, string cig, int firstStart);
    string cigarQooT(string target, string query, string cig, int firstStart);
    void getOneinOut();                 //this may not be needed but it finds all one in one out nodes
    Node * find(string seq); //finds node given prefix string
    void removeNode(Node * node); // removes node that pointer points to
    vector<list<Node *> > nonbranching();    //find nonbranching paths
    void getStartsandEnds();	//getting starts
    vector<Node *> getAllNodes();
    void consolidatePaths(vector <list<Node *> >& nonbranchingPaths);
    void alternateStarts();

    void generateContigs();

    void resetVisits();

    vector<tuple<list <Node *>, list<tuple<Node *, string, pair<int, int>, pair<int, int>>>, int>> findAssemblies();
    void setAssembly();

    void assembly_to_fasta();

    ~repeatGraph(); //deconstructor

};

//edge class to be inserted

//node read, reverse complement, and edge pointing to next node
  //reversecomplement


//root
//insert

//eulerian cycle

//nonbranching paths

//glue repeats, threshold 


//preprossing
  // consolidating homopolymer reads


#endif
