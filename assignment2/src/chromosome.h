#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>
#include <string>

using namespace std;

/* Chromosome class
 * Represents a chromosome and the information associated with the chromosome
 * All bases are stored in a vector of strings
 * chromosome information is access via accessor functions
 * This will typically be used after reading in the chromosome bases from a file
 */

class chromosome{
private:
	vector<string> baseLines; //each string in the vector is a string of bases
	unsigned long int lineCount; // how many strings in the vector
	unsigned long int baseCount; // how many total characters whole vector

public:
	// default constructor setting default values to the variables
	chromosome();//constructor
	~chromosome(); // deconstructor

	void addLine(string&); // add new line of bases to this chromosome

	// request particular base (nucleotide) given an index
	char getIndex(unsigned long int &);

	// request a sequence of bases given starting and ending index
	// using calls to the getIndex(...) function
	string getSeq(unsigned long int &, unsigned long int &);
	string getLine(unsigned long int&); // get a particular line (string of bases)
	unsigned long int getLineCount(); // how many lines of nucleotides do we have
	unsigned long int getBaseCount(); // how many total nucleotides bases do we have

};

//#include "chromosome.cpp"

#endif
