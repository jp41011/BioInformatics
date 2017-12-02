/*
 * Gene class definition
 * Will store Gene information such as all the exons in this gene
 */

#include <iostream>
#include <vector>
#include <limits.h>
#include "Exon.cpp"

using namespace std;

class Gene{
public:
	string chromoID;
	string geneID;
	int geneStart;
	int geneEnd;
	int count;
	vector<Exon> exons;

	// constructor with default values
	Gene(string newChromo="chr1", string newGeneID="GeneID")
	{
		chromoID = newChromo;
		geneID = newGeneID;
		geneStart = INT_MAX;
		geneEnd = INT_MIN;
		count = 0;
		exons = vector<Exon> (0);
	}

	// adding new exon to the list of exons that belong to this gene
	void addExon(Exon ex)
	{
		// see if the gene start and ending index values need to be expanded
		if(ex.exonStart < geneStart)
			geneStart = ex.exonStart;
		if(ex.exonEnd > geneEnd)
			geneEnd = ex.exonEnd;

		exons.push_back(ex);
	}

	/* inRange will take a starting and ending index and return true if those values
	 * are a subset of any of the exons in this gene, otherwise returns false
	 */
	bool inRange(int start, int end)
	{
		// if start and end are out of range of this current gene then return false
		if(start < geneStart || end > geneEnd)
			return false;

		//else go through all exons and see if it is part of any one of them
		for(unsigned int i=0; i < exons.size(); i++)
		{
			bool isSubset = exons[i].inRange(start, end);
			if(isSubset == true)
			{
				count++; // increase count
				return true;
			}
		}

		// if gets to here then it was not part of any exon
		return false;
	}



};
