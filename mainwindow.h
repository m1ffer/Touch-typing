#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QLabel>
#include <QTime>
#include <map>
#include "SettingsDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTrainModeClicked();
    void onLearnModeClicked();
    void switchToTrainingMode();
    void switchToLearningMode();
    void onTypingTimerStarted();
    void onTypingTimerStopped(qint64 elapsedTime);
    void onTypingTimerUpdated(qint64 elapsedTime);
    void onResetButtonClicked();
    void onCloseChoseButtonClicked();
    void showTypingResults();
    void onStatsRepeatRequested();
    void onStatsNextRequested();
    void onSettingsButtonClicked();
    void onInfoButtonClicked();

private:
    const String PATH_TO_LESSONS = "../../res/languages/lessons";
    void applyButtonStyle();
    void initializeLessons();
    void onLessonSelected(const QString &lessonName, const QVariant &data);
    void updateTimerDisplay(qint64 milliseconds);
    void resetTimerDisplay();
    void initializeLessonMap();

    Ui::MainWindow *ui;
    QButtonGroup *modeButtonGroup;
    int m_currentLessonId;
    QString m_currentMode;
    SettingsDialog *m_settingsDialog;
    // ДОБАВЛЕНО: Метод для обновления текста тренировки
    void updateTrainingText();
    // ДОБАВЛЕНО: Текущие настройки
    Settings m_currentSettings;
    std::map<String, QStringList> lessons;
    void enableHands();
    void disableHands();
    void translateToRussian();
    void translateToEnglish();
};

#endif // MAINWINDOW_H
