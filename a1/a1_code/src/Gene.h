/*
 * Gene.h
 *
 *  Created on: Feb 1, 2016
 *      Author: juan
 */

#ifndef GENE_H_
#define GENE_H_

#include <vector>
#include "Exon.h"

using namespace std;

/*
 * Gene class
 * This class is used to represent the information associated with a gene
 * This class includes a vector of Exons and other gene specific information
 * Information is accessed via accessor functions
 */

class Gene {
private:
	static vector<string> geneList; // total unique genes we are looking at TODO not used yet
	vector<Exon> exons; // this is a vector of exons that belong to this gene
	string geneName; // gene name
	unsigned long int exonsNum; // number of exons we have for this particular gene (tracking)
	unsigned long int geneStart; // starting index of the gene
	unsigned long int geneEnd; // ending index of the gene
	bool isStrandPlus; // true if this gene is from the + strand. false if this is from the - strand
	string chromoName; // name of the chromosome this gene is from
public:
	/*
	 * Constructor takes in a string, bool, string
	 * string geneName = name of the gene
	 * bool isPlus = true if this is from the + strand, false if from the - strand
	 * string cName = chromosome name
	 */
	Gene(string, bool, string);
	string getName(); // return name of gene
	string getChromoName(); // get name of chromosome this gene is a part of
	/*
	 * addExon( ... )
	 * This function takes in
		 * string& chromosome name
		 * unsigned long int& sIndex - starting index of the gene
		 * unsigned long int& eIndex - ending index of the gene
		 * string& gName - name of the gene
		 * unsigned long int& eID - exon id
		 * bool strand - true for + strand; false for - strand
	 * and then creates a new instance of Exon and adds it to the vector of exons that belong
	 * to this particular gene
	 */
	void addExon(string&, unsigned long int&, unsigned long int&, string&, unsigned long int&, bool);
	unsigned long int getNumExons(); // get number of exons in this gene
	unsigned long int getGeneStart(); // get the starting index of this gene
	unsigned long int getGeneEnd(); // get the ending index of this gene
	bool isPlus(); // true if this gene belongs to + strand and false if belongs to the - strand
	void newGene(string&); // add new gene name to geneList
	void printSummary(); // print summary of info stored

	static bool geneExists(string&); // does given gene already exist in geneList TODO not used yet
	/*
	 * geneExists2( ... )
	 * this function takes in a reference to a vector of Genes and the name of a gene
	 * it then iterates through the vector of given gene and check if the given gene exists
	 * if so then it returns true and false if the gene is not found in the vector
	 * vector<Gene>& vecG - vector of Gene to search through
	 * string& gName - name of gene to search for in the vector
	 * Output: true if the gene is found in the vector else return false
	 */
	static bool geneExists2(vector<Gene>&, string&);

	/*
	 * maskIntrons( ... )
	 * This function takes in a whole gene as a string.
	 * It will then go through the given gene string and mask all the intron regions,
	 * those regions that are not part of an exon region
	 * using the current Gene's list (vector) of exons (regions)
	 * string& wGene - string representing the whole gene (bases)
	 * Output: string with the intron regions masked with 'N's
	 */
	string maskIntrons(string&); // mask introns sections
	static void printGeneList(); // print out the list of unique genes



	virtual ~Gene();
};

#endif /* GENE_H_ */
