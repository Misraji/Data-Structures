// http://www.geeksforgeeks.org/longest-increasing-subsequence/
#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <algorithm>
using std::max;

// lis == Longest Increasing Subsequence
int lis(const vector<int> &arr) {

	// LIS values for all elements is 1 (themselves)
	vector<int> lis(arr.size(), 1);
	int i, j, ret = 0;

	for (int i = 1; i < arr.size(); i++) {
		for (int j = 0; j < i; j++) {
			if (arr[j] < arr[i]) {
				lis[i] = max<int>(lis[i], lis[j]+1);
			}
		}
	}

	for (int i = 0; i < arr.size(); i++) {
		ret = max(ret, lis[i]); 
	}

	return ret;
}

int main(int argc, char **argv) {

	vector<int> arr = { 10, 22, 9, 33, 21, 50, 41, 60 };
	cout << "Length of LIS is " << lis(arr) << endl;
	return 0;
}
