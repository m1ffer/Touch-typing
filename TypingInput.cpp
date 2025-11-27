// typinginput.cpp
#include "typinginput.h"
#include <QTextBlock>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

TypingInput::TypingInput(QWidget *parent)
    : QTextEdit(parent),
    m_cursorColor(QColor(255, 255, 0)),
    m_correctTextColor(QColor(255, 255, 0)),
    m_incorrectTextColor(QColor(255, 0, 0)),
    m_pendingTextColor(QColor(128, 128, 128)),
    m_backgroundColor(QColor(40, 40, 40)),
    m_currentPosition(0),
    m_errorsCount(0),
    m_totalCharsTyped(0),
    m_timerActive(false),
    m_finalTimeMs(0) // ДОБАВИТЬ
{
    // Настройка внешнего вида
    setFrameStyle(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Настройка шрифта
    QFont font("Consolas", 16, QFont::Normal);
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

            qDebug() << "Записана точка графика:" << currentTime << "мс," << currentSpeed << "CPM";
        }
    });

    // Начальные настройки
    updateStyle();
}

void TypingInput::setCursorColor(const QColor &color)
{
    m_cursorColor = color;
    updateStyle();
}

void TypingInput::setTargetText(const QString &text)
{
    m_targetText = text;
    m_enteredText.clear();  // ДОБАВЛЕНО: очищаем введенный текст
    m_currentPosition = 0;
    clear();

    // ДОБАВЛЕНО: Сбрасываем таймер при установке нового текста
    stopTimer();

    // Устанавливаем серый цвет для всего целевого текста
    QTextCharFormat format;
    format.setForeground(m_pendingTextColor);
    setCurrentCharFormat(format);

    setPlainText(m_targetText);

    // Возвращаем курсор в начало
    updateCursorPosition();

    updateTextColors();
}

void TypingInput::reset()
{
    m_enteredText.clear();  // ДОБАВЛЕНО: очищаем введенный текст
    m_currentPosition = 0;
    // ДОБАВЛЕНО: Сбрасываем таймер
    stopTimer();
    setTargetText(m_targetText);
    m_errorsCount = 0; // ДОБАВЛЕНО
    m_totalCharsTyped = 0; // ДОБАВЛЕНО
    m_speedHistory.clear();     // ДОБАВИТЬ
    m_accuracyHistory.clear();  // ДОБАВИТЬ
    m_finalTimeMs = 0; // СБРАСЫВАЕМ время
}

void TypingInput::keyPressEvent(QKeyEvent *event)
{
    // Игнорируем клавиши навигации
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ||
        event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ||
        event->key() == Qt::Key_Home || event->key() == Qt::Key_End) {
        return;
    }

    // Обрабатываем Backspace - ИЗМЕНЕННАЯ ЛОГИКА
    if (event->key() == Qt::Key_Backspace) {
        if (m_currentPosition > 0) {
            // Проверяем, является ли текущая позиция опорным пробелом
            bool isReferenceSpace = (m_currentPosition > 0 &&
                                     m_targetText[m_currentPosition - 1] == ' ' &&
                                     m_enteredText[m_currentPosition - 1] == ' ');

            if (!isReferenceSpace) {
                m_currentPosition--;
                m_enteredText.chop(1);  // Удаляем последний символ из введенного текста
                checkCharacter(m_currentPosition, QChar());

                // Обновляем позицию курсора после Backspace
                updateCursorPosition();

                // Emit text changed signal
                emit textChanged(m_enteredText);
            }
        }
        return;
    }

    // Игнорируем специальные клавиши
    if (event->key() == Qt::Key_Shift || event->key() == Qt::Key_Control ||
        event->key() == Qt::Key_Alt || event->key() == Qt::Key_Meta) {
        return;
    }

    // Обрабатываем обычные символы - ДОБАВЛЕНО сохранение в m_enteredText
    if (!event->text().isEmpty() && m_currentPosition < m_targetText.length()) {
        // ДОБАВЛЕНО: Запускаем таймер при первом нажатии
        if (!m_timerActive && m_enteredText.isEmpty()) {
            startTimer();
        }
        QChar enteredChar = event->text().at(0);
        m_enteredText += enteredChar;  // ДОБАВЛЕНО: сохраняем введенный символ
        m_totalCharsTyped++; // ДОБАВЛЕНО: Счетчик всех нажатий

        // ДОБАВЛЕНО: Подсчет ошибок
        if (enteredChar != m_targetText.at(m_currentPosition)) {
            m_errorsCount++;
        }
        checkCharacter(m_currentPosition, enteredChar);
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
    }

    update();
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
    for (int i = 0; i < m_targetText.length(); ++i) {
        cursor.setPosition(i);
        cursor.setPosition(i + 1, QTextCursor::KeepAnchor);

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

void TypingInput::updateStyle()
{
    setStyleSheet(QString("QTextEdit { "
                          "background-color: %1; "
                          "color: %2; "
                          "border: 2px solid %3; "
                          "border-radius: 5px; "
                          "padding: 10px; "
                          "caret-color: %4; "  // Цвет курсора
                          "}")
                      .arg(m_backgroundColor.name())
                      .arg(m_pendingTextColor.name())
                      .arg(m_pendingTextColor.darker(150).name())
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
    cursor.setPosition(position);
    cursor.setPosition(position + 1, QTextCursor::KeepAnchor);

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

void TypingInput::updateCursorPosition()
{
    // Обновляем позицию курсора
    QTextCursor cursor = textCursor();
    cursor.setPosition(m_currentPosition);
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
        qDebug() << "Таймер запущен";
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

