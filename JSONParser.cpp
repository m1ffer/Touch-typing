
#include "JSONParser.h"
#include <fstream>
#include <sstream>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <stack>
#include <cctype>

              // Вспомогательная функция для поиска незаэкранированной кавычки
              size_t findUnescapedQuote(const String& str, size_t startPos) {
    for (size_t i = startPos; i < str.length(); i++) {
        if (str[i] == '"') {
            // Проверяем, не является ли кавычка заэкранированной
            if (i == 0 || str[i-1] != '\\') {
                return i;
            }
            // Если заэкранирована, проверяем количество обратных слешей
            size_t backslashCount = 0;
            for (size_t j = i-1; j >= 0 && str[j] == '\\'; j--) {
                backslashCount++;
                if (j == 0) break;
            }
            // Если нечетное количество слешей - кавычка заэкранирована
            if (backslashCount % 2 == 0) {
                return i;
            }
        }
    }
    return String::npos;
}

// Вспомогательная функция для поиска конца JSON объекта/массива
size_t JSONParser::findMatchingBrace(const String& str, size_t startPos, char openBrace, char closeBrace) {
    int depth = 1;
    bool inString = false;

    for (size_t i = startPos + 1; i < str.length(); i++) {
        char c = str[i];

        if (c == '"') {
            // Проверяем, не экранирована ли кавычка
            if (i == 0 || str[i-1] != '\\') {
                inString = !inString;
            } else {
                // Проверяем количество обратных слешей
                size_t backslashCount = 0;
                for (size_t j = i-1; j >= 0 && str[j] == '\\'; j--) {
                    backslashCount++;
                    if (j == 0) break;
                }
                // Если четное количество слешей - кавычка не экранирована
                if (backslashCount % 2 == 0) {
                    inString = !inString;
                }
            }
        } else if (!inString) {
            if (c == openBrace) {
                depth++;
            } else if (c == closeBrace) {
                depth--;
                if (depth == 0) {
                    return i;
                }
            }
        }
    }
    return String::npos;
}

// Вспомогательная функция для извлечения строки JSON
String JSONParser::extractJsonString(const String& str, size_t startPos) {
    size_t quoteStart = findUnescapedQuote(str, startPos);
    if (quoteStart == String::npos) return "";

    size_t quoteEnd = findUnescapedQuote(str, quoteStart + 1);
    if (quoteEnd == String::npos) return "";

    return str.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
}

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

    // Удаляем пробелы и переносы строк для упрощения парсинга
    String cleaned;
    bool inString = false;
    for (size_t i = 0; i < content.length(); i++) {
        char c = content[i];
        if (c == '"') {
            // Проверяем, не экранирована ли кавычка
            if (i == 0 || content[i-1] != '\\') {
                inString = !inString;
            } else {
                // Проверяем количество обратных слешей
                size_t backslashCount = 0;
                for (size_t j = i-1; j >= 0 && content[j] == '\\'; j--) {
                    backslashCount++;
                    if (j == 0) break;
                }
                if (backslashCount % 2 == 0) {
                    inString = !inString;
                }
            }
            cleaned += c;
        } else if (inString || (!isspace(c) && c != '\n' && c != '\r')) {
            cleaned += c;
        }
    }

    // Parse language
    size_t langPos = cleaned.find("\"language\":");
    if (langPos != String::npos) {
        size_t langStart = cleaned.find('"', langPos + 11);
        if (langStart != String::npos) {
            String langStr = extractJsonString(cleaned, langStart);
            if (!langStr.empty()) {
                language = unescapeJSONString(langStr);
            }
        }
    }

    // Parse quotes array
    size_t quotesStart = cleaned.find("\"quotes\":");
    if (quotesStart != String::npos) {
        size_t arrayStart = cleaned.find('[', quotesStart);
        if (arrayStart != String::npos) {
            size_t arrayEnd = findMatchingBrace(cleaned, arrayStart, '[', ']');
            if (arrayEnd != String::npos) {
                String quotesArray = cleaned.substr(arrayStart + 1, arrayEnd - arrayStart - 1);

                size_t pos = 0;
                while (pos < quotesArray.length()) {
                    // Пропускаем запятые и пробелы
                    while (pos < quotesArray.length() && (quotesArray[pos] == ',' || isspace(quotesArray[pos]))) {
                        pos++;
                    }
                    if (pos >= quotesArray.length()) break;

                    // Находим начало объекта
                    size_t objStart = quotesArray.find('{', pos);
                    if (objStart == String::npos) break;

                    // Находим конец объекта
                    size_t objEnd = findMatchingBrace(quotesArray, objStart, '{', '}');
                    if (objEnd == String::npos) break;

                    String quoteObj = quotesArray.substr(objStart, objEnd - objStart + 1);

                    Quote quote;

                    // Parse id
                    size_t idPos = quoteObj.find("\"id\":");
                    if (idPos != String::npos) {
                        size_t valueStart = idPos + 5;
                        // Пропускаем пробелы
                        while (valueStart < quoteObj.length() && isspace(quoteObj[valueStart])) {
                            valueStart++;
                        }
                        if (valueStart < quoteObj.length()) {
                            if (quoteObj[valueStart] == '"') {
                                // ID как строка
                                String idStr = extractJsonString(quoteObj, valueStart);
                                try {
                                    quote.id = std::stoi(idStr);
                                } catch (...) {
                                    quote.id = 0;
                                }
                            } else {
                                // ID как число
                                size_t idEnd = valueStart;
                                while (idEnd < quoteObj.length() &&
                                       (isdigit(quoteObj[idEnd]) || quoteObj[idEnd] == '-')) {
                                    idEnd++;
                                }
                                String idStr = quoteObj.substr(valueStart, idEnd - valueStart);
                                try {
                                    quote.id = std::stoi(idStr);
                                } catch (...) {
                                    quote.id = 0;
                                }
                            }
                        }
                    }

                    // Parse source
                    size_t sourcePos = quoteObj.find("\"source\":");
                    if (sourcePos != String::npos) {
                        String sourceStr = extractJsonString(quoteObj, sourcePos + 9);
                        if (!sourceStr.empty()) {
                            quote.source = unescapeJSONString(sourceStr);
                        }
                    }

                    // Parse text
                    size_t textPos = quoteObj.find("\"text\":");
                    if (textPos != String::npos) {
                        String textStr = extractJsonString(quoteObj, textPos + 7);
                        if (!textStr.empty()) {
                            quote.text = unescapeJSONString(textStr);
                        }
                    }

                    quotes.push_back(quote);
                    pos = objEnd + 1;
                }
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
        size_t startQuote = content.find('"', langPos + 11);
        if (startQuote != String::npos) {
            size_t endQuote = findUnescapedQuote(content, startQuote + 1);
            if (endQuote != String::npos) {
                language = content.substr(startQuote + 1, endQuote - startQuote - 1);
            }
        }
    }
    qDebug() << language;

    // Parse words array
    size_t wordsStart = content.find("\"words\":");
    if (wordsStart != String::npos) {
        size_t arrayStart = content.find('[', wordsStart);
        size_t arrayEnd = content.find(']', arrayStart);

        if (arrayStart != String::npos && arrayEnd != String::npos) {
            String wordsArray = content.substr(arrayStart + 1, arrayEnd - arrayStart - 1);

            size_t pos = 0;
            while (pos < wordsArray.length()) {
                size_t wordStart = wordsArray.find('"', pos);
                if (wordStart == String::npos) break;

                size_t wordEnd = findUnescapedQuote(wordsArray, wordStart + 1);
                if (wordEnd == String::npos) break;

                String word = wordsArray.substr(wordStart + 1, wordEnd - wordStart - 1);
                if (!word.empty()) {
                    words.push_back(word);
                }

                pos = wordEnd + 1;
            }
        }
    }
    qDebug() << words.size();
    return {language, words};
}

Settings JSONParser::parseSettings(const std::string& path) {
    Settings settings = getDefaultSettings();

    std::ifstream file(path);
    if (!file.is_open()) {
        qDebug() << "Файл настроек не найден, используются настройки по умолчанию" << '\n';
        return settings;
    }

    std::string line;
    std::string content;
    while (std::getline(file, line)) {
        content += line;
    }
    file.close();

    // Простой и надежный парсинг для appLanguage
    if (content.find("\"appLanguage\": \"русский\"") != std::string::npos) {
        settings.appLanguage = "русский";
    } else if (content.find("\"appLanguage\": \"english\"") != std::string::npos) {
        settings.appLanguage = "english";
    }

    // Простой и надежный парсинг для trainingLanguage
    if (content.find("\"trainingLanguage\": \"русский\"") != std::string::npos) {
        settings.trainingLanguage = "русский";
    } else if (content.find("\"trainingLanguage\": \"english\"") != std::string::npos) {
        settings.trainingLanguage = "english";
    } else if (content.find("\"trainingLanguage\": \"assembly\"") != std::string::npos) {
        settings.trainingLanguage = "assembly";
    } else if (content.find("\"trainingLanguage\": \"c++\"") != std::string::npos) {
        settings.trainingLanguage = "c++";
    }

    // Парсим boolean значения
    settings.shortWords = (content.find("\"shortWords\": true") != std::string::npos);
    settings.longWords = (content.find("\"longWords\": true") != std::string::npos);
    settings.punctuation = (content.find("\"punctuation\": true") != std::string::npos);
    settings.numbers = (content.find("\"numbers\": true") != std::string::npos);
    settings.quotes = (content.find("\"quotes\": true") != std::string::npos);
    settings.highlight = (content.find("\"highlight\": true") != std::string::npos);
    settings.keyboard = (content.find("\"keyboard\": true") != std::string::npos);

    qDebug() << "Загружены настройки:";
    qDebug() << "  appLanguage:" << QString::fromStdString(settings.appLanguage);
    qDebug() << "  trainingLanguage:" << QString::fromStdString(settings.trainingLanguage);
    qDebug() << "  shortWords:" << settings.shortWords;
    qDebug() << "  longWords:" << settings.longWords;
    qDebug() << "  punctuation:" << settings.punctuation;
    qDebug() << "  numbers:" << settings.numbers;
    qDebug() << "  quotes:" << settings.quotes;
    qDebug() << "  highlight:" << settings.highlight;
    qDebug() << "  keyboard:" << settings.keyboard;

    return settings;
}

Settings JSONParser::getDefaultSettings() {
    Settings settings;
    std::string systemLang = getSystemLanguage();

    settings.appLanguage = systemLang;
    settings.trainingLanguage = systemLang;
    settings.shortWords = false;
    settings.longWords = false;
    settings.punctuation = false;
    settings.numbers = false;
    settings.quotes = false;
    settings.highlight = true;
    settings.keyboard = true;

    return settings;
}

std::string JSONParser::settingsToJSON(const Settings& settings) {
    std::stringstream json;
    json << "{\n";
    json << "  \"appLanguage\": \"" << settings.appLanguage << "\",\n";
    json << "  \"trainingLanguage\": \"" << settings.trainingLanguage << "\",\n";
    json << "  \"shortWords\": " << (settings.shortWords ? "true" : "false") << ",\n";
    json << "  \"longWords\": " << (settings.longWords ? "true" : "false") << ",\n";
    json << "  \"punctuation\": " << (settings.punctuation ? "true" : "false") << ",\n";
    json << "  \"numbers\": " << (settings.numbers ? "true" : "false") << ",\n";
    json << "  \"quotes\": " << (settings.quotes ? "true" : "false") << ",\n";
    json << "  \"highlight\": " << (settings.highlight ? "true" : "false") << ",\n";
    json << "  \"keyboard\": " << (settings.keyboard ? "true" : "false") << "\n";
    json << "}";

    return json.str();
}

std::string JSONParser::getSystemLanguage() {
    QLocale locale = QLocale::system();
    QString language = locale.name().split('_').first();

    if (language == "ru") {
        return "русский";
    } else {
        return "english"; // по умолчанию
    }
}

std::pair<String, Quote> JSONParser::parseStandartText(const String& path) {
    String language;
    Quote quote;

    std::ifstream file(path);
    if (!file.is_open()) {
        qDebug() << "Не удалось открыть файл:" << QString::fromStdString(path);
        return {language, quote};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    String content = buffer.str();
    file.close();

    // Упрощаем парсинг: удаляем пробелы и переносы строк
    String cleaned;
    bool inString = false;
    for (size_t i = 0; i < content.length(); i++) {
        char c = content[i];
        if (c == '"') {
            // Проверяем, не экранирована ли кавычка
            if (i == 0 || content[i-1] != '\\') {
                inString = !inString;
            } else {
                size_t backslashCount = 0;
                for (size_t j = i-1; j >= 0 && content[j] == '\\'; j--) {
                    backslashCount++;
                    if (j == 0) break;
                }
                if (backslashCount % 2 == 0) {
                    inString = !inString;
                }
            }
            cleaned += c;
        } else if (inString || (!isspace(c) && c != '\n' && c != '\r')) {
            cleaned += c;
        }
    }

    // Parse language
    size_t langPos = cleaned.find("\"language\":");
    if (langPos != String::npos) {
        size_t langStart = cleaned.find('"', langPos + 11);
        if (langStart != String::npos) {
            String langStr = extractJsonString(cleaned, langStart);
            if (!langStr.empty()) {
                language = unescapeJSONString(langStr);
            }
        }
    }

    // Parse quote object
    size_t quoteStart = cleaned.find("\"quote\":");
    if (quoteStart != String::npos) {
        size_t objStart = cleaned.find('{', quoteStart);
        if (objStart != String::npos) {
            size_t objEnd = findMatchingBrace(cleaned, objStart, '{', '}');
            if (objEnd != String::npos) {
                String quoteObj = cleaned.substr(objStart, objEnd - objStart + 1);

                // Parse id
                size_t idPos = quoteObj.find("\"id\":");
                if (idPos != String::npos) {
                    size_t valueStart = idPos + 5;
                    while (valueStart < quoteObj.length() && isspace(quoteObj[valueStart])) {
                        valueStart++;
                    }
                    if (valueStart < quoteObj.length()) {
                        if (quoteObj[valueStart] == '"') {
                            String idStr = extractJsonString(quoteObj, valueStart);
                            try {
                                quote.id = std::stoi(idStr);
                            } catch (...) {
                                quote.id = 0;
                            }
                        } else {
                            size_t idEnd = valueStart;
                            while (idEnd < quoteObj.length() &&
                                   (isdigit(quoteObj[idEnd]) || quoteObj[idEnd] == '-')) {
                                idEnd++;
                            }
                            String idStr = quoteObj.substr(valueStart, idEnd - valueStart);
                            try {
                                quote.id = std::stoi(idStr);
                            } catch (...) {
                                quote.id = 0;
                            }
                        }
                    }
                }

                // Parse source
                size_t sourcePos = quoteObj.find("\"source\":");
                if (sourcePos != String::npos) {
                    String sourceStr = extractJsonString(quoteObj, sourcePos + 9);
                    if (!sourceStr.empty()) {
                        quote.source = unescapeJSONString(sourceStr);
                    }
                }

                // Parse text
                size_t textPos = quoteObj.find("\"text\":");
                if (textPos != String::npos) {
                    String textStr = extractJsonString(quoteObj, textPos + 7);
                    if (!textStr.empty()) {
                        quote.text = unescapeJSONString(textStr);
                    }
                }
            }
        }
    }

    return {language, quote};
}

Lesson JSONParser::parseLesson(const String& path) {
    Lesson lesson;

    std::ifstream file(path);
    if (!file.is_open()) {
        qDebug() << "Не удалось открыть файл урока:" << QString::fromStdString(path);
        return lesson;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    String content = buffer.str();
    file.close();

    // Упрощаем парсинг
    String cleaned;
    bool inString = false;
    for (size_t i = 0; i < content.length(); i++) {
        char c = content[i];
        if (c == '"') {
            if (i == 0 || content[i-1] != '\\') {
                inString = !inString;
            } else {
                size_t backslashCount = 0;
                for (size_t j = i-1; j >= 0 && content[j] == '\\'; j--) {
                    backslashCount++;
                    if (j == 0) break;
                }
                if (backslashCount % 2 == 0) {
                    inString = !inString;
                }
            }
            cleaned += c;
        } else if (inString || (!isspace(c) && c != '\n' && c != '\r')) {
            cleaned += c;
        }
    }

    // Parse language
    size_t langPos = cleaned.find("\"language\":");
    if (langPos != String::npos) {
        size_t langStart = cleaned.find('"', langPos + 11);
        if (langStart != String::npos) {
            String langStr = extractJsonString(cleaned, langStart);
            if (!langStr.empty()) {
                lesson.language = unescapeJSONString(langStr);
            }
        }
    }

    // Parse id
    size_t idPos = cleaned.find("\"id\":");
    if (idPos != String::npos) {
        size_t valueStart = idPos + 5;
        while (valueStart < cleaned.length() && isspace(cleaned[valueStart])) {
            valueStart++;
        }
        if (valueStart < cleaned.length()) {
            if (cleaned[valueStart] == '"') {
                String idStr = extractJsonString(cleaned, valueStart);
                try {
                    lesson.id = std::stoi(idStr);
                } catch (...) {
                    lesson.id = 0;
                }
            } else {
                size_t idEnd = valueStart;
                while (idEnd < cleaned.length() &&
                       (isdigit(cleaned[idEnd]) || cleaned[idEnd] == '-')) {
                    idEnd++;
                }
                String idStr = cleaned.substr(valueStart, idEnd - valueStart);
                try {
                    lesson.id = std::stoi(idStr);
                } catch (...) {
                    lesson.id = 0;
                }
            }
        }
    }

    // Parse name
    size_t namePos = cleaned.find("\"name\":");
    if (namePos != String::npos) {
        String nameStr = extractJsonString(cleaned, namePos + 7);
        if (!nameStr.empty()) {
            lesson.name = unescapeJSONString(nameStr);
        }
    }

    // Parse text
    size_t textPos = cleaned.find("\"text\":");
    if (textPos != String::npos) {
        String textStr = extractJsonString(cleaned, textPos + 7);
        if (!textStr.empty()) {
            lesson.text = unescapeJSONString(textStr);
        }
    }

    return lesson;
}

std::map<String, std::priority_queue<Lesson>> JSONParser::parseLessons(const String& path) {
    std::map<String, std::priority_queue<Lesson>> result;

    QDir directory(QString::fromStdString(path));

    if (!directory.exists()) {
        qDebug() << "Директория не существует:" << QString::fromStdString(path);
        return result;
    }

    if (!directory.isReadable()) {
        qDebug() << "Директория недоступна для чтения:" << QString::fromStdString(path);
        return result;
    }

    QStringList filters;
    filters << "*.json";
    directory.setNameFilters(filters);
    directory.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);

    QFileInfoList fileList = directory.entryInfoList();

    if (fileList.isEmpty()) {
        qDebug() << "В директории нет JSON файлов:" << QString::fromStdString(path);
        return result;
    }

    qDebug() << "Найдено JSON файлов уроков:" << fileList.size();

    for (const QFileInfo& fileInfo : fileList) {
        String filePath = fileInfo.absoluteFilePath().toStdString();
        qDebug() << "Обрабатывается файл урока:" << QString::fromStdString(filePath);

        if (!fileInfo.exists() || !fileInfo.isReadable()) {
            qDebug() << "Файл урока недоступен:" << fileInfo.fileName();
            continue;
        }

        Lesson lesson = parseLesson(filePath);

        if (!lesson.language.empty() && !lesson.text.empty()) {
            result[lesson.language].push(lesson);
            qDebug() << "Добавлен урок для языка:" << QString::fromStdString(lesson.language)
                     << "ID:" << lesson.id;
        } else {
            if (lesson.language.empty()) {
                qDebug() << "Не удалось определить язык в файле урока:" << fileInfo.fileName();
            }
            if (lesson.text.empty()) {
                qDebug() << "Не удалось извлечь текст из файла урока:" << fileInfo.fileName();
            }
        }
    }

    qDebug() << "Парсинг уроков завершен. Загружено языков:" << result.size();

    for (const auto& [language, queue] : result) {
        qDebug() << "Язык" << QString::fromStdString(language) << "количество уроков:" << queue.size();
    }

    return result;
}
