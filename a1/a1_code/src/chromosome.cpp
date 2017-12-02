//#include <iostream>
//#include <vector> // included in header file

#include "chromosome.h"
#include <math.h> // for the rounding functions
#define lineWidth 50

//constructor initialize vector
chromosome::chromosome(){
	baseLines = vector<string> (0);
	lineCount = 0;
	baseCount = 0;
}

chromosome::~chromosome(){
	baseLines.clear(); // clear contents and free up memory
	lineCount = 0;
	baseCount = 0;
}

void chromosome::addLine(string& newLine){
	baseLines.push_back(newLine); // add new line of nucleotides
	lineCount++; // increase line count
	baseCount = baseCount + newLine.length(); // increase total base count
}

/*
 * Math used to find the particular index in the vector of strings
 * index is the index we are looking for while lineWidth is the width of the strings in the
 * datafile and in the vector of strings.
 * index/lineWidth and rounded down gives you the row where the index is found (x axis)
 * then we use the index % lineWidth to find the offset index from the first column (y axis)
 */
char chromosome::getIndex(unsigned long int& index){

	unsigned long int row = floor(index/lineWidth);
	unsigned int index2 = index%lineWidth;
	return baseLines[row][index2];

	//return '?';
}

string chromosome::getSeq(unsigned long int& startIndex, unsigned long int& endIndex){
	//vector<char> seq (0);
	string seq = "";
	for(unsigned long int i=startIndex; i<endIndex; i++)
	{
		//seq.push_back(getIndex(i)); // adding to vector
		seq = seq + getIndex(i);
	}
	//string seq (seq.begin(), seq.end());
	return seq;
}

string chromosome::getLine(unsigned long int& lineNumber){
	return baseLines[lineNumber];
}

unsigned long int chromosome::getLineCount(){ return lineCount;}
unsigned long int chromosome::getBaseCount(){ return baseCount;}
