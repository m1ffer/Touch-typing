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
    void translateToRussian();
    void translateToEnglish();
    void updateAllTexts(); // Новая функция для обновления всех текстов

    Settings m_initialState;
    Settings m_currentState;

    QButtonGroup *m_appLanguageGroup;
    QRadioButton *m_appRussianRadio;
    QRadioButton *m_appEnglishRadio;

    QButtonGroup *m_trainingLanguageGroup;
    QRadioButton *m_trainingRussianRadio;
    QRadioButton *m_trainingEnglishRadio;
    QRadioButton *m_trainingAssemblyRadio;
    QRadioButton *m_trainingCppRadio;

    ToggleSwitch *m_shortWordsToggle;
    ToggleSwitch *m_longWordsToggle;
    ToggleSwitch *m_punctuationToggle;
    ToggleSwitch *m_numbersToggle;
    ToggleSwitch *m_quotesToggle;

    ToggleSwitch *m_highlightToggle;
    ToggleSwitch *m_keyboardToggle;

    // Указатели на группы для обновления текста
    QGroupBox *m_appLanguageGroupBox;
    QGroupBox *m_trainingLanguageGroupBox;
    QGroupBox *m_trainingGroupBox;
    QGroupBox *m_learningGroupBox;

    // Указатели на метки
    QLabel *m_shortWordsLabel;
    QLabel *m_longWordsLabel;
    QLabel *m_punctuationLabel;
    QLabel *m_numbersLabel;
    QLabel *m_quotesLabel;
    QLabel *m_highlightLabel;
    QLabel *m_keyboardLabel;
};

#endif // SETTINGSDIALOG_H
