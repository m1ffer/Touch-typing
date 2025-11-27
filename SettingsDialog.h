#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include "ToggleSwitch.h"  // Добавим включение нашего заголовка

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

private slots:
    void onLanguageToggled(int id, bool checked);
    void onTrainingModeToggled(int id, bool checked);
    void onHighlightToggleClicked(bool checked);
    void onKeyboardToggleClicked(bool checked);

private:
    void initializeUI();
    void createLanguageSettings();
    void createTrainingSettings();
    void createLearningSettings();

    QButtonGroup *m_languageGroup;
    QRadioButton *m_englishRadio;
    QRadioButton *m_russianRadio;

    QButtonGroup *m_trainingGroup;
    QRadioButton *m_shortWordsRadio;
    QRadioButton *m_longWordsRadio;
    QRadioButton *m_punctuationRadio;
    QRadioButton *m_numbersRadio;
    QRadioButton *m_quotesRadio;

    // Заменим QCheckBox на ToggleSwitch
    ToggleSwitch *m_highlightToggle;
    ToggleSwitch *m_keyboardToggle;
};

#endif // SETTINGSDIALOG_H
