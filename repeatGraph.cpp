#include "repeatGraph.h"
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

repeatGraph::repeatGraph() {
    return;
}

// Repeat graph constructor
repeatGraph::repeatGraph(string file, unordered_map<string, string> &reads){

    cout << "Constructing repeat graph from paf file..." << endl;

    // make a in stream for the file
    ifstream infile(file);

    // check if file's input is readable
    if (!infile.good())
    {
        cout << "Error, cannot read file: " << file << "\n Exiting Program.\n" << endl;
        return;
    }

    string id, seq;
    string line;
    vector<string> tokens;
    // Read in paf file lines
    while (getline(infile, line).good()) {

        istringstream iss(line);
        string token;
        string queryID, targetID, cigarSeq;
        int queryStart, queryEnd, targetStart, targetEnd;
        int col = 1;
        // Separating tab-deliminated lines
        while (getline(iss, token, '\t')) {

            tokens.push_back(token);
            if (col == 1) {
                queryID = token;
                //cout << queryID << endl;
            }
            else if (col == 3) {
                queryStart = stoi(token);
                //cout << queryStart << endl;
            }
            else if (col == 4) {
                queryEnd = stoi(token);
                //cout << queryEnd << endl;
            }
            else if (col == 6) {
                targetID = token;
                //cout << targetID << endl;
            }
            else if (col == 8) {
                targetStart = stoi(token);
                //cout << targetStart << endl;
            }
            else if (col == 9) {
                targetEnd = stoi(token);
                //cout << targetEnd << endl;
            }
            else if (col == 23) {
                cigarSeq = token.substr(5);
                //cout << cigarSeq << endl;
            }
            col++;
        }

        // If query and target IDs are the same, skip
        if (queryID == targetID) {
            continue;
        }

        // Checking if IDs exist in reads
        auto querySeq = reads.find(queryID);
        auto targetSeq = reads.find(targetID);
        
        // Checking if node exists in graph
        auto queryNode = find(querySeq->second);
        auto targetNode = find(targetSeq->second);
        
        // If queryNode is not in graph, add queryNode
        if (queryNode == NULL) {
            queryNode = new Node(querySeq->first, querySeq->second);
            allNodes.push_back(queryNode); 
        }
        // If targetNode is not in graph, add targetNode
        if (targetNode == NULL) {
            targetNode = new Node(targetSeq->first, targetSeq->second);
            allNodes.push_back(targetNode); 
        }

        // Checking if edge between query and target already exists
        vector<tuple<Node*, string, pair<int, int>, pair<int, int>> > queryNext = (*queryNode).getNext(); // list of query next edges
        vector<tuple<Node*, string, pair<int, int>, pair<int, int>> > queryPrev = (*queryNode).getPrev(); // list of query prev edges
        bool edgeExists = false;
        
        if (queryNext.size() != 0) {
            for (auto edge : queryNext) {
                // If seq of next edge matches target
                if (get<0>(edge)->getSeq() == targetSeq->second) {
                    edgeExists = true;
                }
            }
        }
        if (queryPrev.size() != 0) {
            for (auto edge : queryPrev) {
                // If seq of prev edge matches target
                if (get<0>(edge)->getSeq() == targetSeq->second) {
                    edgeExists = true;
                }
            }
        }
        if (edgeExists) {
            continue;
        }
        
        // Adding edges to graph
        // If target -> query, + strand
        if ((queryStart == 0) && (targetEnd == targetSeq->second.length())) {
            string overlap = cigarTQ(targetSeq->second, querySeq->second, cigarSeq, targetStart);
            (*targetNode).setNext(queryNode, overlap, make_pair(targetStart, targetEnd), make_pair(queryStart,queryEnd));
            //(*queryNode).setPrev(targetNode, overlap, make_pair(queryStart,queryEnd), make_pair(targetStart, targetEnd));
            (*queryNode).setPrev(targetNode, overlap, make_pair(targetStart, targetEnd), make_pair(queryStart,queryEnd));
        }
        // If query -> target, + strand
        else if ((queryEnd == querySeq->second.length()) && (targetStart == 0)) {
            string overlap = cigarQooT(targetSeq->second, querySeq->second, cigarSeq, queryStart);
            (*queryNode).setNext(targetNode, overlap, make_pair(queryStart,queryEnd), make_pair(targetStart, targetEnd));
            //(*targetNode).setPrev(queryNode, overlap, make_pair(targetStart, targetEnd), make_pair(queryStart,queryEnd));
            (*targetNode).setPrev(queryNode, overlap, make_pair(queryStart,queryEnd), make_pair(targetStart, targetEnd));
        }
    }
    cout << "Repeat graph constructed" << endl;
}

string repeatGraph::cigarTQ(string target, string query, string cig, int firstStart) {
    
    // REMOVE AFTER TESTING
    //cout << "CIGARTQ processing..." << endl;

    int currQ=0;
    int currT=firstStart;
    string overlap="";
    list <pair<int, char> > cigQueue;

    string temp="";
    //makes the integer command value pairs
    for (auto c : cig){
        if (isdigit(c)){
            temp+=c;
        }
        else{
            pair <int, char> idk=make_pair(stoi(temp), c);
            cigQueue.push_back(idk);
            temp="";
        }
    }

    //iterates through each pair and creates the overlap
    for (auto duo : cigQueue){
        if (duo.second=='M'){
            overlap+=query.substr(currQ, duo.first);
            currQ+=duo.first;
            currT+=duo.first;
        } else if (duo.second=='D'){
            overlap+=query.substr(currQ, duo.first);
            currQ+=duo.first;
        }
        else{
            overlap+=target.substr(currT, duo.first);
            currT+=duo.first;
        }
    }
    return overlap;
}

string repeatGraph::cigarQooT(string target, string query, string cig, int firstStart) {

    // REMOVE AFTER TESTING
    //cout << "CIGARQooT processing..." << endl;

    int currT=0;
    int currQ=firstStart;
    string overlap="";
    list<pair<int, char>> cigQueue;

    string temp="";
    //makes the integer command value pairs
    for (auto c : cig){
        if (isdigit(c)){
            temp+=c;
        }
        else{
            pair <int, char> idk=make_pair(stoi(temp), c);
            cigQueue.push_back(idk);
            temp="";
        }
    }

    //iterates through each pair and creates the overlap
    for (auto duo : cigQueue){
        if (duo.second=='M'){
            overlap+=target.substr(currT, duo.first);
            currQ+=duo.first;
            currT+=duo.first;
        } else if (duo.second=='I'){
            overlap+=target.substr(currT, duo.first);
            currT+=duo.first;
        }
        else{
            overlap+=query.substr(currQ, duo.first);
            currQ+=duo.first;
        }
    }
    return overlap;
}

Node * repeatGraph::find(string seq){

    //iterate through node list to find node that has exact same seq
    for (auto element : allNodes){
        // cout << element->getID().at(0) << endl;
        // cout << element->getSeq() << endl;
        // cout << endl;
        // cout << seq << endl;
        //compare sequence and return pointer if true
        if (element->getSeq()==seq){
            return element;
        }
    }
    //if not found return false
    return NULL;
    
}

// Inserts all sources and sinks into starts and ends private vectors (graph class)
void repeatGraph::getStartsandEnds()
{
	for (auto node : this->allNodes){
		if (node->getPrev().size() < 1){
			this->starts.push_back(node);
		}
        if(node->getNext().size()<1){
            this->ends.push_back(node);
        }
	}

    // if there is no node with zero prevs, get node with most nexts
    if (this->starts.empty()){
        alternateStarts();
    }
}

// Inserts all one-in-one-out nodes into oneinoneout private vector
void repeatGraph::getOneinOut(){
	for (auto node : allNodes) {
		if (node->getPrev().size() == 1 && node->getNext().size() == 1) {
			this->oneinoneout.push_back(node);
		}
	}
}

// Removing non-high scoring paths
void repeatGraph::consolidatePaths(vector <list<Node *> >& nonbranchingPaths) {

    cout << "Consolidating paths..." << endl;

    // <startNode, endNode, path>
    vector<tuple<Node *, Node *, list<Node*>*>> paths; 

    // For each nonbranching path
    for (auto path : nonbranchingPaths) {

        // Getting beginning of path nodes
        tuple<Node*, string, pair<int, int>, pair<int, int>> prev = path.front()->getPrev()[0];
        Node * prevNode = get<0>(prev);
        // Getting end of path nodes
        tuple<Node*, string, pair<int, int>, pair<int, int>> next = path.back()->getNext()[0];
        Node * nextNode = get<0>(next);
        
        tuple<Node *, Node *, list<Node *> * > tuplePath = make_tuple(prevNode, nextNode, &path);
        paths.push_back(tuplePath);
    }
    // Sorting w/r to start nodes
    sort(paths.begin(), paths.end());

    // Finding paths with the same start and end
    // Until paths is empty
    while (paths.size() != 0) {

        Node * startNode = get<0>(paths[0]); // Getting start node
        unordered_map<Node *, vector<list<Node *> *> > sameStarts; // <endNode, vector of paths>
        vector<tuple<Node *, Node *, list<Node *> *> >::iterator ptr; 
        // For paths with same start node
        for (ptr = paths.begin(); ptr < paths.end(); ptr++) {

            // Verifying the start node is correct
            if (get<0>(*ptr) == startNode) {
                Node * end = get<1>(*ptr);
                // If end node has been found in sameStarts
                if (sameStarts.find(end) != sameStarts.end()) {
                    sameStarts[end].push_back(get<2>(*ptr));
                }
                // If end node did not already exist
                else {
                    vector<list<Node *> *> vect;
                    vect.push_back(get<2>(*ptr));
                    sameStarts[end] = vect;
                }
                paths.erase(paths.begin());
            }
            // If next start node has been reached, continue
            else {
                break;
            }
        }

        // sameStarts is unsorted map with end node and path

        for (auto val : sameStarts) {

            // If there are multiple paths between the same nodes
            if (val.second.size() > 1) {

                // Finding longest path
                int longest = 0;
                list<Node *> * bestPath;
                // For each possible path
                for (auto p : val.second) {
                    // Getting the new longest path
                    if (p->size() > longest) { // p is a list of nodes 
                        // If best path has already been assigned, delete previous path
                        if (longest != 0) {
                            for (auto node : *bestPath) { // for each node in old best path
                                removeNode(node);
                            }
                        }
                        longest = p->size();
                        bestPath = p;
                    }
                    // Else, delete nodes
                    else {
                        for (auto node : *p) { // for each node in current path
                            removeNode(node);
                        }
                    }
                }
            }
        }
    }
    cout << "Paths consolidated" << endl;
    cout << nonbranchingPaths.size() << endl;
    return;
}


//remove node from the graph
void repeatGraph::removeNode(Node * node) {
    node->deletePrev(node);
    node->deleteNext(node);
    node->deletePrev();
    node->deleteNext();
    return;
}


//after contig generation
// Getting a vector of nonbranching paths
vector<list<Node *> > repeatGraph::nonbranching(){

    cout << "Generating nonbranching paths..." << endl;

    // Assigning oneinoneout vector in graph
    getOneinOut();

    vector<list<Node *> > paths;
    list<Node *> p;
    // For each oneinoneout node
    for (auto node : this->oneinoneout){
        // Checks if node is at beginning of path
        auto condition=std::find(this->oneinoneout.begin(), this->oneinoneout.end(), get<0>(node->getPrev().front()));
        // If node was found in oneinoneout, skip node
        if (condition!=this->oneinoneout.end()){
            continue;
        }

        p.push_back(node); // current path to build
        Node * curr=node;
        // Getting path from node
        while (true){
            auto temp= curr->getNext();
            //if presents a problem make condition greater than 1 instead
            if (temp.size()!=1){ // because not one in one out
                break;
            }
            else{
                p.push_back(curr);
                curr = get<0>(temp.front());
            }
        }
        // Add current path to total paths
        paths.push_back(p);
        p.clear();
    }
    cout << "Nonbranching paths generated" << endl;
    return paths;
}

//contigs
//could be cause of memory link
void repeatGraph::generateContigs() {

    cout << "Generating contigs..." << endl;

    // Getting nonbranching paths
    vector <list<Node *> > paths = this->nonbranching();

    // If no paths, don't do anything
    if (paths.empty()) {
        return;
    }

    // For each nonbranching path
    for (auto p : paths) {

        string final=""; // final sequence
        Node * current=p.front();

        // Adding beginning of read (does not include overlap)
        final+=current->getSeq().substr(0, get<2>(current->getNext().front()).first);
        // Adding overlap sequence (because error corrected)
        final+=get<1>(current->getNext().front());
        // Removing first node from current path
        p.pop_front();

        // final: first read with overlap sequence

        // For the remaining nodes
        for (auto node : p) {
            // Getting second node's seq not affected by overlap
            int start = get<3>(node->getPrev().front()).second; // end of overlap of prev node
            int end = get<2>(node->getNext().front()).first; // start of overlap of next node

            final+=node->getSeq().substr(start, end-start); // accounts for multiple overlaps

            // If not last node, append overlap
            if (node!=p.back()){
                final+=get<1>(node->getNext().front());
            }
        }

        Node * end=p.back(); // last node of path
        p.pop_back();

        // Setting path -> single node
        current->setSeq(final);
        current->deleteNext();
        // auto newNext=end->getNext();

        // Reassigning current->Next
        // for (auto pointer : newNext){
        //     current->setNext(get<0>(pointer), get<1>(pointer), get<2>(pointer), get<3>(pointer), get<4>(pointer));
            
        // }

        current->setNext(end, get<1>(end->getPrev().front()), make_pair(final.length()-get<2>(end->getPrev().front()).first, final.length()),
             get<3>(end->getPrev().front()));
        end->setPrev(current, get<1>(end->getPrev().front()), make_pair(final.length()-get<2>(end->getPrev().front()).first, final.length()),
             get<3>(end->getPrev().front()));
        //error might come from here
        for (auto node : p){

            current->addId(node->getID()[0]);
            removeNode(node);
            this->allNodes.erase(remove(this->allNodes.begin(), this->allNodes.end(), node), this->allNodes.end());
            //delete node;
        }

    }
    cout << "Contigs generated" << endl;
}

// if there is no node with zero prevs, get node with most nexts
void repeatGraph::alternateStarts(){
    int maximum=0;
    Node * finStart;

    for (auto node : this->allNodes){
        int condition=node->getNext().size();
        if (condition > maximum){
            maximum=condition;
            finStart=node;
        }
    }
    this->starts.push_back(finStart);
}

// Returns the best possible path for each start node
vector<tuple<list <Node *>, list<tuple<Node *, string, pair<int, int>, pair<int, int>>>, int>> repeatGraph::findAssemblies(){

    cout << "Finding assemblies..." << endl;
    
    // Assumption: first node has no prev pointers

    vector<tuple<list <Node *>, list<tuple<Node *, string, pair<int, int>, pair<int, int>>>, int>> possiblePaths;
    // Initializing starts and ends nodes
    this->getStartsandEnds();
    list<Node *> temp; // path to build
    list <tuple<Node *, string, pair<int, int>, pair<int, int>>> edgeList; // temp's edges

    //for each starting node get longest path
    for (auto node : this->starts){

        Node * curr=node;
        int len=0;
        auto nextCurr=curr->getNext(); // vector of next nodes
        // Adding curr node to current path
        temp.push_back(curr);
        curr->setVisited(true);

        // While there are still possible paths to traverse
        // Building maximal path
        while (nextCurr.size()!=0){

            int overlap=0; // size of overlap
            int slices=0; // size of seq without overlap
            Node * maximum=get<0>(nextCurr.front());
            tuple<Node *, string, pair<int, int>, pair<int, int>> edge = nextCurr.front();
            // For each edge (finding the best possible next edge)
            for (auto e : nextCurr){

                // If already visited, skip
                if (get<0>(e)->getVisited()==true){
                    continue;
                }
                // if length of seq of next edge > length of seq of maximum
                else if (get<0>(e)->getSeq().length() > maximum->getSeq().length()) {
                    maximum=get<0>(e); // setting new maximum
                    overlap=get<1>(e).length(); // setting new overlap val
                    // Adding non-overlap bp
                    slices=abs(get<2>(e).second - get<2>(e).first) + abs(get<3>(e).second - get<3>(e).first);
                    edge=e; // to potential add to edges
                }
            }
            // If maximum has already been visited
            if (maximum->getVisited()==true){
                break;
            }

            // Adding len(overlap)
            len+=slices+overlap+curr->getSeq().length();
            // Adding maximum and edge to path
            temp.push_back(maximum);
            edgeList.push_back(edge);
            // Setting curr to the best next node
            curr=maximum;
            // Setting next selection of edges
            nextCurr=curr->getNext();
            curr->setVisited(true);
        }

        // Adding best path
        possiblePaths.push_back(make_tuple(temp, edgeList, len));
        temp.clear();
        edgeList.clear();
        resetVisits();
    }
    cout << "Assemblies found" << endl;
    return possiblePaths;
}

// Resetting visit parameter for all nodes
void repeatGraph::resetVisits() {
    for (auto node : this->allNodes) {
        node->setVisited(false);
    }
}

// 
void repeatGraph::setAssembly(){

    cout << "Setting assemblies..." << endl;

    // <path of all nodes, list of edges in path, number of nodes in path>
    tuple<list <Node *>, list<tuple<Node *, string, pair<int, int>, pair<int, int>>>, int> maxPath;

    // Returns best possible path for each start node
    vector<tuple<list <Node *>, list<tuple<Node *, string, pair<int, int>, pair<int, int>>>, int>> possible= findAssemblies();
    int maximum=0; // longest sequence

    // For each possible path
    for (auto option : possible){
        if (get<2>(option) > maximum) {
            maximum=get<2>(option);
            maxPath=option;
        }
    }

    // Setting assembly nodes and edges in graph
    this->assemblyNode=get<0>(maxPath);
    this->assemblyEdges=get<1>(maxPath);
    cout << "Assemblies set" << endl;
}

void repeatGraph::assembly_to_fasta(){

    cout << "Assembly to fasta..." << endl;

    ofstream o;
    o.open("assembled.fa");
    o << ">assembled genome from hifi reads\n";
    string prevseq = this->assemblyNode.front()->getSeq();
    int startidx = 0;
    // assembly edge is comprised of:
    // the node with the overlap string over its left hand side (start)
    // the string that is the overlap
    // a pair of values (startofoverlap, endofoverlap) on the first read's string
    // a pair of values (startofoverlap, endofoverlap) on the second read's string
    for (auto edge : this->assemblyEdges) {
        // print sequence BEFORE the overlap now that we have all the info about it
        o << prevseq.substr(startidx, get<2>(edge).first); // to where the overlap starts on first string
        // print the actual overlap
        o << get<1>(edge);
        // save the sequence of the node AFTER the overlap
        prevseq = get<0>(edge)->getSeq();
        // save starting index on the new prevseq
        startidx = get<3>(edge).second;
    }
    o << prevseq.substr(startidx); // print out the last node's sequence not covered by an overlap
    o.close();
    cout << "Done" << endl;
}

// list<Node *> repeatGraph::getAssembly(){
//     return this->assembly;
// }


repeatGraph::~repeatGraph(){
    allNodes.clear();
    oneinoneout.clear();
    starts.clear();
    ends.clear();
    assemblyNode.clear();
    assemblyEdges.clear();
}
