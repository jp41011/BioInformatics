/*
 * Sequence.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: juan
 */

#include "Sequence.h"

//default constructor set default values
Sequence::Sequence() {
	header = "";
	seq = "";
}

Sequence::Sequence(string inHeader, string inSeq){
	header = inHeader;
	seq = inSeq;
}

string Sequence::getHeader(){return header;}
string Sequence::getSeq(){return seq;}

void Sequence::setHeader(string inHeader){header = inHeader;}
void Sequence::setSeq(string inSeq){seq = inSeq;}

Sequence::~Sequence() {
	// TODO Auto-generated destructor stub
}

