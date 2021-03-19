
#include "node.h"
// #include <bits/stdc++.h> 

Node::Node(string id, string seq)
{
	this->seq = seq;
    //push id to vector
    this->id.push_back(id);
}

vector<string> Node::getID()
{
    return this->id;
}

void Node::addId(string givenID){
    this->id.push_back(givenID);
}

string Node::getSeq()
{
    return this->seq;
}


vector<tuple<Node*, string, pair<int, int>, pair<int, int>> > Node::getNext()
{
    return this->Next;
}

vector<tuple<Node*, string, pair<int, int>, pair<int, int>> > Node::getPrev()
{
    return this->Prev;
}

bool Node::getVisited()
{
    return this->visited;
}

void Node::setNext(Node *newNext, string overlap, pair<int, int> query, pair<int, int> target)
{
    this->Next.push_back(make_tuple(newNext, overlap, query, target));
}

void Node::setPrev(Node *newPrev, string overlap, pair<int, int> query, pair<int, int> target)
{
    this->Prev.push_back(make_tuple(newPrev, overlap, query, target));
}

void Node::setVisited(bool newVisited)
{
    this->visited = newVisited;
}

void Node::deletePrev(){
    this->Prev.clear();
}

// Looping through prev of current node and deleting *node
void Node::deletePrev(Node * node) {
    // There should only be one prev in *node
    Node * prev = get<0>(this->getPrev().front());
    // Finding *node in prev->getNext()
    for (int i = 0; i < prev->getNext().size(); i++) {
        if (get<0>(prev->getNext().at(i)) == node) {
            prev->getNext().erase(prev->getNext().begin() + i);
            return;
        }
    }
}

void Node::deleteNext(){
    this->Next.clear();
}

// Looping through next of current node and deleting *node
void Node::deleteNext(Node * node) {
    // There should only be one prev in *node
    Node * next = get<0>(this->getNext().front());
    // Finding *node in prev->getNext()
    for (int i = 0; i < next->getPrev().size(); i++) {
        if (get<0>(next->getPrev().at(i)) == node) {
            next->getPrev().erase(next->getPrev().begin() + i);
            return;
        }
    }
}

void from_fasta(string fasta, vector<Node *> &nodes)
{
    cout << "from_fasta" << endl;
    // make a in stream for the file
    ifstream infile(fasta);

    // check if file's input is readable
    if (!infile.good())
    {
        cout << "Error, cannot read file: " << fasta << "\n Exiting Program.\n"
             << endl;
        return;
    }

    // read the fasta and make new nodes
    string id, seq;
    while (getline(infile, seq).good())
    {
        if (seq.find('=') != string::npos)
        {
            id = seq.substr(seq.find('=') + 1);
        }
        else
        {
            Node *newNode = new Node(id, seq);
            nodes.push_back(newNode);
        }
    }
}

void from_genome(string genome, vector<Node *> &nodes)
{
	cout << "from_genome" << endl;
	// make an in stream for the file
	ifstream infile(genome);

	// check if file's input is readable
	if (!infile.good())
	{
		cout << "Error, cannot read file: " << genome << "\n Exiting Program. \n" << endl;
		return;
	}

	// read the file and make new nodes
	string id, line;
	string seq;
	while (getline(infile, line).good())
	{
		if (line.find('>') != string::npos)
		{
			id = line.substr(line.find('>') + 12);
		}
		else 
		{
			seq = seq + line;
			while (seq.length() > 5000)
			{
				Node *newNode = new Node(id, seq.substr(0, 5000));
				nodes.push_back(newNode);
				seq = seq.substr(5000, seq.length() - 5000);
			}
			//Node *newNode = new Node(id, seq, true);
			//nodes.push_back(newNode);
		}
	}
	Node *newNode = new Node(id, seq);
	nodes.push_back(newNode);
}
void Node::setSeq(string newSeq){
    this->seq=newSeq;
}

// Creating unordered_map that represents the reads from the fasta file
void fasta_to_map(string fasta, unordered_map<string, string> & reads) { // <id, seq>

    cout << "Fasta_to_map..." << endl;

    // make a in stream for the file
    ifstream infile(fasta);

    // check if file's input is readable
    if (!infile.good())
    {
        cout << "Error, cannot read file: " << fasta << "\n Exiting Program.\n"
             << endl;
        return;
    }
    
    string id, seq;
    // reading in the fasta file
    while (getline(infile, seq).good())
    {
        //cout << "E" << endl;
        // Getting the read ID
        if (seq.find('>') != string::npos)
        {
            id = seq.substr(1);
            //cout << "A" << endl;
        }
        // Getting the sequence and inserting into reads
        else
        {
            reads.insert({id, seq});
            //cout << seq << endl;
        }
    }
    cout << "Map created" << endl;
    return;
}
