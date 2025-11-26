#ifndef STATISTICSMANAGER_H
#define STATISTICSMANAGER_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

struct StatisticsRecord {
    QDateTime timestamp;
    int lessonId;
    qint64 timeMs;
    int totalChars;
    int errorsCount;
    double accuracy;
    double speedWpm;
    double speedCpm;

    // Конвертация в JSON
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["timestamp"] = timestamp.toString(Qt::ISODate);
        obj["lessonId"] = lessonId;
        obj["timeMs"] = static_cast<qint64>(timeMs);
        obj["totalChars"] = totalChars;
        obj["errorsCount"] = errorsCount;
        obj["accuracy"] = accuracy;
        obj["speedWpm"] = speedWpm;
        obj["speedCpm"] = speedCpm;
        return obj;
    }

    // Конвертация из JSON
    static StatisticsRecord fromJson(const QJsonObject &obj) {
        StatisticsRecord record;
        record.timestamp = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
        record.lessonId = obj["lessonId"].toInt();
        record.timeMs = obj["timeMs"].toInt();
        record.totalChars = obj["totalChars"].toInt();
        record.errorsCount = obj["errorsCount"].toInt();
        record.accuracy = obj["accuracy"].toDouble();
        record.speedWpm = obj["speedWpm"].toDouble();
        record.speedCpm = obj["speedCpm"].toDouble();
        return record;
    }
};

class StatisticsManager : public QObject
{
    Q_OBJECT

public:
    explicit StatisticsManager(QObject *parent = nullptr);

    void addRecord(const StatisticsRecord &record);
    QVector<StatisticsRecord> getRecords() const;
    QVector<StatisticsRecord> getRecordsForLesson(int lessonId) const;
    QVector<StatisticsRecord> getRecordsForDateRange(const QDateTime &from, const QDateTime &to) const;

    // Агрегированная статистика
    double getAverageSpeed() const;
    double getAverageAccuracy() const;
    StatisticsRecord getBestRecord() const;
    int getTotalPracticeTime() const; // в минутах

    // Сохранение/загрузка
    bool saveToFile(const QString &filename);
    bool loadFromFile(const QString &filename);

private:
    QVector<StatisticsRecord> m_records;
    QString m_dataFile;
};

#endif // STATISTICSMANAGER_H
