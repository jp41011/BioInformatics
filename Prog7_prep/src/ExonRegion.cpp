/*
 * ExonRegion class definition
 * This class will store:
 * 	start index
 * 	end index
 * 	exonRegion name (ID)
 *
 */

#include <iostream>

using namespace std;

class ExonRegion
{
public:
	static int count; // count of how many ExonRegions there are
	// keep track of lowest/highest index reduce the number of check and improve performance
	static int minIndex;
	static int maxIndex;

	int startIndex;
	int endIndex;
	string charName;
	string exonRegionName;
	string zero;
	char strand;

	//Constructor with default values
	ExonRegion(int start = 0, int end = 0, string newName = "0")
	{
		startIndex = start;
		endIndex = end;
		charName = newName;
		exonRegionName = "exonRegion_" + to_string(ExonRegion::count++);
		zero = "0";
		strand = '+';

		// check if min and max need to be updated
		if(startIndex < ExonRegion::minIndex)
			ExonRegion::minIndex = startIndex;

		if(endIndex > ExonRegion::maxIndex)
			ExonRegion::maxIndex = endIndex;

		/*
		cout << "New Region: " << exonRegionName << endl;
		cout << "\tStart: " << startIndex << "\tEnd: " << endIndex << endl;
		*/
	}

	//print exon region information in formated way
	string getInfo()
	{
		return charName + "\t" + to_string(startIndex) + "\t" + to_string(endIndex) + "\t" + exonRegionName + "\t" + zero + "\t" + strand;
	}

	/*Takes in a starting index and an ending index
	 * if the starting index or the ending index over lap with the current
	 * exon region then expand the exon region if necessary.
	*/
	bool expandOverlap(int start, int end)
	{
		if( (startIndex <= start && start <= endIndex) || (startIndex <= end && end <= endIndex) )
		{
			if(start < startIndex) // if new start is lower that current start index
				startIndex = start; // update region start index

			if(end > endIndex) // if new end index is higher that current endindex
				endIndex = end; // update region end index

			// check if min and max need to be updated
			if(startIndex < ExonRegion::minIndex)
				ExonRegion::minIndex = startIndex;

			if(endIndex > ExonRegion::maxIndex)
				ExonRegion::maxIndex = endIndex;

			/*
			cout << "Expanded: " << exonRegionName << endl;
			cout << "\tStart: " << startIndex << "\tEnd: " << endIndex << endl;
			*/

			return true;
		}
		return false;
	}

	/* Takes in and index and returns true if that index
	 * is within the startIndex to endIndex range
	 */
	bool inRange(int i)
	{
		if(startIndex <= i && i <= endIndex)
			return true;
		else
			return false;
	}
};
