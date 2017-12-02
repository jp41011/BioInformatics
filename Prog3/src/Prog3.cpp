/*============================================================================
 Name        : Prog3.cpp
 Author      : Juan Pedraza
 Version     : v4 - 2/29/16
 Copyright   : Your copyright notice
 Description : CSci 291T - BioInformatics
 	 	 	 Task: This program will find the median string and motif from a given set of DNA sequences and the length of the motif.
 	 	 	 	 Will use the branch-and-bound algorithm with L-mer tree.
 	 	 	 Input:
 	 	 	 	 1) Set of DNA sequence of arbitrary length each. Prokaryotic DNA sequence file "HMP-617.fa" from blackboard
				 2) motif length (L-mer size)
 	 	 	 Output: The 5 best median strings with total_distance and their corresponding motif consensus strings, consensus scores and positions.
 	 	 	 Environment: IDE: Eclipse, OS: Ubuntu: 14.04 LTS, Intel i7 2.3 GHz x 8
 	 	 	 Had big problems with Eclipse not recognizing the unordered_map data type.
 	 	 	 Could not switch to something else since I already had alot of stuff in the eclipse environment.

============================================================================
*/

#include <iostream>
#include <vector>
#include <fstream> // to read and write to files
#include "Sequence.h"
#include "SequenceGroup.h"
//#include "MotifFunctions.h"

#define LMER 6 // L-mere size of 6 for this assignment
#define KSIZE 4 // K = 4 because 4 bases: A C G T

using namespace std;


/* Notes
 * 1st find 5 best median strings
 * then find the 5 corresponding motif consensus strings
 * 	and their scores
 * 	and their positions
 */

/* Will use a vector to store a k-ary tree
 * index i starts at 1 not 0 for the first node
 * node with index i
 * c-th child is found at index: (k*i)+1+c
 * parent of i is at floor( (i-1) / k)
 */

//void totalDistance(string v, string d);

//void hammingDistance(string v, string s);



/* This function takes a file name and a reference to a vector.
 * It gets the sequence information from the file and stores it into the vector.
 */
void loadSeq(string fileName, vector<Sequence>& seqVec){
	ifstream seqFile (fileName.c_str());
	string tempLine;

	while(getline(seqFile, tempLine)){
		if(tempLine[0] == '>') // title of string
		{
			Sequence tSeq (tempLine, "");
			getline(seqFile, tempLine); // get next line. is seq line
			tSeq.setSeq(tempLine); // store seq string
			seqVec.push_back(tSeq);
		}
	}

	seqFile.close(); // clean up before exit
	return;
}

int main() {

	vector<Sequence> seqVec (0);
	//string seqFileName = "HMP-617.fa";
	string seqFileName = "sample.fa"; // smaller set for testing
	loadSeq(seqFileName, seqVec); // call func to load seq info into vector of seq
	cout << "vector size: " << seqVec.size() << endl;

	SequenceGroup seqGroup (seqVec, LMER, KSIZE);
	cout << "after seqGroup" << endl;
	vector<Median> medianVec = seqGroup.getMedians();
	cout << "after getMedians();" << endl;
	cout << "medianVec size: " << medianVec.size() << endl;
	/*
	startMotif(medianVec, seqVec, LMER); // results will be store into the medianVec... ea. Median
	cout << "after startMotif(...)" << endl;

	calcConsensus(medianVec, LMER); // set consensus info in each median in medianVec
	cout << "after calcConsensus(...)" << endl;
	//printOutput(medianVec, )
	*/

	return 0;
}
