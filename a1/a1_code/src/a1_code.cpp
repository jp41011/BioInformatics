/*============================================================================
Name        : a1_code.cpp
Author      : Juan
Version     : v1
Copyright   : Your copyright notice
Description : This program reading in chromosome data from a file,
			and stores that information in a chromosome data structure.
			Reads in Gene and exon information from a file and also stores that in
			a vector of Genes.
			This program will then go through the vector of genes and use the exon information in
			each to get the appropriate sections from the chromosome class.
			The resulting strings will have the intron regions masked and only show the exon regions.
			For those genes from the ( - ) strand the resulting string will be the proper reverse compliment.
			The output genes will then be outputted and stored to a file.
//============================================================================
*/
#include <iostream>
#include <fstream> // to read and write to files
#include <sstream> // convert multi digit string to number
#include <algorithm> // for built in reverse
//#include <string> // string stuff
#include <ctime> // to time program run time
#include "chromosome.h" // my chromosome class
#include "Gene.h" // my gene class

using namespace std;

vector<string> Gene::geneList (0); // initialize class list of unique genes

string getReverseCompliment(string&); // function declaration
void getGenesFromFile(vector<Gene>&, string&); // function to get the genes and exon information from file and store

/*************************************************************************/
int main(int argc, char* argv[]) {
	clock_t beginTime = clock(); // startlineCount time

	//prep getting chromosome information from file and storing in chromosome class
	ifstream inFile; // chromosome base file
	string char1File = "chr1.fa";
	inFile.open(char1File.c_str());

	chromosome chromo = chromosome();
	vector<Gene> genesVec; // vectore to store Gene(s) information all in one place

	//reading in the gene exon regions from file
	string geneExonFile = "5RefSeqGene_sorted";
	getGenesFromFile(genesVec, geneExonFile);

	// prep for the output file
	ofstream outFile;
	string outputFile = "assignment1.fa";
	outFile.open(outputFile.c_str());

	string tempStr; // temp string
	/* debugging
	if(outFile.is_open())
		cout << "file is open" << endl;
	else
		cout << "file is NOT open" << endl;
	outFile << "testing" << endl;
	*/

	//Reading in information from chromosome file into the chromosome class
	getline(inFile, tempStr); // get the first line out of the way ">char1"
	//cout << "First line: " << tempStr << endl;

	while(getline(inFile, tempStr)) // start reading in bases
	{
		chromo.addLine(tempStr);
	}

	//summary of what was read
	cout << "Final Line count: " << chromo.getLineCount() << endl;
	cout << "chromo.length = " << chromo.getBaseCount() << endl;

	/*
	 * Go through each gene in the gene vector
	 * 	for each gene get the corresponding bases from the chromosome class
	 * 	then mask all the intron regions.
	 * 	if gene is from the ( + ) strand then finished
	 * 	if gene is from the ( - ) strand then we have to get the reverse compliment
	 * 		we do that by calling a function to get the reverse compliment of the given string (already masked)
	 * 	Then we write the masked gene out to a file with the summary information as a preface.
	 */
	for(unsigned int i=0; i < genesVec.size(); i++){
		unsigned long int geneStartIndex = genesVec[i].getGeneStart();
		unsigned long int geneEndIndex = genesVec[i].getGeneEnd();
		string geneString = chromo.getSeq(geneStartIndex, geneEndIndex);
		//cout << "GStart: " << geneStartIndex << "\t GEnd: " << geneEndIndex << endl;
		cout << "Gene: " << endl << geneString << endl;
		geneString = genesVec[i].maskIntrons(geneString); // geneString now has the introns masked with 'N'
		cout << "Masked: " << genesVec[i].getName() << endl;
		cout << geneString << endl;
		if(genesVec[i].isPlus() == false){ // if it is on the ( - ) strand then we have to get the reverse compliment
			geneString = getReverseCompliment(geneString);
			cout << "Reversed/Compliment: " << endl;
		}
		cout << "Final: " << endl << geneString << endl;

		outFile << ">" << genesVec[i].getChromoName() << "." << genesVec[i].getGeneStart() << "." << genesVec[i].getGeneEnd() << "." << genesVec[i].getName() << ".";
		outFile << (genesVec[i].isPlus() ? "+" : "-") << endl;
		outFile << geneString << endl;
		//geneString = getReverseCompliment(geneString);
		//cout << geneString << endl;
	}

	//clean up and summary stats
	outFile.close();
	inFile.close();
	clock_t endTime = clock(); // end time
	double totalTime = double(endTime - beginTime) / CLOCKS_PER_SEC;
	cout << "Approximate run time: " << totalTime << " seconds" << endl;
	return 0;
}
/*************************************************************************/

/* getReverseCompliment(string&)
 * Takes in a reference to a string which is composed of bases (nucleotides) and returns the reverse and compliment of the base sequence
 * Input: string of bases as a string
 * Output: string of complimentary base in reverse order
 */
string getReverseCompliment(string& inString)
{
	reverse(inString.begin(), inString.end()); // use built reverse function from <algorithm>
	char tChar;
	char opChar;
	string result = "";

	for(unsigned int i=0; i < inString.size(); i++)
	{
		//cout << "start loop" << endl;
		tChar = inString[i]; // temp storage. get next char
		//cout << "start loop: "<< tChar << endl;
		// get the complimentary nucleotide
		if(tChar == 'A' || tChar == 'a')
			opChar = 'T';
		else if (tChar == 'T' || tChar == 't')
			opChar = 'A';
		else if (tChar == 'C' || tChar == 'c')
			opChar = 'G';
		else if (tChar == 'G' || tChar == 'g')
			opChar = 'C';
		else if (tChar == 'N' || tChar == 'n')// incase of 'N' return 'N'
			opChar = 'N';
		else
			opChar = '?'; // flag - something went wrong

		result = result+opChar;
	}

	return result;
}

void getGenesFromFile(vector<Gene>& geneVec, string& fileName){
	ifstream geneFile; // chromosome base file
	geneFile.open(fileName.c_str()); // open gene file

	// information variables we need to create exon and add to gene
	string cName;
	unsigned long int sIndex;
	unsigned long int eIndex;
	string gName;
	unsigned long int eID;
	bool isStrandPlus;

	string tempLine;
	string tempS;
	char delimiter = '\t';

	vector<string> exonRow (0); // to store input from file before parsing it into sections

	while(getline(geneFile, tempLine)){
		stringstream stream(tempLine);

		while(getline(stream,tempS, delimiter)){
			exonRow.push_back(tempS);
		}
		cName = exonRow[0];
		stringstream stream1(exonRow[1]);
		stream1 >> sIndex;
		stringstream stream2(exonRow[2]);
		stream2 >> eIndex;

		gName = exonRow[3];
		isStrandPlus = (exonRow[5] == "+" ? true : false);

		//cout << "exonRow[4]: " << exonRow[4] << "\t isStrandPlus: " << (isStrandPlus ? "+" : "-") << endl;

		exonRow.clear();
		stringstream stream3(gName);
		while(getline(stream3, tempS, '_')){
			exonRow.push_back(tempS);
		}
		gName = exonRow[0]+"_"+exonRow[1];
		istringstream ss(exonRow[3]);
		ss >> eID;
		exonRow.clear();
		/*
		cout << cName << endl;
		cout << sIndex << endl;
		cout << eIndex << endl;
		cout << gName << endl;
		cout << eID << endl;
		cout << (isStrandPlus ? '+' : '-') << endl;
		*/

		if(Gene::geneExists2(geneVec, gName) == false) // gene is not already in our list
		{
			//cout << "New Gene: " << gName << endl;
			Gene newGene (gName, isStrandPlus, cName); // create new gene
			newGene.addExon(cName, sIndex, eIndex, gName, eID, isStrandPlus); // create new exon
			geneVec.push_back(newGene); // add new gene with exon to vector of genes
			cout << "New Gene added: " << gName << endl;
		}else{ //gene already exists so find and add a new exon
			for(unsigned long int i=0; i < geneVec.size(); i++)
			{
				if(gName == geneVec[i].getName()) // if you find the proper gene
				{
					cout << "Not new Gene: " << gName << endl;
					geneVec[i].addExon(cName, sIndex, eIndex, gName, eID, isStrandPlus);
					break;
				}
			}
		}
	}

	for(unsigned int i=0; i < geneVec.size(); i++){
		geneVec[i].printSummary();
	}

	/*
	Gene::printGeneList();
	string temp11 = "NM_003689";
	if(Gene::geneExists(temp11))
		cout << "Gene NM_003689 exists" << endl;
	else
		cout << "Gene NM_003689 NOT exists" << endl;
	*/

	geneFile.close(); // close before returning
	return;
}
