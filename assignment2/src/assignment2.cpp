//============================================================================
// Name        : CSci 291T - assignment2.cpp
// Author      : Juan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream> // to read and write to files
#include <sstream> // convert multi digit string to number
#include <algorithm> // for built in reverse
//#include <string> // string stuff
#include <ctime> // to time program run time
#include "chromosome.h" // my chromosome class
#include "Exon.h"
//#include "Gene.h" // my gene class

using namespace std;


// fucntion to populate a vector of exons from an input file
// populate a exon vector with exon information from an given input file
void getExonsFromFile(vector<Exon>& exonVec, string& fileName)
{
	ifstream exonFile (fileName.c_str());
	char delimiter1 = '.';
	string tempLine; // to temp store lines from file
	string cName;
	unsigned long int sIndex;
	unsigned long int eIndex;
	string gName;
	unsigned long int eID;
	bool isStrandPlus;
	string basePattern;

	while(getline(exonFile, tempLine))
	{
		// first get should start with >chr1.start.end.geneName.+/-
		stringstream sLine(tempLine);
		vector<string> exonRow (0); // temp storage of line from file
		string temp2;
		while(getline(sLine,temp2, delimiter1)){ // using . as delimiter
			exonRow.push_back(temp2);
		}
		cName = exonRow[0];

		stringstream stream2(exonRow[1]);
		stream2 >> sIndex;
		stringstream stream3(exonRow[2]);
		stream3 >> eIndex;

		gName = exonRow[3];
		isStrandPlus = (exonRow[4] == "+" ? true : false);

		stringstream stream4(gName); // have to split this one up by '_'
		exonRow.clear();
		while(getline(stream4, temp2, '_')){
			exonRow.push_back(temp2);
		}
		gName = exonRow[0]+"_"+exonRow[1];
		stringstream ss(exonRow[3]);
		ss >> eID;
		exonRow.clear();
		getline(exonFile, basePattern); // get next line and save as patter

		// now that we have all the information from the 2 lines from the file
		// create exon and add to vector
		Exon tempExon (cName, sIndex, eIndex, gName, eID, isStrandPlus, basePattern);
		tempExon.printSummary();
		exonVec.push_back(tempExon); // to vector
	}

	exonFile.close(); // close before exiting
	return;
}

// return the max of the 4 given values
int getMax(int first, int match, int insert, int del){
	int tempMax = first;
	if(match > tempMax)
		tempMax = match;
	if(insert > tempMax)
		tempMax = insert;
	if(del > tempMax)
		tempMax = del;

	return tempMax;
}

/*smith-waterman implementation
 * Scoring:
 * Insertion: 	-1
 * Deletion: 	-1
 * Mismatch: 	-1
 * Match: 		+2
 */
void smith_waterman(const string& iString, chromosome& chromo, int& swScore, int& swLen, int& swI, int& swJ )
{
	cout << "In smith_waterman" << endl;
	int insertion = -1;
	int deletion = -1;
	int mismatch = -1;
	int match = 2;

	unsigned long int jLen = chromo.getBaseCount();
	int iLen = iString.length();

	vector< vector<int> > colVec (2, vector<int> (iLen + 1, 0)); // 2 cols many rows


	int curMax = 0; // current Max value per cell
	int curLen = 0; // current max length
	int curI = 0;
	int curJ = 0;

	int tempMax = -1;
	int tempLen = 0; // temp current length
	//int tempI = 0;
	//int tempJ = 0;

	//bool isMatch = false;
	char tchar1;
	char tchar2;

	for(unsigned long int j=1; j < jLen; j++){ // loop through the chromosome

		tchar1 = chromo.getIndex(j);
		//cout << "j: " << j << " - " << tchar1 << endl;
		/* tried to skip the N's since it takes very very long time to go through everything
		if(tchar1 == 'N' || tchar1 == 'n') // if N
			continue; // then skip out of this loop don't have to check for N's
		*/
		for(int i=1; i < iLen; i++){ // loop through the 2D vector Length of the pattern we are looking for
			tchar2 = iString[i];
			//cout << tchar1 << " == " << tchar2 << endl;
			if(tchar1 == tchar2){ // if they match
				//isMatch = true;
				tempMax = getMax(0, colVec[0][i-1]+match, colVec[1][i-1]+insertion, colVec[0][i]+deletion);
			}else{
				//isMatch = false;
				tempMax = getMax(0, colVec[0][i-1]+mismatch, colVec[1][i-1]+insertion, colVec[0][i]+deletion);
			}

			if(tempMax <= 0){ // if max is 0 or below
				if(tempLen > curLen)
					curLen = tempLen;
				tempLen = 0; // reset length new alignment started
			}else{
				tempLen++; // increase the len by one base
			}

			if(tempMax > curMax){
				curMax = tempMax;
				curI = i;
				curJ = j;
				//cout << "New Max: " << curMax << "\t";
			}

			colVec[1][i] = tempMax; // set current cell value
		}
		// went all the way to the bottom of the column.
		// now have to move all colVec[x][1] => colVec[x][0] before going to the next base in chr1
		for(unsigned int t=0; t < colVec[0].size(); t++)
			colVec[0][t] = colVec[1][t];

	}

	swScore = curMax;
	swLen = curLen;
	swI = curI;
	swJ = curJ;
	/*
	cout << endl << "swScore: " << swScore << endl;
	cout << "swLen: " << swLen << endl;
	cout << "swI: " << swI << endl;
	cout << "swJ: " << swJ << endl;
	*/
	return;
}

/*************************************************************************/
int main(int argc, char* argv[]) {
	clock_t beginTime = clock(); // startlineCount time

	//prep getting chromosome information from file and storing in chromosome class
	ifstream inFile; // chromosome base file
	string char1File = "chr1.fa";
	inFile.open(char1File.c_str());

	// prep for the output file
	ofstream outFile;
	string outputFile = "assignment2.text";
	outFile.open(outputFile.c_str());

	chromosome chromo = chromosome();
	vector<Exon> exonVec (0);

	//read in exon information
	//string tempStr = "input_short.fa";
	string tempStr = "Prog2-input-NM_032291-10exon-seqs.fa";
	getExonsFromFile(exonVec, tempStr);
	cout << "back to main" << endl;
	//return 0;

	//Reading in information from chromosome file into the chromosome class
	getline(inFile, tempStr); // get the first line out of the way ">char1"
	while(getline(inFile, tempStr)){ // start reading in bases
		chromo.addLine(tempStr);
	}

	//summary of what was read
	//cout << "Final Line count: " << chromo.getLineCount() << endl;
	//cout << "chromo.length = " << chromo.getBaseCount() << endl;

	cout << "chromo.length = " << chromo.getBaseCount() << endl;

	//string wholeChromo = chromo.getSeq(cStart, cEnd); // get the whole string!!
	//cout << "after whole string" << endl; !!! takes too long

	for(unsigned int i=0; i < exonVec.size(); i++)
	{
		int swScore=0; // score returned by smith_waterman
		int swLen = 0; // max length returned by smith_waterman
		int swI = 0; // i value returned
		int swJ = 0; // j value returned
		string patternStr = exonVec[i].getPattern();

		outFile << exonVec[i].getChromoName();
		outFile << "." << exonVec[i].getStart();
		outFile << "." << exonVec[i].getEnd();
		outFile << "." << exonVec[i].getGeneName() << "_exon_" << exonVec[i].getExonID();
		outFile << "." << (exonVec[i].isPlusStrand() ? "+" : "-");
		outFile << "\tlen=" << exonVec[i].getEnd()-exonVec[i].getStart() << '\n';
		outFile << "ref(1-10) = NNNNNNNNNN ; "; // todo
		outFile << "pattern = " << patternStr << "\n";

		smith_waterman(patternStr, chromo, swScore, swLen, swI, swJ);

		outFile << "--- Optimum Smith-Waterman score = " << swScore;
		outFile << " (i=" << swI << ", j=" << swJ << ")\n\n"; // add 2nd line to make break noticable
	}

	//clean up and summary stats
	outFile.close();
	inFile.close();
	clock_t endTime = clock(); // end time
	double totalTime = double(endTime - beginTime) / CLOCKS_PER_SEC;
	cout << "Approximate run time: " << totalTime << " seconds" << endl;
	return 0;
}


