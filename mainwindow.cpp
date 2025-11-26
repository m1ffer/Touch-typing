#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QButtonGroup>
#include <QDebug>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , modeButtonGroup(new QButtonGroup(this))
{
    ui->setupUi(this);

    qDebug() << "=== Начало инициализации MainWindow ===";

    // Находим кнопки по их objectName
    QPushButton *trainButton = findChild<QPushButton*>("trainButton");
    QPushButton *learnButton = findChild<QPushButton*>("learnButton");

    qDebug() << "Найден trainButton:" << (trainButton != nullptr);
    qDebug() << "Найден learnButton:" << (learnButton != nullptr);

    if (trainButton && learnButton) {
        // УБИРАЕМ все что связано с checkable и группами
        // trainButton->setCheckable(false); // или просто не устанавливаем
        // learnButton->setCheckable(false);

        // Прямое подключение кликов к разным слотам
        connect(trainButton, &QPushButton::clicked, this, &MainWindow::onTrainModeClicked);
        connect(learnButton, &QPushButton::clicked, this, &MainWindow::onLearnModeClicked);

        qDebug() << "Обычные обработчики подключены";

        // Если нужен общий стиль (но уже без активного/неактивного состояния)
        applyButtonStyle();

    } else {
        qDebug() << "ОШИБКА: Не все кнопки найдены!";

        // Диагностика
        QList<QPushButton*> allButtons = findChildren<QPushButton*>();
        for (QPushButton *btn : allButtons) {
            qDebug() << "Button:" << btn->objectName() << "Text:" << btn->text();
        }
    }
    setStyleSheet("background-color: #1a1a1a; color: #e6e6e6;");

    // Прямой доступ к TypingInput через UI
    TypingInput *input = ui -> typingInput;
    input->setTargetText("Этопримертекстадлятренировкислепойпечати...Этопримертекстадлятренировкислепойпечати... Это пример текста для тренировки слепой печати... Это пример текста для тренировки слепой печати... Это пример текста для тренировки слепой печати... Это пример текста для тренировки слепой печати... Это пример текста для тренировки слепой печати... Это пример текста для тренировки слепой печати... Это пример текста для тренировки слепой печати... Это пример текста для тренировки слепой печати...");

    connect(input, &TypingInput::textChanged, [](const QString &text) {
        qDebug() << "Введенный текст:" << text;
    });

    connect(input, &TypingInput::inputCompleted, []() {
        qDebug() << "Ввод завершен!";
    });
    qDebug() << "=== Завершение инициализации MainWindow ===";

}

void MainWindow::applyButtonStyle()
{
    qDebug() << "Применение стиля к кнопкам...";

    QPushButton *trainButton = findChild<QPushButton*>("trainButton");
    QPushButton *learnButton = findChild<QPushButton*>("learnButton");

    if (!trainButton || !learnButton) {
        qDebug() << "ОШИБКА: Кнопки не найдены в applyButtonStyle";
        return;
    }

    QString monkeyTypeStyle = R"(
        QPushButton {
            font-family: 'Roboto Mono', 'Consolas', monospace;
            font-size: 12px;
            font-weight: 400;
            padding: 8px 16px;
            border: 1px solid #3a3a3a;
            background-color: #2a2a2a;
            color: #cccccc;
            border-radius: 0px;
            text-transform: none;
            letter-spacing: 0.3px;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: #323232;
            border-color: #4a4a4a;
            color: #e6e6e6;
        }
        QPushButton:checked {
            background-color: #3a3a3a;
            color: #ffffff;
            border-color: #5a5a5a;
            font-weight: 500;
        }
        QPushButton:pressed {
            background-color: #454545;
        }
        QPushButton:focus {
            outline: none;
        }
    )";

    trainButton->setStyleSheet(monkeyTypeStyle);
    learnButton->setStyleSheet(monkeyTypeStyle);

    // Сегментированный вид
    trainButton->setStyleSheet(trainButton->styleSheet() +
                               " QPushButton {"
                               "     border-top-left-radius: 3px;"
                               "     border-bottom-left-radius: 3px;"
                               "     border-right: 1px solid #1a1a1a;"
                               " }");

    learnButton->setStyleSheet(learnButton->styleSheet() +
                               " QPushButton {"
                               "     border-top-right-radius: 3px;"
                               "     border-bottom-right-radius: 3px;"
                               "     border-left: none;"
                               " }");

    qDebug() << "Стиль успешно применен";
}


void MainWindow::onTrainModeClicked()
{
    qDebug() << "Режим тренировки активирован";
    // Логика для режима тренировки

}

void MainWindow::onLearnModeClicked()
{
    qDebug() << "Режим обучения активирован";
    // Логика для режима обучения

}

MainWindow::~MainWindow()
{
    delete ui;
    delete modeButtonGroup;
}
