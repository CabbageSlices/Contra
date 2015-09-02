#include "StringManipulator.h"

using std::string;

string extractFirstWordInString(string &toExtractFrom) {

    //find first word in string
    //ignore any leading whitespace
    size_t firstNonWhiteSpace = toExtractFrom.find_first_not_of(" \t");

    if(firstNonWhiteSpace == string::npos) {

        //no characters found
        return string();
    }

    //first white space after the sequance of non white space characters
    size_t whiteSpaceAfterFirstWord = toExtractFrom.find_first_of(" \t", firstNonWhiteSpace);
    size_t wordLength = whiteSpaceAfterFirstWord - firstNonWhiteSpace;

    string extractedWord = toExtractFrom.substr(firstNonWhiteSpace, wordLength);

    //remove the word from the original string
    toExtractFrom.erase(firstNonWhiteSpace, wordLength);
    return extractedWord;
}
