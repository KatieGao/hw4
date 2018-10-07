#include "Dictionary.h"
#include "Exceptions.h"

#include <algorithm>
#include <fstream>
#include <iostream>

Dictionary::Dictionary(std::string dictionary_file_name) {
    std::ifstream dictFileStream(dictionary_file_name);

	if(!dictFileStream)
	{
		throw FileException("DICTIONARY");
	}

    while(!dictFileStream.eof())
	{
		std::string word;

		dictFileStream >> word;

        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
		this->all_words.push_back(word);
	}

    std::sort(this->all_words.begin(), this->all_words.end());
}

bool Dictionary::isLegalWord(std::string const & word) const {
    return std::binary_search(this->all_words.begin(), this->all_words.end(), word);
}

Dictionary::~Dictionary() {
    
}