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
                              const QVector<QPair<qint64, double>>& speedHistory,  // Добавили историю
                              QWidget *parent = nullptr);

private slots:
    void closeWindow();

private:
    void createSpeedChart(const QVector<QPair<qint64, double>>& speedHistory);

    QPushButton *m_closeButton;
    QChartView *m_chartView;  // Для отображения графика
};

#endif // STATISTICSWIDGET_H
