#ifndef STRINGMANIPULATOR_H_INCLUDED
#define STRINGMANIPULATOR_H_INCLUDED

#include <string>

//takes out the first word in the string and returns it
//changing teh string so that it no longer contains the word
//all whitespaces will remain in the string
//words are any sequence of characters seperated by white spaces
//returns empty string if nothing is found
std::string extractFirstWordInString(std::string &toExtractFrom);

#endif // STRINGMANIPULATOR_H_INCLUDED
