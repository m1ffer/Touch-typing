#include "SettingsDialog.h"
#include <QPushButton>
#include <QFrame>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , m_appLanguageGroup(nullptr)
    , m_trainingLanguageGroup(nullptr)
    , m_trainingGroup(nullptr)
{
    setWindowTitle("Настройки");
    setModal(true);
    setFixedSize(400, 550);

    initializeUI();
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

    m_trainingGroup = new QButtonGroup(this);
    m_shortWordsRadio = new QRadioButton("Короткие слова", trainingGroup);
    m_longWordsRadio = new QRadioButton("Длинные слова", trainingGroup);
    m_punctuationRadio = new QRadioButton("Пунктуация", trainingGroup);
    m_numbersRadio = new QRadioButton("Цифры", trainingGroup);
    m_quotesRadio = new QRadioButton("Цитаты", trainingGroup);

    m_shortWordsRadio->setChecked(true);

    m_trainingGroup->addButton(m_shortWordsRadio, 0);
    m_trainingGroup->addButton(m_longWordsRadio, 1);
    m_trainingGroup->addButton(m_punctuationRadio, 2);
    m_trainingGroup->addButton(m_numbersRadio, 3);
    m_trainingGroup->addButton(m_quotesRadio, 4);

    connect(m_trainingGroup, &QButtonGroup::buttonToggled, [this](QAbstractButton *button, bool checked) {
        int id = m_trainingGroup->id(button);
        onTrainingModeToggled(id, checked);
    });

    layout->addWidget(m_shortWordsRadio);
    layout->addWidget(m_longWordsRadio);
    layout->addWidget(m_punctuationRadio);
    layout->addWidget(m_numbersRadio);
    layout->addWidget(m_quotesRadio);

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

void SettingsDialog::onTrainingModeToggled(int id, bool checked)
{
    Q_UNUSED(id)
    Q_UNUSED(checked)
}

void SettingsDialog::onHighlightToggleClicked(bool checked)
{
    Q_UNUSED(checked)
}

void SettingsDialog::onKeyboardToggleClicked(bool checked)
{
    Q_UNUSED(checked)
}
