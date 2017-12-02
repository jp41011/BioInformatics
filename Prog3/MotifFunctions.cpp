/*
 * MotifFunctions.cpp
 *
 *  Created on: Feb 29, 2016
 *      Author: juan
 */


#include "MotifFunctions.h"

/* function will take in
 * outputVec: a reference to an output vector of Medians to store the results
 * medianVec: vector of Medians as input to use for calculations
 * seqVec: vector of sequences
 * lMer = length of lmere currently using
 */
void startMotif(vector<Median>& medianVec, vector<Sequence> seqVec, int lMer)
{
	cout << "startMotif(...)" << endl;
	for(unsigned int i=0; i < medianVec.size(); i++)
	{
		vector<Motif> motifs (0);
		for(unsigned int j=0; j < seqVec.size(); j++)
		{
			string tempMedian = medianVec[i].getStr();
			string tempSeq = seqVec[j].getSeq();

			Motif tMotif = findMotif(tempSeq, tempMedian, lMer); // get best motif

			motifs.push_back(tMotif); // add new motif to vector
		}
		medianVec[i].setMotifs(motifs);
	}

	return;
}

Motif findMotif(string seqStr , string inMedian, int lMer)
{
	cout << "findMotif(...)" << endl;
	int curBestScore = 0; // initialize current best score to 0
	string curBestStr = ""; // initialize to empty
	int bestIndex = 0; // starting location of the best

	int lastIndex = seqStr.length() - inMedian.length() + 1; // last index that needs to be checked
	for(int i=0; i < lastIndex; i++)
	{
		int tempLen = i+inMedian.length();
		string tempSeq = seqStr.substr(i, tempLen);
		int tempScore = getScore(tempSeq, inMedian); // calculate score

		if(tempScore > curBestScore) // if better score
		{
			curBestScore = tempScore; // update best score
			curBestStr = tempSeq; // save new best seq
			bestIndex = i; // save the location of were the best seq was
		}
	}
	Motif finalMotif (curBestStr, bestIndex); // create motif with proper info
	return finalMotif;
}

/* Given 2 strings as input.
 * Calculate and return score comparing both input strings.
 * The more similar the strings the higher the score.
 * 1 char match = +1 point to score.
 */
int getScore(string seqStr, string inMedian)
{
	int finalScore = 0; // initialize to 0
	for(unsigned int i=0; i < inMedian.length(); i++)
	{
		if( tolower(seqStr[i]) == tolower(inMedian[i]) ) // if they match
		{
			finalScore++; // increment finalScore counter
		}
	}
	return finalScore;
}

/*Calculate and set the consensus score and string information,
 * for each median in the given median vector reference
 */
void calcConsensus(vector<Median>& medianVec, int lMer)
{
	for(unsigned int i=0; i < medianVec.size(); i++)
	{
		string curString = ""; // initialize to empty
		int curScore = 0; // initialize to 0

		for(int j=0; j < lMer; j++)
		{

			//unordered_map<Base,int> baseMap;
			BaseMap baseMap;
			vector<Motif> tempMotifVec = medianVec[i].getMotifs();
			for(unsigned int j=0; j < tempMotifVec.size(); j++)
			{
				string tString = tempMotifVec[j].getStr();
				//char tChar = tString[i];
				Base base1 = stringToBase(&tString[i]);

				int  tScore = baseMap[base1];
				baseMap[base1] = tScore + 1; // increment by 1

			}
			int tempScore=0; // initialize then pass by ref to getConsensus()
			Base base2 = getConsensus(baseMap, tempScore);
			curString = curString + baseToString(base2);

		}
	}
	return;
}


//Base getConsensus(unordered_map<Base, int> bMap, int& inScore)
Base getConsensus(BaseMap bMap, int& inScore)
{
	Base b1;
	for(unordered_map<Base, int>::iterator it = bMap.begin(); it != bMap.end(); it++)
	{
		int tVal = it->second;
		if(tVal > inScore)
		{
			inScore = tVal;
			b1 = it->first;
		}
	}
	return b1;
}


string baseToString(Base base)
{
	switch(base){
	case A:
		return "A";
	case C:
		return "C";
	case G:
		return "G";
	case T:
		return "T";
	}
}

Base stringToBase(string base)
{
	Base i;
	switch(base){
	case "A":
		i = A;
		return i;
	case "C":
		i = C;
		return i;
	case "G":
		i=G;
		return i;
	case "T":
		i=T;
		return i;
	}
}

