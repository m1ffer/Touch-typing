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
#include "JSONParser.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    Settings getCurrentSettings() const { return m_currentState; }
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
    void applySettings(const Settings&);
    void initializeUI();
    void createAppLanguageSettings();
    void createTrainingLanguageSettings();
    void createTrainingSettings();
    void createLearningSettings();
    void updateQuotesState(bool checked);
    void saveInitialStates();
    void compareStates();
    void loadSettings();
    void saveSettings();

    Settings m_initialState;
    Settings m_currentState;

    QButtonGroup *m_appLanguageGroup;
    QRadioButton *m_appRussianRadio;
    QRadioButton *m_appEnglishRadio;

    QButtonGroup *m_trainingLanguageGroup;
    QRadioButton *m_trainingRussianRadio;
    QRadioButton *m_trainingEnglishRadio;

    ToggleSwitch *m_shortWordsToggle;
    ToggleSwitch *m_longWordsToggle;
    ToggleSwitch *m_punctuationToggle;
    ToggleSwitch *m_numbersToggle;
    ToggleSwitch *m_quotesToggle;

    ToggleSwitch *m_highlightToggle;
    ToggleSwitch *m_keyboardToggle;
};

#endif // SETTINGSDIALOG_H
