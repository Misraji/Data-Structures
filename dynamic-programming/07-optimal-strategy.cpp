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

// Assuming a
int optimal_strategy(const vector<int> &values) {

	vector<vector<int> > dp_table(values.size(), vector<int>(values.size(), 0));
	// min values for individual values are 0 
	
	// max values for individual elements are their own values.
	for (int i = 0; i < values.size(); i++) {
		dp_table[i][i] = values[i];
	}

	for(int l = 1;  l <= values.size(); l++) {
		for (int i = 0, j=i+l; j < values.size(); i++, j++ ) {
			int x, y, z;

			x = ((i+2) <= j) ? dp_table[i+2][j] : 0;
			y = ((i+1) <= (j-1)) ? dp_table[i+1][j-1] : 0;
			z = (i <= (j-2)) ? dp_table[i][j-2] : 0;

			dp_table[i][j] = max( (values[i] + min(x,y) ),
								(values[j] + min(y,z) ) );
			
		}
	}

	return dp_table[0][values.size()-1];
}
//=============================================================================

int main(int argc, char **argv) {

	vector<int> values = {8, 15, 3, 7};
	cout << optimal_strategy(values) << endl;

	values = {2, 2, 2, 2};
	cout << optimal_strategy(values) << endl;

	values = {20, 30, 2, 2, 2, 10};
	cout << optimal_strategy(values) << endl;

	return 0;
}

//=============================================================================
