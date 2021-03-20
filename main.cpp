#include <stdio.h>
#include <time.h>
#include "node.cpp"
#include "repeatGraph.cpp"
using namespace std;
 
int main(int argc, char **argv)
{
    cout << "Starting the ASSembler, pleb" << endl;
    time_t tt = time(NULL);
    cout << ctime(&tt) << endl;
 
    std::cout << "\nRunning..." << endl;
    
    // Checking the number of command line args
    if (argc <= 1)
    {
        cerr << "Bad Command Line Arguments, " << argv[0] << "." << endl;
        return -1;
    }
 
    // using the minimap2 commandline tool to produce a PAF file, called "overlaps.paf"
    char command[250]; 
    string tempoutput = "overlaps.paf";
    int x = tempoutput.length();
    char output[x + 1];
    strcpy(output, tempoutput.c_str());
    sprintf(command, "minimap2 -H -k19 -Xw5 -n -g10000 -m9000 --max-chain-skip 25 -c -t 4 --for-only %s %s -o %s", argv[1], argv[1], output);
    system(command);
 
    // decompressing the compressed reads into a decompressed reads.fa file
    // char anothercommand[100];
    // string tempfasta = "reads.fa";
    // int n = tempfasta.length();
    // char fasta[n + 1];
    // strcpy(fasta, tempfasta.c_str());
    // sprintf(anothercommand, "zcat %s > %s", argv[1], fasta);
    // system(anothercommand);
 
    // Getting the reads from the FASTA input
    unordered_map<string, string> reads;
    fasta_to_map(argv[1], reads);
    
    // Creating the repeat graph
    repeatGraph overlapGraph = repeatGraph(tempoutput, reads);
 
    // Getting nonbranching paths
    vector <list<Node *> > nonbranchingPaths = overlapGraph.nonbranching(); 
    
    // Consolidating paths, getting highest scoring paths
    overlapGraph.consolidatePaths(nonbranchingPaths);
 
    // list(Nodes) -> single node
    overlapGraph.generateContigs();
 
    // find the assemblyu
    overlapGraph.setAssembly();
 
    // Generating fasta file
    overlapGraph.assembly_to_fasta();
 
}
 

