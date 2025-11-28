#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include "ToggleSwitch.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

private slots:
    void onAppLanguageToggled(int id, bool checked);
    void onTrainingLanguageToggled(int id, bool checked);
    void onTrainingModeToggled(int id, bool checked);
    void onHighlightToggleClicked(bool checked);
    void onKeyboardToggleClicked(bool checked);

private:
    void initializeUI();
    void createAppLanguageSettings();
    void createTrainingLanguageSettings();
    void createTrainingSettings();
    void createLearningSettings();

    // Язык приложения
    QButtonGroup *m_appLanguageGroup;
    QRadioButton *m_appRussianRadio;
    QRadioButton *m_appEnglishRadio;

    // Язык обучения
    QButtonGroup *m_trainingLanguageGroup;
    QRadioButton *m_trainingRussianRadio;
    QRadioButton *m_trainingEnglishRadio;

    // Режим тренировки
    QButtonGroup *m_trainingGroup;
    QRadioButton *m_shortWordsRadio;
    QRadioButton *m_longWordsRadio;
    QRadioButton *m_punctuationRadio;
    QRadioButton *m_numbersRadio;
    QRadioButton *m_quotesRadio;

    // Режим обучения
    ToggleSwitch *m_highlightToggle;
    ToggleSwitch *m_keyboardToggle;
};

#endif // SETTINGSDIALOG_H
