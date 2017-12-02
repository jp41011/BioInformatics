/*============================================================================
 Name        : Prog4.cpp
 Author      : Juan Pedraza
 Version     : v3 - 3/14/16
 Copyright   : Your copyright notice
 Description : CSci 291T - BioInformatics
				The purpose of Prog4 assignment is to build a non-redundant
				microbiome database from HMP (Human Microbiome Project) and
				MetaHIT databases. Non-redundant union.
				Input files: MetaHIT.fa and HMP.fa
					Also use BLASTn with
						Evalue = 10^-5
						Alignment identity >= 95%
						90% <= alignment_length / query_length <= 110%
				Output file: output.txt
				Steps:
					1) Run makeblastdb with HMP-2000.fa (will be the DB first)
					2) Run blastn with HMP2000 as the DB and MetaHIT-2000.fa as the query -> blast_DB_HMP2000
						w/ options evalue=1e-5 and -perc_identity 95
					3) Read in blast_DB_HMP2000 find all those queries where this is true: 90% <= alignment_length / query_length <= 110
						and put them in listA
					4) remove all sequences that are in listA from the MetaHIT.fa file creating -> MetaHIT-nr-HMP.fa file
					5) MetaHIT-nr-HMP.fa file now because your database and HMP-2000.fa is the query
					6) Run makeblastdb with MetaHIT-nr-HMP.fa
					7) Run blastn with MetaHIT_HMP as the DB and HMP-2000.fa as the query -> blast_DB_MetaHIT_HMP
						w/ options evalue=1e-5 and -perc_identity 95
					8) Read in blast_DB_MetaHIT_HMP and keep track of sequences where this is true: 90% <= alignment_length / query_length <= 110
						and put them in listB
					9) remove all sequences that are in listB from HMP2000.fa creating -> HMP2000-nr-(MetaHIT-nr-HMP).fa
					10) Combine sequences from MetaHIT-nr-HMP.fa and HMP2000-nr-(MetaHIT-nr-HMP).fa -> final.fa

============================================================================

*/

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>

using namespace std;

//typedef unordered_map<string, vector<string> > UOMap;
typedef unordered_map<string, int> UOMap;

fstream inFile;
//ofstream outFile;

/* Inputs:
 * UOMap map1: unordered_map of sequences names
 * string iFile: input file name
 * string oFile: output file name
 * Descriptions: Will open file with iFile name and will copy over all the sequences that are not in map1 over to oFile.
 * 	So oFile name will have all the sequences that where in iFile name and not in map1.
 */
void fastaRemove(UOMap map1, string iFile, string oFile)
{
	fstream inF(iFile);
	ofstream outF(oFile);

	string fileLine = "";

	//while(getline(inF, fileLine)) // read in lines from file
	while(inF >> fileLine)
	{
		if(fileLine[0] == '>') // starts with '>'
		{
			// then its a seq name so check if it is in the map
			string tempSeqName = fileLine.substr(1); // remove just the first '>'
			if(map1.count(tempSeqName) == 1) // was in list of seq to be removed
			{
				if(iFile == "HMP-2000.fa")
					inF >> fileLine; // get the rid of 2nd string on the first line only if this file is HMP-2000.fa

				inF >> fileLine; // get the sequences out of the way
			}else{ // sequence was not in list so move that to the new file
				outF << fileLine << endl; // write sequence name to new file

				if(iFile == "HMP-2000.fa")
					inF >> fileLine; // only if this is HMP-2000.fa which has extra stuff on the first line

				inF >> fileLine; // move the seq line out of the way

				//outF << fileLine << endl; // add the seq line to the file output
			}
		}
	}

	inF.close();
	outF.close();
}

int main() {
	UOMap map1;
	stringstream ss;

	inFile.open("blast_DB_HMP2000"); // file with results of blast 1st iteration
	//inFile.open("blast_output2");
	string tempLine = ""; // to store line from file

	while(getline(inFile, tempLine))
	{
		if(tempLine.find("Query=") != string::npos) // found query
		{
			//bool endOfQuery = false;
			ss.clear();
			ss.str(tempLine); // put string into stream
			string tempS = ""; // temp
			ss >> tempS; // to remove the "Query=" from the beginning
			string queryName = "";
			ss >> queryName; // store query name
			// next line is the query length need to store that
			//getline(inFile, tempLine); // next 2 lines empty
			inFile >> tempLine; // now has the line with "Length=xxx"
			size_t eqPosition = tempLine.find("=");
			eqPosition++; // remove the =


			tempS = tempLine.substr(eqPosition); // number after "Length=" aka query length
			ss.clear();
			ss.str(tempS);
			int queryLen = 0;
			ss >> queryLen; // get query length

			while(getline(inFile, tempLine)) // now go through and look for query name and alignment numbers
			{
				if(tempLine.find("Effective") != string::npos) // means this line starts with "Effective"
				{
					//endOfQuery = true;
					break; // break out of this loop
				}
				if(tempLine.find("Identities") != string::npos) // line has "Identities" in the line
				{
					size_t slashPosition = tempLine.find("/");
					slashPosition++;
					tempS = tempLine.substr(slashPosition); // get the digits of the denominator number
					ss.clear();
					ss.str(tempS);
					ss >> tempS; // sound only be the next number (space delimited

					ss.clear();
					ss.str(tempS);

					int alignLen = 0;
					ss >> alignLen; // get number
					float ratio = float(alignLen) / float(queryLen); // get the ratio
					if(float(0.90) <= ratio && ratio <= float(1.10)) // matches the criteria
					{
						// then store the query name so that we can remove it from the fasta file later
						map1[queryName] = 1; // insert query name into map ... 2nd value 1 ... just because don't really need it anyway
					}
				}

			} // end of while
			//if(endOfQuery == true)
			//	break; // break out of the out while loop too. and start new query loop
		}
	}

	cout << "unordered_map size: " << map1.size() << endl;
	//fastaRemove(map1, "MetaHIT-20000.fa", "MetaHIT-nr-HMP.fa"); // first iteration
	fastaRemove(map1, "MetaHIT-20000.fa", "MetaHIT-nr-HMP0.fa"); // first iteration with only the sequence names and not the sequences
	//fastaRemove(map1, "HMP-2000.fa", "HMP-nr-(MetaHIT-nr-HMP).fa");

	inFile.close(); // clean up
	return 0;
}
