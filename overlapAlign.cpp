// #include <seqan3/alignment/all.hpp>
// #include <seqan3/core/all.hpp>
// #include <seqan3/core/debug_stream.hpp>

#include "overlapAlign.h"


overlapAlign::overlapAlign(string one, string two){
    this->read1=one;
    this->read2=two;
    this->length1=one.length();
    this->length2=two.length();

    this->scoringMatrix= new int*[this->length1+1];

    for (int i=0; i<this->length1+1; i++){
        this->scoringMatrix[i]=new int[this->length2+1];
    }
}

void overlapAlign::createMatrix(){
    //match=1
    //indel/mismatch=-2
    // const int temp1=this->length1+1;
    // const int temp2=this->length2+1;
    // int matrix[temp1][temp2];

    
    this->scoringMatrix[0][0]=0;
    for (int i=1; i<this->length1+1; i++){
        this->scoringMatrix[i][0]=this->scoringMatrix[i-1][0]-1;
    }
    for (int i=1; i<this->length2+1; i++){
        this->scoringMatrix[0][i]=this->scoringMatrix[0][i-1]-1;
    }

    for (int i=1; i<this->length1+1; i++){
        for (int j=1; j<this->length2+1; j++){
            int match=-1;
            if (this->read1[i-1]==this->read2[j-1]){
                match=2;
            }

            int d=this->scoringMatrix[i-1][j]-1;
            int r=this->scoringMatrix[i][j-1]-1;
            int s=this->scoringMatrix[i-1][j-1]+match;

            int temp=max(d,r);

            this->scoringMatrix[i][j]=max(temp, s);
        }
    }

    
}

void overlapAlign::getScorePlusPosition(){

    //referencing is off
    // int lastRow=*this->scoringMatrix.length;

    int maximum=-100000;
    int position=0;
    for (int i=0; i<this->length2+1; i++){
        int temp=this->scoringMatrix[this->length1][i];
        if (temp>maximum){
            maximum=temp;
            position=i;
        }
        // cout<<i<<endl;
        // cout<<this->scoringMatrix[this->length1][i]<<endl;
    }

    this->score=maximum;
    this->position=position;
    // cout<<this->score<<endl;
    // cout<<this->position<<endl;

}

pair<string, string> overlapAlign::backtrack(){
    string top="";
    string bottom="";

    int i=this->length1;
    int j=this->position;
    // cout<<i<<","<<j<<endl;
    int current=this->scoringMatrix[i][j];
    while (i>0 && j>0){
        if (current==this->scoringMatrix[i-1][j]){
            top=this->read1[i-1]+top;
            bottom='-'+bottom;
            i--;
        }
        else if (current==this->scoringMatrix[i][j-1]){
            bottom=this->read2[j-1]+bottom;
            top='-'+top;
            j--;
        }
        else{
            top=this->read1[i-1]+top;
            bottom=this->read2[j-1]+bottom;
            i--;
            j--;
        }

    }
    // cout<<top<<endl;
    // cout<<bottom<<endl;
    return make_pair(top, bottom);
}

overlapAlign::~overlapAlign(){
    delete scoringMatrix;
}