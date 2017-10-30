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

bool has_subset_sum(const vector<int> &set, int sum) {
	
	vector<vector<bool> > dp_table((set.size()+1), vector<bool>(sum + 1));

	// If sum is not 0 and set is empty, then answer is false
	for(int j = 1; j <= sum; j++) {
		dp_table[0][j] = false;
	}

	// If sum is 0, then answer is true
	for(int i = 0; i <= set.size(); i++) {
		dp_table[i][0] = true;
	}

	for(int i = 1; i <= set.size(); i++) {
		for(int j = 1; j <= sum; j++) {

			int curr_elem = set[i-1];

			// Let us take care of the case where element set[i] is NOT a
			// member of the subset.
			dp_table[i][j] = dp_table[i-1][j];

			if (curr_elem <= j) {
				dp_table[i][j] = dp_table[i][j] || 
					dp_table[i-1][(j - curr_elem)];
			}
		}
	}

	return dp_table[set.size()][sum];
}
//=============================================================================

int main(int argc, char **argv) {

	vector<int> set = {1,2,3};
	int sum = 7;

	if (has_subset_sum(set, sum)) {
		cout << "Found subset with given sum." << endl;

	} else {
		cout << "Could not find subset with given sum." << endl;

	}

	return 0;
}

//=============================================================================
