/*
 * SequenceGroup.cpp
 *
 *  Created on: Feb 28, 2016
 *      Author: juan
 */

#include "SequenceGroup.h"

SequenceGroup::SequenceGroup() {
	curIndex = 0;
	//seqVec (0);
	lMer = 0;
	kSize = 0;
	//s(0);
	leavesVisited = 0;
	PQ_Size = 5;
	//priorityQ (0);
}

SequenceGroup::SequenceGroup(vector<Sequence>& inSeqVec, int inLmer, int inK){
	curIndex = 0;
	seqVec = inSeqVec;
	lMer = inLmer;
	kSize = inK;
	//s(0);
	leavesVisited = 0;
	PQ_Size = 5;
	//priorityQ (0);
}

vector<Median> SequenceGroup::getMedians(){
	cout << "In getMedians()" << endl;
	//initialize the s vector with 1's AKA A's (base)
	s.clear(); // make sure its empty before we start using
	for(int i=0; i <= lMer; i++) // add an extra one so that indices match pseudo code
		s.push_back('1'); // 1 => A, 2 => C ... etc.

	int bestDistance = INT_MAX; // set the initial best dist to max possible
	curIndex = lMer; // set starting point

	while(curIndex > 0)
	{
		cout << "\tfront of while loop curIndex: " << curIndex << endl;
		if(curIndex < lMer) // non leaf node
		{
			cout << "\tnon leaf node" << endl;
			/*//something about the bounded method is not right
			string prefix = getText(s, curIndex);
			int tempDist = getTotalDistance(prefix);
			if(tempDist > bestDistance){
				byPass(); // skip subtree don't need to look at children nodes
			}else{
				nextVertex();
			}
			*/
			nextVertex();
		}else{ // leaf node
			cout << "\tleaf node" << endl;
			cout << "\tcurIndex: " << curIndex << endl;
			string tWord = getText(s);
			cout << "\ttWord: " << tWord << endl;
			leavesVisited++; // increment leaf visited count;
			int tDist = getTotalDistance(tWord);
			cout << "\tcurIndex: " << curIndex << endl;
			cout << "\ttDist: " << tDist << endl;
			cout << "\tbestDistance: " << bestDistance << endl;
			if(tDist < bestDistance){ // if better distance (smaller = better)
				bestDistance = tDist; // update bestDistance to new best distance
				cout << "\tNew bestDistance: " << bestDistance << endl;
			}
			Median tempMedian (tWord, tDist); // create new temp Median
			priorityQ.push(tempMedian);
			cout << "\tadded to priorityQ" << endl;

			nextVertex(); // move on to next vertex
		}
		cout << "\tend of while loop curIndex: " << curIndex << endl;
	}
	// finally return vector of medians
	return getBestMedians();
}

string SequenceGroup::getText(vector<char> inputS, int inputIndex){
	cout << "getText(inputS, inputIndex)" << endl;
	string final = "";
	for(int i=0; i <= inputIndex; i++)
		final = final+inputS[i];

	//string final = (inputS.begin(), inputS.begin()+inputIndex+1);

	return final;
}

// convert string from numbers to their corresponding bases
string SequenceGroup::getText(vector<char> inputS){
	cout << "getText(inputS)" << endl;
	string final = ""; // final string
	for(unsigned int i=1; i < inputS.size(); i++)
	{
		if(inputS[i] == '1')
			final += 'A';
		else if(inputS[i] == '2')
			final += 'C';
		else if(inputS[i] == '3')
			final += 'G';
		else if(inputS[i] == '4')
			final += 'T';
		else // should not get to this part
			final += 'N';
	}
	return final;
}

// calculates the total distance. looking at all sequences
int SequenceGroup::getTotalDistance(string inWord)
{
	cout << "getTotalDistance( ... )" << endl;
	int totalDistance = 0; //initialize total distance

	for(unsigned int i=0; i < seqVec.size(); i++)
	{
		string tSequence = seqVec[i].getSeq();
		int tempBestDist = INT_MAX;

		//calculate last index we have to check. since we don't have to go to the very end
		int lastIndex = tSequence.length() - inWord.length() + 1;

		for(int i2 = 0; i2 < lastIndex; i2++)
		{
			string seqChars = tSequence.substr(i2, i2+inWord.length());
			int tDist = getDistance(seqChars, inWord); // calculate distance from 2 strings

			if(tDist < tempBestDist) // if new distance is smaller
				tempBestDist = tDist; // update the current best distance
		}
		totalDistance += tempBestDist; // accumulate into totalDistance
	}

	return totalDistance;
}

// given two strings calculates distance
// the more similar they are the lower the distance
int SequenceGroup::getDistance(string seqV, string leafV)
{
	//cout << "getDistance( ... )" << endl;
	int dist = 0;
	for(unsigned int i=0; i < seqV.length(); i++)
	{
		if( tolower(seqV[i]) != tolower(leafV[i]) ) // if they dont match
			dist++; //increase dist count
	}
	return dist;
}

// byPass all child nodes and go to next node on the same level
void SequenceGroup::byPass()
{
	cout << "byPass()" << endl;

	for(int j=curIndex; j>=1; j--)
	{
		int ts = s[j] - '0'; // convert char to int
		if(ts < kSize)
		{
			s[j] = nextChar(s[j]); // set next chare
			curIndex = j; // update index
			return; // exit
		}
	}
	//if went through all of them then set index to 0
	curIndex = 0;
	return;


	/*for(int i = curIndex; i >= 1; i--)
	{
		int si = (int)s[i] - '0';
		if( si < kSize){
			s[i] = nextChar(s[i]);
			curIndex = i;
			return;
		}
	}
	curIndex = 0; // end
	return;
	*/
}

void SequenceGroup::nextVertex(){
	cout << "nextVertex()" << endl;
	cout << "\t curIndex: " << curIndex << endl;
	cout << "\t kSize: " << kSize << endl;
	cout << "\t lMer: " << lMer << endl;

	if(curIndex < lMer){ // non leaf
		curIndex++;
		s[curIndex] = '1';
		cout << "\t curIndex: " << curIndex << endl;
		return;
	}else{ // leaf
		for(int i = lMer; i > 0; i--)
		{
			cout << "\t s[" << i << "]: " << s[i]; // << endl;
			/*
			char ts [1];
			ts[1] = s[i];
			int si = atoi(ts);
			*/
			int si = (int)s[i] - '0';
			cout << "\t " << si << endl;
			if( si < kSize)
			{
				s[i] = nextChar(s[i]);
				curIndex=i;
				cout << "\t curIndex: " << curIndex << endl;
				return;
			}
		}
	}
	curIndex = 0; // default if it reaches this part
	cout << "\t curIndex: " << curIndex << endl;;
	return;
}

// get next char (number)
char SequenceGroup::nextChar(char in){
	cout << "nextChar(...)" << endl;
	char out = (char)in+1;
	return out;
}

// returns the PQ_Size best medians that have been found
vector<Median> SequenceGroup::getBestMedians()
{
	cout << "getBestMedians()" << endl;
	cout << "\tPQ_Size: " << PQ_Size << endl;
	cout << "\tpriorityQ.size: " << priorityQ.size() << endl;

	vector<Median> bestMedians (0);
	for(int i=0; i < PQ_Size; i++) // && i < priorityQ.size()
	{
		Median temp = priorityQ.top();
		cout << "\t" << temp.getStr() << ": " << temp.getTotalDistance() << endl;
		priorityQ.pop();
		bestMedians.push_back(temp);
	}
	cout << "\tgetBestMedians() - after loop" << endl;
	for(unsigned int i=0; i < bestMedians.size(); i++)
		cout << bestMedians[i].getStr() << endl;

	return bestMedians;
}

SequenceGroup::~SequenceGroup() {
	// TODO Auto-generated destructor stub
}

