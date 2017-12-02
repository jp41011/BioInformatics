/*
 * Motif.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: juan
 */

#include "Motif.h"

Motif::Motif() {
	str = "";
	index = 0;
}

Motif::Motif(string inStr, int inIndex){
	str = inStr;
	index = inIndex;
}
//setter functions
void Motif::setStr(string inStr){str = inStr;}
void Motif::setIndex(int inIndex){index = inIndex;}
//getter functions
string Motif::getStr(){return str;}
int Motif::getIndex(){return index;}

Motif::~Motif() {
	// TODO Auto-generated destructor stub
}

