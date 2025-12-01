// MessageHelper.cpp (с разными стилями)
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
    createAndShowDialog(parent, title, text, "#3498db");
}

void MessageHelper::showWarning(QWidget* parent, const QString& title, const QString& text)
{
    createAndShowDialog(parent, title, text, "#f39c12");
}

void MessageHelper::showError(QWidget* parent, const QString& title, const QString& text)
{
    createAndShowDialog(parent, title, text, "#e74c3c");
}

void MessageHelper::createAndShowDialog(QWidget* parent, const QString& title,
                                        const QString& text, const QString& color)
{
    QDialog dialog(parent);
    dialog.setWindowTitle(title);
    dialog.setModal(true);

    // Стиль с динамическим цветом
    QString styleSheet = QString(
                             "QDialog {"
                             "    background-color: #2a2a2a;"
                             "    border: 2px solid %1;"
                             "    border-radius: 8px;"
                             "}"
                             "QLabel {"
                             "    color: #e6e6e6;"
                             "    font-family: 'Consolas', monospace;"
                             "    font-size: 12px;"
                             "    padding: 15px;"
                             "}"
                             "QPushButton {"
                             "    background-color: %1;"
                             "    color: #ffffff;"
                             "    border: none;"
                             "    border-radius: 4px;"
                             "    padding: 8px 20px;"
                             "    font-weight: bold;"
                             "    font-size: 12px;"
                             "    min-width: 80px;"
                             "}"
                             "QPushButton:hover {"
                             "    background-color: %2;"
                             "}"
                             "QPushButton:pressed {"
                             "    background-color: %3;"
                             "}"
                             ).arg(color).arg(darkenColor(color)).arg(darkenColor(darkenColor(color)));

    dialog.setStyleSheet(styleSheet);

    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // Текст
    QLabel *textLabel = new QLabel(text, &dialog);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setMinimumWidth(300);

    // Кнопка
    QPushButton *okButton = new QPushButton("OK", &dialog);
    okButton->setFixedSize(80, 30);
    QObject::connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    // Центрирование кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();

    mainLayout->addWidget(textLabel);
    mainLayout->addLayout(buttonLayout);

    // Размер
    QFontMetrics fm(textLabel->font());
    int textWidth = fm.horizontalAdvance(text);
    int dialogWidth = qMin(qMax(textWidth + 60, 350), 600);
    dialog.setFixedWidth(dialogWidth);
    dialog.setFixedHeight(150);

    dialog.exec();
}

QString MessageHelper::darkenColor(const QString& color)
{
    // Простая функция для затемнения цвета
    if (color == "#3498db") return "#2980b9"; // синий
    if (color == "#f39c12") return "#d68910"; // оранжевый
    if (color == "#e74c3c") return "#c0392b"; // красный
    return color;
}
