#ifndef DATATAGPAIR_H_INCLUDED
#define DATATAGPAIR_H_INCLUDED

#include <utility>
#include <string>

//every piece of data saved has a tag
//this tagpair stores the begin and end tag for some type of data
//first in the pair is the tag at the beginning of the data block, and the second item is the ending tag.
typedef std::pair<std::string, std::string> DataTagPair;

#endif // DATATAGPAIR_H_INCLUDED
