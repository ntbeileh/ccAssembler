#ifndef NODE_H
#define NODE_H
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>

using namespace std;

class Node
{
private:
    //changed to vector to allow for duplicate sequences
    vector<string> id;               // the ID of the fasta read that this sequence came from
    string seq;              // sequence from FASTA
    vector< tuple<Node*, string, pair<int, int>, pair<int, int>> > Next;   //Pointer to the next node, NULL if None/not set yet
    vector< tuple<Node*, string, pair<int, int>, pair<int, int>> > Prev;   //Pointer to the previous node, NULL if none/not set yet
    bool visited = false;            // Has the node been visited already by the assembler
public:
    Node(string id, string seq);      // default constructor
    vector<string> getID();                   // returns the FASTA ID of the sequence stored
    string getSeq();                  // returns private variable sequence
    vector<tuple<Node*, string, pair<int, int>, pair<int, int>> > getNext();    // returns vector of next nodes (node pointer, overlap)
    vector<tuple<Node*, string, pair<int, int>, pair<int, int>> > getPrev();    // returns the private variable prev node

    void addId(string id);                 //add id to id list
    bool getVisited();                // returns the private variable visited
    void setNext(Node *newNext, string overlap, pair<int, int> query, pair<int, int> target);      // sets the private variable next
    void setPrev(Node *newPrev, string overlap, pair<int, int> query, pair<int, int> target);      // sets the pointer to the variable prev node
    void setVisited(bool newVisited); // sets the private variable visited
    void setSeq(string newSeq);
    void deletePrev();
    void deletePrev(Node *node); // finding *node in this->node's prev
    void deleteNext();
    void deleteNext(Node *node); // finding *node in this->node's next
};

void from_fasta(string fasta, vector<Node*> &nodes); // Pass in the name of a FASTA file, returns array of Node pointers
void fasta_to_map(string fasta, unordered_map<string, string> & reads);
#endif
