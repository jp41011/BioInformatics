/*
 * Gene.cpp
 *
 *  Created on: Feb 1, 2016
 *      Author: juan
 */

#include "Gene.h"
#include <limits.h>
#include <iostream>

Gene::Gene(string gName="", bool isPlus=true, string cName="cName0") {
	geneName = gName;
	Gene::geneList.push_back(gName);
	exons = vector<Exon> (0);
	exonsNum = 0;
	geneStart = ULONG_MAX;
	geneEnd = 0;
	isStrandPlus = isPlus;
	chromoName=cName;
}

string Gene::getName(){return geneName;}

string Gene::getChromoName(){return chromoName;}

void Gene::addExon(string& cName, unsigned long int& sIndex, unsigned long int& eIndex, string& gName, unsigned long int& eID, bool strand){
	Exon tempExon(cName, sIndex, eIndex, gName, eID, strand); // create new exon with proper info
	exons.push_back(tempExon); // add new exon to list of exons
	exonsNum++; // increase exon count
	//check if we have to update the global gene starting and ending indexs
	if(sIndex < geneStart)
		geneStart = sIndex;
	if(eIndex > geneEnd)
		geneEnd = eIndex;
	//check if this is a new gene that is not already in the list of geneList
	if(geneExists(gName))
		newGene(gName); // if gene name not already in list then add to the list

	return;
}
unsigned long int Gene::getNumExons(){return exonsNum;}
unsigned long int Gene::getGeneStart(){return geneStart;}
unsigned long int Gene::getGeneEnd(){return geneEnd;}
bool Gene::isPlus(){return isStrandPlus;}

// return true if this gene is already in the list of geneList (genes we have seen)
bool Gene::geneExists(string& gName){
	for(unsigned long int i=0; i < geneList.size(); i++)
	{
		if(geneList[i] == gName)
			return true;
	}

	return false;
}

bool Gene::geneExists2(vector<Gene>& vecG, string& gName){
	for(unsigned int i=0; i < vecG.size(); i++){
		if(vecG[i].getName() == gName)
			return true;
	}
	return false;
}

// adds new gene name to the list of geneList
void Gene::newGene(string& gName){
	geneList.push_back(gName);
	return;
}

string Gene::maskIntrons(string& wGene){
	unsigned long int curIndex;
	string maskedString = "";
	bool foundInExon = false; // flag
	//cout << "gStart: " << geneStart << "\t gEnd: " << geneEnd << endl;

	for(unsigned long int i=0; i < wGene.length(); i++){
		curIndex = geneStart + i; // current index we are at
		foundInExon = false;
		for(unsigned long int e=0; e < exons.size(); e++){ // go exon by exon and see if this index is part of an exon
			if(exons[e].isInExon(curIndex)){ // index is part of this exon

				maskedString += wGene[i]; // keep the base (nucleotide) don't mask it
				foundInExon = true;
				break; // don't have to keep checking the rest of the exons if we already know its part of an exon
			}
		}
		//went through all the exons and did not find one which this index was a part of
		if(foundInExon == false)
			maskedString += 'N';
	}
	return maskedString;
}

void Gene::printGeneList(){
	std::cout << "GeneList: " << endl << "\t";
	for(unsigned int i=0; i < geneList.size(); i++)
		std::cout << Gene::geneList[i] << ", ";
	std::cout << "----" << endl;
}

void Gene::printSummary(){
	std::cout << "---- Gene -----" << endl;
	std::cout << "Gene name: " << geneName << endl;
	std::cout << "Start: " << geneStart << endl;
	std::cout << "End: " << geneEnd << endl;
	std::cout << "Exons: " << exonsNum << endl;
	std::cout << "Strand: " << (isStrandPlus ? "+" : "-") << endl;
	std::cout << "---- -----" << endl;
}

Gene::~Gene() {
	// empty out both vectors and free up space
	geneList.clear();
	exons.clear();
}

