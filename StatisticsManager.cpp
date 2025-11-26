#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(QObject *parent)
    : QObject(parent)
    , m_dataFile("typing_statistics.json")
{
    loadFromFile(m_dataFile);
}

void StatisticsManager::addRecord(const StatisticsRecord &record)
{
    m_records.append(record);
    saveToFile(m_dataFile);
    qDebug() << "Добавлена запись статистики для урока" << record.lessonId;
}

QVector<StatisticsRecord> StatisticsManager::getRecords() const
{
    return m_records;
}

QVector<StatisticsRecord> StatisticsManager::getRecordsForLesson(int lessonId) const
{
    QVector<StatisticsRecord> result;
    for (const auto &record : m_records) {
        if (record.lessonId == lessonId) {
            result.append(record);
        }
    }
    return result;
}

QVector<StatisticsRecord> StatisticsManager::getRecordsForDateRange(const QDateTime &from, const QDateTime &to) const
{
    QVector<StatisticsRecord> result;
    for (const auto &record : m_records) {
        if (record.timestamp >= from && record.timestamp <= to) {
            result.append(record);
        }
    }
    return result;
}

double StatisticsManager::getAverageSpeed() const
{
    if (m_records.isEmpty()) return 0.0;
    double total = 0.0;
    for (const auto &record : m_records) {
        total += record.speedWpm;
    }
    return total / m_records.size();
}

double StatisticsManager::getAverageAccuracy() const
{
    if (m_records.isEmpty()) return 0.0;
    double total = 0.0;
    for (const auto &record : m_records) {
        total += record.accuracy;
    }
    return total / m_records.size();
}

StatisticsRecord StatisticsManager::getBestRecord() const
{
    if (m_records.isEmpty()) return StatisticsRecord();

    StatisticsRecord best = m_records.first();
    for (const auto &record : m_records) {
        if (record.speedWpm > best.speedWpm && record.accuracy >= 95.0) {
            best = record;
        }
    }
    return best;
}

int StatisticsManager::getTotalPracticeTime() const
{
    qint64 totalMs = 0;
    for (const auto &record : m_records) {
        totalMs += record.timeMs;
    }
    return totalMs / 60000; // конвертация в минуты
}

bool StatisticsManager::saveToFile(const QString &filename)
{
    QJsonArray recordsArray;
    for (const auto &record : m_records) {
        recordsArray.append(record.toJson());
    }

    QJsonDocument doc(recordsArray);
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Не удалось открыть файл для записи статистики:" << filename;
        return false;
    }

    file.write(doc.toJson());
    file.close();
    return true;
}

bool StatisticsManager::loadFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл для чтения статистики:" << filename;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        qWarning() << "Некорректный формат файла статистики";
        return false;
    }

    m_records.clear();
    QJsonArray array = doc.array();
    for (const auto &value : array) {
        if (value.isObject()) {
            m_records.append(StatisticsRecord::fromJson(value.toObject()));
        }
    }

    qDebug() << "Загружено записей статистики:" << m_records.size();
    return true;
}
