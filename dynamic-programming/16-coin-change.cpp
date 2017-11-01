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
int count(const vector<int> &coins, const int value) {

	vector<vector<int> > dp_table((coins.size() + 1), vector<int>(value+1));

	for(int j = 0; j < (value+1); j++) {
		dp_table[0][j] = 0;
	}

	for(int i = 0; i < (coins.size()+1); i++) {
		dp_table[i][0] = 1;
	}

	for(int i = 1; i <= coins.size(); i++) {
		for(int j=1; j <= value; j++) {

			dp_table[i][j] = dp_table[i-1][j];
			if (coins[i-1] <= j) {
				dp_table[i][j] += dp_table[i][(j-coins[i-1])];
			}
		}
	}

	return dp_table[coins.size()][value];
}

//=============================================================================

int main(int argc, char **argv) {
	vector<int> coins = {1,2,3};
	int value = 4;
	cout << "Number of ways " <<  count(coins, value) << endl;

	return 0;
}

//=============================================================================
