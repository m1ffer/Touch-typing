#include "TypingInput.h"
#include <QPainter>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QFontMetrics>
#include <QDebug>
#include <QFontDatabase>
#include <QScrollBar>

// Реализация TextDisplayWidget
QSize TypingInput::TextDisplayWidget::sizeHint() const
{
    if (m_parent->m_textLines.isEmpty()) {
        return QSize(600, 100);
    }

    QFontMetrics fm(m_parent->m_textFont);
    int lineHeight = fm.height();
    int padding = 30;

    int height = m_parent->m_textLines.size() * lineHeight + padding;
    int width = m_parent->viewport()->width();

    return QSize(width, height);
}

QPoint TypingInput::TextDisplayWidget::getCursorPosition() const
{
    if (m_parent->m_currentPosition >= m_parent->m_targetText.length()) return QPoint(-1, -1);

    QFontMetrics fm(m_parent->m_textFont);
    int lineHeight = fm.height();
    int y = 15 + fm.ascent();

    // Ищем в какой строке находится текущая позиция
    for (const TypingInput::TextLine &line : m_parent->m_textLines) {
        if (m_parent->m_currentPosition >= line.startPos && m_parent->m_currentPosition < line.endPos) {
            int x = 15;

            // Считаем ширину текста до курсора в этой строке
            for (int i = line.startPos; i < m_parent->m_currentPosition; ++i) {
                if (i >= m_parent->m_targetText.length()) break;
                x += fm.horizontalAdvance(m_parent->m_targetText.at(i));
            }

            return QPoint(x, y);
        }

        y += lineHeight;
    }

    return QPoint(-1, -1);
}

void TypingInput::TextDisplayWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // Заливаем фон
    painter.fillRect(rect(), QColor("#1a1a1a"));

    // Рисуем текст
    drawText(painter);

    // Рисуем курсор если есть фокус
    if (m_parent->hasFocus() && m_parent->m_cursorVisible && !m_parent->isCompleted()) {
        drawCursor(painter);
    }
}

void TypingInput::TextDisplayWidget::drawText(QPainter &painter)
{
    painter.setFont(m_parent->m_textFont);
    QFontMetrics fm(m_parent->m_textFont);

    int lineHeight = fm.height();
    int y = 15 + fm.ascent();

    // Рисуем каждую строку
    for (const TypingInput::TextLine &line : m_parent->m_textLines) {
        int x = 15;

        // Рисуем символы в строке
        for (int i = line.startPos; i < line.endPos; ++i) {
            if (i >= m_parent->m_targetText.length()) break;

            QChar ch = m_parent->m_targetText.at(i);
            int state = getCharState(i);
            QColor color = getColorForState(state);
            int charWidth = fm.horizontalAdvance(ch);

            painter.setPen(color);

            // Для текущей позиции добавляем подсветку фона
            if (i == m_parent->m_currentPosition && m_parent->hasFocus() && !m_parent->isCompleted()) {
                painter.fillRect(x, y - fm.ascent(), charWidth, fm.height(),
                                 QColor(226, 183, 20, 50));
            }

            // Для ошибок добавляем подчеркивание
            if (state == 2) {
                painter.fillRect(x, y + 2, charWidth, 2, m_parent->m_incorrectColor);
            }

            painter.drawText(x, y, QString(ch));
            x += charWidth;
        }

        y += lineHeight;
    }
}

void TypingInput::TextDisplayWidget::drawCursor(QPainter &painter)
{
    QPoint cursorPos = getCursorPosition();
    if (cursorPos.x() < 0) return;

    QFontMetrics fm(m_parent->m_textFont);

    painter.setPen(m_parent->m_cursorColor);
    painter.drawLine(cursorPos.x(), cursorPos.y() - fm.ascent() + 2,
                     cursorPos.x(), cursorPos.y() + fm.descent() - 2);
}

int TypingInput::TextDisplayWidget::getCharState(int position) const
{
    if (position < m_parent->m_currentPosition) {
        if (position < m_parent->m_typedText.length() &&
            m_parent->m_typedText.at(position) == m_parent->m_targetText.at(position)) {
            return 1; // correct - желтый
        } else {
            return 2; // incorrect - красный
        }
    } else {
        return 0; // pending - серый
    }
}

QColor TypingInput::TextDisplayWidget::getColorForState(int state) const
{
    switch (state) {
    case 0: return m_parent->m_pendingColor;
    case 1: return m_parent->m_correctColor;
    case 2: return m_parent->m_incorrectColor;
    default: return m_parent->m_pendingColor;
    }
}

// Основной класс TypingInput
TypingInput::TypingInput(QWidget *parent)
    : QScrollArea(parent)
    , m_currentPosition(0)
    , m_errorsCount(0)
    , m_correctChars(0)
    , m_cursorVisible(true)
    , m_cursorTimer(new QTimer(this))
    , m_layoutDirty(true)
    , m_textWidget(new TextDisplayWidget(this))
{
    // Настройка QScrollArea
    setWidget(m_textWidget);
    setWidgetResizable(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);

    // Стиль
    setStyleSheet("QScrollArea { background-color: #1a1a1a; border: 1px solid #444444; border-radius: 4px; }");

    // Критически важные настройки
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(400, 150);

    // Цвета как в MonkeyType
    m_pendingColor = QColor("#666666");
    m_correctColor = QColor("#e2b714");
    m_incorrectColor = QColor("#e74c3c");
    m_cursorColor = QColor("#e2b714");

    // Настройка красивого шрифта
    QStringList preferredFonts = {
        "Fira Code", "Cascadia Code", "JetBrains Mono",
        "Consolas", "Roboto Mono", "Monospace"
    };

    for (const QString &fontName : preferredFonts) {
        if (QFontDatabase::hasFamily(fontName)) {
            m_textFont = QFont(fontName, 16);
            qDebug() << "Using font:" << fontName;
            break;
        }
    }

    if (m_textFont.family().isEmpty()) {
        m_textFont = QFont("Monospace", 16);
        m_textFont.setStyleHint(QFont::Monospace);
    }

    m_textFont.setStyleStrategy(QFont::PreferAntialias);

    m_cursorTimer->start(500);
    connect(m_cursorTimer, &QTimer::timeout, this, &TypingInput::toggleCursor);

    // Тестовый текст
    m_targetText = "Это пример текста для тренировки слепой печати с поддержкой переноса строк как в настоящем MonkeyType. "
                   "Этот текст должен автоматически переноситься на новые строки и поддерживать прокрутку если не помещается в видимую область. "
                   "Теперь вы можете печатать длинные тексты без проблем с отображением.";
}

void TypingInput::setTargetText(const QString &text)
{
    m_targetText = text;
    m_layoutDirty = true;
    reset();
}

void TypingInput::reset()
{
    m_typedText.clear();
    m_currentPosition = 0;
    m_errorsCount = 0;
    m_correctChars = 0;
    m_layoutDirty = true;
    updateTextWidget();
    ensureCursorVisible();
}

double TypingInput::accuracy() const
{
    if (m_currentPosition == 0) return 100.0;
    return (static_cast<double>(m_correctChars) / m_currentPosition) * 100.0;
}

void TypingInput::wrapText()
{
    m_textLines.clear();
    if (m_targetText.isEmpty()) return;

    QFontMetrics fm(m_textFont);
    int availableWidth = viewport()->width() - 30;
    int lineHeight = fm.height();

    int startPos = 0;
    int textLength = m_targetText.length();

    while (startPos < textLength) {
        int endPos = startPos;
        int lineWidth = 0;

        while (endPos < textLength) {
            QChar ch = m_targetText.at(endPos);
            int charWidth = fm.horizontalAdvance(ch);

            if (lineWidth + charWidth <= availableWidth) {
                lineWidth += charWidth;
                endPos++;
            } else {
                break;
            }
        }

        if (endPos == startPos) {
            endPos = startPos + 1;
        }

        // Пытаемся найти границу слова для красивого переноса
        if (endPos < textLength) {
            int lastSpace = -1;
            for (int i = endPos - 1; i >= startPos; --i) {
                if (m_targetText.at(i).isSpace()) {
                    lastSpace = i;
                    break;
                }
            }

            if (lastSpace > startPos) {
                endPos = lastSpace + 1;
            }
        }

        TextLine line;
        line.startPos = startPos;
        line.endPos = endPos;
        m_textLines.append(line);

        startPos = endPos;
    }

    m_layoutDirty = false;
}

void TypingInput::updateTextLines()
{
    if (m_layoutDirty) {
        wrapText();
    }
}

void TypingInput::ensureCursorVisible()
{
    QPoint cursorPos = m_textWidget->getCursorPosition();
    if (cursorPos.x() < 0) return;

    QFontMetrics fm(m_textFont);
    int lineHeight = fm.height();

    // Вычисляем область вокруг курсора для прокрутки
    int cursorY = cursorPos.y() - fm.ascent();
    QRect cursorRect(0, cursorY, 10, lineHeight);

    // Прокручиваем к курсору
    ensureVisible(cursorRect.x(), cursorRect.y() + lineHeight/2, 10, lineHeight);
}

void TypingInput::keyPressEvent(QKeyEvent *event)
{
    if (isCompleted()) {
        event->ignore();
        return;
    }

    if (event->key() == Qt::Key_Backspace) {
        if (m_currentPosition > 0) {
            m_currentPosition--;

            if (m_currentPosition < m_typedText.length() &&
                m_typedText.at(m_currentPosition) != m_targetText.at(m_currentPosition)) {
                m_errorsCount--;
            }

            if (m_currentPosition < m_typedText.length()) {
                m_typedText.remove(m_currentPosition, 1);
            }

            updateTextWidget();
            ensureCursorVisible();
        }
        return;
    }

    if (event->key() == Qt::Key_Shift || event->key() == Qt::Key_Control ||
        event->key() == Qt::Key_Alt || event->key() == Qt::Key_Meta ||
        event->key() == Qt::Key_CapsLock || event->key() == Qt::Key_Tab ||
        event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        return;
    }

    QString text = event->text();
    if (!text.isEmpty() && text.at(0).isPrint()) {
        QChar typedChar = text.at(0);
        QChar targetChar = m_targetText.at(m_currentPosition);

        bool isCorrect = (typedChar == targetChar);

        if (m_currentPosition >= m_typedText.length()) {
            m_typedText.append(typedChar);
        } else {
            m_typedText[m_currentPosition] = typedChar;
        }

        if (isCorrect) {
            m_correctChars++;
        } else {
            m_errorsCount++;
            emit errorMade();
        }

        m_currentPosition++;

        emit characterTyped(isCorrect, typedChar);

        if (isCompleted()) {
            emit typingFinished();
        }

        updateTextWidget();
        ensureCursorVisible();
    }
}

void TypingInput::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    m_cursorVisible = true;
    updateTextWidget();
}

void TypingInput::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    m_cursorVisible = false;
    updateTextWidget();
}

void TypingInput::resizeEvent(QResizeEvent *event)
{
    m_layoutDirty = true;
    QScrollArea::resizeEvent(event);
    updateTextWidget();
}

void TypingInput::toggleCursor()
{
    if (hasFocus() && !isCompleted()) {
        m_cursorVisible = !m_cursorVisible;
        updateTextWidget();
    }
}

void TypingInput::updateTextWidget()
{
    updateTextLines();
    m_textWidget->update();
    m_textWidget->updateGeometry();
}
