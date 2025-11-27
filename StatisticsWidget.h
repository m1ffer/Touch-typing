#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QGroupBox>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>

class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(double accuracy, double speedCpm,
                              qint64 timeMs, int errorsCount, int totalChars,
                              const QVector<QPair<qint64, double>>& speedHistory,
                              QWidget *parent = nullptr);

signals:
    void repeatRequested();  // Сигнал для кнопки "Повторить"
    void nextRequested();    // Сигнал для кнопки "Дальше"

private slots:
    void onRepeatClicked();  // Слот для кнопки "Повторить"
    void onNextClicked();    // Слот для кнопки "Дальше"

private:
    void createSpeedChart(const QVector<QPair<qint64, double>>& speedHistory);

    QPushButton *m_repeatButton;  // Кнопка "Повторить"
    QPushButton *m_nextButton;    // Кнопка "Дальше"
    QChartView *m_chartView;
};

#endif // STATISTICSWIDGET_H
