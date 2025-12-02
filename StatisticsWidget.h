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
    void repeatRequested();
    void nextRequested();

private slots:
    void onRepeatClicked();
    void onNextClicked();

private:
    void createSpeedChart(const QVector<QPair<qint64, double>>& speedHistory);

    QPushButton *m_repeatButton;
    QPushButton *m_nextButton;
    QChartView *m_chartView;
    // Константы для настройки количества делений на осях
    static const int TIME_AXIS_TICKS = 15;    // Количество делений по времени (оси X)
    static const int SPEED_AXIS_TICKS = 10;    // Количество делений по скорости (оси Y)
};

#endif // STATISTICSWIDGET_H
