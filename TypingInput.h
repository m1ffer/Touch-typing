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

    void setTargetText(const QString &text);
    void reset();

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

signals:
    void textChanged(const QString &inputText);
    void inputCompleted();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateTextColors();
    void updateStyle();
    void ensureCursorVisible();
    void checkCharacter(int position, QChar enteredChar);
    void updateCursorPosition();
    int findLastCorrectSpace();

    QColor m_cursorColor;
    QColor m_correctTextColor;
    QColor m_incorrectTextColor;
    QColor m_pendingTextColor;
    QColor m_backgroundColor;

    QString m_targetText;
    int m_currentPosition;
};

#endif // TYPINGINPUT_H
