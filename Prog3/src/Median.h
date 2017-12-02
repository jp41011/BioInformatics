/*
 * Median.h
 *
 *  Created on: Feb 28, 2016
 *      Author: juan
 */

#ifndef MEDIAN_H_
#define MEDIAN_H_

#include <string>
#include <vector>
#include "Motif.h"

using namespace std;

class Median {
private:
	string str; // actual median string
	int totalDistance; // total distance
	string consensusStr; // consensus string
	int consensusScore; // consensus score

	vector<Motif> motifs; // vector with motifs

public:
	Median();
	Median(string, int);

	//setter functions
	void setStr(string);
	void setTotalDistance(int);
	void setConsensusStr(string);
	void setConsensusScore(int);
	void setMotifs(vector<Motif>&);

	//getter functions
	string getStr();
	int getTotalDistance();
	string getConsensusStr();
	int getConsensusScore();
	vector<Motif> getMotifs();

	virtual ~Median();
};

#endif /* MEDIAN_H_ */
