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
        // Настраиваем кнопки как переключаемые
        trainButton->setCheckable(true);
        learnButton->setCheckable(true);

        qDebug() << "Кнопки сделаны checkable";

        // Добавляем кнопки в группу с ID
        modeButtonGroup->addButton(trainButton, 0);
        modeButtonGroup->addButton(learnButton, 1);

        qDebug() << "Кнопки добавлены в QButtonGroup";

        // Включаем исключительный выбор
        modeButtonGroup->setExclusive(true);

        // Устанавливаем режим по умолчанию
        trainButton->setChecked(true);
        qDebug() << "trainButton установлен как активный по умолчанию";

        // УНИВЕРСАЛЬНОЕ ПОДКЛЮЧЕНИЕ СИГНАЛА
        // Используем лямбда-функцию для обхода проблем с сигнатурами
        connect(modeButtonGroup, &QButtonGroup::idClicked,
                this, &MainWindow::onModeChanged);
        qDebug() << "Сигнал подключен через idClicked";

        // ДУБЛИРУЮЩЕЕ ПОДКЛЮЧЕНИЕ для надежности
        connect(trainButton, &QPushButton::toggled, this, [this](bool checked) {
            if (checked) {
                qDebug() << "trainButton toggled -> calling onModeChanged(0)";
                this->onModeChanged(0);
            }
        });

        connect(learnButton, &QPushButton::toggled, this, [this](bool checked) {
            if (checked) {
                qDebug() << "learnButton toggled -> calling onModeChanged(1)";
                this->onModeChanged(1);
            }
        });

        qDebug() << "Дублирующие сигналы подключены";

        // Применяем стиль
        applyButtonStyle();
        qDebug() << "Стиль применен";

        qDebug() << "Текущий активный ID:" << modeButtonGroup->checkedId();

    } else {
        qDebug() << "ОШИБКА: Не все кнопки найдены!";

        // Выведем все доступные QPushButton для диагностики
        qDebug() << "=== Все QPushButton в интерфейсе: ===";
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

void MainWindow::onModeChanged(int id)
{
    qDebug() << "=== СЛОТ onModeChanged ВЫЗВАН ===";
    qDebug() << "Получен ID:" << id;
    qDebug() << "Текущий checkedId:" << modeButtonGroup->checkedId();

    // Блокировка повторного нажатия на активную кнопку
    int currentId = modeButtonGroup->checkedId();
    if (currentId == id) {
        qDebug() << "Повторное нажатие на активную кнопку - игнорируем";
        return;
    }

    // Логика переключения режимов
    switch (id) {
    case 0:
        qDebug() << ">>> Активирован режим: Обучение (trainButton)";
        // Ваш код для режима обучения
        break;
    case 1:
        qDebug() << ">>> Активирован режим: Свободный (learnButton)";
        // Ваш код для свободного режима
        break;
    default:
        qDebug() << ">>> Неизвестный режим ID:" << id;
        break;
    }

    qDebug() << "=== ЗАВЕРШЕНИЕ onModeChanged ===";
}

MainWindow::~MainWindow()
{
    delete ui;
    delete modeButtonGroup;
}
