#ifndef FILEMANIPULATORS_H_INCLUDED
#define FILEMANIPULATORS_H_INCLUDED

#include <fstream>
#include <string>
#include <sstream>

//takes the given file and places the stream reader to the line past the given line
//if the line isn't found it places the stream to the end of file
bool readAfterLine(std::fstream &file, const std::string &line);

//save a block of text enclosed by two tags from a file to the given string stream
bool saveBlockToStringStream(std::fstream &file, const std::string &beginTag, const std::string &endTag, std::stringstream &destination);

#endif // FILEMANIPULATORS_H_INCLUDED
