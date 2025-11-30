// typinginput.h
#ifndef TYPINGINPUT_H
#define TYPINGINPUT_H

#include <QWidget>
#include <QTextEdit>
#include <QTextCursor>
#include <QPainter>
#include <QScrollBar>
#include <QKeyEvent>
#include <QFont>
#include <QTimer>
#include <QTextCharFormat>
#include "JSONParser.h"
#include <map> // ДОБАВИТЬ
#include <random> // ДОБАВИТЬ для случайных чисел
#include <vector> // ДОБАВИТЬ

class TypingInput : public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(QColor cursorColor READ cursorColor WRITE setCursorColor DESIGNABLE true)
    Q_PROPERTY(QColor correctTextColor READ correctTextColor WRITE setCorrectTextColor DESIGNABLE true)
    Q_PROPERTY(QColor incorrectTextColor READ incorrectTextColor WRITE setIncorrectTextColor DESIGNABLE true)
    Q_PROPERTY(QColor pendingTextColor READ pendingTextColor WRITE setPendingTextColor DESIGNABLE true)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor DESIGNABLE true)

public:
    explicit TypingInput(QWidget *parent = nullptr);

    // ДОБАВЛЕНО: Метод для генерации текста по настройкам
    QString makeTextFromSettings(const Settings& settings);

    void setTargetText(const QString &text);
    void reset();
    // Методы для работы со временем
    void startTimer();
    void stopTimer();
    qint64 getElapsedTime() const;
    bool isTimerRunning() const;

    QColor cursorColor() const { return m_cursorColor; }
    void setCursorColor(const QColor &color);

    QColor correctTextColor() const { return m_correctTextColor; }
    void setCorrectTextColor(const QColor &color) { m_correctTextColor = color; updateTextColors(); }

    QColor incorrectTextColor() const { return m_incorrectTextColor; }
    void setIncorrectTextColor(const QColor &color) { m_incorrectTextColor = color; updateTextColors(); }

    QColor pendingTextColor() const { return m_pendingTextColor; }
    void setPendingTextColor(const QColor &color) { m_pendingTextColor = color; updateTextColors(); }

    QColor backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor &color) { m_backgroundColor = color; updateStyle(); }
    bool setTargetTextFromFile(const QString &filePath);
    // ДОБАВЛЕНО: Методы для статистики
    int getErrorsCount() const { return m_errorsCount; }
    int getTotalCharsTyped() const { return m_totalCharsTyped; }
    double getAccuracy() const;
    double getSpeedWpm() const; // Words Per Minute
    double getSpeedCpm() const; // Characters Per Minute
    // ДОБАВИТЬ: Методы для получения данных графика
    QVector<QPair<qint64, double>> getSpeedHistory() const { return m_speedHistory; }
    QVector<QPair<qint64, double>> getAccuracyHistory() const { return m_accuracyHistory; }
     //QVector<QPair<qint64, double>> getSpeedHistory() const;
signals:
    void textChanged(const QString &inputText);
    void inputCompleted();
    void timerStarted(); // ДОБАВЛЕНО
    void timerStopped(qint64 elapsedTime); // ДОБАВЛЕНО
    void timerUpdated(qint64 elapsedTime); // ДОБАВЛЕНО

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateTextColors();
    void updateStyle();
    void ensureCursorVisible();
    void checkCharacter(int position, QChar enteredChar);
    void updateCursorPosition();
    void initializeStandartText();

    QColor m_cursorColor;
    QColor m_correctTextColor;
    QColor m_incorrectTextColor;
    QColor m_pendingTextColor;
    QColor m_backgroundColor;

    QString m_targetText;
    QString m_enteredText;
    int m_currentPosition;

    // ДОБАВЛЕНО: Переменные для отслеживания времени
    QElapsedTimer m_typingTimer;
    QTimer *m_updateTimer;
    bool m_timerActive;
    // ДОБАВЛЕНО: Статистика ошибок
    int m_errorsCount;
    int m_totalCharsTyped;
    // ДОБАВИТЬ: История для графика
    QVector<QPair<qint64, double>> m_speedHistory;    // время -> скорость
    QVector<QPair<qint64, double>> m_accuracyHistory; // время -> точность
    QTimer *m_historyTimer;  // Таймер для записи истории
     qint64 m_finalTimeMs; // ДОБАВИТЬ: финальное время
    std::map<std::string, std::vector<Quote>> quotes;
    std::map<std::string, std::vector<Word>> shortWords;
    std::map<std::string, std::vector<Word>> longWords;

    // ДОБАВЛЕНО: Константы для генерации текста
                  static const int MIN_WORDS = 15;
    static const int MAX_WORDS = 30;
    static const double PUNCTUATION_PROBABILITY;
    static const double NUMBER_PROBABILITY;
    static const int MIN_NUMBER = 1;
    static const int MAX_NUMBER = 999;

    // ДОБАВЛЕНО: Вспомогательные методы для генерации текста
    QString getRandomWord(const Settings& settings);
    Quote getRandomQuote(const std::string& language);
    QString capitalizeWord(const QString& word);
    QString addPunctuation(const QString& word);
    QString addNumber(const QString& word);

    // ДОБАВЛЕНО: Генератор случайных чисел
    std::random_device m_rd;
    std::mt19937 m_gen;
    const std::string PATH_TO_STANDART_TEXT = "../../res/languages/standartText";
    std::map<String, String> standartText;
};

#endif // TYPINGINPUT_H
