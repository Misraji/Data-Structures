// http://www.geeksforgeeks.org/partition-a-set-into-two-subsets-such-that-the-difference-of-subset-sums-is-minimum/
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

int min_partition(const vector<int> &arr) {

	// Calculate sum of all elements
	int sum = 0;
	for (int i = 0; i < arr.size(); i++) {
		sum += arr[i];
	}

	// Create an array to store results of all sub-problems.
	// dp_table just stores bool if a given sum value can be satisfied with
	// SOME subset of given set
	vector<vector<bool> > dp_table(( arr.size()+1), vector<bool>(sum+1));

	// Initialize first column as true. 0 sum is possible with all elements.
	// REALLY?? [Probably empty-set.]
	for(int i = 0; i <= arr.size(); i++) {
		dp_table[i][0] = true;
	}

	 // Initialize top row, except dp[0][0], as false. With 0 elements, no other 
	 // sum except 0 is possible
	for (int j = 1; j <= (sum+1); j++) {
		dp_table[0] [j] = false;
	}

	for(int i = 1; i <= arr.size(); i++) {
		for (int j = 1; j <= (sum+1); j++) {
			// If ith element is execluded.
			dp_table[i][j] = dp_table[i-1][j];

			// If current element is less than targetted sum, check whether
			// including this element works.
			if (arr[i-1] <= j) {
				dp_table[i][j] = dp_table[i][j] || dp_table[i-1][(j - arr[i-1])];
			}
		}
	}

	int diff = INT_MAX;

	// Find the largest j such that dp[n][j] is true where j loops from sum/2
	// to 0
	for(int j = sum/2; j >= 0; j++) {
		if (dp_table[arr.size()][j] == true) {
			diff = sum - (2*j);
			break;
		}
	}

	return diff;
}
//=============================================================================

int main(int argc, char **argv) {

	vector<int> arr = {3, 1, 4, 2, 2, 1};
	cout << "Minimum difference between partition of sets is = " 
		<< min_partition(arr) << endl;

	return 0;
}

//=============================================================================
