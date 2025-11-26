#include "StatisticsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

StatisticsWidget::StatisticsWidget(StatisticsManager *statsManager, QWidget *parent)
    : QWidget(parent)
    , m_statsManager(statsManager)
{
    setWindowTitle("Статистика печати");
    setMinimumSize(800, 600);

    // Основной layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Создаем вкладки
    m_tabWidget = new QTabWidget(this);

    // Вкладка с графиками
    QWidget *chartsTab = new QWidget();
    QVBoxLayout *chartsLayout = new QVBoxLayout(chartsTab);

    // Создаем графики
    createSpeedChart();
    createAccuracyChart();
    createProgressChart();

    chartsLayout->addWidget(m_speedChartView);
    chartsLayout->addWidget(m_accuracyChartView);
    chartsLayout->addWidget(m_progressChartView);

    // Вкладка со сводкой
    createSummaryPanel();

    m_tabWidget->addTab(chartsTab, "Графики");
    m_tabWidget->addTab(m_summaryWidget, "Сводка");

    mainLayout->addWidget(m_tabWidget);

    // Обновляем данные
    updateCharts();
    updateSummary();
}

void StatisticsWidget::createSpeedChart()
{
    QLineSeries *series = new QLineSeries();
    series->setName("Скорость (слов/мин)");

    auto records = m_statsManager->getRecords();
    for (int i = 0; i < records.size(); ++i) {
        series->append(i, records[i].speedWpm);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Прогресс скорости печати");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Сессии");
    axisX->setLabelFormat("%d");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Слов в минуту");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_speedChartView = new QChartView(chart);
    m_speedChartView->setRenderHint(QPainter::Antialiasing);
}

void StatisticsWidget::createAccuracyChart()
{
    QLineSeries *series = new QLineSeries();
    series->setName("Точность (%)");

    auto records = m_statsManager->getRecords();
    for (int i = 0; i < records.size(); ++i) {
        series->append(i, records[i].accuracy);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Прогресс точности печати");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Сессии");
    axisX->setLabelFormat("%d");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Точность (%)");
    axisY->setRange(0, 100);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_accuracyChartView = new QChartView(chart);
    m_accuracyChartView->setRenderHint(QPainter::Antialiasing);
}

void StatisticsWidget::createProgressChart()
{
    QBarSet *speedSet = new QBarSet("Скорость");
    QBarSet *accuracySet = new QBarSet("Точность");

    // Группируем по дням
    QMap<QDate, QList<StatisticsRecord>> dailyRecords;
    auto records = m_statsManager->getRecords();
    for (const auto &record : records) {
        QDate date = record.timestamp.date();
        dailyRecords[date].append(record);
    }

    // Вычисляем средние значения по дням
    QVector<QDate> dates = dailyRecords.keys().toVector();
    std::sort(dates.begin(), dates.end());

    for (const QDate &date : dates) {
        double avgSpeed = 0.0;
        double avgAccuracy = 0.0;
        const auto &dayRecords = dailyRecords[date];

        for (const auto &record : dayRecords) {
            avgSpeed += record.speedWpm;
            avgAccuracy += record.accuracy;
        }

        avgSpeed /= dayRecords.size();
        avgAccuracy /= dayRecords.size();

        *speedSet << avgSpeed;
        *accuracySet << avgAccuracy;
    }

    QBarSeries *series = new QBarSeries();
    series->append(speedSet);
    series->append(accuracySet);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Ежедневная статистика");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    m_progressChartView = new QChartView(chart);
    m_progressChartView->setRenderHint(QPainter::Antialiasing);
}

void StatisticsWidget::createSummaryPanel()
{
    m_summaryWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(m_summaryWidget);

    QGroupBox *summaryGroup = new QGroupBox("Общая статистика");
    QFormLayout *formLayout = new QFormLayout(summaryGroup);

    m_totalSessionsLabel = new QLabel("0");
    m_totalTimeLabel = new QLabel("0 мин");
    m_avgSpeedLabel = new QLabel("0 слов/мин");
    m_avgAccuracyLabel = new QLabel("0%");
    m_bestSpeedLabel = new QLabel("0 слов/мин");

    formLayout->addRow("Всего сессий:", m_totalSessionsLabel);
    formLayout->addRow("Общее время:", m_totalTimeLabel);
    formLayout->addRow("Средняя скорость:", m_avgSpeedLabel);
    formLayout->addRow("Средняя точность:", m_avgAccuracyLabel);
    formLayout->addRow("Лучшая скорость:", m_bestSpeedLabel);

    layout->addWidget(summaryGroup);
    layout->addStretch();
}

void StatisticsWidget::updateCharts()
{
    // Обновляем все графики
    createSpeedChart();
    createAccuracyChart();
    createProgressChart();
}

void StatisticsWidget::updateSummary()
{
    auto records = m_statsManager->getRecords();
    StatisticsRecord best = m_statsManager->getBestRecord();

    m_totalSessionsLabel->setText(QString::number(records.size()));
    m_totalTimeLabel->setText(QString::number(m_statsManager->getTotalPracticeTime()) + " мин");
    m_avgSpeedLabel->setText(QString::number(m_statsManager->getAverageSpeed(), 'f', 1) + " слов/мин");
    m_avgAccuracyLabel->setText(QString::number(m_statsManager->getAverageAccuracy(), 'f', 1) + "%");
    m_bestSpeedLabel->setText(QString::number(best.speedWpm, 'f', 1) + " слов/мин");
}
