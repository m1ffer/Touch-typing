#include "StatisticsWidget.h"
#include <QDebug>
#include <QApplication>
#include <QScreen>

StatisticsWidget::StatisticsWidget(double accuracy, double speedCpm,
                                   qint64 timeMs, int errorsCount, int totalChars,
                                   const QVector<QPair<qint64, double>>& speedHistory,
                                   QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Результаты тренировки");
    setMinimumSize(500, 600);

    // Устанавливаем модальность и убираем кнопки управления
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // Основной layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Создаем график если есть данные
    if (!speedHistory.isEmpty()) {
        createSpeedChart(speedHistory);
        mainLayout->addWidget(m_chartView);
    }

    // Группа с статистикой
    QGroupBox *statsGroup = new QGroupBox("Статистика тренировки");
    QFormLayout *formLayout = new QFormLayout(statsGroup);

    // Исправляем отображение времени
    int totalSeconds = timeMs / 1000;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    QString timeString;

    if (minutes > 0) {
        timeString = QString("%1 мин %2 сек").arg(minutes).arg(seconds);
    } else {
        timeString = QString("%1 сек").arg(seconds);
    }

    // Создаем метки для статистики
    QLabel *timeLabel = new QLabel(timeString);
    QLabel *accuracyLabel = new QLabel(QString("%1%").arg(accuracy, 0, 'f', 1));
    QLabel *speedCpmLabel = new QLabel(QString("%1 симв/мин").arg(speedCpm, 0, 'f', 1));
    QLabel *errorsLabel = new QLabel(QString::number(errorsCount));
    QLabel *charsTypedLabel = new QLabel(QString::number(totalChars));

    // Делаем основные показатели более заметными
    QFont boldFont = speedCpmLabel->font();
    boldFont.setPointSize(12);
    boldFont.setBold(true);
    speedCpmLabel->setFont(boldFont);
    accuracyLabel->setFont(boldFont);

    formLayout->addRow("Время:", timeLabel);
    formLayout->addRow("Точность:", accuracyLabel);
    formLayout->addRow("Скорость:", speedCpmLabel);
    formLayout->addRow("Ошибки:", errorsLabel);
    formLayout->addRow("Символов введено:", charsTypedLabel);

    // Создаем кнопки "Повторить" и "Дальше"
    m_repeatButton = new QPushButton("Повторить", this);
    m_nextButton = new QPushButton("Дальше", this);

    // Подключаем кнопки к слотам
    connect(m_repeatButton, &QPushButton::clicked, this, &StatisticsWidget::onRepeatClicked);
    connect(m_nextButton, &QPushButton::clicked, this, &StatisticsWidget::onNextClicked);

    // Layout для кнопок
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(m_repeatButton);
    buttonsLayout->addWidget(m_nextButton);

    // Добавляем все в основной layout
    mainLayout->addWidget(statsGroup);
    mainLayout->addLayout(buttonsLayout);

    // Центрируем окно относительно родителя
    if (parent) {
        QRect parentRect = parent->geometry();
        QPoint center = parentRect.center();
        move(center - rect().center());
    }

    qDebug() << "=== СТАТИСТИКА ===";
    qDebug() << "Время:" << timeMs << "мс (" << timeString << ")";
    qDebug() << "Точность:" << accuracy << "%";
    qDebug() << "Скорость CPM:" << speedCpm;
    qDebug() << "Ошибки:" << errorsCount;
    qDebug() << "Символов:" << totalChars;
    qDebug() << "Точек на графике:" << speedHistory.size();
}

void StatisticsWidget::createSpeedChart(const QVector<QPair<qint64, double>>& speedHistory)
{
    // Создаем серию данных для графика
    QLineSeries *series = new QLineSeries();
    series->setName("Скорость (симв/мин)");

    // Добавляем точки данных (время в секундах, скорость в CPM)
    for (const auto& point : speedHistory) {
        double timeInSeconds = point.first / 1000.0;  // конвертируем мс в секунды
        series->append(timeInSeconds, point.second);
    }

    // Создаем график
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Динамика скорости во время тренировки");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Настраиваем ось X (время в секундах)
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Время (секунды)");
    axisX->setLabelFormat("%.1f");
    axisX->setMin(0);
    if (!speedHistory.isEmpty()) {
        double maxTime = speedHistory.last().first / 1000.0;
        axisX->setMax(maxTime);
    }
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Настраиваем ось Y (скорость в CPM)
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Скорость (симв/мин)");
    axisY->setMin(0);

    // Находим максимальную скорость для установки верхней границы
    double maxSpeed = 0;
    for (const auto& point : speedHistory) {
        if (point.second > maxSpeed) {
            maxSpeed = point.second;
        }
    }
    axisY->setMax(qMax(maxSpeed * 1.1, 100.0)); // Добавляем 10% запаса

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Создаем view для графика
    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setMinimumHeight(250);
}

void StatisticsWidget::onRepeatClicked()
{
    qDebug() << "Кнопка 'Повторить' нажата";
    emit repeatRequested();  // Испускаем сигнал
    this->close();           // Закрываем окно
}

void StatisticsWidget::onNextClicked()
{
    qDebug() << "Кнопка 'Дальше' нажата";
    emit nextRequested();    // Испускаем сигнал
    this->close();           // Закрываем окно
}
