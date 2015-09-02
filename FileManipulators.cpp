#include "FileManipulators.h"

using std::fstream;
using std::string;

bool readAfterLine(fstream &file, const string &line) {

    //first place the stream operator to the beginning of the file
    file.seekg(0, std::ios_base::beg);

    string readDestination;

    while(getline(file, readDestination)) {

        if(readDestination == line) {

            //found the required position and the getline function automatically places stream reader to next lien
            return true;
        }

        //not found so clear the destination string for future read
        readDestination = "";
    }

    //stream reader should be at end of file since the given line wasn't found
    return false;
}
