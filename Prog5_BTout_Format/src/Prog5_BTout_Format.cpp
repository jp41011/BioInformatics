//============================================================================
// Name        : Prog5_BTOutput.cpp
// Author      : Juan Pedraza
// Version     : v1 - 3/28/16
// Copyright   : Your copyright notice
// Description : Program to format bowtie output file to the way we want them
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

int main() {
	// do this for 50, 70, 100
	ifstream inFile("BTout50"); // non formated file
	ofstream outFile("BTout50.txt"); // formated file
	vector<string> row (0);

	string fileLine = "";

	while(getline(inFile, fileLine))
	{
		row.clear();
		stringstream ss (fileLine);
		string temp="";
		while(ss >> temp)
			row.push_back(temp);

		int len = row.size();
		string dummyScore = 0;
		//outFile << row[2] << "\t" << row[3] << "\t" << (stoi(row[3],nullptr,10) + row[4].length()) << "\t" << row[0] << "\t" << dummyScore << "\t" << row[1] << endl;
		outFile << row[2] << "\t" << row[3] << "\t" << (atoi(row[3].c_str()) + row[4].length()) << "\t" << row[0] << "\t" << dummyScore << "\t" << row[1] << endl;
	}

	inFile.close();
	outFile.close();
	return 0;
}
