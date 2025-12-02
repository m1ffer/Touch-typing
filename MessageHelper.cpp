#include "MessageHelper.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QPushButton>
#include <QApplication>
#include <QFontMetrics>

void MessageHelper::showInfo(QWidget* parent, const QString& title, const QString& text)
{
    createAndShowDialog(parent, title, text, "#ffd700");
}

void MessageHelper::showWarning(QWidget* parent, const QString& title, const QString& text)
{
    createAndShowDialog(parent, title, text, "#ffd700");
}

void MessageHelper::showError(QWidget* parent, const QString& title, const QString& text)
{
    createAndShowDialog(parent, title, text, "#ffd700");
}

void MessageHelper::createAndShowDialog(QWidget* parent, const QString& title,
                                        const QString& text, const QString& color)
{
    QDialog dialog(parent);
    dialog.setWindowTitle(title);
    dialog.setModal(true);

    // Устанавливаем только крестик закрытия
    dialog.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    // ЖЕСТКО ЗАКОДИРОВАННЫЙ ЖЕЛТЫЙ ЦВЕТ
    QString styleSheet = R"(
        QDialog {
            background-color: #1a1a1a;
            border: 2px solid #ffd700;
            border-radius: 0px;
        }
        QLabel {
            color: #ffd700;  /* ЖЕЛТЫЙ - прямо в коде */
            font-family: 'Roboto Mono', 'Consolas', monospace;
            font-size: 18px;
            padding: 30px;
        }
    )";

    dialog.setStyleSheet(styleSheet);

    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    // Текст с увеличенным шрифтом
    QLabel *textLabel = new QLabel(text, &dialog);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setMinimumWidth(400);

    // Дополнительно увеличим шрифт через QFont
    QFont font = textLabel->font();
    font.setPointSize(16);
    font.setBold(false);
    textLabel->setFont(font);

    mainLayout->addWidget(textLabel);

    // Размер окна
    QFontMetrics fm(textLabel->font());
    int textWidth = fm.horizontalAdvance(text);
    int dialogWidth = qMin(qMax(textWidth + 100, 450), 700);
    int lines = text.count('\n') + 1;
    int lineHeight = fm.height();
    int dialogHeight = qMax(lines * lineHeight + 100, 200);

    dialog.setFixedSize(dialogWidth, dialogHeight);

    dialog.exec();
}
