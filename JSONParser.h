#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <QLocale>

using String = std::string;
using Word = String;

struct Quote {
    String text;
    String source;
    unsigned int id;
};

struct Settings {
    std::string appLanguage;        // "русский" или "english"
    std::string trainingLanguage;   // "русский" или "english"
    bool shortWords;
    bool longWords;
    bool punctuation;
    bool numbers;
    bool quotes;
    bool highlight;
    bool keyboard;
};

class JSONParser {
public:
    static std::map<String, Quote> defaultQuote;
    static std::pair<String, std::vector<Quote>> parseQuotes(String path);
    static std::pair<String, std::vector<Word>> parseWords(String path);

    // Методы для работы с настройками
    static Settings parseSettings(const std::string& path);
    static std::string settingsToJSON(const Settings& settings);
    static Settings getDefaultSettings();
    static std::string getSystemLanguage();
    static std::pair<String, Quote> parseStandartText(const String& path);
private:
    static String unescapeJSONString(const String& str) {
        String result;
        result.reserve(str.length());

        for (size_t i = 0; i < str.length(); i++) {
            if (str[i] == '\\' && i + 1 < str.length()) {
                switch (str[i + 1]) {
                case '"': result += '"'; i++; break;
                case '\\': result += '\\'; i++; break;
                case '/': result += '/'; i++; break;
                case 'b': result += '\b'; i++; break;
                case 'f': result += '\f'; i++; break;
                case 'n': result += '\n'; i++; break;
                case 'r': result += '\r'; i++; break;
                case 't': result += '\t'; i++; break;
                default: result += str[i]; // оставляем как есть
                }
            } else {
                result += str[i];
            }
        }
        return result;
    }
};

#endif // JSONPARSER_H
