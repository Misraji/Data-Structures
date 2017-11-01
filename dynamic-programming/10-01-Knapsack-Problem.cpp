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

int knapsack(int W, const vector<int> &weight, const vector<int> &val) {
	
	vector<vector<int> > dp_table((weight.size()+1), vector<int>(W+1));

	// For no items in back, value of knapsack will be 0
	for(int j = 0; j <= W; j++) {
		dp_table[0][j] = 0;
	}

	// For target weight = 0, all subsets have value 0.
	for(int i = 0; i <= weight.size(); i++) {
		dp_table[i][0] = 0;
	}

	for (int i = 1; i <= weight.size(); i++) {
		for(int j=1; j <= W; j++) {
			dp_table[i][j] = dp_table[i-1][j];
			if (weight[i-1] <= j) {
				dp_table[i][j] = max(dp_table[i][j], 
						(val[i-1] + dp_table[i-1][(j - weight[i-1])]));
			}
		}
	}

	return dp_table[weight.size()][W];
}
//=============================================================================

int main(int argc, char **argv) {

	vector<int> val = { 60, 100, 120 };
	vector<int> weight = {10, 20, 30};
	int W = 50;

	cout << "Value = " << knapsack(W, weight, val) << endl;
	return 0;
}

//=============================================================================
