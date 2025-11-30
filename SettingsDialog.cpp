#include "SettingsDialog.h"
#include <QPushButton>
#include <QFrame>
#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <fstream>
#include <sstream>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , m_appLanguageGroup(nullptr)
    , m_trainingLanguageGroup(nullptr)
{
    setWindowTitle("Настройки");
    setModal(true);
    setFixedSize(400, 600);
    initializeUI();
    loadSettings();
    saveInitialStates();
}

void SettingsDialog::closeEvent(QCloseEvent *event)
{
    // Сохраняем текущие состояния перед сравнением
    m_currentState.appLanguage = (m_appLanguageGroup->checkedId() == 0) ? "русский" : "english";
    m_currentState.trainingLanguage = (m_trainingLanguageGroup->checkedId() == 0) ? "русский" : "english";
    m_currentState.shortWords = m_shortWordsToggle->isChecked();
    m_currentState.longWords = m_longWordsToggle->isChecked();
    m_currentState.punctuation = m_punctuationToggle->isChecked();
    m_currentState.numbers = m_numbersToggle->isChecked();
    m_currentState.quotes = m_quotesToggle->isChecked();
    m_currentState.highlight = m_highlightToggle->isChecked();
    m_currentState.keyboard = m_keyboardToggle->isChecked();

    compareStates();
    saveSettings();

    qDebug() << "Окно настроек закрыто, настройки сохранены";
    event->accept();
}

void SettingsDialog::loadSettings()
{
    const String SETTINGS_FILE_PATH = "tmp/settings.json";
    Settings settings = JSONParser::parseSettings(SETTINGS_FILE_PATH);
    applySettings(settings);
}

void SettingsDialog::applySettings(const Settings& settings)
{
    // Устанавливаем язык приложения
    if (settings.appLanguage == "русский") {
        m_appRussianRadio->setChecked(true);
    } else {
        m_appEnglishRadio->setChecked(true);
    }

    // Устанавливаем язык обучения
    if (settings.trainingLanguage == "русский") {
        m_trainingRussianRadio->setChecked(true);
    } else {
        m_trainingEnglishRadio->setChecked(true);
    }

    // Устанавливаем настройки тренировки
    m_shortWordsToggle->setChecked(settings.shortWords);
    m_longWordsToggle->setChecked(settings.longWords);
    m_punctuationToggle->setChecked(settings.punctuation);
    m_numbersToggle->setChecked(settings.numbers);
    m_quotesToggle->setChecked(settings.quotes);

    // Устанавливаем настройки обучения
    m_highlightToggle->setChecked(settings.highlight);
    m_keyboardToggle->setChecked(settings.keyboard);
}

void SettingsDialog::saveSettings()
{
    const QString SETTINGS_FILE_PATH = "tmp/settings.json";

    // Создаем директорию, если она не существует
    QDir dir;
    if (!dir.exists("tmp")) {
        if (!dir.mkpath("tmp")) {
            qDebug() << "Не удалось создать директорию tmp";
            return;
        }
    }

    QFile file(SETTINGS_FILE_PATH);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл настроек для записи";
        return;
    }

    QTextStream out(&file);
    std::string jsonContent = JSONParser::settingsToJSON(m_currentState);
    out << QString::fromStdString(jsonContent);
    file.close();
}

void SettingsDialog::saveInitialStates()
{
    m_initialState.appLanguage = (m_appLanguageGroup->checkedId() == 0) ? "русский" : "english";
    m_initialState.trainingLanguage = (m_trainingLanguageGroup->checkedId() == 0) ? "русский" : "english";
    m_initialState.shortWords = m_shortWordsToggle->isChecked();
    m_initialState.longWords = m_longWordsToggle->isChecked();
    m_initialState.punctuation = m_punctuationToggle->isChecked();
    m_initialState.numbers = m_numbersToggle->isChecked();
    m_initialState.quotes = m_quotesToggle->isChecked();
    m_initialState.highlight = m_highlightToggle->isChecked();
    m_initialState.keyboard = m_keyboardToggle->isChecked();

    m_currentState = m_initialState;
}

void SettingsDialog::compareStates()
{
    qDebug() << "=== СРАВНЕНИЕ НАСТРОЕК ===";

    if (m_initialState.appLanguage != m_currentState.appLanguage) {
        qDebug() << "Язык приложения изменен:" << QString::fromStdString(m_initialState.appLanguage) << "->" << QString::fromStdString(m_currentState.appLanguage);
    }

    if (m_initialState.trainingLanguage != m_currentState.trainingLanguage) {
        qDebug() << "Язык обучения изменен:" << QString::fromStdString(m_initialState.trainingLanguage) << "->" << QString::fromStdString(m_currentState.trainingLanguage);
    }

    if (m_initialState.shortWords != m_currentState.shortWords) {
        qDebug() << "Короткие слова:" << m_initialState.shortWords << "->" << m_currentState.shortWords;
    }
    if (m_initialState.longWords != m_currentState.longWords) {
        qDebug() << "Длинные слова:" << m_initialState.longWords << "->" << m_currentState.longWords;
    }
    if (m_initialState.punctuation != m_currentState.punctuation) {
        qDebug() << "Пунктуация:" << m_initialState.punctuation << "->" << m_currentState.punctuation;
    }
    if (m_initialState.numbers != m_currentState.numbers) {
        qDebug() << "Цифры:" << m_initialState.numbers << "->" << m_currentState.numbers;
    }
    if (m_initialState.quotes != m_currentState.quotes) {
        qDebug() << "Цитаты:" << m_initialState.quotes << "->" << m_currentState.quotes;
    }

    if (m_initialState.highlight != m_currentState.highlight) {
        qDebug() << "Подсветка:" << m_initialState.highlight << "->" << m_currentState.highlight;
    }
    if (m_initialState.keyboard != m_currentState.keyboard) {
        qDebug() << "Клавиатура:" << m_initialState.keyboard << "->" << m_currentState.keyboard;
    }

    qDebug() << "=== КОНЕЦ СРАВНЕНИЯ ===";
}

void SettingsDialog::initializeUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    createAppLanguageSettings();
    createTrainingLanguageSettings();
    createTrainingSettings();
    createLearningSettings();

    mainLayout->addStretch();
}

void SettingsDialog::createAppLanguageSettings()
{
    QGroupBox *appLanguageGroup = new QGroupBox("Язык приложения / Application language", this);
    QVBoxLayout *layout = new QVBoxLayout(appLanguageGroup);

    m_appLanguageGroup = new QButtonGroup(this);
    m_appRussianRadio = new QRadioButton("Русский", appLanguageGroup);
    m_appEnglishRadio = new QRadioButton("English", appLanguageGroup);

    // По умолчанию русский
    m_appRussianRadio->setChecked(true);

    m_appLanguageGroup->addButton(m_appRussianRadio, 0);
    m_appLanguageGroup->addButton(m_appEnglishRadio, 1);

    connect(m_appLanguageGroup, &QButtonGroup::buttonToggled, [this](QAbstractButton *button, bool checked) {
        int id = m_appLanguageGroup->id(button);
        onAppLanguageToggled(id, checked);
    });

    layout->addWidget(m_appRussianRadio);
    layout->addWidget(m_appEnglishRadio);

    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    mainLayout->insertWidget(0, appLanguageGroup);
}

void SettingsDialog::createTrainingLanguageSettings()
{
    QGroupBox *trainingLanguageGroup = new QGroupBox("Язык обучения", this);
    QVBoxLayout *layout = new QVBoxLayout(trainingLanguageGroup);

    m_trainingLanguageGroup = new QButtonGroup(this);
    m_trainingRussianRadio = new QRadioButton("Русский", trainingLanguageGroup);
    m_trainingEnglishRadio = new QRadioButton("Английский", trainingLanguageGroup);

    // По умолчанию русский
    m_trainingRussianRadio->setChecked(true);

    m_trainingLanguageGroup->addButton(m_trainingRussianRadio, 0);
    m_trainingLanguageGroup->addButton(m_trainingEnglishRadio, 1);

    connect(m_trainingLanguageGroup, &QButtonGroup::buttonToggled, [this](QAbstractButton *button, bool checked) {
        int id = m_trainingLanguageGroup->id(button);
        onTrainingLanguageToggled(id, checked);
    });

    layout->addWidget(m_trainingRussianRadio);
    layout->addWidget(m_trainingEnglishRadio);

    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    mainLayout->insertWidget(1, trainingLanguageGroup);
}

void SettingsDialog::createTrainingSettings()
{
    QGroupBox *trainingGroup = new QGroupBox("Режим тренировки", this);
    QVBoxLayout *layout = new QVBoxLayout(trainingGroup);

    // Создаем свитчеры для всех опций тренировки
    QHBoxLayout *shortWordsLayout = new QHBoxLayout();
    QLabel *shortWordsLabel = new QLabel("Короткие слова", trainingGroup);
    m_shortWordsToggle = new ToggleSwitch(trainingGroup);
    shortWordsLayout->addWidget(shortWordsLabel);
    shortWordsLayout->addStretch();
    shortWordsLayout->addWidget(m_shortWordsToggle);

    QHBoxLayout *longWordsLayout = new QHBoxLayout();
    QLabel *longWordsLabel = new QLabel("Длинные слова", trainingGroup);
    m_longWordsToggle = new ToggleSwitch(trainingGroup);
    longWordsLayout->addWidget(longWordsLabel);
    longWordsLayout->addStretch();
    longWordsLayout->addWidget(m_longWordsToggle);

    QHBoxLayout *punctuationLayout = new QHBoxLayout();
    QLabel *punctuationLabel = new QLabel("Пунктуация", trainingGroup);
    m_punctuationToggle = new ToggleSwitch(trainingGroup);
    punctuationLayout->addWidget(punctuationLabel);
    punctuationLayout->addStretch();
    punctuationLayout->addWidget(m_punctuationToggle);

    QHBoxLayout *numbersLayout = new QHBoxLayout();
    QLabel *numbersLabel = new QLabel("Цифры", trainingGroup);
    m_numbersToggle = new ToggleSwitch(trainingGroup);
    numbersLayout->addWidget(numbersLabel);
    numbersLayout->addStretch();
    numbersLayout->addWidget(m_numbersToggle);

    QHBoxLayout *quotesLayout = new QHBoxLayout();
    QLabel *quotesLabel = new QLabel("Цитаты", trainingGroup);
    m_quotesToggle = new ToggleSwitch(trainingGroup);
    quotesLayout->addWidget(quotesLabel);
    quotesLayout->addStretch();
    quotesLayout->addWidget(m_quotesToggle);

    // По умолчанию включаем "Короткие слова"
    m_shortWordsToggle->setChecked(true);

    // Подключаем сигналы
    connect(m_shortWordsToggle, &ToggleSwitch::toggled, this, &SettingsDialog::onShortWordsToggled);
    connect(m_longWordsToggle, &ToggleSwitch::toggled, this, &SettingsDialog::onLongWordsToggled);
    connect(m_punctuationToggle, &ToggleSwitch::toggled, this, &SettingsDialog::onPunctuationToggled);
    connect(m_numbersToggle, &ToggleSwitch::toggled, this, &SettingsDialog::onNumbersToggled);
    connect(m_quotesToggle, &ToggleSwitch::toggled, this, &SettingsDialog::onQuotesToggled);

    layout->addLayout(shortWordsLayout);
    layout->addLayout(longWordsLayout);
    layout->addLayout(punctuationLayout);
    layout->addLayout(numbersLayout);
    layout->addLayout(quotesLayout);

    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    mainLayout->insertWidget(2, trainingGroup);
}

void SettingsDialog::createLearningSettings()
{
    QGroupBox *learningGroup = new QGroupBox("Режим обучения", this);
    QVBoxLayout *layout = new QVBoxLayout(learningGroup);

    // Подсветка свитчер
    QHBoxLayout *highlightLayout = new QHBoxLayout();
    QLabel *highlightLabel = new QLabel("Подсветка:", learningGroup);
    m_highlightToggle = new ToggleSwitch(learningGroup);

    // Клавиатура свитчер
    QHBoxLayout *keyboardLayout = new QHBoxLayout();
    QLabel *keyboardLabel = new QLabel("Клавиатура:", learningGroup);
    m_keyboardToggle = new ToggleSwitch(learningGroup);

    highlightLayout->addWidget(highlightLabel);
    highlightLayout->addStretch();
    highlightLayout->addWidget(m_highlightToggle);

    keyboardLayout->addWidget(keyboardLabel);
    keyboardLayout->addStretch();
    keyboardLayout->addWidget(m_keyboardToggle);

    // Подключаем сигналы
    connect(m_highlightToggle, &ToggleSwitch::toggled, this, &SettingsDialog::onHighlightToggleClicked);
    connect(m_keyboardToggle, &ToggleSwitch::toggled, this, &SettingsDialog::onKeyboardToggleClicked);

    layout->addLayout(highlightLayout);
    layout->addLayout(keyboardLayout);

    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    mainLayout->insertWidget(3, learningGroup);
}

void SettingsDialog::updateQuotesState(bool checked)
{
    if (checked) {
        // Если включаем "Цитаты", выключаем все остальные
        m_shortWordsToggle->setChecked(false);
        m_longWordsToggle->setChecked(false);
        m_punctuationToggle->setChecked(false);
        m_numbersToggle->setChecked(false);
    }
}

void SettingsDialog::onAppLanguageToggled(int id, bool checked)
{
    Q_UNUSED(id)
    Q_UNUSED(checked)
}

void SettingsDialog::onTrainingLanguageToggled(int id, bool checked)
{
    Q_UNUSED(id)
    Q_UNUSED(checked)
}

void SettingsDialog::onShortWordsToggled(bool checked)
{
    if (checked && m_quotesToggle->isChecked()) {
        m_quotesToggle->setChecked(false);
    }
}

void SettingsDialog::onLongWordsToggled(bool checked)
{
    if (checked && m_quotesToggle->isChecked()) {
        m_quotesToggle->setChecked(false);
    }
}

void SettingsDialog::onPunctuationToggled(bool checked)
{
    if (checked && m_quotesToggle->isChecked()) {
        m_quotesToggle->setChecked(false);
    }
}

void SettingsDialog::onNumbersToggled(bool checked)
{
    if (checked && m_quotesToggle->isChecked()) {
        m_quotesToggle->setChecked(false);
    }
}

void SettingsDialog::onQuotesToggled(bool checked)
{
    updateQuotesState(checked);
}

void SettingsDialog::onHighlightToggleClicked(bool checked)
{
    Q_UNUSED(checked)
}

void SettingsDialog::onKeyboardToggleClicked(bool checked)
{
    Q_UNUSED(checked)
}
