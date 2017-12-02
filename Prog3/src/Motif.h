/*
 * Motif.h
 *
 *  Created on: Feb 28, 2016
 *      Author: juan
 */

#ifndef MOTIF_H_
#define MOTIF_H_

#include <string>

using namespace std;

class Motif {
private:
	string str; // motif string
	int index; // motif location in sequence

public:
	//constructors
	Motif();
	Motif(string, int);

	//setter functions
	void setStr(string);
	void setIndex(int);

	//getter functions
	string getStr();
	int getIndex();

	virtual ~Motif();
};

#endif /* MOTIF_H_ */
