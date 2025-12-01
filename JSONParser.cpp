#include "JSONParser.h"
#include <fstream>
#include <sstream>
#include <QFile>
#include <QDir>
#include <QTextStream>


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
        size_t startQuote = content.find('"', langPos + 11);
        if (startQuote != String::npos) {
            size_t endQuote = findUnescapedQuote(content, startQuote + 1);
            if (endQuote != String::npos) {
                String langValue = content.substr(startQuote + 1, endQuote - startQuote - 1);
                language = unescapeJSONString(langValue);
            }
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
                    size_t sourceStart = quoteObj.find('"', sourcePos + 9);
                    if (sourceStart != String::npos) {
                        size_t sourceEnd = findUnescapedQuote(quoteObj, sourceStart + 1);
                        if (sourceEnd != String::npos) {
                            String sourceValue = quoteObj.substr(sourceStart + 1, sourceEnd - sourceStart - 1);
                            quote.source = unescapeJSONString(sourceValue);
                        }
                    }
                }

                // Parse text
                size_t textPos = quoteObj.find("\"text\":");
                if (textPos != String::npos) {
                    size_t textStart = quoteObj.find('"', textPos + 7);
                    if (textStart != String::npos) {
                        size_t textEnd = findUnescapedQuote(quoteObj, textStart + 1);
                        if (textEnd != String::npos) {
                            String textValue = quoteObj.substr(textStart + 1, textEnd - textStart - 1);
                            quote.text = unescapeJSONString(textValue);
                        }
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
        size_t startQuote = content.find('"', langPos + 11);
        if (startQuote != String::npos) {
            size_t endQuote = findUnescapedQuote(content, startQuote + 1);
            if (endQuote != String::npos) {
                language = content.substr(startQuote + 1, endQuote - startQuote - 1);
            }
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

    // Простой и надежный парсинг
    if (content.find("\"appLanguage\": \"русский\"") != std::string::npos) {
        settings.appLanguage = "русский";
    } else if (content.find("\"appLanguage\": \"english\"") != std::string::npos) {
        settings.appLanguage = "english";
    }

    if (content.find("\"trainingLanguage\": \"русский\"") != std::string::npos) {
        settings.trainingLanguage = "русский";
    } else if (content.find("\"trainingLanguage\": \"english\"") != std::string::npos) {
        settings.trainingLanguage = "english";
    }

    // Парсим boolean значения - ищем полные строки с ключами
    settings.shortWords = (content.find("\"shortWords\": true") != std::string::npos);
    settings.longWords = (content.find("\"longWords\": true") != std::string::npos);
    settings.punctuation = (content.find("\"punctuation\": true") != std::string::npos);
    settings.numbers = (content.find("\"numbers\": true") != std::string::npos);
    settings.quotes = (content.find("\"quotes\": true") != std::string::npos);
    settings.highlight = (content.find("\"highlight\": true") != std::string::npos);
    settings.keyboard = (content.find("\"keyboard\": true") != std::string::npos);
    qDebug() << settings.shortWords << '\n' << settings.longWords << '\n' << settings.punctuation << '\n' << settings.numbers << '\n' << settings.quotes << settings.highlight << '\n' << settings.keyboard;
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

    // Проверяем, что файл не пустой
    if (content.empty()) {
        qDebug() << "Файл пустой:" << QString::fromStdString(path);
        return {language, quote};
    }

    // Parse language
    size_t langPos = content.find("\"language\":");
    if (langPos != String::npos) {
        size_t startQuote = content.find('"', langPos + 11);
        if (startQuote != String::npos) {
            size_t endQuote = findUnescapedQuote(content, startQuote + 1);
            if (endQuote != String::npos) {
                String langValue = content.substr(startQuote + 1, endQuote - startQuote - 1);
                language = unescapeJSONString(langValue);
            }
        }
    }

    // Parse quote object
    size_t quoteStart = content.find("\"quote\":");
    if (quoteStart == String::npos) {
        qDebug() << "Не найдено поле 'quote' в файле:" << QString::fromStdString(path);
        return {language, quote};
    }

    size_t objStart = content.find('{', quoteStart);
    size_t objEnd = content.find('}', objStart);

    if (objStart == String::npos || objEnd == String::npos) {
        qDebug() << "Не удалось найти объект quote в файле:" << QString::fromStdString(path);
        return {language, quote};
    }

    String quoteObj = content.substr(objStart, objEnd - objStart + 1);

    // Parse text
    size_t textPos = quoteObj.find("\"text\":");
    if (textPos != String::npos) {
        size_t textStart = quoteObj.find('"', textPos + 7);
        if (textStart != String::npos) {
            size_t textEnd = findUnescapedQuote(quoteObj, textStart + 1);
            if (textEnd != String::npos) {
                String textValue = quoteObj.substr(textStart + 1, textEnd - textStart - 1);
                quote.text = unescapeJSONString(textValue);
            }
        }
    }

    // Parse source
    size_t sourcePos = quoteObj.find("\"source\":");
    if (sourcePos != String::npos) {
        size_t sourceStart = sourcePos + 9;
        sourceStart = quoteObj.find('"', sourceStart);
        if (sourceStart != String::npos) {
            size_t sourceEnd = findUnescapedQuote(quoteObj, sourceStart + 1);
            if (sourceEnd != String::npos) {
                String sourceValue = quoteObj.substr(sourceStart + 1, sourceEnd - sourceStart - 1);
                quote.source = unescapeJSONString(sourceValue);
            }
        }
    }

    // Parse id
    size_t idPos = quoteObj.find("\"id\":");
    if (idPos != String::npos) {
        size_t idStart = idPos + 5;
        idStart = quoteObj.find_first_not_of(" \n\r\t", idStart);
        if (idStart != String::npos) {
            if (quoteObj[idStart] == '"') {
                // ID как строка
                size_t idValueStart = idStart + 1;
                size_t idEnd = findUnescapedQuote(quoteObj, idValueStart);
                if (idEnd != String::npos) {
                    String idStr = quoteObj.substr(idValueStart, idEnd - idValueStart);
                    try {
                        quote.id = std::stoi(idStr);
                    } catch (...) {
                        quote.id = 0;
                    }
                }
            } else {
                // ID как число
                size_t idEnd = quoteObj.find_first_not_of("0123456789", idStart);
                if (idEnd == String::npos) {
                    idEnd = quoteObj.length();
                }
                String idStr = quoteObj.substr(idStart, idEnd - idStart);
                try {
                    quote.id = std::stoi(idStr);
                } catch (...) {
                    quote.id = 0;
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

    // Parse language
    size_t langPos = content.find("\"language\":");
    if (langPos != String::npos) {
        size_t startQuote = content.find('"', langPos + 11);
        if (startQuote != String::npos) {
            size_t endQuote = findUnescapedQuote(content, startQuote + 1);
            if (endQuote != String::npos) {
                String langValue = content.substr(startQuote + 1, endQuote - startQuote - 1);
                lesson.language = unescapeJSONString(langValue);
            }
        }
    }

    // Parse id
    size_t idPos = content.find("\"id\":");
    if (idPos != String::npos) {
        size_t idStart = idPos + 5;
        idStart = content.find_first_not_of(" \n\r\t", idStart);
        if (idStart != String::npos) {
            if (content[idStart] == '"') {
                // ID как строка
                size_t idValueStart = idStart + 1;
                size_t idEnd = findUnescapedQuote(content, idValueStart);
                if (idEnd != String::npos) {
                    String idStr = content.substr(idValueStart, idEnd - idValueStart);
                    try {
                        lesson.id = std::stoi(idStr);
                    } catch (...) {
                        lesson.id = 0;
                    }
                }
            } else {
                // ID как число
                size_t idEnd = content.find_first_not_of("0123456789", idStart);
                if (idEnd == String::npos) {
                    idEnd = content.length();
                }
                String idStr = content.substr(idStart, idEnd - idStart);
                try {
                    lesson.id = std::stoi(idStr);
                } catch (...) {
                    lesson.id = 0;
                }
            }
        }
    }

    // Parse name
    size_t namePos = content.find("\"name\":");
    if (namePos != String::npos) {
        size_t nameStart = content.find('"', namePos + 7);
        if (nameStart != String::npos) {
            size_t nameEnd = findUnescapedQuote(content, nameStart + 1);
            if (nameEnd != String::npos) {
                String nameValue = content.substr(nameStart + 1, nameEnd - nameStart - 1);
                lesson.name = unescapeJSONString(nameValue);
            }
        }
    }

    // Parse text
    size_t textPos = content.find("\"text\":");
    if (textPos != String::npos) {
        size_t textStart = content.find('"', textPos + 7);
        if (textStart != String::npos) {
            size_t textEnd = findUnescapedQuote(content, textStart + 1);
            if (textEnd != String::npos) {
                String textValue = content.substr(textStart + 1, textEnd - textStart - 1);
                lesson.text = unescapeJSONString(textValue);
            }
        }
    }

    return lesson;
}

std::map<String, std::priority_queue<Lesson>> JSONParser::parseLessons(const String& path) {
    std::map<String, std::priority_queue<Lesson>> result;

    QDir directory(QString::fromStdString(path));

    // Проверяем существование директории
    if (!directory.exists()) {
        qDebug() << "Директория не существует:" << QString::fromStdString(path);
        return result;
    }

    // Проверяем доступность директории для чтения
    if (!directory.isReadable()) {
        qDebug() << "Директория недоступна для чтения:" << QString::fromStdString(path);
        return result;
    }

    // Устанавливаем фильтр для JSON файлов
    QStringList filters;
    filters << "*.json";
    directory.setNameFilters(filters);
    directory.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);

    // Получаем список всех JSON файлов в директории
    QFileInfoList fileList = directory.entryInfoList();

    if (fileList.isEmpty()) {
        qDebug() << "В директории нет JSON файлов:" << QString::fromStdString(path);
        return result;
    }

    qDebug() << "Найдено JSON файлов уроков:" << fileList.size();

    for (const QFileInfo& fileInfo : fileList) {
        String filePath = fileInfo.absoluteFilePath().toStdString();
        qDebug() << "Обрабатывается файл урока:" << QString::fromStdString(filePath);

        // Проверяем, что файл существует и доступен для чтения
        if (!fileInfo.exists() || !fileInfo.isReadable()) {
            qDebug() << "Файл урока недоступен:" << fileInfo.fileName();
            continue;
        }

        // Парсим урок из файла
        Lesson lesson = parseLesson(filePath);

        if (!lesson.language.empty() && !lesson.text.empty()) {
            // Добавляем урок в priority_queue для соответствующего языка
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

    // Выводим статистику по каждому языку
    for (const auto& [language, queue] : result) {
        qDebug() << "Язык" << QString::fromStdString(language) << "количество уроков:" << queue.size();
    }

    return result;
}
