/*
 * Exon.cpp
 * Exon class definition
 * This class will store exon related information
 */

#include <iostream>
#include <limits.h>

using namespace std;

class Exon{
public:
	string chromoID;
	string geneID;
	string exonID;
	int exonStart;
	int exonEnd;
	char strand;
	int count;

	// default constructor with default values
	Exon(string newChromo="char1", string newGeneID="", string newExonID="0", int newStart=INT_MAX, int newEnd=INT_MIN, char newStrand='_')
	{
		chromoID = newChromo;
		geneID = newGeneID;
		exonID = newExonID;
		exonStart = newStart;
		exonEnd = newEnd;
		strand = newStrand;
		count = 0;
	}

	/* inRange(int start, int end )
	 * Takes in 2 indexes a starting index and an ending index
	 * Will then check if both of these indexes are in the range of this exon
	 * if they are both completely in the range then returns true
	 * otherwise returns false
	 */
	bool inRange(int start, int end)
	{
		// In this exon region
		if(exonStart <= start && start <= exonEnd && exonStart <= end && end <= exonEnd)
		{
			count++; // increase exon count
			return true;
		}
		else
			return false;
	}

};
