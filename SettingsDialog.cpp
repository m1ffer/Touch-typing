#include "SettingsDialog.h"
#include <QPushButton>
#include <QFrame>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , m_languageGroup(nullptr)
    , m_trainingGroup(nullptr)
{
    setWindowTitle("Настройки");
    setModal(true);
    setFixedSize(400, 450);

    initializeUI();
}

void SettingsDialog::initializeUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    createLanguageSettings();
    createTrainingSettings();
    createLearningSettings();

    mainLayout->addStretch();
}

void SettingsDialog::createLanguageSettings()
{
    QGroupBox *languageGroup = new QGroupBox("Язык", this);
    QVBoxLayout *layout = new QVBoxLayout(languageGroup);

    m_languageGroup = new QButtonGroup(this);
    m_englishRadio = new QRadioButton("Английский", languageGroup);
    m_russianRadio = new QRadioButton("Русский", languageGroup);

    m_russianRadio->setChecked(true);

    m_languageGroup->addButton(m_englishRadio, 0);
    m_languageGroup->addButton(m_russianRadio, 1);

    connect(m_languageGroup, &QButtonGroup::buttonToggled, [this](QAbstractButton *button, bool checked) {
        int id = m_languageGroup->id(button);
        onLanguageToggled(id, checked);
    });

    layout->addWidget(m_englishRadio);
    layout->addWidget(m_russianRadio);

    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    mainLayout->insertWidget(0, languageGroup);
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
    mainLayout->insertWidget(1, trainingGroup);
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
    mainLayout->insertWidget(2, learningGroup);
}

void SettingsDialog::onLanguageToggled(int id, bool checked)
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
