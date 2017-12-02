/*
============================================================================
Name        : Prog5_Reads.cpp
Author      : Juan Pedraza
Version     : v1 - 3/27/16
Copyright   : Your copyright notice
Description : Will generate reads of lengths 50, 70 and 100 from chr1-250.fa
			We will then format the ouputs with awk because its faster.
============================================================================
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
//#include <stdlib.h>

using namespace std;

int main() {
	int readSize = 100; // 50, 70, 100
	string fileLine;

	ifstream inFile("chr1-250.fa"); // read file
	string readSize_string = to_string(readSize);
	string outFileName = "reads"+readSize_string+".txt";
	ofstream outFile(outFileName);
	//ofstream outFile("reads50.txt");

	while(inFile >> fileLine) // read line in
	{
		string geneName = fileLine;
		string seq;

		inFile >> seq;

		if(seq.length() > readSize)
		{
			//int seqIndex = 0;
			int count = 0;
			for(unsigned int i=0; i <= seq.length() - readSize; i++)
			{
				string tempSeq = seq.substr(i, readSize);
				string tempGeneName = geneName + "." + to_string(count++);
				outFile << tempGeneName << endl << tempSeq << endl;
			}
		}else{
			string tempGeneName = geneName + ".0";
			outFile << tempGeneName << endl << seq << endl;
		}

	}


	inFile.close();
	outFile.close();
	return 0;
}
