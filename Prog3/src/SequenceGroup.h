/*
 * SequenceGroup.h
 *
 *  Created on: Feb 28, 2016
 *      Author: juan
 */

#ifndef SEQUENCEGROUP_H_
#define SEQUENCEGROUP_H_

#include <iostream>
#include <vector>
#include "Median.h"
#include "Sequence.h"
#include <limits.h>
#include <ctype.h> // tolower()
#include <queue> // priority queue
#include <string>
#include <stdlib.h>

using namespace std;

// class used specifically to compare Medians for the priority queue
class MedianCompare{
public:
	bool operator()(Median m1, Median m2)
	{
		// want the highest total distance to be the lowest ... right?
		return m1.getTotalDistance() > m2.getTotalDistance();
	}
};

class SequenceGroup {
private:
	int curIndex; // keep track of current index
	vector<Sequence> seqVec; // vector of DNA sequences
	int lMer; // store length of lemer
	int kSize; //
	vector<char> s; // leaf nodes ... called S in alot of notes
	int leavesVisited; // keep track of number of leaves
	int PQ_Size; // priority queue size

	priority_queue<Median, vector<Median>, MedianCompare> priorityQ;

public:
	SequenceGroup();
	SequenceGroup(vector<Sequence>& inSeqVec, int inLmer, int inK);

	vector<Median> getMedians();
	string getText(vector<char>, int);
	string getText(vector<char>);

	int getTotalDistance(string);
	int getDistance(string, string);

	void byPass();
	void nextVertex();

	char nextChar(char);

	vector<Median> getBestMedians();

	virtual ~SequenceGroup();
};

#endif /* SEQUENCEGROUP_H_ */
