/*============================================================================
Name        : Prog5.cpp
Author      : Juan Pedraza
Version     : v1 - 3/27/16
Copyright   : Your copyright notice
Description : Program for checking the mappability of the 250 gene sequences within
			the scope of HG19 chr1.
			Given the outputs of the bowtie program we will then compute the
			mappability of all the genes.
			Removed the genes that had 0's for all three readsizes.

============================================================================
*/

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <stdlib.h>

using namespace std;

typedef unordered_map<string, int> UOMap;

int main() {

	UOMap map1; // map for the gene counts
	UOMap map2; // map for the total number of tiles

	ifstream inFile("BTout100.txt");
	ofstream outFile("mapability100.txt");

	string line="";
	vector<string> cols (0);

	while(getline(inFile, line))
	{
		cols.clear();
		stringstream ss (line);
		string temp="";
		while(ss >> temp)
			cols.push_back(temp);

		string startMatch_str = cols[1];
		int startMatch = atoi(startMatch_str.c_str()); // convert to int
		string endMatch_str = cols[2];
		int endMatch = atoi(endMatch_str.c_str());
		string geneLine = cols[3];
		string delimiter = ".";

		size_t pos=0;
		string token;
		vector<string> geneInfo (0);
		while( (pos = geneLine.find(delimiter)) != string::npos)
		{
			token = geneLine.substr(0,pos);
			geneInfo.push_back(token);
			geneLine.erase(0,pos+delimiter.length());
		}

		//int len = geneInfo.size();
		string chromo = geneInfo[0];
		string startIndex_str = geneInfo[1];
		int startIndex = atoi(startIndex_str.c_str());
		string endIndex_str = geneInfo[2];
		int endIndex = atoi(endIndex_str.c_str());
		string geneName_str = geneInfo[3];
		string strand_str = geneInfo[4];
		string geneName = chromo+"."+startIndex_str+"."+endIndex_str+"."+geneName_str+"."+strand_str;

		if(startIndex <= startMatch && endMatch <= endIndex) // in the gene region
		{
			//add to match count
			//first have to find it in the map
			if(map1.count(geneName) == 1) // gene already in the map
			{
				map1[geneName] = map1[geneName]+1;
			}else{ // gene not yet in the map
				map1[geneName] = 1;
			}
		}else{ // no match
			map1[geneName] = 0;
		}

		if(map2.count(geneName) == 0) // if not already in our map2 then insert with total number of possible tiles
		{
			int tempGeneLen = endIndex - startIndex;
			int tempReadLen = endMatch - startMatch;
			map2[geneName] = tempGeneLen - tempReadLen + 1;
		}

	} //end of loop

	cout << "map1.size(): " << map1.size()<< endl;
	cout << "map2.size(): " << map2.size()<< endl;

	//iterate through the unordered map and calculate the mappability
	outFile << "GeneID\tReadLen100" << endl; // change for 70 and 100

	for(auto it = map1.begin(); it != map1.end(); ++it)
	{
		string gName = it->first;
		int gCount = it->second;

		int gTotalTiles = map2[gName];
		double mappability = (double)gCount / (double)gTotalTiles;
		mappability = mappability*100;
		outFile << gName << "\t" << mappability << endl;
	}

	inFile.close();
	outFile.close();
	return 0;
}
