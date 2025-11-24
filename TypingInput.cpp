// typinginput.cpp
#include "typinginput.h"
#include <QTextBlock>
#include <QDebug>

TypingInput::TypingInput(QWidget *parent)
    : QTextEdit(parent),
    m_cursorColor(QColor(255, 255, 0)),
    m_correctTextColor(QColor(255, 255, 0)),
    m_incorrectTextColor(QColor(255, 0, 0)),
    m_pendingTextColor(QColor(128, 128, 128)),
    m_backgroundColor(QColor(40, 40, 40)),
    m_currentPosition(0)
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
    m_currentPosition = 0;
    clear();

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
    m_currentPosition = 0;
    setTargetText(m_targetText);
}

void TypingInput::keyPressEvent(QKeyEvent *event)
{
    // Игнорируем клавиши навигации
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ||
        event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ||
        event->key() == Qt::Key_Home || event->key() == Qt::Key_End) {
        return;
    }

    // Обрабатываем Backspace
    if (event->key() == Qt::Key_Backspace) {
        if (m_currentPosition > 0) {
            m_currentPosition--;
            checkCharacter(m_currentPosition, QChar());

            // Обновляем позицию курсора после Backspace
            updateCursorPosition();

            // Emit text changed signal
            QString currentText = toPlainText();
            emit textChanged(currentText.left(m_currentPosition));
        }
        return;
    }

    // Игнорируем специальные клавиши
    if (event->key() == Qt::Key_Shift || event->key() == Qt::Key_Control ||
        event->key() == Qt::Key_Alt || event->key() == Qt::Key_Meta) {
        return;
    }

    // Обрабатываем обычные символы
    if (!event->text().isEmpty() && m_currentPosition < m_targetText.length()) {
        QChar enteredChar = event->text().at(0);
        checkCharacter(m_currentPosition, enteredChar);
        m_currentPosition++;

        // Обновляем позицию курсора после ввода символа
        updateCursorPosition();

        // Emit text changed signal
        QString currentText = toPlainText();
        emit textChanged(currentText.left(m_currentPosition));

        ensureCursorVisible();

        // Check if input is completed
        if (m_currentPosition >= m_targetText.length()) {
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

    // Прокручиваем чтобы курсор был виден
    QTextEdit::ensureCursorVisible();
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
