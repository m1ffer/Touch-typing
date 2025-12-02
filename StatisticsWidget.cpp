#include "StatisticsWidget.h"
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

StatisticsWidget::StatisticsWidget(double accuracy, double speedCpm,
                                   qint64 timeMs, int errorsCount, int totalChars,
                                   const QVector<QPair<qint64, double>>& speedHistory,
                                   QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Результаты тренировки");
    setMinimumSize(900, 700);
    resize(950, 750);  // Увеличили высоту

    // Устанавливаем модальность и убираем кнопки управления
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // ЖЕЛТЫЙ СТИЛЬ ПРИЛОЖЕНИЯ
    setStyleSheet(R"(
        StatisticsWidget {
            background-color: #1a1a1a;
        }
        QLabel {
            color: #ffd700;
            font-family: 'Roboto Mono', 'Consolas', monospace;
            font-size: 14px;
        }
        QGroupBox {
            border: 1px solid #3a3a3a;
            border-radius: 0px;
            margin-top: 10px;
            padding-top: 10px;
            color: #ffd700;
            font-weight: 500;
            font-family: 'Roboto Mono', 'Consolas', monospace;
            font-size: 14px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
            color: #ffd700;
        }
        QPushButton {
            font-family: 'Roboto Mono', 'Consolas', monospace;
            font-size: 14px;
            font-weight: 400;
            padding: 10px 20px;
            border: 1px solid #3a3a3a;
            background-color: #2a2a2a;
            color: #ffd700;
            border-radius: 0px;
            min-width: 120px;
        }
        QPushButton:hover {
            background-color: #404040;
            border-color: #ffd700;
            color: #ffd700;
        }
        QPushButton:pressed {
            background-color: #505050;
            color: #ffd700;
        }
        QPushButton:focus {
            outline: none;
        }
    )");

    // Основной layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Создаем график если есть данные (УВЕЛИЧЕННЫЙ РАЗМЕР)
    if (!speedHistory.isEmpty()) {
        createSpeedChart(speedHistory);
        m_chartView->setMinimumHeight(450);  // Увеличили график
        m_chartView->setMinimumWidth(850);
        mainLayout->addWidget(m_chartView, 0, Qt::AlignCenter);
    }

    // Группа с статистикой
    QGroupBox *statsGroup = new QGroupBox("Статистика тренировки");
    QFormLayout *formLayout = new QFormLayout(statsGroup);
    formLayout->setContentsMargins(20, 20, 20, 20);
    formLayout->setSpacing(12);

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

    // Устанавливаем увеличенный шрифт
    QFont boldFont = speedCpmLabel->font();
    boldFont.setPointSize(14);
    boldFont.setBold(true);
    speedCpmLabel->setFont(boldFont);
    accuracyLabel->setFont(boldFont);

    QFont labelFont = timeLabel->font();
    labelFont.setPointSize(13);
    timeLabel->setFont(labelFont);
    errorsLabel->setFont(labelFont);
    charsTypedLabel->setFont(labelFont);

    formLayout->addRow("Время:", timeLabel);
    formLayout->addRow("Точность:", accuracyLabel);
    formLayout->addRow("Скорость:", speedCpmLabel);
    formLayout->addRow("Ошибки:", errorsLabel);
    formLayout->addRow("Символов введено:", charsTypedLabel);

    // Создаем кнопки
    m_repeatButton = new QPushButton("Повторить", this);
    m_nextButton = new QPushButton("Дальше", this);

    // Увеличиваем размер кнопок
    m_repeatButton->setMinimumSize(140, 45);
    m_nextButton->setMinimumSize(140, 45);

    // Подключаем кнопки к слотам
    connect(m_repeatButton, &QPushButton::clicked, this, &StatisticsWidget::onRepeatClicked);
    connect(m_nextButton, &QPushButton::clicked, this, &StatisticsWidget::onNextClicked);

    // Layout для кнопок
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(m_repeatButton);
    buttonsLayout->addSpacing(30);
    buttonsLayout->addWidget(m_nextButton);
    buttonsLayout->addStretch();

    // Добавляем все в основной layout
    mainLayout->addWidget(statsGroup);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonsLayout);

    // Центрируем окно относительно родителя
    if (parent) {
        QRect parentRect = parent->geometry();
        QPoint center = parentRect.center();
        move(center - rect().center());
    }
}

void StatisticsWidget::createSpeedChart(const QVector<QPair<qint64, double>>& speedHistory)
{
    // Создаем серию данных для графика
    QLineSeries *series = new QLineSeries();
    series->setName("Изменение скорости");

    // Устанавливаем ЖЕЛТЫЙ цвет и толстую линию
    QPen pen(QColor(255, 215, 0));  // ЖЕЛТЫЙ ЦВЕТ
    pen.setWidth(3);
    series->setPen(pen);

    // Простое сглаживание данных (скользящее среднее)
    QVector<QPair<qint64, double>> smoothedHistory;
    const int windowSize = 3;

    for (int i = 0; i < speedHistory.size(); ++i) {
        double sum = 0;
        int count = 0;

        for (int j = qMax(0, i - windowSize); j <= qMin(speedHistory.size() - 1, i + windowSize); ++j) {
            sum += speedHistory[j].second;
            count++;
        }

        double smoothedValue = sum / count;
        smoothedHistory.append(qMakePair(speedHistory[i].first, smoothedValue));
    }

    // Используем сглаженные данные или исходные
    const QVector<QPair<qint64, double>>& dataToUse =
        (speedHistory.size() > 10) ? smoothedHistory : speedHistory;

    // Добавляем точки данных
    for (const auto& point : dataToUse) {
        double timeInSeconds = point.first / 1000.0;
        series->append(timeInSeconds, point.second);
    }

    // Создаем график
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Динамика скорости во время тренировки");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Устанавливаем темный фон графика
    chart->setBackgroundBrush(QBrush(QColor(26, 26, 26)));  // #1a1a1a

    // Устанавливаем ЖЕЛТЫЙ цвет заголовка
    chart->setTitleBrush(QBrush(QColor(255, 215, 0)));

    // Увеличиваем размер шрифта заголовка
    QFont titleFont("Roboto Mono, Consolas, monospace", 14);
    titleFont.setBold(true);
    chart->setTitleFont(titleFont);

    // Настраиваем ось X (время в секундах)
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Время (секунды)");
    axisX->setTitleBrush(QBrush(QColor(255, 215, 0)));  // ЖЕЛТЫЙ
    axisX->setLabelsBrush(QBrush(QColor(255, 215, 0)));  // ЖЕЛТЫЙ

    QFont axisFont("Roboto Mono, Consolas, monospace", 11);
    axisX->setTitleFont(axisFont);
    axisFont.setPointSize(10);
    axisX->setLabelsFont(axisFont);

    if (!speedHistory.isEmpty()) {
        double maxTime = speedHistory.last().first / 1000.0;
        axisX->setRange(0, maxTime);
        axisX->setTickCount(TIME_AXIS_TICKS);  // Используем константу
        axisX->setLabelFormat("%.0f");
    } else {
        axisX->setRange(0, 10);
        axisX->setTickCount(TIME_AXIS_TICKS);
    }

    // Устанавливаем цвет линий и меток оси X
    axisX->setLinePenColor(QColor(90, 90, 90));
    axisX->setGridLineColor(QColor(60, 60, 60));

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Настраиваем ось Y (скорость в CPM)
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Скорость (симв/мин)");
    axisY->setTitleBrush(QBrush(QColor(255, 215, 0)));  // ЖЕЛТЫЙ
    axisY->setLabelsBrush(QBrush(QColor(255, 215, 0)));  // ЖЕЛТЫЙ

    axisY->setTitleFont(axisFont);
    axisY->setLabelsFont(axisFont);

    // Находим максимальную скорость
    double maxSpeed = 0;
    for (const auto& point : speedHistory) {
        if (point.second > maxSpeed) {
            maxSpeed = point.second;
        }
    }

    // Устанавливаем диапазон с запасом
    axisY->setMin(0);
    double upperBound = qMax(maxSpeed * 1.1, 50.0);
    axisY->setMax(upperBound);
    axisY->setTickCount(SPEED_AXIS_TICKS);  // Используем константу
    axisY->setLabelFormat("%.0f");

    // Устанавливаем цвет линий и меток оси Y
    axisY->setLinePenColor(QColor(90, 90, 90));
    axisY->setGridLineColor(QColor(60, 60, 60));

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Настраиваем легенду
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setLabelColor(QColor(255, 215, 0));  // ЖЕЛТЫЙ

    QFont legendFont("Roboto Mono, Consolas, monospace", 11);
    chart->legend()->setFont(legendFont);

    // Создаем view для графика
    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    // Устанавливаем фон view в темный
    m_chartView->setBackgroundBrush(QBrush(QColor(26, 26, 26)));
}

void StatisticsWidget::onRepeatClicked()
{
    qDebug() << "Кнопка 'Повторить' нажата";
    emit repeatRequested();
    this->close();
}

void StatisticsWidget::onNextClicked()
{
    qDebug() << "Кнопка 'Дальше' нажата";
    emit nextRequested();
    this->close();
}
