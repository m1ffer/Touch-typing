#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QLabel> // ДОБАВЛЕНО
#include <QTime> // ДОБАВЛЕНО

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
    // Добавляем слоты для переключения страниц
    void switchToTrainingMode();
    void switchToLearningMode();
    // ДОБАВЛЕНО: Слоты для работы с таймером
    void onTypingTimerStarted();
    void onTypingTimerStopped(qint64 elapsedTime);
    void onTypingTimerUpdated(qint64 elapsedTime);

    void onResetButtonClicked();
    void onCloseChoseButtonClicked();

private:
    void applyButtonStyle();  // Добавляем объявление метода
    void initializeLessons();
    void onLessonSelected(const QString &lessonName, const QVariant &data);
    // ДОБАВЛЕНО: Методы для работы со временем и статистикой
    void updateTimerDisplay(qint64 milliseconds);
    void saveStatistics(qint64 timeMs, int lessonId);
    void resetTimerDisplay();
    Ui::MainWindow *ui;
    QButtonGroup *modeButtonGroup;
    // ДОБАВЛЕНО: Переменные для статистики
    QList<qint64> m_typingTimes;
    int m_currentLessonId;
};

#endif // MAINWINDOW_H
