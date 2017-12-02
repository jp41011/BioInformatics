/*
 * Sequence.h
 *
 *  Created on: Feb 28, 2016
 *      Author: juan
 */

#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include <string>

using namespace std;

class Sequence {
private:
	string header; // store header of the DNA Sequence
	string seq; // store actual sequence as string
public:
	Sequence();
	Sequence(string inHeader, string inSeq);
	string getHeader();
	string getSeq();
	void setHeader(string inHeader);
	void setSeq(string inSeq);
	virtual ~Sequence();
};

#endif /* SEQUENCE_H_ */
