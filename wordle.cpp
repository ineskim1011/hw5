#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Checks if created permutation is a real word in the English dictionary 
bool isAWord(
		const std::string& in,
		const std::set<std::string>& dict)
{
		if (dict.find(in) != dict.end()) {
			return true;
		}
		return false;
}


// Wordle helper for recursive use
void wordMaker(
			std::string& semiWord,
			std::string floats,
			const std::set<std::string>& dict,
			std::set<std::string>& results,
			std::string& letters,
			size_t position, 
			size_t dashRemaining)
{
	// Holds original float string for backtracking purposes 
	std::string floatsHolder = floats;
	// If no dashes left in word, checks if permutation is a real word
	// If so, inserts into results 
	if (position == semiWord.length()) {
		if (isAWord(semiWord, dict)) {
			results.insert(semiWord);
		}
		return;
	}
	// If current position already has a letter, calls helper for next position in word 
	else if (semiWord[position] != '-') {
		wordMaker(semiWord, floats, dict, results, letters, position + 1, dashRemaining);
	}
	// If the remaining empty characters can only be floats (i.e. number of letters remaining is same number of floats)
	// iterate ONLY through floats (not the whole alphabet)
	else if (dashRemaining == floats.size()) {
		for (size_t i = 0; i < floats.size(); i++){
			semiWord[position] = floats[i];
			dashRemaining--; 
			// Erases current float from string so that future recursive calls on next positions 
			// will not repeat it 
			floats.erase(floats.begin() + i);
			// Recursive call to helper using next position 
			// Will iterate through floats ONLY due to number of floats being equal to remaining dashes 
			wordMaker(semiWord, floats, dict, results, letters, position + 1, dashRemaining);
			// Backtracking: get rid of current float and iterate to next float 
			dashRemaining++;
			floats = floatsHolder;
			semiWord[position] = '-'; 
		}
	}
	// Iterates through alphabet 
	else {
		for (size_t i = 0; i < letters.size(); i++) {
			semiWord[position] = letters[i];
			dashRemaining -= 1; 
			// If a floater is used, remove it from float string 
			for (size_t j = 0;  j < floats.size(); j++){
				if (floats[j] == letters[i]){
					floats.erase(floats.begin() + j);
					break;
				}
			}
			wordMaker(semiWord, floats, dict, results, letters, position + 1, dashRemaining);
			// Backtracking (same as for floaters)
			dashRemaining++;
			semiWord[position] = '-';
			floats = floatsHolder;
		}
	}
}

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
		std::set<std::string> results;
		std::string startWord = in;
		std::string floats = floating;
		// String to iterate through alphabet 
		std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
		// If word size is zero, return results which is empty 
		if (in.size() == 0) {
			return results;
		}

		// Counts number of dashes in the given string (to use in helper)
		int dashCount = 0;
		for (size_t i = 0; i < in.size(); i++) {
			if (in[i] == '-') {
				dashCount++;
			}
		}

		// Call helper starting at first position of given string (0th index)
		wordMaker(startWord, floats, dict, results, alphabet, 0, dashCount);
	
		return results;
}