#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QGridLayout>
#include <QLabel>
#include <QTabWidget>
#include "StatisticsManager.h"

QT_CHARTS_USE_NAMESPACE

    class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(StatisticsManager *statsManager, QWidget *parent = nullptr);

private slots:
    void updateCharts();
    void updateSummary();

private:
    void createSpeedChart();
    void createAccuracyChart();
    void createProgressChart();
    void createSummaryPanel();

    StatisticsManager *m_statsManager;
    QTabWidget *m_tabWidget;

    // Графики
    QChartView *m_speedChartView;
    QChartView *m_accuracyChartView;
    QChartView *m_progressChartView;

    // Панель сводки
    QWidget *m_summaryWidget;
    QLabel *m_totalSessionsLabel;
    QLabel *m_totalTimeLabel;
    QLabel *m_avgSpeedLabel;
    QLabel *m_avgAccuracyLabel;
    QLabel *m_bestSpeedLabel;
};

#endif // STATISTICSWIDGET_H
