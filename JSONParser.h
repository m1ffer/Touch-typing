#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>
#include <vector>
#include <utility>

using String = std::string;
using Word = String;

struct Quote {
    String text;
    String source;
    unsigned int id;
};

class JSONParser {
public:
    static std::pair<String, std::vector<Quote>> parseQuotes(String path);
    static std::pair<String, std::vector<Word>> parseWords(String path);
};

#endif // JSONPARSER_H
