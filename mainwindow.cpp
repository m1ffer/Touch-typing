#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QButtonGroup>
#include <QDebug>
#include <QList>
#include <QLabel> // ДОБАВЛЕНО
#include "StatisticsWidget.h"  // ДОБАВИТЬ

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , modeButtonGroup(new QButtonGroup(this))
    , m_currentLessonId(0)
    , m_settingsDialog(nullptr)
{
    ui->setupUi(this);

    // Загрузка 6 файлов с использованием языка из парсера
    auto quoteData = JSONParser::parseQuotes("res/languages/russian/quotes.json");
    quotes[quoteData.first] = quoteData.second;

    quoteData = JSONParser::parseQuotes("res/languages/english/quotes.json");
    quotes[quoteData.first] = quoteData.second;

    auto wordsData = JSONParser::parseWords("res/languages/russian/shortWords.json");
    shortWords[wordsData.first] = wordsData.second;

    wordsData = JSONParser::parseWords("res/languages/english/shortWords.json");
    shortWords[wordsData.first] = wordsData.second;

    wordsData = JSONParser::parseWords("res/languages/russian/longWords.json");
    longWords[wordsData.first] = wordsData.second;

    wordsData = JSONParser::parseWords("res/languages/english/longWords.json");
    longWords[wordsData.first] = wordsData.second;

    qDebug() << "=== Начало инициализации MainWindow ===";

    QPushButton *trainButton = findChild<QPushButton*>("trainButton");
    QPushButton *learnButton = findChild<QPushButton*>("learnButton");

    if (trainButton && learnButton) {
        connect(trainButton, &QPushButton::clicked, this, &MainWindow::onTrainModeClicked);
        connect(learnButton, &QPushButton::clicked, this, &MainWindow::onLearnModeClicked);
        connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::onResetButtonClicked);
        connect(ui->closeChoseButton, &QPushButton::clicked, this, &MainWindow::onCloseChoseButtonClicked);
        connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);
        connect(ui->typingInput, &TypingInput::inputCompleted, this, &MainWindow::showTypingResults);

        applyButtonStyle();
    } else {
        qDebug() << "ОШИБКА: Не все кнопки найдены!";
    }

    setStyleSheet("background-color: #1a1a1a; color: #e6e6e6;");

    // Прямой доступ к TypingInput через UI
    TypingInput *input = ui->typingInput;
    input->setTargetText("Этопримертекстадлятренировкислепойпечати...");

    // Подключаем сигналы таймера
    connect(input, &TypingInput::timerStarted, this, &MainWindow::onTypingTimerStarted);
    connect(input, &TypingInput::timerStopped, this, &MainWindow::onTypingTimerStopped);
    connect(input, &TypingInput::timerUpdated, this, &MainWindow::onTypingTimerUpdated);

    // Сбрасываем отображение таймера
    resetTimerDisplay();

    // Устанавливаем начальную страницу
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
            "Урок 7: Тренировка точности",
            ",t,t,t",
            "djsf",
            "sfe",
            "es",
            "efsef"
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
    //saveStatistics(elapsedTime, m_currentLessonId);

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
    /*void MainWindow::saveStatistics(qint64 timeMs, int lessonId)
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
}*/

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

// ДОБАВИТЬ метод для расчета и показа статистики
/*void MainWindow::calculateAndShowStatistics()
{
    TypingInput *input = ui->typingInput;

    // Создаем запись статистики
    StatisticsRecord record;
    record.timestamp = QDateTime::currentDateTime();
    record.lessonId = m_currentLessonId;
    record.timeMs = input->getElapsedTime();
    record.totalChars = input->getTotalCharsTyped();
    record.errorsCount = input->getErrorsCount();
    record.accuracy = input->getAccuracy();
    record.speedWpm = input->getSpeedWpm();
    record.speedCpm = input->getSpeedCpm();

    // Добавляем запись в менеджер
    m_statsManager->addRecord(record);

    // Создаем и показываем виджет статистики
    StatisticsWidget *statsWidget = new StatisticsWidget(m_statsManager, this);
    statsWidget->setWindowModality(Qt::ApplicationModal);
    statsWidget->setAttribute(Qt::WA_DeleteOnClose);
    statsWidget->resize(900, 700);
    statsWidget->show();

    qDebug() << "=== СТАТИСТИКА ===";
    qDebug() << "Урок:" << record.lessonId;
    qDebug() << "Время:" << record.timeMs << "мс";
    qDebug() << "Символов:" << record.totalChars;
    qDebug() << "Ошибки:" << record.errorsCount;
    qDebug() << "Точность:" << record.accuracy << "%";
    qDebug() << "Скорость:" << record.speedWpm << "слов/мин";
    qDebug() << "Скорость:" << record.speedCpm << "символов/мин";
}*/

// ДОБАВИТЬ метод для показа статистики
void MainWindow::showTypingResults()
{
    qDebug() << "Показ результатов набора...";

    TypingInput *input = ui->typingInput;

    // Получаем статистику из TypingInput
    double accuracy = input->getAccuracy();
    double speedWpm = input->getSpeedWpm();
    double speedCpm = input->getSpeedCpm();
    qint64 timeMs = input->getElapsedTime();
    int errorsCount = input->getErrorsCount();
    int totalChars = input->getTotalCharsTyped();
    QVector<QPair<qint64, double>> speedHistory = input->getSpeedHistory();
    // ДОБАВИМ ОТЛАДКУ
    qDebug() << "=== ОТЛАДКА СТАТИСТИКИ ===";
    qDebug() << "timeMs:" << timeMs;
    qDebug() << "accuracy:" << accuracy;
    qDebug() << "speedWpm:" << speedWpm;
    qDebug() << "speedCpm:" << speedCpm;
    qDebug() << "errorsCount:" << errorsCount;
    qDebug() << "totalChars:" << totalChars;
     qDebug() << "Размер истории скорости:" << speedHistory.size();


    // Если время равно 0, используем минимальное значение
    if (timeMs == 0) {
        timeMs = 1; // Минимальное значение чтобы избежать деления на 0
        qDebug() << "Время было 0, установлено в 1 мс";
    }

    // Создаем и показываем виджет статистики с графиком
    StatisticsWidget *statsWidget = new StatisticsWidget(accuracy, speedCpm,
                                                         timeMs, errorsCount, totalChars,
                                                         speedHistory,  // Передаем историю
                                                         this);
    // ДОБАВЛЕНО: Подключаем сигналы от виджета статистики
    connect(statsWidget, &StatisticsWidget::repeatRequested, this, &MainWindow::onStatsRepeatRequested);
    connect(statsWidget, &StatisticsWidget::nextRequested, this, &MainWindow::onStatsNextRequested);

    // ДОБАВЛЕНО: Блокируем основное окно и центрируем
    statsWidget->setWindowModality(Qt::ApplicationModal);
    statsWidget->setAttribute(Qt::WA_DeleteOnClose);

    statsWidget->setWindowModality(Qt::ApplicationModal);
    statsWidget->setAttribute(Qt::WA_DeleteOnClose);
    // Убираем кнопки управления окном
    statsWidget->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    statsWidget->show();
    qDebug() << "Окно статистики показано";
}

// ДОБАВЛЕНО: Слот для кнопки "Повторить"
void MainWindow::onStatsRepeatRequested()
{
    qDebug() << "Обработка кнопки 'Повторить'";
    onResetButtonClicked();  // Вызываем сброс как при нажатии кнопки Reset
}

// ДОБАВЛЕНО: Слот для кнопки "Дальше" (пока пустой)
void MainWindow::onStatsNextRequested()
{
    qDebug() << "Обработка кнопки 'Дальше'";
    // Можно добавить логику перехода к следующему уроку
    // или другой функционал в будущем
}

MainWindow::~MainWindow()
{
    delete ui;
    delete modeButtonGroup;
    if (m_settingsDialog) {
        delete m_settingsDialog;
    }
}

void MainWindow::onSettingsButtonClicked()
{
    qDebug() << "Кнопка настроек нажата";

    if (!m_settingsDialog) {
        m_settingsDialog = new SettingsDialog(this);
    }

    m_settingsDialog->exec();
}


