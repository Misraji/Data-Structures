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

#include <unordered_set>
using std::unordered_set;

//=============================================================================

bool word_break(const string &str, const unordered_set<string> &dict) {

	// Row is length of substring, Column is start of string.
	vector<vector<bool> > dp_table((str.size() + 1), vector<bool>(str.size(), false));
	
	for(int j=0; j < str.size(); j++) {
		dp_table[0][j] = true;
	}

	for(int l = 1; l <= str.size(); l++) {
	
		for(int j = 0; j <= (str.size() - l); j++) {
		
			// if substring j- (j+l-1) is in dictionary, we are done.
			if (dict.count(str.substr(j, l)) > 0) {
				dp_table[l][j] = true;
				break;

			} else {
				// If not directly in substring, check for break.
				// k is now the sub-string length
				for(int k=1; k < l; k++) {
					if (dp_table[k][j] && dp_table[l-k][j+k]) {
						dp_table[l][j] = true;
						break;
					}
				}
			}
		}
	}

	return dp_table[str.size()][0];
}

//=============================================================================

int main(int argc, char **argv) {

	unordered_set<string> dict = {"mobile","samsung","sam","sung","man","mango",
	                           "icecream","and","go","i","like","ice","cream"};

	word_break("ilikesamsung", dict)? cout <<"Yes": cout << "No";
	cout << endl;

	word_break("samsungandmangok", dict)? cout <<"Yes": cout << "No";
	cout << endl;

	return 0;
}

//=============================================================================
