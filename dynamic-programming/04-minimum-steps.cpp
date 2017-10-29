// http://www.geeksforgeeks.org/count-number-of-ways-to-cover-a-distance/
#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <algorithm>
using std::max;
using std::min;

//=============================================================================

int count_ways(const int distance) {

	int count[distance+1];

	// G4G initializes count[0] as 1. WTF?? 
	// They also start the loop from i=3 instead of i=4. 
	// Have no clue what is going on there.
	count[0] = 0; count[1] = 1; count[2] = 2; count[3] = 3;

	for(int i = 4; i <= distance; i++) {
		count[i] = count[i-1] + count[i-2] + count[i-3];
	}

	return count[distance];
}

//=============================================================================

int main(int argc, char **argv) {

	int dist = 4;
	cout << "Number of ways = " << count_ways(4) << endl;
	return 0;
}

//=============================================================================
