/*
 * Exon.h
 *
 *  Created on: Feb 1, 2016
 *      Author: juan
 */

#ifndef EXON_H_
#define EXON_H_

#include <string>
#include <iostream>
using namespace std;

/*
 * Exon Class
 * This class is used to represent a gene exon and the information and the information related to the exon
 * Information stored in variables
 * Access information through accessor functions
 */

class Exon {
private:

	string chromoName; // string to store the name of the chromosome this exon is from
	unsigned long int startIndex; // starting index of the exon
	unsigned long int endIndex; // ending index of the exon
	string geneName; // name of the gene this exon is a part of
	unsigned long int exonID; // exon id number
	bool isPlus; // strand type + or -

public:
	// constructor functions creates exon and sets default values 2nd constructor should normally be used
	Exon();
	/* constructor that takes in the following information
	 * string& chromosome name
	 * unsigned long int& starting index of the exon
	 * unsigned long int& ending index of the exon
	 * string& gene name
	 * unsigned long int& exon id
	 * bool isPlus flag set to true if this is from the + strand
	*/
	Exon(string&, unsigned long int&, unsigned long int&, string&, unsigned long int&, bool);
	// accessor functions
	string getChromoName(); // returns the name of the chromosome (chromoName)
	unsigned long int getStart(); // returns the exon starting index (startIndex)
	unsigned long int getEnd(); // returns the exon ending index (endIndex)
	string getGeneName(); // returns the name of the gene (geneName)
	unsigned long int getExonID(); // returns the exon id number (exondID)
	bool isPlusStrand(); // returns true if the exon comes from the + strand. returns false otherise
	bool isInExon(unsigned long int); // returns true if the given index is part of this exon
	void printSummary(); // prints summary of the exon information

	virtual ~Exon(); // free up memory on destruction
};

#endif /* EXON_H_ */
