#include "entryexittimes.h"

EntryExitTimes::EntryExitTimes()
{
}

QString EntryExitTimes::getIpAddress() const
{
    return this->ipAddress;
}

QDateTime EntryExitTimes::getEntryTime() const
{
    return this->entryTime;
}

QDateTime EntryExitTimes::getExitTime() const
{
    return this->exitTime;
}

int EntryExitTimes::getTime() const
{
    return this->time;
}

void EntryExitTimes::setIpAddress(QString value)
{
    this->ipAddress = value;
}

void EntryExitTimes::setEntryTime(QDateTime value)
{
    this->entryTime = value;
}

void EntryExitTimes::setExitTime(QDateTime value)
{
    this->exitTime = value;
}

void EntryExitTimes::setTime(int value)
{
    this->time = value;
}
