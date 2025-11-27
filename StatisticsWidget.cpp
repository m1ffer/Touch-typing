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
    setMinimumSize(900, 800);
    resize(950, 850);

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

    // Увеличим размер кнопок
    m_repeatButton->setMinimumSize(120, 35);
    m_nextButton->setMinimumSize(120, 35);

    // Подключаем кнопки к слотам
    connect(m_repeatButton, &QPushButton::clicked, this, &StatisticsWidget::onRepeatClicked);
    connect(m_nextButton, &QPushButton::clicked, this, &StatisticsWidget::onNextClicked);

    // Layout для кнопок
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(m_repeatButton);
    buttonsLayout->addWidget(m_nextButton);
    buttonsLayout->addStretch();

    // Добавляем все в основной layout
    mainLayout->addWidget(statsGroup);
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
    series->setName("Скорость (симв/мин)");

    // Увеличиваем толщину линии
    QPen pen = series->pen();
    pen.setWidth(2);
    series->setPen(pen);

    // Простое сглаживание данных (скользящее среднее)
    QVector<QPair<qint64, double>> smoothedHistory;
    const int windowSize = 3; // Размер окна для сглаживания

    for (int i = 0; i < speedHistory.size(); ++i) {
        double sum = 0;
        int count = 0;

        // Берем окно вокруг текущей точки
        for (int j = qMax(0, i - windowSize); j <= qMin(speedHistory.size() - 1, i + windowSize); ++j) {
            sum += speedHistory[j].second;
            count++;
        }

        double smoothedValue = sum / count;
        smoothedHistory.append(qMakePair(speedHistory[i].first, smoothedValue));
    }

    // Используем сглаженные данные или исходные, если точек мало
    const QVector<QPair<qint64, double>>& dataToUse =
        (speedHistory.size() > 10) ? smoothedHistory : speedHistory;

    // Добавляем точки данных (время в секундах, скорость в CPM)
    for (const auto& point : dataToUse) {
        double timeInSeconds = point.first / 1000.0;
        series->append(timeInSeconds, point.second);
    }

    // Создаем график
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Динамика скорости во время тренировки");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Увеличиваем размер шрифта заголовка
    QFont titleFont = chart->titleFont();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    chart->setTitleFont(titleFont);

    // Настраиваем ось X (время в секундах)
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Время (секунды)");
    axisX->setTitleFont(QFont("Arial", 12, QFont::Bold));

    if (!speedHistory.isEmpty()) {
        double maxTime = speedHistory.last().first / 1000.0;
        axisX->setRange(0, maxTime);

        // Используем константу для количества делений
        axisX->setTickCount(TIME_AXIS_TICKS);

        // Форматируем подписи как целые числа для лучшей читаемости
        axisX->setLabelFormat("%.0f");
    } else {
        axisX->setRange(0, 10);
        axisX->setTickCount(TIME_AXIS_TICKS);
    }

    // Увеличиваем шрифт подписей на оси X
    QFont axisFont = axisX->labelsFont();
    axisFont.setPointSize(9);
    axisX->setLabelsFont(axisFont);

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Настраиваем ось Y (скорость в CPM)
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Скорость (симв/мин)");
    axisY->setTitleFont(QFont("Arial", 12, QFont::Bold));

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

    // Используем константу для количества делений
    axisY->setTickCount(SPEED_AXIS_TICKS);

    // Форматируем подписи как целые числа
    axisY->setLabelFormat("%.0f");

    // Увеличиваем шрифт подписей на оси Y
    axisY->setLabelsFont(axisFont);

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Устанавливаем темную тему для графика
    chart->setTheme(QChart::ChartThemeDark);

    // Улучшаем легенду
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Создаем view для графика
    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setMinimumHeight(500);
    m_chartView->setMinimumWidth(800);
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
