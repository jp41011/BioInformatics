/*
============================================================================
Name        : Prog7_prep.cpp
Author      : Juan Pedraza
Version     : v2 - 5/2/16
Copyright   : Your copyright notice
Description : This program will take an annotation file as input and will output a
				formatted and masked output file.
				Also I sorted the annotation file by the starting index before inputting it
				into this program.
				Part 1: Collapse all gene annotations.
					Given the annotation file as input.
					So that
					chr1 6469 6628 NR_024540_exon_3_0_chr1_6470_r 0 -
 	 	 	 	 	chr1 6469 6631 NR_028269_exon_3_0_chr1_6470_r 0 -
 	 	 	 	 	Becomes:
 	 	 	 	 	chr1 6469 6631 x 0 +
 	 	 	 	Part 2: using the output file form part 1 we will then use it to
 	 	 	 		mask-compliment. Masking all non-exon regions (introns) with N's
 	 	 	 		Input for this section will be a chr1.fa file and the
 	 	 	 		output from Part1.
 	 	 	 Environment:
 	 	 	 	 Ubuntu 14.04 LTS - Intel i7 - Quad core - 8 GB
 	 	 	 	 Eclipse IDE
============================================================================
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "ExonRegion.cpp"
#include <limits.h>


using namespace std;

// initialize class static variables
int ExonRegion::count = 0;
int ExonRegion::minIndex = INT_MAX;
int ExonRegion::maxIndex = INT_MIN;


// forward declarations
void collapse_exons(string, vector<ExonRegion>&);

void print_collapse_exons(string, vector<ExonRegion>&);

void mask_introns(string, string, vector<ExonRegion>&);

void write_line(ofstream&, vector<char>&);

int main() {

	string annot_filename = "exon_annot_sorted_chr1.txt"; // annotation filename
	string chr1_filename = "chr1.fa"; // chr1 filename
	//string chr1_filename = "chr1_short.fa"; // chr1 filename for testing
	string chr_masked = "chr1_masked.fa"; // chr1 file with masked intron regions
	string exons_filename = "combined_exons.txt";

	vector<ExonRegion> exonVec (0);

	// calculate and generate combined exon regions
	collapse_exons(annot_filename, exonVec);

	// TODO have to run this again to generate this combined exon file
	// print genereated exon region information to file output
	print_collapse_exons(exons_filename, exonVec);

	/*
	cout << exonVec[0].getInfo() << endl;
	cout << exonVec[exonVec.size()-1].getInfo() << endl;
	*/

	cout << "Total Regions: " << exonVec.size() << endl;
	cout << "Min Index: " << ExonRegion::minIndex << endl;
	cout << "Max Index: " << ExonRegion::maxIndex << endl;


	//mask_introns(chr1_filename, chr_masked, exonVec);

	return 0;
}


/* This function will take in a file and an ExonRegion vector
 * It will then go through all the exons and combine all the exons whos
 * indexes (starting or ending) overlap
 * The ExonRegion vector will store all the distinct regions
*/
void collapse_exons(string exon_filename, vector<ExonRegion>& exonVec)
{
	ifstream inFile (exon_filename.c_str()); // input file with the exons

	string chromo_name = "";

	while(inFile >> chromo_name)
	{
		int start_index = 0;
		int end_index = 0;
		string exon_name = "";
		string zeroString = "";
		string strand = "";

		// get info from line
		inFile >> start_index;
		inFile >> end_index;
		inFile >> exon_name;
		inFile >> zeroString;
		inFile >> strand;

		bool expandedRegion = false; // expanded region flag
		// try to expand / merge this new exon with existing exon regions
		for(unsigned int i=0; i < exonVec.size(); i++)
		{
			expandedRegion = exonVec[i].expandOverlap(start_index, end_index);
			if(expandedRegion == true) // if this expanded a exon region
				break; // then break out of the loop
		}

		// if did not expand a region then have to create a new region and add to the vector
		if(expandedRegion == false)
		{
			ExonRegion tempExon (start_index, end_index, chromo_name);
			exonVec.push_back(tempExon);
		}

	}

	//clean up and close files
	inFile.close();

	return;
}

/* Function will print exon region information from a vector to an output file
 * Will take in:
 * string filename - name of file that should be created for output
 * vector<ExonRegion>& vec - vector with the exon region information
 */
void print_collapse_exons(string filename, vector<ExonRegion>& vec)
{
	ofstream outfile(filename);

	for(unsigned int i=0; i < vec.size(); i++)
	{
		outfile << vec[i].getInfo() << "\n";
	}

	outfile.close();
	return;
}

/* This function will take in 2 strings and a ExonRegion vector
 * infile string is filename of chr1 file to read in from
 * outfile string output file name to have the masked version of chr1
 * exonVec vector that holds the exon regions
 * use exonVec to mask intron regions of the chr1 file and save to the outfile
 */
void mask_introns(string infile, string outfile, vector<ExonRegion>& exonVec)
{
	ifstream chrFile(infile);
	ofstream outFile(outfile);

	string line="";
	getline(chrFile, line); // first line has the ">chr1"
	outFile << line << "\n"; // move that first line into the new output file
	char tempBase = '_';
	//string outline = "";
	vector<char> outline (0);
	int index = 0;

	while(chrFile >> tempBase) // get one char at a time
	{
		tempBase = toupper(tempBase); // conver to uppercase
		char outBase = '_'; // char to be outputted to file

		// if this char is an 'N' or the index is not in the min max range of all our exons leave as 'N'
		if(tempBase == 'N' || index < ExonRegion::minIndex || index > ExonRegion::maxIndex)
			outBase = 'N';
		else{
			// go through exonRegions and check if it belongs in one of those regions
			for(unsigned int i=0; i < exonVec.size(); i++)
			{
				if(exonVec[i].inRange(index) == true) // if in range
				{
					outBase = tempBase; //
					break;
				}
			}

			// could of set default case to 'N' but wanted to see how often this occurs
			if(outBase == '_') // didn't change from default
				outBase = 'N'; // mask to N because it wasn't in the range
		}

		outline.push_back(outBase); // add to the end of the output string
		if( (index+1) % 50 == 0) // 50th char on the line
			outline.push_back('\n'); // start a new line

		// minimize the number of I/O operations
		if(outline.size() >= 5000000) // arbitrarily picked 5,000,000 characters
		{
			write_line(outFile, outline); // write vector char contents to file
			outline.clear(); // clear vector
		}

		index++;
	}

	// if string buffer not empty then write the final buffer to the file
	if(outline.size() > 0)
		write_line(outFile, outline);


	//clean up
	chrFile.close();
	outFile.close();
	return;
}

// takes in an output file reference and an vector of characters to print to the file
void write_line(ofstream& outfile, vector<char>& vec)
{
	//cout << "Writing to file: " << vec.size() << endl;
	for(unsigned int i=0; i < vec.size(); i++)
	{
		outfile << vec[i];
	}
}

