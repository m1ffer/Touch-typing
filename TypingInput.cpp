// typinginput.cpp
#include "typinginput.h"
#include <QTextBlock>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QTextStream>
#include "MessageHelper.h"
#include <algorithm> // ДОБАВИТЬ для std::shuffle
#include <QRegularExpression>
#include <QContextMenuEvent> // ДОБАВИТЬ для контекстного меню
#include <QMimeData>        // ДОБАВИТЬ для работы с буфером обмена
#include <QMouseEvent>      // ДОБАВИТЬ для обработки мыши


// ДОБАВЛЕНО: Определение констант
const double TypingInput::PUNCTUATION_PROBABILITY = 0.3; // 30% вероятность пунктуации
const double TypingInput::NUMBER_PROBABILITY = 0.2;      // 20% вероятность числа

TypingInput::TypingInput(QWidget *parent)
    : QTextEdit(parent),
    m_cursorColor(QColor(255, 215, 0)),          // Желтый курсор
    m_correctTextColor(QColor(255, 215, 0)),     // Желтый правильный текст
    m_incorrectTextColor(QColor(255, 100, 100)), // Светло-красный для ошибок
    m_pendingTextColor(QColor(255, 215, 0, 150)), // Полупрозрачный желтый
    m_backgroundColor(QColor(34, 34, 34)), // #222222 - светлее чем #1a1a1a
    m_currentPosition(0),
    m_errorsCount(0),
    m_totalCharsTyped(0),
    m_timerActive(false),
    m_finalTimeMs(0),
    m_gen(m_rd())
{
    initializeStandartText();
    // Настройка внешнего вида
    setFrameStyle(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // ДОБАВЛЕНО: Отключаем все возможности вставки и копирования
    setAcceptDrops(false);                     // Отключаем drag-and-drop
    setTextInteractionFlags(Qt::NoTextInteraction); // Отключаем все взаимодействия с текстом
    setFocusPolicy(Qt::StrongFocus);           // Только фокус через Tab/клик

    // Но разрешаем ввод с клавиатуры
    setTextInteractionFlags(textInteractionFlags() | Qt::TextEditable);

    // Убираем возможность выделения текста
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);

    // Настройка шрифта
    QFont font("Consolas", 22, QFont::Normal);
    font.setStyleHint(QFont::TypeWriter);
    setFont(font);

    // Используем стандартный курсор, но делаем его желтым
    setCursorWidth(2);

    // ДОБАВЛЕНО: Инициализация таймера обновления
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, [this]() {
        if (m_timerActive) {
            emit timerUpdated(m_typingTimer.elapsed());
        }
    });

    // ДОБАВИТЬ: Таймер для записи истории (каждую секунду)
    m_historyTimer = new QTimer(this);
    connect(m_historyTimer, &QTimer::timeout, this, [this]() {
        if (m_timerActive && m_totalCharsTyped > 0) {
            qint64 currentTime = m_typingTimer.elapsed();
            double currentSpeed = getSpeedCpm();

            m_speedHistory.append(qMakePair(currentTime, currentSpeed));

            //qDebug() << "Записана точка графика:" << currentTime << "мс," << currentSpeed << "CPM";
        }
    });

    // ДОБАВЛЕНО: Загрузка данных из JSON файлов
    // Русские цитаты
    auto Quotes = JSONParser::parseQuotes("../../res/languages/russian/quotes.json");
    quotes[Quotes.first] = Quotes.second;
    // Английские цитаты
    Quotes = JSONParser::parseQuotes("../../res/languages/english/quotes.json");
    quotes[Quotes.first] = Quotes.second;
    //С++ куски кода
    Quotes = JSONParser::parseQuotes("../../res/languages/c++/quotes.json");
    quotes[Quotes.first] = Quotes.second;
    //asm куски кода
    Quotes = JSONParser::parseQuotes("../../res/languages/asm/quotes.json");
    quotes[Quotes.first] = Quotes.second;

    //Русские короткие слова
    auto ShortWords = JSONParser::parseWords("../../res/languages/russian/shortWords.json");
    shortWords[ShortWords.first] = ShortWords.second;
    //Английские короткие слова
    ShortWords = JSONParser::parseWords("../../res/languages/english/shortWords.json");
    shortWords[ShortWords.first] = ShortWords.second;
    //С++ короткие слова
    ShortWords = JSONParser::parseWords("../../res/languages/c++/shortWords.json");
    shortWords[ShortWords.first] = ShortWords.second;
    //asm короткие слова
    ShortWords = JSONParser::parseWords("../../res/languages/asm/shortWords.json");
    shortWords[ShortWords.first] = ShortWords.second;

    //Русские длинные слова
    auto LongWords = JSONParser::parseWords("../../res/languages/russian/longWords.json");
    longWords[LongWords.first] = LongWords.second;
    //Английские длинные слова
    LongWords = JSONParser::parseWords("../../res/languages/english/longWords.json");
    longWords[LongWords.first] = LongWords.second;
    //c++ короткие слова
    LongWords = JSONParser::parseWords("../../res/languages/c++/longWords.json");
    longWords[LongWords.first] = LongWords.second;
    //asm длинные слова
    LongWords = JSONParser::parseWords("../../res/languages/asm/longWords.json");
    longWords[LongWords.first] = LongWords.second;
    // Начальные настройки
    updateStyle();
}

void TypingInput::setCursorColor(const QColor &color)
{
    m_cursorColor = color;
    updateStyle();
}

void TypingInput::setTargetText(QString text)
{
    d = 0;
    m_targetText = text;
    //m_targetText.replace("\t", "");
    m_enteredText.clear();  // ДОБАВЛЕНО: очищаем введенный текст
    m_currentPosition = 0;
    clear();

    // ДОБАВЛЕНО: Сбрасываем таймер при установке нового текста
    stopTimer();

    // Устанавливаем серый цвет для всего целевого текста
    QTextCharFormat format;
    format.setForeground(m_pendingTextColor);
    setCurrentCharFormat(format);
    QString aaa = QChar(0x23CE);
    aaa.append("\n");
    text.replace("\n", aaa);
    setPlainText(text);

    // Возвращаем курсор в начало
    updateCursorPosition();

    updateTextColors();
    m_errorsCount = 0; // ДОБАВЛЕНО
    m_totalCharsTyped = 0; // ДОБАВЛЕНО
    m_speedHistory.clear();     // ДОБАВИТЬ
    m_accuracyHistory.clear();  // ДОБАВИТЬ
    m_finalTimeMs = 0; // СБРАСЫВАЕМ время
}

void TypingInput::reset()
{
    setTargetText(m_targetText);
}

// ДОБАВЛЕНО: Отключаем контекстное меню
void TypingInput::contextMenuEvent(QContextMenuEvent *event)
{
    // Полностью игнорируем событие контекстного меню
    event->accept();
}

// ДОБАВЛЕНО: Запрещаем вставку из буфера обмена
bool TypingInput::canInsertFromMimeData(const QMimeData *source) const
{
    Q_UNUSED(source);
    return false; // Полностью запрещаем вставку
}

// ДОБАВЛЕНО: Запрещаем вставку из буфера обмена
void TypingInput::insertFromMimeData(const QMimeData *source)
{
    Q_UNUSED(source);
    // Ничего не делаем - вставка полностью запрещена
}

// ДОБАВЛЕНО: Переопределяем keyPressEvent для дополнительных ограничений
void TypingInput::keyPressEvent(QKeyEvent *event)
{
    auto key = event -> key();
    // Блокируем все комбинации клавиш для копирования/вставки
    if (event->matches(QKeySequence::Copy) ||
        event->matches(QKeySequence::Cut) ||
        event->matches(QKeySequence::Paste) ||
        event->matches(QKeySequence::SelectAll) ||
        key == Qt::Key_Insert || // Block Insert key
        (event->modifiers() == Qt::ControlModifier && key == Qt::Key_A) ||
        (event->modifiers() == Qt::ControlModifier && key == Qt::Key_C) ||
        (event->modifiers() == Qt::ControlModifier && key == Qt::Key_X) ||
        (event->modifiers() == Qt::ControlModifier && key == Qt::Key_V) ||
        (event->modifiers() == Qt::ShiftModifier && key == Qt::Key_Insert)) {

        event->ignore(); // Полностью игнорируем эти комбинации
        return;
    }

    // Игнорируем клавиши навигации (уже есть, но оставляем для полноты)
    if (key == Qt::Key_Left || key == Qt::Key_Right ||
        key == Qt::Key_Up || key == Qt::Key_Down ||
        key == Qt::Key_Home || key == Qt::Key_End ||
        key == Qt::Key_PageUp || key == Qt::Key_PageDown) {
        event->ignore();
        return;
    }

    // Игнорируем специальные клавиши
    if (key == Qt::Key_Shift || key == Qt::Key_Control ||
        key == Qt::Key_Alt || key == Qt::Key_Meta ||
        key == Qt::Key_AltGr || key == Qt::Key_CapsLock ||
        key == Qt::Key_NumLock || key == Qt::Key_ScrollLock) {
        event->ignore();
        return;
    }

    // Обрабатываем Backspace - ИЗМЕНЕННАЯ ЛОГИКА
    if (key == Qt::Key_Backspace) {
        if (m_currentPosition > 0) {
            // Проверяем, является ли текущая позиция опорным пробелом
            bool isReferenceSpace = (m_targetText[m_currentPosition - 1] == ' ' &&
                                     m_enteredText[m_currentPosition - 1] == ' ') ||
                                    (m_targetText[m_currentPosition - 1] == '\xa' &&
                                     m_enteredText[m_currentPosition - 1] == '\xa') ||
                                    (m_targetText[m_currentPosition - 1] == '\t');

            if (!isReferenceSpace) {
                if (m_targetText[m_currentPosition - 1] == '\xa'){
                    d--;
                }
                m_currentPosition--;
                m_enteredText.chop(1);  // Удаляем последний символ из введенного текста
                checkCharacter(m_currentPosition, QChar());

                // Обновляем позицию курсора после Backspace
                updateCursorPosition();

                // Emit text changed signal
                emit textChanged(m_enteredText);
            }
        }
        event->accept();
        return;
    }

    // Обрабатываем Delete отдельно (запрещаем)
    if (key == Qt::Key_Delete) {
        event->ignore();
        return;
    }

    // Обрабатываем обычные символы - ДОБАВЛЕНО сохранение в m_enteredText
    if (!event->text().isEmpty() && m_currentPosition < m_targetText.length()) {
        // ДОБАВЛЕНО: Запускаем таймер при первом нажатии
        if (!m_timerActive && m_enteredText.isEmpty()) {
            startTimer();
        }
        QChar enteredChar = event->text().at(0);
        if (enteredChar.unicode() == '\xd'){
            enteredChar = '\xa';
        }
        m_enteredText += enteredChar;  // ДОБАВЛЕНО: сохраняем введенный символ
        m_totalCharsTyped++; // ДОБАВЛЕНО: Счетчик всех нажатий

        // ДОБАВЛЕНО: Подсчет ошибок
        checkCharacter(m_currentPosition, enteredChar);
        if (enteredChar != m_targetText.at(m_currentPosition)) {
            m_errorsCount++;
        }
        if (m_targetText.at(m_currentPosition) == '\xa')
            d++;
        qDebug() << enteredChar.unicode() << ' ' << m_targetText.at(m_currentPosition).unicode();
        m_currentPosition++;
        // Обновляем позицию курсора после ввода символа
        updateCursorPosition();

        // Emit text changed signal
        emit textChanged(m_enteredText);

        ensureCursorVisible();

        // Check if input is completed
        if (m_currentPosition >= m_targetText.length()) {
            // ДОБАВЛЕНО: Останавливаем таймер при завершении
            stopTimer();
            emit inputCompleted();
        }
        else if(m_targetText.at(m_currentPosition) == QChar('\t')){
            m_currentPosition++;
            updateCursorPosition();
        }
    }

    event->accept();
    update();
}

// ДОБАВЛЕНО: Запрещаем выделение мышью
void TypingInput::mousePressEvent(QMouseEvent *event)
{
    // Разрешаем только левую кнопку мыши для фокуса
    if (event->button() == Qt::LeftButton) {
        setFocus(); // Только устанавливаем фокус
        QTextEdit::mousePressEvent(event);
    }
    event->ignore(); // Игнорируем все другие действия мышью
}

// ДОБАВЛЕНО: Запрещаем двойной клик
void TypingInput::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore(); // Полностью игнорируем двойной клик
}

// ДОБАВЛЕНО: Запрещаем перемещение мыши с зажатой кнопкой
void TypingInput::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore(); // Игнорируем перемещение мыши
}

// ДОБАВЛЕНО: Запрещаем отпускание кнопки мыши
void TypingInput::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore(); // Игнорируем отпускание кнопки
}

void TypingInput::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);
    ensureCursorVisible();
}

void TypingInput::updateTextColors()
{
    if (m_targetText.isEmpty()) return;

    QTextCursor cursor = textCursor();
    int oldPosition = cursor.position();

    // Устанавливаем цвета для всех символов
    for (int i = 0, tmp = 0; i < m_targetText.length(); ++i) {
        cursor.setPosition(i + tmp);
        cursor.setPosition(i + 1 + tmp, QTextCursor::KeepAnchor);

        QTextCharFormat format;
        if (i < m_currentPosition) {
            // Уже введенные символы
            if (toPlainText().length() > i) {
                QChar enteredChar = toPlainText().at(i);
                if (enteredChar == m_targetText.at(i)) {
                    format.setForeground(m_correctTextColor); // Правильный - желтый
                } else {
                    format.setForeground(m_incorrectTextColor); // Неправильный - красный
                }
            }
        } else {
            // Еще не введенные символы
            format.setForeground(m_pendingTextColor); // Серый
        }

        cursor.setCharFormat(format);
    }

    // Восстанавливаем позицию курсора
    cursor.setPosition(oldPosition);
    setTextCursor(cursor);
}

// В методе updateStyle() ИЗМЕНИТЕ:
void TypingInput::updateStyle()
{
    setStyleSheet(QString(R"(
        QTextEdit {
            background-color: %1;
            color: %2;
            border: 2px solid #3a3a3a;
            border-radius: 0px;
            padding: 15px;
            caret-color: %3;
            font-family: 'Roboto Mono', 'Consolas', monospace;
            font-size: 24px;
            line-height: 1.5;
            selection-background-color: #3a3a3a;
        }
        QScrollBar:vertical {
            background: #2a2a2a;
            width: 10px;
            border-radius: 0px;
        }
        QScrollBar::handle:vertical {
            background: #ffd700;
            border-radius: 0px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background: #ffed4a;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )")
                      .arg(m_backgroundColor.name())
                      .arg(m_pendingTextColor.name())
                      .arg(m_cursorColor.name()));
}

void TypingInput::ensureCursorVisible()
{
    if (m_currentPosition >= m_targetText.length()) return;

    // Получаем прямоугольник позиции курсора
    QTextCursor cursor = textCursor();
    cursor.setPosition(m_currentPosition);
    QRect rect = cursorRect(cursor);

    // Получаем видимую область
    QRect visibleRect = viewport()->rect();

    // Вычисляем высоту строки (примерную)
    int lineHeight = rect.height();

    // Вычисляем положение предпоследней строки (на одну строку выше низа)
    int penultimateLineBottom = visibleRect.bottom() - lineHeight;

    // Если курсор выходит за предпоследнюю строку, прокручиваем вниз
    if (rect.bottom() > penultimateLineBottom) {
        verticalScrollBar()->setValue(verticalScrollBar()->value() +
                                      rect.bottom() - penultimateLineBottom);
    }

    // Если курсор выходит за верхнюю границу, прокручиваем вверх
    if (rect.top() < visibleRect.top()) {
        verticalScrollBar()->setValue(verticalScrollBar()->value() -
                                      (visibleRect.top() - rect.top()));
    }
}

void TypingInput::checkCharacter(int position, QChar enteredChar)
{
    if (position >= m_targetText.length()) return;

    QTextCursor cursor = textCursor();
    cursor.setPosition(position + d);
    cursor.setPosition(position + d + 1, QTextCursor::KeepAnchor);

    QTextCharFormat format;

    if (enteredChar.isNull()) {
        // Backspace - возвращаем серый цвет
        format.setForeground(m_pendingTextColor);
    } else {
        // Проверяем правильность
        if (enteredChar == m_targetText.at(position)) {
            format.setForeground(m_correctTextColor); // Правильный - желтый
        } else {
            format.setForeground(m_incorrectTextColor); // Неправильный - красный
        }
    }

    cursor.setCharFormat(format);

    // Обновляем отображение
    update();
}

void TypingInput::updateCursorPosition(int f)
{
    // Обновляем позицию курсора
    QTextCursor cursor = textCursor();
    cursor.setPosition(m_currentPosition + d);
    setTextCursor(cursor);
}

bool TypingInput::setTargetTextFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    // Заменяем все специальные символы на пробелы
    // Включая переводы строк, табуляции и другие непечатаемые символы
    QRegularExpression specialChars("[\\t\\n\\r\\f\\v]");
    content.replace(specialChars, " ");

    // Убираем множественные пробелы, оставляя одинарные
    QRegularExpression multipleSpaces("\\s+");
    content.replace(multipleSpaces, " ");

    // Убираем пробелы в начале и конце
    content = content.trimmed();

    // Устанавливаем обработанный текст как целевой
    setTargetText(content);

    return true;
}

// ДОБАВЛЕНО: Методы для работы с таймером
void TypingInput::startTimer()
{
    if (!m_timerActive) {
        m_typingTimer.start();
        m_timerActive = true;
        m_updateTimer->start(100); // Обновление каждые 100 мс
        m_historyTimer->start(100); // Записывать каждую секунду
        emit timerStarted();
        //qDebug() << "Таймер запущен";
    }
}

void TypingInput::stopTimer()
{
    if (m_timerActive) {
        m_timerActive = false;
        m_finalTimeMs = m_typingTimer.elapsed(); // СОХРАНЯЕМ время
        m_updateTimer->stop();
        m_historyTimer->stop();  // ДОБАВИТЬ
        qint64 elapsed = m_typingTimer.elapsed();
        emit timerStopped(elapsed);
        qDebug() << "Таймер остановлен, время:" << elapsed << "мс";
        qDebug() << "Активен ли таймер:" << m_timerActive;
        qDebug() << "Таймер действительно измеряет:" << m_typingTimer.isValid();
    }
}

qint64 TypingInput::getElapsedTime() const
{
    return m_timerActive ? m_typingTimer.elapsed() : m_finalTimeMs;
}

bool TypingInput::isTimerRunning() const
{
    return m_timerActive;
}

// ДОБАВЛЯЕМ МЕТОДЫ ДЛЯ СТАТИСТИКИ
double TypingInput::getAccuracy() const
{
    if (m_totalCharsTyped == 0) return 100.0;
    return ((m_totalCharsTyped - m_errorsCount) / (double)m_totalCharsTyped) * 100.0;
}

double TypingInput::getSpeedWpm() const
{
    if (m_typingTimer.elapsed() == 0) return 0.0;
    // Стандартное слово = 5 символов
    double words = m_totalCharsTyped / 5.0;
    double minutes = m_typingTimer.elapsed() / 60000.0;
    return words / minutes;
}

double TypingInput::getSpeedCpm() const
{
    if (m_typingTimer.elapsed() == 0) return 0.0;
    double minutes = m_typingTimer.elapsed() / 60000.0;
    return m_totalCharsTyped / minutes;
}

// Добавим метод для получения истории скорости:
/*QVector<QPair<qint64, double>> TypingInput::getSpeedHistory() const
{
    return m_speedHistory;
}*/

// ДОБАВЛЕНО: Реализация метода makeTextFromSettings
QString TypingInput::makeTextFromSettings(const Settings& settings)
{
    // Если включены цитаты - возвращаем случайную цитату
    if (settings.quotes) {
        auto q = getRandomQuote(settings.trainingLanguage);
        return QString::fromStdString(q.text + "\n" + (settings.appLanguage == "english" ? "Source: " : "Источник: ") + q.source);
    }
    if (!settings.longWords && !settings.shortWords && !settings.numbers && !settings.punctuation){
        return QString::fromStdString(standartText[settings.trainingLanguage]);
    }
    if (!settings.longWords && !settings.shortWords){
        Settings tmp = settings;
        tmp.longWords = tmp.shortWords = true;
        return makeTextFromSettings(tmp);
    }


    // Генерируем случайное количество слов
    std::uniform_int_distribution<> wordCountDist(MIN_WORDS, MAX_WORDS);
    int wordCount = wordCountDist(m_gen);

    QStringList words;

    for (int i = 0; i < wordCount; ++i) {
        QString word = getRandomWord(settings);

        // Добавляем пунктуацию с вероятностью
        if (settings.punctuation) {
            std::uniform_real_distribution<> punctDist(0.0, 1.0);
            if (punctDist(m_gen) < PUNCTUATION_PROBABILITY) {
                word = addPunctuation(word);
            }
        }

        // Добавляем число с вероятностью
        if (settings.numbers) {
            std::uniform_real_distribution<> numberDist(0.0, 1.0);
            if (numberDist(m_gen) < NUMBER_PROBABILITY) {
                word = addNumber(word);
            }
        }

        words.append(word);
    }

    return words.join(" ");
}

// ДОБАВЛЕНО: Вспомогательные методы

QString TypingInput::getRandomWord(const Settings& settings)
{
    const std::string& lang = settings.trainingLanguage;

    // Определяем, из каких источников брать слова
    bool useShort = settings.shortWords || (!settings.shortWords && !settings.longWords);
    bool useLong = settings.longWords || (!settings.shortWords && !settings.longWords);

    // Выбираем случайный источник
    std::vector<const std::vector<Word>*> sources;

    if (useShort && shortWords.count(lang) && !shortWords[lang].empty()) {
        sources.push_back(&shortWords[lang]);
    }
    if (useLong && longWords.count(lang) && !longWords[lang].empty()) {
        sources.push_back(&longWords[lang]);
    }

    if (sources.empty()) {
        return "word"; // fallback
    }

    // Выбираем случайный источник
    std::uniform_int_distribution<> sourceDist(0, sources.size() - 1);
    const std::vector<Word>* selectedSource = sources[sourceDist(m_gen)];

    // Выбираем случайное слово из выбранного источника
    std::uniform_int_distribution<> wordDist(0, selectedSource->size() - 1);
    return QString::fromStdString((*selectedSource)[wordDist(m_gen)]);
}

Quote TypingInput::getRandomQuote(const std::string& language)
{
    if (!quotes[language].empty()) {
        std::uniform_int_distribution<> quoteDist(0, quotes[language].size() - 1);
        return quotes[language][quoteDist(m_gen)];
    }

    return {"Default quote text", "Default", 0}; // fallback
}

QString TypingInput::capitalizeWord(const QString& word)
{
    if (word.isEmpty()) return word;
    return word[0].toUpper() + word.mid(1);
}

QString TypingInput::addPunctuation(const QString& word)
{
    static const QVector<QString> punctuationMarks = {",", ".", "!", "?", ";", ":", "—"};
    std::uniform_int_distribution<> punctDist(0, punctuationMarks.size() - 1);
    std::uniform_int_distribution<> capDist(0, 1);
    if (capDist(m_gen))
        return capitalizeWord(word + punctuationMarks[punctDist(m_gen)]);
    return word + punctuationMarks[punctDist(m_gen)];
}

QString TypingInput::addNumber(const QString& word)
{
    std::uniform_int_distribution<> numberDist(MIN_NUMBER, MAX_NUMBER);
    int randomNumber = numberDist(m_gen);

    return word + " " + QString::number(randomNumber);
}

void TypingInput::initializeStandartText() {
    standartText.clear();

    QDir directory(QString::fromStdString(PATH_TO_STANDART_TEXT));

    // Проверяем существование директории
    if (!directory.exists()) {
        qDebug() << "Директория не существует:" << QString::fromStdString(PATH_TO_STANDART_TEXT);
        return;
    }

    // Проверяем доступность директории для чтения
    if (!directory.isReadable()) {
        qDebug() << "Директория недоступна для чтения:" << QString::fromStdString(PATH_TO_STANDART_TEXT);
        return;
    }

    // Устанавливаем фильтр для JSON файлов
    QStringList filters;
    filters << "*.json";
    directory.setNameFilters(filters);
    directory.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);

    // Получаем список всех JSON файлов в директории
    QFileInfoList fileList = directory.entryInfoList();

    if (fileList.isEmpty()) {
        qDebug() << "В директории нет JSON файлов:" << QString::fromStdString(PATH_TO_STANDART_TEXT);
        return;
    }

    qDebug() << "Найдено JSON файлов:" << fileList.size();

    for (const QFileInfo& fileInfo : fileList) {
        String filePath = fileInfo.absoluteFilePath().toStdString();
        qDebug() << "Обрабатывается файл:" << QString::fromStdString(filePath);

        // Проверяем, что файл существует и доступен для чтения
        if (!fileInfo.exists() || !fileInfo.isReadable()) {
            qDebug() << "Файл недоступен:" << fileInfo.fileName();
            continue;
        }

        // Парсим стандартный текст из файла
        std::pair<String, Quote> pQuote = JSONParser::parseStandartText(filePath);

        if (!pQuote.second.text.empty()) {
            // Получаем язык из имени файла (например, "english.json" -> "english")
            String fileName = fileInfo.baseName().toStdString();
            standartText[pQuote.first] = pQuote.second.text;
            qDebug() << "Добавлен стандартный текст для языка:" << QString::fromStdString(fileName);
        } else {
            qDebug() << "Не удалось извлечь текст из файла:" << fileInfo.fileName();
        }
    }

    qDebug() << "Инициализация стандартных текстов завершена. Загружено языков:" << standartText.size();
}

void TypingInput::setLesson(const unsigned int lessonId, const String& currentLang){
    if (lessons[currentLang].empty()){
        MessageHelper::showError(this, "Ошибка", "Для выбранного языка нет уроков");
        reset();
        return;
    }
    if (lessonId >= lessons[currentLang].size()){
        MessageHelper::showError(this, "Ошибка", "Непредвиденная ошибка. Попробуйте выбрать урок вручную");
        reset();
        return;
    }
    setTargetText(lessons[currentLang][lessonId]);
}

void TypingInput::initializeLessons(const std::map<String, std::vector<Lesson>>& mp){
    for (const auto& [lang, v] : mp){
        lessons[lang].resize(v.size());
        for (int i = 0; i < v.size(); i++){
            lessons[lang][v.size() - i - 1] = QString::fromStdString(v[i].text);
        }
    }
}
