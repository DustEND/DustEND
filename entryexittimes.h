#ifndef ENTRYEXITTIMES_H
#define ENTRYEXITTIMES_H

#include <QString>
#include <QDateTime>

class EntryExitTimes
{
public:
    EntryExitTimes();

    QString getIpAddress() const;
    QDateTime getEntryTime() const;
    QDateTime getExitTime() const;
    int getTime() const;

    void setIpAddress(QString value);
    void setEntryTime(QDateTime value);
    void setExitTime(QDateTime value);
    void setTime(int value);

private:
    QString ipAddress;
    QDateTime entryTime;
    QDateTime exitTime;
    int time;
};

#endif // ENTRYEXITTIMES_H
