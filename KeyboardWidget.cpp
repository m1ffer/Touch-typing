#include "KeyboardWidget.h"
#include <QDebug>

KeyboardWidget::KeyboardWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(3);

    initHandKeys();
    createLayout();
    updateDisplay();
    setButtonColors();
}

KeyboardWidget::~KeyboardWidget()
{
    qDeleteAll(buttons);
    buttons.clear();
}


void KeyboardWidget::initHandKeys()
{
    // Клавиши, которые нажимаются левой рукой (для них используем правый Shift)
    leftHandKeys = {
        "`", "1", "2", "3", "4", "5", "6",
        "Q", "W", "E", "R", "T",
        "A", "S", "D", "F", "G",
        "Z", "X", "C", "V", "B",
        "Tab", "Caps", "Shift", "Ctrl", "Win", "Alt"
    };

    // Клавиши, которые нажимаются правой рукой (для них используем левый Shift)
    rightHandKeys = {
        "7", "8", "9", "0", "-", "=", "Backspace",
        "Y", "U", "I", "O", "P", "[", "]", "\\",
        "H", "J", "K", "L", ";", "'", "Enter",
        "N", "M", ",", ".", "/",
        "Shift_R", "Ctrl_R", "AltGr", "Space"
    };
}

void KeyboardWidget::createLayout()
{
    if (mainLayout) {
        QLayoutItem* item;
        while ((item = mainLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    buttons.clear();

    // Ряд 1: ` 1 2 3 4 5 6 7 8 9 0 - = Backspace
    QHBoxLayout *row1 = new QHBoxLayout();
    row1->setSpacing(2);

    QStringList row1Keys = {"`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "Backspace"};
    for (const QString &key : row1Keys) {
        QPushButton *btn = new QPushButton(key, this);
        btn->setFixedSize(40, 40);
        if (key == "Backspace") btn->setFixedWidth(80);
        row1->addWidget(btn);
        buttons[key] = btn;
    }
    mainLayout->addLayout(row1);

    // Ряд 2: Tab Q W E R T Y U I O P [ ]
    QHBoxLayout *row2 = new QHBoxLayout();
    row2->setSpacing(2);

    QStringList row2Keys = {"Tab", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\\"};
    for (const QString &key : row2Keys) {
        QPushButton *btn = new QPushButton(key, this);
        btn->setFixedSize(40, 40);
        if (key == "Tab") btn->setFixedWidth(60);
        if (key == "\\") btn->setFixedWidth(60);
        row2->addWidget(btn);
        buttons[key] = btn;
    }
    mainLayout->addLayout(row2);

    // Ряд 3: Caps A S D F G H J K L ; ' Enter
    QHBoxLayout *row3 = new QHBoxLayout();
    row3->setSpacing(2);

    QStringList row3Keys = {"Caps", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "Enter"};
    for (const QString &key : row3Keys) {
        QPushButton *btn = new QPushButton(key, this);
        btn->setFixedSize(40, 40);
        if (key == "Caps") btn->setFixedWidth(60);
        if (key == "Enter") btn->setFixedWidth(80);
        row3->addWidget(btn);
        buttons[key] = btn;
    }
    mainLayout->addLayout(row3);

    // Ряд 4: Shift Z X C V B N M , . / Shift
    QHBoxLayout *row4 = new QHBoxLayout();
    row4->setSpacing(2);

    QStringList row4Keys = {"Shift", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "Shift_R"};
    for (int i = 0; i < row4Keys.size(); i++) {
        QString key = row4Keys[i];
        QPushButton *btn = new QPushButton(key, this);
        btn->setFixedSize(40, 40);
        if (key.contains("Shift")) btn->setFixedWidth(80);
        row4->addWidget(btn);
        buttons[key] = btn;
    }
    mainLayout->addLayout(row4);

    // Ряд 5: Ctrl Win Alt Space AltGr Ctrl
    QHBoxLayout *row5 = new QHBoxLayout();
    row5->setSpacing(2);

    QStringList row5Keys = {"Ctrl", "Win", "Alt", "Space", "AltGr", "Ctrl_R"};
    for (const QString &key : row5Keys) {
        QPushButton *btn = new QPushButton(key, this);
        btn->setFixedSize(40, 40);
        if (key == "Ctrl" || key == "Ctrl_R") btn->setFixedWidth(50);
        else if (key == "Alt" || key == "AltGr" || key == "Win") btn->setFixedWidth(50);
        else if (key == "Space") btn->setFixedWidth(200);
        row5->addWidget(btn);
        buttons[key] = btn;
    }
    mainLayout->addLayout(row5);
}

void KeyboardWidget::updateDisplay()
{
    // Полные карты для английской и русской раскладок
    static QMap<QString, QString> engNormalMap = {
        {"`", "`"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"}, {"6", "6"}, {"7", "7"}, {"8", "8"}, {"9", "9"}, {"0", "0"},
        {"-", "-"}, {"=", "="}, {"[", "["}, {"]", "]"}, {"\\", "\\"}, {";", ";"}, {"'", "'"}, {",", ","}, {".", "."}, {"/", "/"},
        {"Q", "q"}, {"W", "w"}, {"E", "e"}, {"R", "r"}, {"T", "t"}, {"Y", "y"}, {"U", "u"}, {"I", "i"}, {"O", "o"}, {"P", "p"},
        {"A", "a"}, {"S", "s"}, {"D", "d"}, {"F", "f"}, {"G", "g"}, {"H", "h"}, {"J", "j"}, {"K", "k"}, {"L", "l"},
        {"Z", "z"}, {"X", "x"}, {"C", "c"}, {"V", "v"}, {"B", "b"}, {"N", "n"}, {"M", "m"}
    };

    static QMap<QString, QString> engShiftMap = {
        {"`", "~"}, {"1", "!"}, {"2", "@"}, {"3", "#"}, {"4", "$"}, {"5", "%"}, {"6", "^"}, {"7", "&&"}, {"8", "*"}, {"9", "("}, {"0", ")"},
        {"-", "_"}, {"=", "+"}, {"[", "{"}, {"]", "}"}, {"\\", "|"}, {";", ":"}, {"'", "\""}, {",", "<"}, {".", ">"}, {"/", "?"},
        {"Q", "Q"}, {"W", "W"}, {"E", "E"}, {"R", "R"}, {"T", "T"}, {"Y", "Y"}, {"U", "U"}, {"I", "I"}, {"O", "O"}, {"P", "P"},
        {"A", "A"}, {"S", "S"}, {"D", "D"}, {"F", "F"}, {"G", "G"}, {"H", "H"}, {"J", "J"}, {"K", "K"}, {"L", "L"},
        {"Z", "Z"}, {"X", "X"}, {"C", "C"}, {"V", "V"}, {"B", "B"}, {"N", "N"}, {"M", "M"}
    };

    static QMap<QString, QString> ruNormalMap = {
        {"`", "ё"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"}, {"6", "6"}, {"7", "7"}, {"8", "8"}, {"9", "9"}, {"0", "0"},
        {"-", "-"}, {"=", "="}, {"[", "х"}, {"]", "ъ"}, {"\\", "\\"}, {";", "ж"}, {"'", "э"}, {",", "б"}, {".", "ю"}, {"/", "."},
        {"Q", "й"}, {"W", "ц"}, {"E", "у"}, {"R", "к"}, {"T", "е"}, {"Y", "н"}, {"U", "г"}, {"I", "ш"}, {"O", "щ"}, {"P", "з"},
        {"A", "ф"}, {"S", "ы"}, {"D", "в"}, {"F", "а"}, {"G", "п"}, {"H", "р"}, {"J", "о"}, {"K", "л"}, {"L", "д"},
        {"Z", "я"}, {"X", "ч"}, {"C", "с"}, {"V", "м"}, {"B", "и"}, {"N", "т"}, {"M", "ь"}
    };

    static QMap<QString, QString> ruShiftMap = {
        {"`", "Ё"}, {"1", "!"}, {"2", "\""}, {"3", "№"}, {"4", ";"}, {"5", "%"}, {"6", ":"}, {"7", "?"}, {"8", "*"}, {"9", "("}, {"0", ")"},
        {"-", "_"}, {"=", "+"}, {"[", "Х"}, {"]", "Ъ"}, {"\\", "/"}, {";", "Ж"}, {"'", "Э"}, {",", "Б"}, {".", "Ю"}, {"/", ","},
        {"Q", "Й"}, {"W", "Ц"}, {"E", "У"}, {"R", "К"}, {"T", "Е"}, {"Y", "Н"}, {"U", "Г"}, {"I", "Ш"}, {"O", "Щ"}, {"P", "З"},
        {"A", "Ф"}, {"S", "Ы"}, {"D", "В"}, {"F", "А"}, {"G", "П"}, {"H", "Р"}, {"J", "О"}, {"K", "Л"}, {"L", "Д"},
        {"Z", "Я"}, {"X", "Ч"}, {"C", "С"}, {"V", "М"}, {"B", "И"}, {"N", "Т"}, {"M", "Ь"}
    };

    // Определяем, какую карту использовать
    QMap<QString, QString> *currentMap;
    if (currentLayout == "русский") {
        currentMap = shiftPressed ? &ruShiftMap : &ruNormalMap;
    } else {
        currentMap = shiftPressed ? &engShiftMap : &engNormalMap;
    }

    // Обновляем текст на всех кнопках
    for (auto it = buttons.begin(); it != buttons.end(); ++it) {
        QString key = it.key();
        QPushButton *btn = it.value();

        // Пропускаем специальные клавиши (они не меняются)
        if (key == "Tab" || key == "Caps" || key == "Enter" || key == "Backspace" ||
            key == "Shift" || key == "Shift_R" || key == "Ctrl" || key == "Ctrl_R" ||
            key == "Alt" || key == "AltGr" || key == "Win" || key == "Space") {
            continue;
        }

        // Обновляем текст кнопки из карты
        if (currentMap->contains(key)) {
            btn->setText((*currentMap)[key]);
        }
    }
}

void KeyboardWidget::highlightKey(const QString &key)
{

    if (!keyboardEnabled) {
        return;
    }

    // Сначала сбрасываем все подсветки
    resetHighlights();

    // Проверяем, является ли клавиша специальной (не требующей Shift)
    // Это клавиши, которые не имеют символьного представления
    QStringList specialKeys = {"Enter", "Space", "Backspace", "Tab", "Caps",
                               "Shift", "Shift_R", "Ctrl", "Ctrl_R", "Alt", "AltGr", "Win"};

    // Проверяем, является ли это одним из специальных символов
    QString processedKey = key;
    if (key == " " || key == "Space" || key == "space") {
        processedKey = "Space";
    } else if (key == "\n" || key == "\r" || key == "\r\n" || key == "Enter") {
        processedKey = "Enter";
    }

    // Если это специальная клавиша, просто подсвечиваем ее
    if (specialKeys.contains(processedKey)) {
        if (buttons.contains(processedKey)) {
            buttons[processedKey]->setStyleSheet(
                "QPushButton {"
                "  background-color: #FF6464;"
                "  border: 2px solid #FF0000;"
                "  border-radius: 3px;"
                "  color: black;"
                "}"
                );
        }
        return;
    }

    // Определяем, нужен ли Shift для этого символа
    bool needsShift = false;
    QString searchKey = "";

    // Для символа '&' нужно использовать специальную обработку
    QString displayKey = processedKey;
    if (processedKey == "&") {
        displayKey = "&&";
    }

    if (processedKey.length() == 1) {
        QChar ch = processedKey[0];

        // Карты для обратного поиска (символ -> ключ)
        static QMap<QString, QString> engNormalToKey;
        static QMap<QString, QString> engShiftToKey;
        static QMap<QString, QString> ruNormalToKey;
        static QMap<QString, QString> ruShiftToKey;

        // Инициализируем обратные карты при первом вызове
        if (engNormalToKey.isEmpty()) {
            // Английская нормальная
            engNormalToKey = {
                {"`", "`"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"}, {"6", "6"}, {"7", "7"}, {"8", "8"}, {"9", "9"}, {"0", "0"},
                {"-", "-"}, {"=", "="}, {"[", "["}, {"]", "]"}, {"\\", "\\"}, {";", ";"}, {"'", "'"}, {",", ","}, {".", "."}, {"/", "/"},
                {"q", "Q"}, {"w", "W"}, {"e", "E"}, {"r", "R"}, {"t", "T"}, {"y", "Y"}, {"u", "U"}, {"i", "I"}, {"o", "O"}, {"p", "P"},
                {"a", "A"}, {"s", "S"}, {"d", "D"}, {"f", "F"}, {"g", "G"}, {"h", "H"}, {"j", "J"}, {"k", "K"}, {"l", "L"},
                {"z", "Z"}, {"x", "X"}, {"c", "C"}, {"v", "V"}, {"b", "B"}, {"n", "N"}, {"m", "M"}
            };

            // Английская с Shift
            engShiftToKey = {
                {"~", "`"}, {"!", "1"}, {"@", "2"}, {"#", "3"}, {"$", "4"}, {"%", "5"}, {"^", "6"}, {"&&", "7"}, {"*", "8"}, {"(", "9"}, {")", "0"},
                {"_", "-"}, {"+", "="}, {"{", "["}, {"}", "]"}, {"|", "\\"}, {":", ";"}, {"\"", "'"}, {"<", ","}, {">", "."}, {"?", "/"},
                {"Q", "Q"}, {"W", "W"}, {"E", "E"}, {"R", "R"}, {"T", "T"}, {"Y", "Y"}, {"U", "U"}, {"I", "I"}, {"O", "O"}, {"P", "P"},
                {"A", "A"}, {"S", "S"}, {"D", "D"}, {"F", "F"}, {"G", "G"}, {"H", "H"}, {"J", "J"}, {"K", "K"}, {"L", "L"},
                {"Z", "Z"}, {"X", "X"}, {"C", "C"}, {"V", "V"}, {"B", "B"}, {"N", "N"}, {"M", "M"}
            };

            // Русская нормальная
            ruNormalToKey = {
                {"ё", "`"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"}, {"6", "6"}, {"7", "7"}, {"8", "8"}, {"9", "9"}, {"0", "0"},
                {"-", "-"}, {"=", "="}, {"х", "["}, {"ъ", "]"}, {"\\", "\\"}, {"ж", ";"}, {"э", "'"}, {"б", ","}, {"ю", "."}, {".", "/"},
                {"й", "Q"}, {"ц", "W"}, {"у", "E"}, {"к", "R"}, {"е", "T"}, {"н", "Y"}, {"г", "U"}, {"ш", "I"}, {"щ", "O"}, {"з", "P"},
                {"ф", "A"}, {"ы", "S"}, {"в", "D"}, {"а", "F"}, {"п", "G"}, {"р", "H"}, {"о", "J"}, {"л", "K"}, {"д", "L"},
                {"я", "Z"}, {"ч", "X"}, {"с", "C"}, {"м", "V"}, {"и", "B"}, {"т", "N"}, {"ь", "M"}
            };

            // Русская с Shift
            ruShiftToKey = {
                {"Ё", "`"}, {"!", "1"}, {"\"", "2"}, {"№", "3"}, {";", "4"}, {"%", "5"}, {":", "6"}, {"?", "7"}, {"*", "8"}, {"(", "9"}, {")", "0"},
                {"_", "-"}, {"+", "="}, {"Х", "["}, {"Ъ", "]"}, {"/", "\\"}, {"Ж", ";"}, {"Э", "'"}, {"Б", ","}, {"Ю", "."}, {",", "/"},
                {"Й", "Q"}, {"Ц", "W"}, {"У", "E"}, {"К", "R"}, {"Е", "T"}, {"Н", "Y"}, {"Г", "U"}, {"Ш", "I"}, {"Щ", "O"}, {"З", "P"},
                {"Ф", "A"}, {"Ы", "S"}, {"В", "D"}, {"А", "F"}, {"П", "G"}, {"Р", "H"}, {"О", "J"}, {"Л", "K"}, {"Д", "L"},
                {"Я", "Z"}, {"Ч", "X"}, {"С", "C"}, {"М", "V"}, {"И", "B"}, {"Т", "N"}, {"Ь", "M"}
            };
        }

        // Выбираем нужную карту для поиска
        QMap<QString, QString> *searchMap = nullptr;
        if (currentLayout == "русский") {
            // Проверяем русские символы
            if (ruShiftToKey.contains(displayKey)) {
                searchMap = &ruShiftToKey;
                needsShift = true;
            } else if (ruNormalToKey.contains(displayKey)) {
                searchMap = &ruNormalToKey;
            }
        } else {
            // Проверяем английские символы
            if (engShiftToKey.contains(displayKey)) {
                searchMap = &engShiftToKey;
                needsShift = true;
            } else if (engNormalToKey.contains(displayKey)) {
                searchMap = &engNormalToKey;
            }
        }

        // Если нашли символ в карте
        if (searchMap && searchMap->contains(displayKey)) {
            searchKey = (*searchMap)[displayKey];
        }
    }

    // Если не нашли через карту, ищем по тексту на кнопках
    if (searchKey.isEmpty()) {
        for (auto it = buttons.begin(); it != buttons.end(); ++it) {
            QPushButton *btn = it.value();
            QString btnText = btn->text();

            // Сравниваем текст кнопки с искомым символом
            // Для символа '&' нужно сравнивать с "&&"
            QString btnCompareText = btnText;
            if (btnText == "&&") {
                btnCompareText = "&";
            }

            if (btnCompareText.compare(processedKey, Qt::CaseInsensitive) == 0) {
                searchKey = it.key();

                // Определяем, нужен ли Shift для найденного символа
                if (btnText.length() == 1 || btnText == "&&") {
                    QChar ch = (btnText == "&&") ? QChar('&') : btnText[0];
                    // Если это заглавная буква или спецсимвол, требующий Shift
                    static QString engShiftSymbols = "!@#$%^&*()_+{}|:\"<>?~";
                    static QString ruShiftSymbols = "!\"№;%:?*()_+/";

                    bool isUpper = ch.isUpper();
                    bool isEngSpecial = engShiftSymbols.contains(ch);
                    bool isRuSpecial = ruShiftSymbols.contains(ch);

                    if (isUpper || isEngSpecial || isRuSpecial) {
                        needsShift = true;
                    }
                }
                break;
            }
        }
    }

    // Если клавиша не найдена, выходим
    if (searchKey.isEmpty()) {
        return;
    }

    // Активируем режим Shift если нужно
    if (needsShift && !shiftPressed) {
        shiftPressed = true;
        updateDisplay();
    }

    // Определяем, какой Shift использовать (только если нужен Shift)
    QString shiftKeyToHighlight = "";
    if (needsShift) {
        if (leftHandKeys.contains(searchKey)) {
            // Клавиша левой руки, используем правый Shift
            shiftKeyToHighlight = "Shift_R";
        } else if (rightHandKeys.contains(searchKey)) {
            // Клавиша правой руки, используем левый Shift
            shiftKeyToHighlight = "Shift";
        }
    }

    // Подсвечиваем клавишу
    if (buttons.contains(searchKey)) {
        buttons[searchKey]->setStyleSheet(
            "QPushButton {"
            "  background-color: #FF6464;"
            "  border: 2px solid #FF0000;"
            "  border-radius: 3px;"
            "  color: black;"
            "}"
            );
    }

    // Подсвечиваем Shift, если нужно
    if (!shiftKeyToHighlight.isEmpty() && buttons.contains(shiftKeyToHighlight)) {
        buttons[shiftKeyToHighlight]->setStyleSheet(
            "QPushButton {"
            "  background-color: #FF6464;"
            "  border: 2px solid #FF0000;"
            "  border-radius: 3px;"
            "  color: black;"
            "}"
            );
    }
}

void KeyboardWidget::resetHighlights()
{

    if (!keyboardEnabled) {
        return;
    }

    shiftPressed = false;
    updateDisplay();
    setButtonColors();
}

void KeyboardWidget::setLayoutType(const QString &layout)
{

    if (layout == currentLayout || !keyboardEnabled) {
        return;
    }

    currentLayout = layout;
    resetHighlights();
}

void KeyboardWidget::setButtonColors()
{

    if (!keyboardEnabled) {
        return;
    }

    // Сначала сбрасываем все кнопки к серому цвету
    for (auto it = buttons.begin(); it != buttons.end(); ++it) {
        it.value()->setStyleSheet("background-color: #DCDCDC; border: 1px solid #888; border-radius: 3px; color: black;");
    }

    // Теперь красим цветные зоны

    // Левый мизинец (`, 1, 2, Q, A, Z) - светло-розовый
    QStringList leftPinky = {"`", "1", "2", "Q", "A", "Z"};
    for (const QString &key : leftPinky) {
        if (buttons.contains(key)) {
            buttons[key]->setStyleSheet("background-color: #FFB6C1; border: 1px solid #888; border-radius: 3px; color: black;");
        }
    }

    // Левый безымянный (3, W, S, X) - светло-оранжевый
    QStringList leftRing = {"3", "W", "S", "X"};
    for (const QString &key : leftRing) {
        if (buttons.contains(key)) {
            buttons[key]->setStyleSheet("background-color: #FFDAB9; border: 1px solid #888; border-radius: 3px; color: black;");
        }
    }

    // Левый средний (4, E, D, C) - светло-желтый
    QStringList leftMiddle = {"4", "E", "D", "C"};
    for (const QString &key : leftMiddle) {
        if (buttons.contains(key)) {
            buttons[key]->setStyleSheet("background-color: #FFFACD; border: 1px solid #888; border-radius: 3px; color: black;");
        }
    }

    // Левый указательный (5, 6, R, T, F, G, V, B) - светло-зеленый
    QStringList leftIndex = {"5", "6", "R", "T", "F", "G", "V", "B"};
    for (const QString &key : leftIndex) {
        if (buttons.contains(key)) {
            buttons[key]->setStyleSheet("background-color: #98FB98; border: 1px solid #888; border-radius: 3px; color: black;");
        }
    }

    // Правый указательный (7, Y, U, H, J, N, M) - светло-голубой
    QStringList rightIndex = {"7", "Y", "U", "H", "J", "N", "M"};
    for (const QString &key : rightIndex) {
        if (buttons.contains(key)) {
            buttons[key]->setStyleSheet("background-color: #E0FFFF; border: 1px solid #888; border-radius: 3px; color: black;");
        }
    }

    // Правый средний (8, I, K, ,) - лавандовый
    QStringList rightMiddle = {"8", "I", "K", ","};
    for (const QString &key : rightMiddle) {
        if (buttons.contains(key)) {
            buttons[key]->setStyleSheet("background-color: #E6E6FA; border: 1px solid #888; border-radius: 3px; color: black;");
        }
    }

    // Правый безымянный (9, O, L, .) - светло-фиолетовый
    QStringList rightRing = {"9", "O", "L", "."};
    for (const QString &key : rightRing) {
        if (buttons.contains(key)) {
            buttons[key]->setStyleSheet("background-color: #D8BFD8; border: 1px solid #888; border-radius: 3px; color: black;");
        }
    }

    // Правый мизинец (0, -, =, P, [, ], \, ;, ', /, Shift_R, Enter, Backspace) - розовый
    QStringList rightPinky = {"0", "-", "=", "P", "[", "]", "\\", ";", "'", "/", "Shift_R", "Enter", "Backspace"};
    for (const QString &key : rightPinky) {
        if (buttons.contains(key)) {
            buttons[key]->setStyleSheet("background-color: #FFC0CB; border: 1px solid #888; border-radius: 3px; color: black;");
        }
    }

    // Левый Shift (отдельно, т.к. левый мизинец)
    if (buttons.contains("Shift")) {
        buttons["Shift"]->setStyleSheet("background-color: #FFB6C1; border: 1px solid #888; border-radius: 3px; color: black;");
    }

    // Пробел - цвет для больших пальцев (светло-коричневый, чтобы был заметен)
    if (buttons.contains("Space")) {
        buttons["Space"]->setStyleSheet("background-color: #D2B48C; border: 1px solid #888; border-radius: 3px; color: black;");
    }

    // Остальные клавиши остаются серыми (как было установлено в начале):
    // Tab, Caps, Ctrl, Ctrl_R, Alt, Win, AltGr
    // Эти клавиши уже имеют серый цвет #DCDCDC
}

void KeyboardWidget::enable()
{
    if (keyboardEnabled) {
        return;
    }

    keyboardEnabled = true;
    this->setVisible(true);
    this->updateGeometry();
    setButtonColors();

    // Обновляем layout родителя
    if (parentWidget() && parentWidget()->layout()) {
        parentWidget()->layout()->activate();
    }
}

void KeyboardWidget::disable()
{
    if (!keyboardEnabled) {
        return;
    }

    keyboardEnabled = false;
    this->setVisible(false);
    this->updateGeometry();

    // Обновляем layout родителя
    if (parentWidget() && parentWidget()->layout()) {
        parentWidget()->layout()->activate();
    }
}

bool KeyboardWidget::isEnabled() const
{
    return keyboardEnabled;
}
