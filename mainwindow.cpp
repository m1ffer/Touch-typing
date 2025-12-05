#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QButtonGroup>
#include <QDebug>
#include <QList>
#include <QLabel> // ДОБАВЛЕНО
#include "StatisticsWidget.h"  // ДОБАВИТЬ
#include <QString>
#include <QRegularExpression>
#include "MessageHelper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , modeButtonGroup(new QButtonGroup(this))
    , m_currentLessonId(0)
    , m_settingsDialog(nullptr)
{
    ui->setupUi(this);
    ui -> gridLayout_17 -> setAlignment(Qt::AlignCenter); // Выравнивание всех виджетов по центру
    ui->centralwidget->setStyleSheet(R"(
        QWidget {
            background-color: #1a1a1a;
        }
        QLabel {
            color: #ffd700;
            font-family: 'Roboto Mono', 'Consolas', monospace;
        }
    )");

    // Устанавливаем фон главного окна
    /*setStyleSheet(R"(
        QMainWindow {
            background-color: #1a1a1a;
        }
    )");*/
    m_currentMode = ui -> trainButton->text();
    m_settingsDialog = new SettingsDialog(this);
    m_currentSettings = m_settingsDialog->getCurrentSettings();
    updateTrainingText();
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

    // Используйте полный стиль Monkey Type:
    setStyleSheet(R"(
    QMainWindow, QDialog, QWidget {
        background-color: #1a1a1a;
        color: #e6e6e6;
        font-family: 'Roboto Mono', 'Consolas', monospace;
    }
    QGroupBox {
        border: 1px solid #3a3a3a;
        border-radius: 4px;
        margin-top: 10px;
        padding-top: 10px;
        color: #cccccc;
        font-weight: 500;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        left: 10px;
        padding: 0 5px 0 5px;
    }
)");

    TypingInput* input = ui -> typingInput;
    // Подключаем сигналы таймера
    connect(input, &TypingInput::timerStarted, this, &MainWindow::onTypingTimerStarted);
    connect(input, &TypingInput::timerStopped, this, &MainWindow::onTypingTimerStopped);
    connect(input, &TypingInput::timerUpdated, this, &MainWindow::onTypingTimerUpdated);
    // Устанавливаем начальную страницу
    ui->modesStackedWidget->setCurrentWidget(ui->inputMode);
    m_currentMode = ui -> trainButton-> text();
    initializeLessonMap();
    qDebug() << "bebebe";
    ui -> typingInput -> setKeyboard(ui -> keyboard);
    ui -> typingInput -> highlight();
    if (!m_currentSettings.keyboard)
        ui -> keyboard -> disable();
    else
        ui ->keyboard ->enable();
    ui -> leftHand -> setImage("../../res/leftHand.png");
    ui -> rightHand -> setImage("../../res/rightHand.png");
    if (!m_currentSettings.highlight)
        disableHands();

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
            font-size: 16px;
            font-weight: 400;
            padding: 8px 16px;
            border: 1px solid #3a3a3a;
            background-color: #2a2a2a;
            color: #ffd700;
            border-radius: 20px;
            text-transform: none;
            letter-spacing: 0.3px;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #3a3a3a;
            border-color: #ffd700;
            color: #ffd700;
        }
        QPushButton:checked {
            background-color: #3a3a3a;
            color: #ffd700;
            border-color: #ffd700;
            font-weight: 500;
        }
        QPushButton:pressed {
            background-color: #4a4a2a;
            color: #ffd700;
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
                               "     border-top-left-radius: 0px;"
                               "     border-bottom-left-radius: 0px;"
                               "     border-right: 1px solid #1a1a1a;"
                               " }");

    learnButton->setStyleSheet(learnButton->styleSheet() +
                               " QPushButton {"
                               "     border-top-right-radius: 0px;"
                               "     border-bottom-right-radius: 0px;"
                               "     border-left: none;"
                               " }");

    qDebug() << "Стиль успешно применен";
}


void MainWindow::onTrainModeClicked()
{
    qDebug() << "Режим тренировки активирован";
    if (m_currentMode == ui -> trainButton -> text() && !(ui -> modesStackedWidget -> currentWidget() == ui -> choseMode))
        updateTrainingText();
    else
        switchToTrainingMode();
    // Логика для режима тренировки
    // ДОБАВЛЕНО: Обновляем текст при переключении на тренировку
}

void MainWindow::onLearnModeClicked()
{
    if(lessons[m_currentSettings.trainingLanguage].empty()){
        qDebug() << "Для текущего языка уроков нет, переключение не произошло";
        //вывести соо
        MessageHelper::showWarning(this, "Предупреждение", "Для выбранного языка нет уроков");
    }
    else{
        qDebug() << "Режим обучения активирован";
        switchToLearningMode();
    }
    // Логика для режима обучения

}

// НОВЫЕ МЕТОДЫ ДЛЯ ПЕРЕКЛЮЧЕНИЯ СТРАНИЦ
void MainWindow::switchToTrainingMode()
{
    // Переключаем modesStackedWidget на страницу inputMode
    ui->modesStackedWidget->setCurrentWidget(ui->inputMode);
    updateTrainingText();
    m_currentMode = ui -> trainButton-> text();
    qDebug() << "Переключено на страницу тренировки (inputMode)";
}

void MainWindow::switchToLearningMode()
{
    // Переключаем modesStackedWidget на страницу choseMode
    if (m_currentSettings.keyboard)
        ui -> keyboard -> enable();
    else
        ui -> keyboard -> disable();
    initializeLessons();
    ui->modesStackedWidget->setCurrentWidget(ui->choseMode);
    qDebug() << "Переключено на страницу выбора уроков (choseMode)";

    // Здесь можно добавить инициализацию списка уроков
}

void MainWindow::initializeLessons()
{
    ScrollButtonWidget *lessonWidget = ui->choseWidget;
    lessonWidget -> clearButtons();
    lessonWidget -> addButtons(lessons[m_currentSettings.trainingLanguage]);
    connect(lessonWidget, &ScrollButtonWidget::buttonClicked,
            this, &MainWindow::onLessonSelected);
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

int findFirstNum(const QString& str) {
    QRegularExpression regex("\\d+"); // Регулярное выражение для поиска последовательности цифр
    QRegularExpressionMatch match = regex.match(str);

    if (match.hasMatch()) {
        bool ok;
        int result = match.captured(0).toInt(&ok);
        if (ok) {
            return result;
        }
    }

    return -1; // Возвращаем -1 если число не найдено
}

// Модифицируем метод выбора урока
void MainWindow::onLessonSelected(const QString &text, const QVariant &data)
{
    ui->modesStackedWidget->setCurrentWidget(ui -> inputMode);
    m_currentMode = ui -> learnButton -> text();
    int lessonId = findFirstNum(text);
    m_currentLessonId = lessonId; // ДОБАВЛЕНО: Сохраняем ID текущего урока

    qDebug() << "Выбран урок:" << text << "ID:" << lessonId;
    ui -> typingInput -> setLesson(lessonId - 1, m_currentSettings.trainingLanguage);
    resetTimerDisplay();
}

void MainWindow::onCloseChoseButtonClicked(){
    ui->modesStackedWidget->setCurrentWidget(ui -> inputMode);
    //ui -> modesStackedWidget ->
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
    if (m_currentMode == ui -> trainButton -> text()){
        updateTrainingText();
    }
    else{
        if (m_currentLessonId != lessons[m_currentSettings.trainingLanguage].size()){
            onLessonSelected(QString::fromStdString(std::to_string(m_currentLessonId + 1)), NULL);
        }
        else{
            MessageHelper::showInfo(this, "Поздравляем", "Это был последний урок в этом языке");
            onResetButtonClicked();
            onLearnModeClicked();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete modeButtonGroup;
    delete m_settingsDialog;
    ui ->choseWidget ->clearButtons();
}

void MainWindow::onSettingsButtonClicked()
{
    qDebug() << "Кнопка настроек нажата";
    // Сохраняем текущие настройки до открытия диалога
            Settings oldSettings = m_currentSettings;

    m_settingsDialog->exec();

    // Получаем обновленные настройки после закрытия диалога
    m_currentSettings = m_settingsDialog->getCurrentSettings();

    // ДОБАВЛЕНО: Проверяем, изменились ли настройки тренировки
    bool trainingSettingsChanged =
        (oldSettings.trainingLanguage != m_currentSettings.trainingLanguage) ||
        (oldSettings.shortWords != m_currentSettings.shortWords) ||
        (oldSettings.longWords != m_currentSettings.longWords) ||
        (oldSettings.punctuation != m_currentSettings.punctuation) ||
        (oldSettings.numbers != m_currentSettings.numbers) ||
        (oldSettings.quotes != m_currentSettings.quotes);

    if (trainingSettingsChanged)
    {
        if (oldSettings.trainingLanguage != m_currentSettings.trainingLanguage){
            initializeLessons();
        }
        if (m_currentMode == ui -> trainButton -> text()) {
            qDebug() << "Настройки тренировки изменились, обновляем текст";
            updateTrainingText();
        }
        else{

        }
    }
        if (!oldSettings.keyboard && m_currentSettings.keyboard){
            ui -> typingInput -> enableKeyboard();
        }
        if (oldSettings.keyboard && !m_currentSettings.keyboard){
            ui -> typingInput -> disableKeyboard();
        }
        if (!oldSettings.highlight && m_currentSettings.highlight)
            enableHands();
        if (oldSettings.keyboard && !m_currentSettings.highlight)
            disableHands();
}

// ДОБАВЛЕНО: Метод для обновления текста тренировки
void MainWindow::updateTrainingText()
{
    if (m_currentSettings.keyboard){
        ui -> keyboard -> enable();

    }
    else{
        ui -> keyboard -> disable();
    }
    QString trainingText = ui->typingInput->makeTextFromSettings(m_currentSettings);
    ui->typingInput->setTargetText(trainingText);
    updateTimerDisplay(0);
    qDebug() << "Текст тренировки обновлен по настройкам";

}

void MainWindow::initializeLessonMap(){
    auto LessonsMap = JSONParser::parseLessons(PATH_TO_LESSONS);
    std::map<String, std::vector<Lesson>> mp;
    for (auto [lang, que] : LessonsMap){
        mp[lang].resize(que.size());
        for(int i = 0; !que.empty(); i++){
            mp[lang][i] = que.top();
            que.pop();
        }
        for (int i = 0; i < mp[lang].size(); i++){
            lessons[lang].push_front(QString::fromStdString(mp[lang][i].name));
        }
    }
    //Тут передать в TypingInput
    ui -> typingInput -> initializeLessons(mp);
}

void MainWindow::enableHands(){
    ui -> leftHand -> enable();
    ui -> rightHand -> enable();
}

void MainWindow::disableHands(){
    ui -> leftHand -> disable();
    ui -> rightHand -> disable();
}
