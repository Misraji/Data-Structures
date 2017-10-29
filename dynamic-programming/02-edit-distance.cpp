// http://www.geeksforgeeks.org/dynamic-programming-set-5-edit-distance/
#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <algorithm>
using std::min;

int edit_distance(const string &str1, const string &str2) {
	
	vector<vector<int> > table(str1.size() + 1, vector<int>(str2.size() + 1));

	// When str2 is of size 0
	for(int i = 0; i < str1.size(); i++) {
		table[i][0] = i;
	}

	// When str1 is of size 0
	for(int j = 0; j < str2.size(); j++) {
		table[0][j] = j;
	}	

	for(int j = 1; j <= str2.size(); j++) {
		for (int i = 1; i <= str1.size(); i++) {
			
			if (str1[i] == str2[j]) {
				table[i][j] = table[i-1][j-1];

			} else {
				table[i][j] = 1 + min(
					table[i][j-1],			// Insert
					min( table[i-1][j],		// Delete
						 table[i-1][j-1]	// Replace
						) 
					);
			}
		}
	}

	return table[str1.size()][str2.size()];	
}

int main(int argc, char **argv) {

    // your code goes here
    string str1 = "sunday";
    string str2 = "saturday";

	cout << "Edit distance = " << edit_distance(str1, str2) << endl;

}
