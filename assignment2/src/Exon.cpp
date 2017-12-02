/*
 * Exon.cpp
 *
 *  Created on: Feb 1, 2016
 *      Author: juan
 */

#include "Exon.h"

// set default constructor values
Exon::Exon() {
	chromoName = "chromoName0";
	startIndex = 0;
	endIndex = 0;
	geneName = "geneName0";
	exonID = 0;
	isPlus = true;
	pattern = "";
}

Exon::Exon(string& cName, unsigned long int& sIndex, unsigned long int& eIndex, string& gName, unsigned long int& eID, bool strand, string pattern0 =""){
	chromoName=cName;
	startIndex = sIndex;
	endIndex = eIndex;
	geneName = gName;
	exonID = eID;
	isPlus = strand;
	pattern = pattern0;
}

// accessor functions definitions
string Exon::getChromoName(){return chromoName;}
unsigned long int Exon::getStart(){return startIndex;}
unsigned long int Exon::getEnd(){return endIndex;}
string Exon::getGeneName(){return geneName;}
unsigned long int Exon::getExonID(){return exonID;}
bool Exon::isPlusStrand(){return isPlus;}
bool Exon::isInExon(unsigned long int checkIndex){
	//if(checkIndex >= startIndex && checkIndex < endIndex)
	if(startIndex <= checkIndex && checkIndex < endIndex)
		return true;
	else
		return false;
}
void Exon::printSummary(){
	cout << "---- Exon ----" << endl;
	cout << "Chromo: " << chromoName << endl;
	cout << "Gene: " << geneName << endl;
	cout << "Exon: " << exonID << endl;
	cout << "Strand: " << (isPlus ? '+' : '-') << endl;
	cout << "Start: " << startIndex << endl;
	cout << "End: " << endIndex << endl;
	cout << "Pattern: " << pattern << endl;
	cout << "---- -----" << endl;
}

string Exon::getPattern(){return pattern;}

//Destructor
Exon::~Exon() {
	// Nothing to do here
}

