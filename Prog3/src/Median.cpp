/*
 * Median.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: juan
 */

#include "Median.h"

Median::Median() {
	str = "";
	totalDistance = 0;
	consensusStr = "";
	consensusScore = 0;
}

Median::Median(string inStr, int inTotalDist){
	str = inStr;
	totalDistance = inTotalDist;
	consensusStr = "";
	consensusScore = 0;
}

// setter functions
void Median::setStr(string in){str = in;}
void Median::setTotalDistance(int in){totalDistance = in;}
void Median::setConsensusStr(string in){consensusStr = in;}
void Median::setConsensusScore(int in){consensusScore = in;}
void Median::setMotifs(vector<Motif>& in){motifs = in;}

//getter functions
string Median::getStr(){return str;}
int Median::getTotalDistance(){return totalDistance;}
string Median::getConsensusStr(){return consensusStr;}
int Median::getConsensusScore(){return consensusScore;}
vector<Motif> Median::getMotifs(){return motifs;}

Median::~Median() {
	// TODO Auto-generated destructor stub
}

