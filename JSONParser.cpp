#include "JSONParser.h"
#include <fstream>
#include <sstream>

std::pair<String, std::vector<Quote>> JSONParser::parseQuotes(String path) {
    std::vector<Quote> quotes;
    String language;

    std::ifstream file(path);
    if (!file.is_open()) {
        return {language, quotes};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    String content = buffer.str();
    file.close();

    // Parse language
    size_t langPos = content.find("\"language\":");
    if (langPos != String::npos) {
        size_t startQuote = content.find('\"', langPos + 11);
        size_t endQuote = content.find('\"', startQuote + 1);
        if (startQuote != String::npos && endQuote != String::npos) {
            language = content.substr(startQuote + 1, endQuote - startQuote - 1);
        }
    }

    // Parse quotes array
    size_t quotesStart = content.find("\"quotes\":");
    if (quotesStart != String::npos) {
        size_t arrayStart = content.find('[', quotesStart);
        size_t arrayEnd = content.find(']', arrayStart);

        if (arrayStart != String::npos && arrayEnd != String::npos) {
            String quotesArray = content.substr(arrayStart, arrayEnd - arrayStart + 1);

            size_t pos = 0;
            while (pos < quotesArray.length()) {
                size_t quoteStart = quotesArray.find('{', pos);
                if (quoteStart == String::npos) break;

                size_t quoteEnd = quotesArray.find('}', quoteStart);
                if (quoteEnd == String::npos) break;

                String quoteObj = quotesArray.substr(quoteStart, quoteEnd - quoteStart + 1);

                Quote quote;

                // Parse id
                size_t idPos = quoteObj.find("\"id\":");
                if (idPos != String::npos) {
                    size_t idStart = quoteObj.find_first_of("0123456789", idPos);
                    size_t idEnd = quoteObj.find_first_not_of("0123456789", idStart);
                    if (idStart != String::npos) {
                        String idStr = quoteObj.substr(idStart, idEnd - idStart);
                        quote.id = std::stoi(idStr);
                    }
                }

                // Parse source
                size_t sourcePos = quoteObj.find("\"source\":");
                if (sourcePos != String::npos) {
                    size_t sourceStart = quoteObj.find('\"', sourcePos + 9);
                    size_t sourceEnd = quoteObj.find('\"', sourceStart + 1);
                    if (sourceStart != String::npos && sourceEnd != String::npos) {
                        quote.source = quoteObj.substr(sourceStart + 1, sourceEnd - sourceStart - 1);
                    }
                }

                // Parse text
                size_t textPos = quoteObj.find("\"text\":");
                if (textPos != String::npos) {
                    size_t textStart = quoteObj.find('\"', textPos + 7);
                    size_t textEnd = quoteObj.find('\"', textStart + 1);
                    if (textStart != String::npos && textEnd != String::npos) {
                        quote.text = quoteObj.substr(textStart + 1, textEnd - textStart - 1);
                    }
                }

                quotes.push_back(quote);
                pos = quoteEnd + 1;
            }
        }
    }

    return {language, quotes};
}

std::pair<String, std::vector<Word>> JSONParser::parseWords(String path) {
    std::vector<Word> words;
    String language;

    std::ifstream file(path);
    if (!file.is_open()) {
        return {language, words};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    String content = buffer.str();
    file.close();

    // Parse language
    size_t langPos = content.find("\"language\":");
    if (langPos != String::npos) {
        size_t startQuote = content.find('\"', langPos + 11);
        size_t endQuote = content.find('\"', startQuote + 1);
        if (startQuote != String::npos && endQuote != String::npos) {
            language = content.substr(startQuote + 1, endQuote - startQuote - 1);
        }
    }

    // Parse words array
    size_t wordsStart = content.find("\"words\":");
    if (wordsStart != String::npos) {
        size_t arrayStart = content.find('[', wordsStart);
        size_t arrayEnd = content.find(']', arrayStart);

        if (arrayStart != String::npos && arrayEnd != String::npos) {
            String wordsArray = content.substr(arrayStart + 1, arrayEnd - arrayStart - 1);

            size_t pos = 0;
            while (pos < wordsArray.length()) {
                size_t wordStart = wordsArray.find('\"', pos);
                if (wordStart == String::npos) break;

                size_t wordEnd = wordsArray.find('\"', wordStart + 1);
                if (wordEnd == String::npos) break;

                String word = wordsArray.substr(wordStart + 1, wordEnd - wordStart - 1);
                if (!word.empty()) {
                    words.push_back(word);
                }

                pos = wordEnd + 1;
            }
        }
    }

    return {language, words};
}
