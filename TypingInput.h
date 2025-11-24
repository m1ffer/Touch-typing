#ifndef TYPINGINPUT_H
#define TYPINGINPUT_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include <QColor>
#include <QVector>
#include <QFont>
#include <QScrollArea>

class TypingInput : public QScrollArea
{
    Q_OBJECT
    Q_PROPERTY(QString targetText READ targetText WRITE setTargetText NOTIFY targetTextChanged)

public:
    explicit TypingInput(QWidget *parent = nullptr);

    QString targetText() const { return m_targetText; }
    void setTargetText(const QString &text);

    void reset();
    bool isCompleted() const { return m_currentPosition >= m_targetText.length(); }
    int currentPosition() const { return m_currentPosition; }
    int errorsCount() const { return m_errorsCount; }
    double accuracy() const;

    // Методы для установки цветов
    void setPendingColor(const QColor &color) { m_pendingColor = color; updateTextWidget(); }
    void setCorrectColor(const QColor &color) { m_correctColor = color; updateTextWidget(); }
    void setIncorrectColor(const QColor &color) { m_incorrectColor = color; updateTextWidget(); }
    void setCursorColor(const QColor &color) { m_cursorColor = color; updateTextWidget(); }

signals:
    void targetTextChanged();
    void characterTyped(bool correct, QChar character);
    void typingFinished();
    void errorMade();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void toggleCursor();

private:
    struct TextLine {
        int startPos;
        int endPos;
    };

    class TextDisplayWidget : public QWidget
    {
    public:
        TextDisplayWidget(TypingInput *parent) : QWidget(parent), m_parent(parent) {}
        QSize sizeHint() const override;

        // Публичный метод для доступа к позиции курсора
        QPoint getCursorPosition() const;

    protected:
        void paintEvent(QPaintEvent *event) override;

    private:
        void drawText(QPainter &painter);
        void drawCursor(QPainter &painter);
        int getCharState(int position) const;
        QColor getColorForState(int state) const;

        TypingInput *m_parent;
    };

    void updateTextLines();
    void wrapText();
    void ensureCursorVisible();
    void updateTextWidget();

    QString m_targetText;
    QString m_typedText;
    int m_currentPosition;
    int m_errorsCount;
    int m_correctChars;

    // Цвета как в MonkeyType
    QColor m_pendingColor;
    QColor m_correctColor;
    QColor m_incorrectColor;
    QColor m_cursorColor;

    // Курсор
    bool m_cursorVisible;
    QTimer *m_cursorTimer;

    // Для многострочного текста
    QVector<TextLine> m_textLines;
    bool m_layoutDirty;
    QFont m_textFont;

    // Виджет для отображения текста
    TextDisplayWidget *m_textWidget;
};

#endif // TYPINGINPUT_H
