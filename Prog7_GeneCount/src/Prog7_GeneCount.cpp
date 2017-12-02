/*
============================================================================
 Name        : Prog7_GeneCount.cpp
 Author      : Juan Pedraza
 Version     : v1 - 5/2/16
 Copyright   : Your copyright notice
 Description : This is a helper programmer for Prog7 (CSci 291T)
				This program will take in a formatted Bowtie output and will also take in a
				exon annotation file.
				1. will use the exon annotation file to create a map/list of all exons and their regions
					which includes their starting and ending index
				2. will read in an already generated bowtie output file and will go through and count
					all instances of exon and gene reads
				3. will output to file a summary of all the gene read counts
============================================================================
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Gene.cpp"
// #include "Exon.cpp" // already included in the Gene.cpp

using namespace std;

void read_genes_exons(string, vector<Gene>&);
void geneVec_addExon(vector<Gene>&, Exon);
void countGenes(string, vector<Gene>&);
void printGeneCounts(string, vector<Gene>&);


int main() {

	string exon_annot_fileName = "HG19-refseq-exon-annot-chr1-2016"; // exon annotation file name
	string BTfileName = "BTout_m1_v3_formatted"; // bowtie output file name
	string gene_count_fileName = "geneCount.txt"; // file name to store the gene count summary

	vector<Gene> geneVec (0);

	read_genes_exons(exon_annot_fileName, geneVec);

	/* for testing
	for(unsigned int i=0; i<geneVec.size(); i++)
	{
		cout << geneVec[i].geneID << "\t" << geneVec[i].exons.size() << endl;
		for(unsigned j=0; j<geneVec[i].exons.size(); j++)
		{
			cout << "\t" << geneVec[i].exons[j].exonStart << "\t" << geneVec[i].exons[j].exonEnd << "\t" << geneVec[i].exons[j].exonID << endl;
		}
	}
	*/
	/* testing
	int exonCount = 0;
	for(unsigned int i=0; i<geneVec.size(); i++)
	{
		exonCount += geneVec[i].exons.size();
	}
	cout << "Total Exons: " << exonCount << endl;
	*/

	countGenes(BTfileName, geneVec); // count the genes
	printGeneCounts(gene_count_fileName, geneVec); // print results to output file

	return 0;
}

/* read_genes_exons(string filename, vector<Gene>& geneVec)
 * Will take in filename to read from and an reference to a gene vector
 * to store the gene and exon information that is read in from the file
 */
void read_genes_exons(string filename, vector<Gene>& geneVec)
{
	ifstream inFile (filename.c_str());
	string line = "";
	//int testCount = 1; // just for testing

	while(getline(inFile, line)) // get the whole line from the file
	{
		stringstream lineStream(line); // convert to a stream
		// values that will be read in from the line
		string chromoID = "";
		int startIndex = 0;
		int endIndex = 0;
		string geneExonStr = "";
		string zero = "";
		char strand = '_';

		// now actually get the values
		lineStream >> chromoID;
		lineStream >> startIndex;
		lineStream >> endIndex;
		lineStream >> geneExonStr;
		lineStream >> zero;
		lineStream >> strand;

		// now have to split geneExonStr into its parts to get the geneID and the exonID ... don't need the rest
		stringstream geneExonStream(geneExonStr);
		string geneID = ""; // this will be the whole geneID
		string geneID1 = ""; // need these 2 because the gene name has a _ in the name so have to combine them after getting
		string geneID2 = ""; // both parts of the geneID
		string exonID = "";
		string temp = ""; // just to store string that needs to be removed from buffer but we don't actually need
		getline(geneExonStream, geneID1, '_'); // get the first part of the geneID
		getline(geneExonStream, geneID2, '_'); // get the second part
		getline(geneExonStream, temp, '_'); // have to remove the '_exon_'
		getline(geneExonStream, exonID, '_'); // now actually get the exonID
		geneID = geneID1+"_"+geneID2; // combine to get whole geneID

		// create new exon to be added to the gene vector
		Exon newExon (chromoID, geneID, exonID, startIndex, endIndex, strand);
		geneVec_addExon(geneVec, newExon); // add new exon to the gene vector

		/* for testing
		if(testCount >= 50)
			break;
		else
			testCount++;
		*/
	}

	inFile.close();
	return;
}

/* this fucntion will take in a gene vector and an exon
 * it will go through the vector of genes and look for the gene that this new exon
 * belongs to. if it finds the gene then it will add it to the vectors of exons for
 * that gene. if a gene is not found then will create a new gene in the gene vector
 * and add this exon to that new gene.
 */
void geneVec_addExon(vector<Gene>& geneVec, Exon exon)
{
	// first have to look to see if gene already exists
	for(unsigned int i=0; i<geneVec.size(); i++)
	{
		if(geneVec[i].geneID == exon.geneID) // found existing gene
		{
			//now add to the list of exons for this gene
			geneVec[i].addExon(exon);
			return;
		}
	}
	// when through all the genes and did not find an already existing one
	// have to create a new gene entry and insert exon into there
	Gene newGene(exon.chromoID, exon.geneID);
	geneVec.push_back(newGene); // add new gene to vector
	geneVec[geneVec.size()-1].addExon(exon); // add new exon to newly created gene

	return;
}

/* countGenes(string filename, vector<Gene>& geneVec)
 * Takes in a filename to read from
 * Takes in reference to a vector of genes
 * Will go through the file and get the start and ending indexes
 * goes through each exon in each gene and if the starting and ending indexes are a subset
 * of the exon range then will increase the count for that exon and gene
 */
void countGenes(string filename, vector<Gene>& geneVec)
{
	ifstream infile (filename.c_str());
	string line = "";

	while(getline(infile, line)) // get whole line from the file
	{
		stringstream ss(line); // convert line into stream
		int startIndex = 0;
		int endIndex = 0;
		string temp = ""; // to move garbage
		ss >> temp; // chr1
		ss >> startIndex;
		ss >> endIndex;

		// go through all genes and exons
		for(unsigned int i=0; i < geneVec.size(); i++)
		{
			bool found = geneVec[i].inRange(startIndex, endIndex);
			if(found == true) // found
				break; // then don't have to check all the other genes and exons so break out
		}

	}

	infile.close();
	return;
}

/* prints to output file summary counts of the genes */
void printGeneCounts(string filename, vector<Gene>& geneVec)
{
	ofstream outfile(filename.c_str());

	for(unsigned int i=0; i < geneVec.size(); i++)
	{
		outfile << geneVec[i].geneID << "\t" << geneVec[i].count << endl;
	}

	outfile.close();
	return;
}
