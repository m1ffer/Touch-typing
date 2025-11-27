#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QLabel>
#include <QTime>

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
    void showTypingResults();  // Показ статистики

private:
    void applyButtonStyle();
    void initializeLessons();
    void onLessonSelected(const QString &lessonName, const QVariant &data);
    void updateTimerDisplay(qint64 milliseconds);
    void resetTimerDisplay();

    Ui::MainWindow *ui;
    QButtonGroup *modeButtonGroup;
    int m_currentLessonId;
};

#endif // MAINWINDOW_H
