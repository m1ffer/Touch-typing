#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QButtonGroup>
#include <QDebug>
#include <QList>
#include <QLabel> // ДОБАВЛЕНО

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , modeButtonGroup(new QButtonGroup(this))
    , m_currentLessonId(0) // ДОБАВЛЕНО
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
        connect(ui -> resetButton, &QPushButton :: clicked, this, &MainWindow :: onResetButtonClicked);
        connect(ui -> closeChoseButton, &QPushButton :: clicked, this, &MainWindow :: onCloseChoseButtonClicked);
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

    // ДОБАВЛЕНО: Подключаем сигналы таймера
    connect(input, &TypingInput::timerStarted, this, &MainWindow::onTypingTimerStarted);
    connect(input, &TypingInput::timerStopped, this, &MainWindow::onTypingTimerStopped);
    connect(input, &TypingInput::timerUpdated, this, &MainWindow::onTypingTimerUpdated);

    connect(input, &TypingInput::textChanged, [](const QString &text) {
        qDebug() << "Введенный текст:" << text;
    });

    connect(input, &TypingInput::inputCompleted, []() {
        qDebug() << "Ввод завершен!";
    });

    connect(input, &TypingInput::textChanged, [](const QString &text) {
        qDebug() << "Введенный текст:" << text;
    });

    connect(input, &TypingInput::inputCompleted, []() {
        qDebug() << "Ввод завершен!";
    });
    // ДОБАВЛЕНО: Сбрасываем отображение таймера
    resetTimerDisplay();

    // Устанавливаем начальную страницу (тренировка по умолчанию)
    initializeLessons();
    switchToTrainingMode();

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
    switchToTrainingMode();
    // Логика для режима тренировки

}

void MainWindow::onLearnModeClicked()
{
    qDebug() << "Режим обучения активирован";
    switchToLearningMode();
    // Логика для режима обучения

}

// НОВЫЕ МЕТОДЫ ДЛЯ ПЕРЕКЛЮЧЕНИЯ СТРАНИЦ
void MainWindow::switchToTrainingMode()
{
    // Переключаем modesStackedWidget на страницу inputMode
    ui->modesStackedWidget->setCurrentWidget(ui->inputMode);
    qDebug() << "Переключено на страницу тренировки (inputMode)";
}

void MainWindow::switchToLearningMode()
{
    // Переключаем modesStackedWidget на страницу choseMode
    ui->modesStackedWidget->setCurrentWidget(ui->choseMode);
    qDebug() << "Переключено на страницу выбора уроков (choseMode)";

    // Здесь можно добавить инициализацию списка уроков
    // initializeLessons();
}

void MainWindow::initializeLessons()
{
    // Здесь можно инициализировать список уроков для ScrollButtonWidget
    ScrollButtonWidget *lessonWidget = ui->choseWidget; // предполагая, что это ваш ScrollButtonWidget

    if (lessonWidget) {
        lessonWidget->clearButtons();

        // Добавляем уроки (пример)
        QStringList lessons = {
            "Урок 1: Основной ряд",
            "Урок 2: Верхний ряд",
            "Урок 3: Нижний ряд",
            "Урок 4: Цифры",
            "Урок 5: Символы",
            "Урок 6: Тренировка скорости",
            "Урок 7: Тренировка точности"
        };

        lessonWidget->addButtons(lessons);

        // Подключаем обработчик выбора урока
        connect(lessonWidget, &ScrollButtonWidget::buttonClicked,
                this, &MainWindow::onLessonSelected);
    }
}

void MainWindow :: onResetButtonClicked(){
    ui -> typingInput -> reset();
    resetTimerDisplay();
}

// ДОБАВЛЕНО: Слоты для работы с таймером
void MainWindow::onTypingTimerStarted()
{
    qDebug() << "Таймер набора текста запущен";
    // Можно добавить визуальную индикацию начала отсчета
}

void MainWindow::onTypingTimerStopped(qint64 elapsedTime)
{
    qDebug() << "Таймер остановлен. Затраченное время:" << elapsedTime << "мс";

    // Сохраняем статистику
    saveStatistics(elapsedTime, m_currentLessonId);

    // Обновляем отображение финального времени
    updateTimerDisplay(elapsedTime);
}

void MainWindow::onTypingTimerUpdated(qint64 elapsedTime)
{
    // Обновляем отображение времени в реальном времени
    updateTimerDisplay(elapsedTime);
}

// ДОБАВЛЕНО: Метод для обновления отображения таймера
void MainWindow::updateTimerDisplay(qint64 milliseconds)
{
    // Преобразуем миллисекунды в формат MM:SS.mmm
    int minutes = (milliseconds / 60000) % 60;
    int seconds = (milliseconds / 1000) % 60;
    int ms = milliseconds % 1000;

    QString timeString = QString("%1:%2.%3")
                             .arg(minutes, 2, 10, QLatin1Char('0'))
                             .arg(seconds, 2, 10, QLatin1Char('0'))
                             .arg(ms, 3, 10, QLatin1Char('0'));

    // Ищем или создаем QLabel для отображения времени
    QLabel *timerLabel = findChild<QLabel*>("timerLabel");
    if (!timerLabel) {
        // Создаем новый QLabel если не найден
        timerLabel = new QLabel(this);
        timerLabel->setObjectName("timerLabel");
        timerLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 16px; font-family: 'Consolas'; background-color: #333333; padding: 5px; border-radius: 3px; }");
        timerLabel->setAlignment(Qt::AlignCenter);

        // Добавляем в layout (например, в gridLayout_13 рядом с nameLabel)
        // Вам нужно будет выбрать подходящее место в вашем UI
        // Это временное решение - лучше добавить QLabel через дизайнер
    }

    timerLabel->setText(timeString);
    timerLabel->adjustSize();
}

// ДОБАВЛЕНО: Метод для сброса отображения таймера
void MainWindow::resetTimerDisplay()
{
    updateTimerDisplay(0);
}

// ДОБАВЛЕНО: Метод для сохранения статистики
void MainWindow::saveStatistics(qint64 timeMs, int lessonId)
{
    // Сохраняем время для текущего урока
    m_typingTimes.append(timeMs);

    // Вычисляем среднее время
    qint64 totalTime = 0;
    for (qint64 time : m_typingTimes) {
        totalTime += time;
    }
    qint64 averageTime = m_typingTimes.isEmpty() ? 0 : totalTime / m_typingTimes.size();

    qDebug() << "=== СТАТИСТИКА ===";
    qDebug() << "Урок:" << lessonId;
    qDebug() << "Текущее время:" << timeMs << "мс";
    qDebug() << "Среднее время:" << averageTime << "мс";
    qDebug() << "Количество попыток:" << m_typingTimes.size();

    // Здесь можно добавить сохранение в файл или базу данных
    // saveToFile(lessonId, timeMs, averageTime);
}

// Модифицируем метод выбора урока
void MainWindow::onLessonSelected(const QString &text, const QVariant &data)
{
    int lessonId = data.toInt();
    m_currentLessonId = lessonId; // ДОБАВЛЕНО: Сохраняем ID текущего урока

    qDebug() << "Выбран урок:" << text << "ID:" << lessonId;

    // Сбрасываем таймер при выборе нового урока
    ui->typingInput->reset();
    resetTimerDisplay();

    // Переключаемся обратно на страницу тренировки
    switchToTrainingMode();

    // Устанавливаем пример текста в зависимости от выбранного урока
    //QString lessonText = getLessonText(lessonId);
    //ui->typingInput->setTargetText(lessonText);
}

void MainWindow::onCloseChoseButtonClicked(){
    switchToTrainingMode();
    onResetButtonClicked();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete modeButtonGroup;
}


