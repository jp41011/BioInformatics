/*============================================================================
Name        : Prog6.cpp
Author      : Juan Pedraza
Version     : v3 - 4/17/16
Copyright   : Your copyright notice
Description : The purpose of this project is to implement the Smith-Waterman algorithm with
			Affine gap scoring and trace-back operation.
			Protein data is used in for this project and BLOSUM62 scoring matrix is used.
			This program will read in a query file and a database file and will then call
			the Smith-Waterman function to get the optimum local alignment score.
			The query file will contain only 1 protein sequence.
			The database file will contain multiple protein sequences.
			So the program must call the Smith-Waterman function for each of the database sequences.
			Affine values:
				* open = -11
				* extension = -1
			Input:
				* Query fasta file containing 1 protein sequence (EColi-query1.fa)
				* Database fasta file containing multiple protein sequences (swissprot-100.fa)
					 only using the first 100 sequences of swissprot.fa
			Output:
				A text file showing (for each of the reference protein sequence):
				 * The DBseq label (1st 50 chars)
				 * Best local alignment score
				 * Its location [i, j] indices
				 * alignment

			Environment: Eclipse - Ubuntu 14.04 LTS - i7 (Quad core) - 8GB
============================================================================
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

//Gap open and extend penalties for blastp
#define GAP_OPEN 11
#define GAP_EXTEND 1

// global output file
ofstream outFile;

// codons/amino acids used for the scoring matrix
// * at the very end is the catch all case, else case
const int alphabet_size = 25;
const char alphabet[25] = {'A','R','N','D','C','Q','E','G','H','I','L','K','M','F','P','S','T','W','Y','V','B','J','Z','X','*'};


// Scoring Matrix given
const int scoreMatrix_size = 25;
const int scoreMatrix[25][25] = {
{ 4, -1, -2, -2,  0, -1, -1,  0, -2, -1, -1, -1, -1, -2, -1,  1,  0, -3, -2,  0, -2, -1, -1, -1, -4},
{-1,  5,  0, -2, -3,  1,  0, -2,  0, -3, -2,  2, -1, -3, -2, -1, -1, -3, -2, -3, -1, -2,  0, -1, -4},
{-2,  0,  6,  1, -3,  0,  0,  0,  1, -3, -3,  0, -2, -3, -2,  1,  0, -4, -2, -3,  4, -3,  0, -1, -4},
{-2, -2,  1,  6, -3,  0,  2, -1, -1, -3, -4, -1, -3, -3, -1,  0, -1, -4, -3, -3,  4, -3,  1, -1, -4},
{ 0, -3, -3, -3,  9, -3, -4, -3, -3, -1, -1, -3, -1, -2, -3, -1, -1, -2, -2, -1, -3, -1, -3, -1, -4},
{-1,  1,  0,  0, -3,  5,  2, -2,  0, -3, -2,  1,  0, -3, -1,  0, -1, -2, -1, -2,  0, -2,  4, -1, -4},
{-1,  0,  0,  2, -4,  2,  5, -2,  0, -3, -3,  1, -2, -3, -1,  0, -1, -3, -2, -2,  1, -3,  4, -1, -4},
{ 0, -2,  0, -1, -3, -2, -2,  6, -2, -4, -4, -2, -3, -3, -2,  0, -2, -2, -3, -3, -1, -4, -2, -1, -4},
{-2,  0,  1, -1, -3,  0,  0, -2,  8, -3, -3, -1, -2, -1, -2, -1, -2, -2,  2, -3,  0, -3,  0, -1, -4},
{-1, -3, -3, -3, -1, -3, -3, -4, -3,  4,  2, -3,  1,  0, -3, -2, -1, -3, -1,  3, -3,  3, -3, -1, -4},
{-1, -2, -3, -4, -1, -2, -3, -4, -3,  2,  4, -2,  2,  0, -3, -2, -1, -2, -1,  1, -4,  3, -3, -1, -4},
{-1,  2,  0, -1, -3,  1,  1, -2, -1, -3, -2,  5, -1, -3, -1,  0, -1, -3, -2, -2,  0, -3,  1, -1, -4},
{-1, -1, -2, -3, -1,  0, -2, -3, -2,  1,  2, -1,  5,  0, -2, -1, -1, -1, -1,  1, -3,  2, -1, -1, -4},
{-2, -3, -3, -3, -2, -3, -3, -3, -1,  0,  0, -3,  0,  6, -4, -2, -2,  1,  3, -1, -3,  0, -3, -1, -4},
{-1, -2, -2, -1, -3, -1, -1, -2, -2, -3, -3, -1, -2, -4,  7, -1, -1, -4, -3, -2, -2, -3, -1, -1, -4},
{ 1, -1,  1,  0, -1,  0,  0,  0, -1, -2, -2,  0, -1, -2, -1,  4,  1, -3, -2, -2,  0, -2,  0, -1, -4},
{ 0, -1,  0, -1, -1, -1, -1, -2, -2, -1, -1, -1, -1, -2, -1,  1,  5, -2, -2,  0, -1, -1, -1, -1, -4},
{-3, -3, -4, -4, -2, -2, -3, -2, -2, -3, -2, -3, -1,  1, -4, -3, -2, 11,  2, -3, -4, -2, -2, -1, -4},
{-2, -2, -2, -3, -2, -1, -2, -3,  2, -1, -1, -2, -1,  3, -3, -2, -2,  2,  7, -1, -3, -1, -2, -1, -4},
{ 0, -3, -3, -3, -1, -2, -2, -3, -3,  3,  1, -2,  1, -1, -2, -2,  0, -3, -1,  4, -3,  2, -2, -1, -4},
{-2, -1,  4,  4, -3,  0,  1, -1,  0, -3, -4,  0, -3, -3, -2,  0, -1, -4, -3, -3,  4, -3,  0, -1, -4},
{-1, -2, -3, -3, -1, -2, -3, -4, -3,  3,  3, -3,  2,  0, -3, -2, -1, -2, -1,  2, -3,  3, -3, -1, -4},
{-1,  0,  0,  1, -3,  4,  4, -2,  0, -3, -3,  1, -1, -3, -1,  0, -1, -2, -2, -2,  0, -3,  4, -1, -4},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -4},
{-4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,  1}
};

/* custom sequence class*/
class Sequence{
public:
	string name; // sequence name
	string seq; // actual string sequence
	// constructor and set default values if empty constructor
	Sequence(string newName = "", string newSeq = "")
	{
		name = newName;
		seq = newSeq;
	}

	//getter functions
	//string getName(){return name;} // get sequence name
	//string getSeq(){return seq;} // get sequence

	virtual ~Sequence(){
		//not doing anything here
	}
};

// enumeration to keep track of which direction the scoring came from
enum Direction {TopLeft, Top, Left, NoDir};

// custom Cell class that will score information about the cell of the matrix
// such as char, direction, and score
class Cell{
public:
	char ch;
	Direction dir;
	int val;
	bool isGap;

	// constructor with default variables
	Cell(char newCh = '_', Direction newDir = NoDir, int newVal = 0, bool newGap = false)
	{
		ch = newCh;
		dir = newDir;
		val = newVal;
		isGap = newGap;
	}
};


// advanced smith_waterman declaration. database sequence and query sequence
void smith_waterman(Sequence db_seq, Sequence query_seq);

// getter function to get the score from the scoring table given 2 chars
int getScore(char, char);

// take in a file and return the sequence information from that file
Sequence getSequence(string filename);

// take in file name and return a vector of all the sequences in that file
vector<Sequence> getAllSequences(string filename);

// function to get the max score and the direction. return by reference to the last 2 vars
void getMax_n_Dir(int TLval, int TopVal, int LeftVal, int& finalScore, Direction& finalDir);

// for debuggin print out the given matrix of cells
void printMatrix(string dbStr, string queryStr, vector< vector<Cell> > matrix);

int main() {

	string queryFile = "EColi-query1.fa";
	string dbFile = "swissprot-100.fa";
	string outputFile = "Prog6_output.txt";

	outFile.open(outputFile.c_str());
	outFile << "Juan Pedraza - Prog6 - Output File" << endl;
	outFile << "==================================" << endl << endl;

	Sequence query = getSequence(queryFile);
	vector<Sequence> dbSeqVec = getAllSequences(dbFile);

	outFile << "--query len = " << query.seq.length() << endl;
	outFile << query.name << endl;
	outFile << query.seq << endl << endl;

	/* testing
	cout << dbSeqVec.size() << endl;
	cout << dbSeqVec[0].getName() << endl << dbSeqVec[0].getSeq() << endl;
	*/
	//cout << query.getName() << endl; //testing
	//cout << query.getSeq() << endl;

	// todo later change from 1 to dbSeqVec.size()
	for(unsigned int i=0; i < dbSeqVec.size(); i++)
	{
		smith_waterman(dbSeqVec[i], query);
		//smith_waterman(query, dbSeqVec[i]);
	}

	outFile.close(); // clean up
	return 0;
}



/* getSequnce(string filename)
 * This function takes in a filename and then goes through the file and returns the sequences
 * information in a custom sequence data struct
 */
Sequence getSequence(string filename)
{
	ifstream infile(filename.c_str());
	string name = "";
	string seq = "";
	string temp = "";

	//first line should be the name of the sequence
	getline(infile, name);

	// next get the entire sequence that may be split over several lines
	while(getline(infile, temp))
	{
		seq+=temp;
	}

	Sequence returnVal(name, seq);

	infile.close();
	return returnVal;
}
/* getAllSequences(string filename)
 *
 */
vector<Sequence> getAllSequences(string filename)
{
	ifstream infile(filename.c_str());
	vector<Sequence> seqVec (0);

	string name = "";
	string seq = "";
	string temp = "";

	while(getline(infile, temp)) // first get the seq name and other information
	{
		name = temp;
		getline(infile, temp); // now get the seq itself
		seq = temp;
		Sequence tempSeq(name, seq); // create new temp sequence instance
		seqVec.push_back(tempSeq);
	}

	infile.close();
	return seqVec;
}

/* getScore(char db_char, char query_char)
 * Getter function for the scoring matrix.
 * Given 2 chars, one from the db sequence and one from the query sequence
 * this function will look up and return the value from the scoring matrix
 * Inputs:
 * 	db_char: character from the database sequence
 * 	query_char: character from the query sequence
 */
int getScore(char db_char, char query_char)
{
	int db_index = -1;
	int query_index = -1;
	//first go through and find the column for the db char
	for(int i=0; i < alphabet_size; i++)
	{
		if(alphabet[i] == db_char) // if this matches
			db_index = i; // store the index of the update
		if(alphabet[i] == query_char) // if this matches
			query_index = i; // store the index of the match

		//if we are at the end of the alphabet and we haven't found a match (still -1) then set to the catch all * [24]
		if(i == alphabet_size-1)
		{
			if(db_index == -1)
				db_index = i;
			if(query_index == -1)
				query_index = i;
		}
	}

	//now use the indexes and return the score
	return scoreMatrix[db_index][query_index];
}

/* smith_waterman implementation using affine gap scoring
 * Inputs:
 * 	db_seq: sequence that will act as the database sequence
 * 	query_seq: sequence that will act as the query sequence ... always the same for this project 6
*/
void smith_waterman(Sequence db_seq, Sequence query_seq)
{
	string db = "0" + db_seq.seq;
	string query = "0" + query_seq.seq;
	// get the lengths of the sequences with will be the dimensions of the matrix
	int dbLen = db.length(); // len of the top row
	int queryLen = query.length(); //len of the leftmost col

	//int matrix[dbLen][queryLen]; // automatically initialized to 0's
	//vector< vector<int> > matrix (dbLen, vector<int>(queryLen, 0) );

	//Cell tempNewCell = Cell('', NoDir, 0);

	vector< vector<Cell> > matrix(dbLen, vector<Cell>(queryLen, Cell() ) );

	//cout << "Row len: " << matrix.size() << endl;
	//cout << "Col len: " << matrix[0].size() << endl;


	int curMax = 0; // keep track of current max score
	int curMaxi = 0; // i index of current max score (x)
	int curMaxj = 0; // j index of current max score (y)

	//vector<bool> isPrevMatch(dbLen, true); // initialize with true

	//go through all cells in matrix and set scores
	for(int y=1; y < queryLen; y++)
	{
		for(int x=1; x < dbLen; x++)
		{
			// get values
			int topLeftVal = matrix[x-1][y-1].val;
			int topVal = matrix[x][y-1].val;
			int leftVal = matrix[x-1][y].val;

			int matchMismatchVal = getScore(db[x], query[y]); // get from scoring table
			int TLval = topLeftVal + matchMismatchVal; // the score if coming from the top left

			int topPenalty = 0;
			if(matrix[x][y-1].isGap) // if the cell above (top) is part of a gap
			{
				if(matrix[x][y-1].dir == Top) // extending the gap
				{
					topPenalty = GAP_EXTEND;
				}else // this is the start of a new gap
					topPenalty = GAP_OPEN + GAP_EXTEND; //open penalty and extend penalty
			}else // previous cell was not a gap so start of new gap
				topPenalty = GAP_OPEN + GAP_EXTEND;

			int TopVal = topVal - topPenalty; // the score if coming from the top

			int leftPenalty = 0;
			if(matrix[x-1][y].isGap) // if the left cell is also part of a gap
			{
				if(matrix[x-1][y].dir == Left) // extending the gap
					leftPenalty = GAP_EXTEND;
				else // new gap and extending
					leftPenalty = GAP_OPEN + GAP_EXTEND;
			}else // new gap
				leftPenalty = GAP_OPEN + GAP_EXTEND;

			int LeftVal = leftVal - leftPenalty;

			int finalScore = 0;
			Direction finalDir = NoDir;

			// get the max score and the direction
			getMax_n_Dir(TLval, TopVal, LeftVal, finalScore, finalDir);

			// store final value into the matrix
			matrix[x][y].val = finalScore;
			matrix[x][y].dir = finalDir;
			if(finalDir == Top || finalDir == Left)
				matrix[x][y].isGap = true;
			else
				matrix[x][y].isGap = false;

			// keep track of where the last current max value is located
			if(matrix[x][y].val > curMax)
			{
				curMax = matrix[x][y].val;
				curMaxi = x;
				curMaxj = y;
			}

		}
	}

	//printMatrix(db, query, matrix);

	// done with the matrix now do the traceback

	string db_align = "";
	string query_align = "";
	string diff_str = "";

	int curX = curMaxi;
	int curY = curMaxj;
	//int nextVal = matrix[curX][curY].val;

	int prevX=0;
	int prevY=0;

	// get the alignment sequences
	while(matrix[curX][curY].val != 0) // stop when you get to a 0
	{
		prevX = curX;
		prevY = curY;

		if(matrix[curX][curY].dir == TopLeft) // direction top left case
		{
			db_align += db[curX];
			query_align += query[curY];
			curX = curX-1;
			curY = curY-1;
		}else if (matrix[curX][curY].dir == Top) // direction top  case
		{
			db_align += "-";
			query_align += query[curY];
			//curX = curX; // stays the same
			curY = curY-1;
		}else if(matrix[curX][curY].dir == Left) // last option
		{
			db_align += db[curX];
			query_align += "-";
			curX = curX-1;
			//nextY = curY; // stays the same
		}else{ // should never get to here. or at a 0 and dir = NoDir
			cout << "Error" << endl;
		}

		//nextVal = matrix[nextX][nextY].val;
	}

	// now reverse the strings
	reverse(db_align.begin(), db_align.end() );
	reverse(query_align.begin(), query_align.end() );

	//generate the difference row
	for(unsigned i=0; i < db_align.length(); i++)
	{
		if(db_align[i] == query_align[i]){ // if the same letter then show same letter
			diff_str += db_align[i];
		}else if(db_align[i] == '-' || query_align[i] == '-'){ // if one of them is a gap snow nothing
			diff_str += "_";
		}else if(getScore(db_align[i], query_align[i]) >= 0){ // if letters different but score 0 or better show +
			diff_str += "+";
		}else{ // else insert nothing
			diff_str += "_";
		}
	}


	/*
	string testing = "abcdef";
	reverse(testing.begin(), testing.end());
	cout << "testing: " << testing;
	*/

	//outputfile printout
	outFile << db_seq.name << endl;
	outFile << "SW_Score = " << curMax << "\t";
	outFile << "(i = " << prevX << ", j = " << prevY << ")";
	outFile << "\talignment_len = " << query_align.length() << endl;

	outFile << "Query:\t" << prevY << "\t"  << query_align << "\t" << curMaxj << endl;
	outFile << "\t\t\t" << diff_str << endl; // difference row
	outFile << "Sbjct:\t" << prevX << "\t" <<  db_align << "\t" << curMaxi << endl << endl;
}


/* getMax_n_Dir()
 * Inputs:
 * 	TLVal - value from the top left cell
 * 	TopVal - value from the top cell
 * 	LeftVal - value from the left cell
 * 	finalScore - reference to store the max of the first three variables
 * 	finalDir - reference to store the direction of the max
 *
 * 	Order in which you check is important: TopLeft, Top, Left, then 0 case
 */
void getMax_n_Dir(int TLval, int TopVal, int LeftVal, int& finalScore, Direction& finalDir)
{
	// start off with top left
	int curVal = TLval;
	Direction curDir = TopLeft;

	// then check top
	if(TopVal > curVal)
	{
		curVal = TopVal;
		curDir = Top;
	}

	//then check left
	if(LeftVal > curVal)
	{
		curVal = LeftVal;
		curDir = Left;
	}

	// and finally check if its less than 0
	if(0 > curVal)
	{
		curVal = 0;
		curDir = NoDir;
	}

	//store final values in the reference variables
	finalScore = curVal;
	finalDir = curDir;
	return;
}

// for debugging print out the given matrix of cells
void printMatrix(string dbStr, string queryStr, vector< vector<Cell> > matrix)
{
	cout << endl << "===== Print Matrix =====" << endl;

	cout << " \t";
	for(unsigned int i=0; i < dbStr.length(); i++)
		cout << dbStr[i] << "\t";

	cout << endl;

	for(unsigned int y=0; y < queryStr.length(); y++)
	{
		cout << queryStr[y] << "\t";
		for(unsigned int x=0; x < dbStr.length(); x++)
		{
			cout << matrix[x][y].val << "\t";
		}
		cout << endl;
	}
	cout << "===============" << endl;
	return;
}

