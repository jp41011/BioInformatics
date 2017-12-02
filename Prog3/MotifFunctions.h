/*
 * MotifFunctions.h
 *
 *  Created on: Feb 29, 2016
 *      Author: juan
 */

#ifndef MOTIFFUNCTIONS_H_
#define MOTIFFUNCTIONS_H_

#include <iostream>
#include <vector>
#include <unordered_map>
#include "Median.h"
#include "Sequence.h"


using namespace std;

enum Base {A, C, G, T};

//typedef unordered_map<Base, int> BaseMap;

void startMotif(vector<Median>&, vector<Sequence>, int);
Motif findMotif(string, string, int);
int getScore(string, string);
void calcConsensus(vector<Median>&, int);
//Base getConsensus(BaseMap, int&);
string baseToString(Base);
Base stringToBase(string);

#endif /* MOTIFFUNCTIONS_H_ */
