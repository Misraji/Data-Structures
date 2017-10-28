// Good Explanation: http://jakeboxer.com/blog/2009/12/13/the-knuth-morris-pratt-algorithm-in-my-own-words/
// Code: http://www.sanfoundry.com/cpp-program-implement-kruth-morris-patt-algorithm-kmp/
//
// Code: http://www.geeksforgeeks.org/searching-for-patterns-set-2-kmp-algorithm/

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

//=============================================================================

// preKMP algorithm in the code.
void gen_partial_match_table(const string &pattern, int table[]) {
	int k = -1;

	table[0] = -1;

	for (int i = 1; i < pattern.length(); i++) {
		k = table[i-1];
		while (k >= 0) {
			if (pattern[k] == pattern[i-1]) {
				break;

			} else {
				k = table[k];
			}

			table[i] = k + 1;
		}
	}
}

bool kmp(const string &pattern, const string &target) {

	int table[pattern.length()];
	gen_partial_match_table(pattern, table);

	int i = 0, k = 0;

	while (i < target.length()) {
		if (k == -1) {
			i++;
			k = 0;

		} else if (target[i] == pattern[k]) {
			i++, k++;
			if (k == pattern.length()) {
				cout << "'" << pattern << "' found in target: '" << target <<  
					"' at index: " << i << "." << endl;
				return true;
			}

		} else {
			k = table[k];
		}
	}

	cout << "'" << pattern << "' not found in target: '" << target << "'" << endl;
	return false;
}

int san_foundry_main() {
	string target = "san and linux training";
	string pattern = "lin";

	kmp(pattern, target);

	pattern = "sanfoundry";
	kmp(pattern, target);

	return 0;
}

//=============================================================================
// Geek-For-Geeks solution

void compute_lps_array(const char *pattern, const int pattern_len, int *lps) {

	//	length of previous longest prefix-suffix.
	//	This is also the END character of the PREFIX of the pattern being
	//	currently matched to the character i at the end of the PATTERN.
	int len = 0;
	
	//	lps[0] is ALWAYS 0.
	lps[0 ] = 0;

	// The loop calculates lps[i] for i = 1 to pattern_len-1
	int i = 1;
	while (i < pattern_len) {
		if (pattern[len] == pattern[i]) {
			lps[i++] = ++len;

		} else if (len != 0) {
			//	This is tricky. Consider the example 
			//	AAACAAAA and i = 7. The idea is similar to the search step.
			//	The value of len is 3, at this stage
			len = lps[len-1];
	
			// len characters of PREFIX match len characters BEFORE i (i.e.
			// AAA before C, == AAA before last A.
			// Thus by len = lps[len-1], we are trying to figure how much of
			// characters we have already matched. Its kind of like splitting
			// AAA.

			// Note that we do not assign any value to lps
			// Also, note that we do not increment i here.

		} else {
			lps[i++] = 0;
		}
	}

	// Note that in the above loop, len <= i in loop. That is, i is the
	// pattern index being incremented. len is the INDEX of the END character of
	// the PREFIX of the pattern being matched.
	
	return;
}

void kmp_search(const char *pattern, const char *txt) {
	
	const int PATTERN_LEN = strlen(pattern);
	const int TXT_LEN = strlen(txt);

	int lps[PATTERN_LEN];
	compute_lps_array(pattern, PATTERN_LEN, lps);

	int i = 0 ; // index for txt[]
	int j = 0 ; // index for pattern[]

	while (i < TXT_LEN) {

		if (txt[i] == pattern[j]) {
			i++, j++;

			if (j == PATTERN_LEN) {
				cout << "Found pattern at index " << (i-j) << endl;
				j = lps[j-1];
			}

		} else if (j) {
			j = lps[j-1];

		} else {
			i++;
		}
	}
}

int g4g_main(int argc, char **argv ) {
    const char *txt = "ABABDABACDABABCABAB";
    const char *pat = "ABABCABAB";
    
	kmp_search(pat, txt);
    return 0;
}

//=============================================================================

int main(int argc, char **argv) {
	
	(void) san_foundry_main();
	cout << endl;

	(void) g4g_main(argc, argv);
}
