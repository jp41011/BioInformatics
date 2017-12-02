//============================================================================
// Name        : Prog5_combine.cpp
// Author      : Juan Pedraza
// Version     : v1 - 3/28/16
// Copyright   : Your copyright notice
// Description : Program to combine the 3 mappability files into one file.
//				Removed the genes that had 0's for all three readsizes.
//============================================================================

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;

typedef unordered_map<string, string> UOMap;

void loadMap(string fileName, UOMap& map);

void printMaps(string outFileName, UOMap& map50, UOMap& map70, UOMap& map100);

int main() {
	UOMap map50; // map with values for 50
	UOMap map70; // map with values for 70
	UOMap map100; // map with values for 100
	//UOMap mapPrinted; // map to keep track of those that have been printed

	loadMap("mapability50.txt", map50);
	cout << "map50.size(): " << map50.size() << endl;

	loadMap("mapability70.txt", map70);
	cout << "map70.size(): " << map70.size() << endl;

	loadMap("mapability100.txt", map100);
	cout << "map100.size(): " << map100.size() << endl;

	printMaps("Final_Mappability.txt", map50, map70, map100);

	return 0;
}

void loadMap(string fileName, UOMap& map)
{
	ifstream inFile(fileName);
	string tempLine = "";
	string geneID = "";
	string geneMapScore = "";

	getline(inFile, tempLine); // remove the header line

	while(inFile >> geneID) // get gene ID
	{
		inFile >> geneMapScore; // get gene mapability score
		map[geneID] = geneMapScore; // add to map
	}

	inFile.close();
	return;
}

void printMaps(string outFileName, UOMap& map50, UOMap& map70, UOMap& map100)
{
	ofstream outFile(outFileName);

	UOMap mapUniq; // map with all the unique across the 3 maps

	for(auto it=map50.begin(); it != map50.end(); ++it)
	{
		mapUniq[it->first] = "1"; // add to map doesn't matter what the value is
	}

	for(auto it=map70.begin(); it != map70.end(); ++it)
	{
		mapUniq[it->first] = "1"; // add to map doesn't matter what the value is
	}

	for(auto it=map100.begin(); it != map100.end(); ++it)
	{
		mapUniq[it->first] = "1"; // add to map doesn't matter what the value is
	}

	// now the mapUniq has all the unique geneID accross all 3 files
	outFile << "GeneID\tReadLen50\tReadLen70\tReadLen100" << endl; // header line
	for(auto it=mapUniq.begin(); it != mapUniq.end(); ++it)
	{
		string tGeneID = it->first;
		outFile << tGeneID << "\t"; // print gene ID
		if(map50.count(tGeneID) == 1) // does have a score for this
			outFile << map50[tGeneID] << "\t"; // get and print 50 score
		else // else print out 0's
			outFile << "0.00" << "\t";

		if(map70.count(tGeneID) == 1)
			outFile << map70[tGeneID] << "\t"; // get and print 70 score
		else // no score
			outFile << "0.00" << "\t"; // print out 0.00 instead of blank

		if(map100.count(tGeneID) == 1) // if it has a score
			outFile << map100[tGeneID] << endl; // get and print 100 score and new line
		else // no score
			outFile << "0.00" << endl; // print out 0.00 instead of blank

	}

	cout << "Total GeneIDs: " << mapUniq.size() << endl;
	outFile.close();
	return;
}

