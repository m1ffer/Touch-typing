// scrollbuttonwidget.cpp
#include "scrollbuttonwidget.h"
#include <QScrollBar>

ScrollButtonWidget::ScrollButtonWidget(QWidget *parent)
    : QWidget(parent)
{
    // Создаем основную компоновку
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Создаем область прокрутки
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Создаем виджет для содержимого
    m_contentWidget = new QWidget();
    m_contentLayout = new QVBoxLayout(m_contentWidget);
    m_contentLayout->setAlignment(Qt::AlignTop);
    m_contentLayout->setSpacing(2);
    m_contentLayout->setContentsMargins(2, 2, 2, 2);

    // Устанавливаем виджет содержимого в область прокрутки
    m_scrollArea->setWidget(m_contentWidget);

    // Добавляем область прокрутки в основную компоновку
    mainLayout->addWidget(m_scrollArea);
}

// В методе addButton ДОБАВЬТЕ:
void ScrollButtonWidget::addButton(const QString &text, const QVariant &data)
{
    QPushButton *button = new QPushButton(text, m_contentWidget);
    button->setProperty("buttonData", data);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    button->setMinimumHeight(45);

    QString buttonStyle = R"(
        QPushButton {
            background-color: #2a2a2a;
            color: #ffd700;
            border: 1px solid #3a3a3a;
            padding: 10px 15px;
            text-align: left;
            border-radius: 0px;
            font-family: 'Roboto Mono', 'Consolas', monospace;
            font-size: 16px;
            margin: 2px 0px;
        }
        QPushButton:hover {
            background-color: #3a3a3a;
            border-color: #ffd700;
            color: #ffd700;
        }
        QPushButton:pressed {
            background-color: #4a4a2a;
            color: #ffd700;
        }
        QPushButton:focus {
            outline: none;
            border-color: #ffd700;
        }
    )";

    button->setStyleSheet(buttonStyle);
    connect(button, &QPushButton::clicked, this, &ScrollButtonWidget::onButtonClicked);

    m_contentLayout->addWidget(button);
    m_buttons.append(button);
}

void ScrollButtonWidget::addButtons(const QStringList &texts)
{
    for (const QString &text : texts) {
        addButton(text);
    }
}

void ScrollButtonWidget::clearButtons()
{
    // Удаляем все кнопки из layout
    QLayoutItem *item;
    while ((item = m_contentLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    // Очищаем список кнопок
    m_buttons.clear();
}

void ScrollButtonWidget::setButtonStyle(const QString &style)
{
    for (QPushButton *button : m_buttons) {
        button->setStyleSheet(style);
    }
}

void ScrollButtonWidget::onButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString text = button->text();
        QVariant data = button->property("buttonData");
        emit buttonClicked(text, data);
    }
}

ScrollButtonWidget::~ScrollButtonWidget(){
    delete(m_scrollArea);
}
