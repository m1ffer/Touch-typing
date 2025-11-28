#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QCloseEvent>
#include "ToggleSwitch.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onAppLanguageToggled(int id, bool checked);
    void onTrainingLanguageToggled(int id, bool checked);
    void onShortWordsToggled(bool checked);
    void onLongWordsToggled(bool checked);
    void onPunctuationToggled(bool checked);
    void onNumbersToggled(bool checked);
    void onQuotesToggled(bool checked);
    void onHighlightToggleClicked(bool checked);
    void onKeyboardToggleClicked(bool checked);

private:
    void initializeUI();
    void createAppLanguageSettings();
    void createTrainingLanguageSettings();
    void createTrainingSettings();
    void createLearningSettings();
    void updateQuotesState(bool checked);
    void saveInitialStates();  // Сохраняем начальные состояния
    void compareStates();      // Сравниваем состояния при закрытии

    // Структура для хранения состояний переключателей
    struct SettingsState {
        int appLanguageId;
        int trainingLanguageId;
        bool shortWords;
        bool longWords;
        bool punctuation;
        bool numbers;
        bool quotes;
        bool highlight;
        bool keyboard;
    };

    SettingsState m_initialState;  // Начальные состояния
    SettingsState m_currentState;  // Текущие состояния

    // Язык приложения
    QButtonGroup *m_appLanguageGroup;
    QRadioButton *m_appRussianRadio;
    QRadioButton *m_appEnglishRadio;

    // Язык обучения
    QButtonGroup *m_trainingLanguageGroup;
    QRadioButton *m_trainingRussianRadio;
    QRadioButton *m_trainingEnglishRadio;

    // Режим тренировки
    ToggleSwitch *m_shortWordsToggle;
    ToggleSwitch *m_longWordsToggle;
    ToggleSwitch *m_punctuationToggle;
    ToggleSwitch *m_numbersToggle;
    ToggleSwitch *m_quotesToggle;

    // Режим обучения
    ToggleSwitch *m_highlightToggle;
    ToggleSwitch *m_keyboardToggle;
};

#endif // SETTINGSDIALOG_H
