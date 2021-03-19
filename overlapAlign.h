#include <array>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>



using namespace std;

class overlapAlign{
private:
    string read1;
    string read2;
    int length1;
    int length2;
    int ** scoringMatrix;
    int position;

public:
    int score;
    overlapAlign(string one, string two);
    void createMatrix();
    pair<string, string> backtrack();
    void getScorePlusPosition();
    ~overlapAlign();
};