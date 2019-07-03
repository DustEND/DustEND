#ifndef COMPUTER_H
#define COMPUTER_H

#include <QString>
#include <QDateTime>
#include "cooler.h"

class Computer
{
public:
    Computer();
    ~Computer();
    Computer& operator=(const Computer& orig);
    bool operator!=(const Computer& orig);

    void setId(int value);
    int getId() const;

    void setIpAddress(QString newIpAddress);
    QString getIpAddress() const;

    void setName(QString newName);
    QString getName() const;

    void setOsName(QString newOsName);
    QString getOsName() const;

    void setPlaceType(unsigned short newPlaceType);
    unsigned short getPlaceType() const;

    void setCleanTime(QDateTime newCleanTime);
    QDateTime getCleanTime() const;

    void setFilterTime(QDateTime newFilterTime);
    QDateTime getFilterTime() const;

    void setCleanTimeIsRunning(bool newCleanTimeIsRunning);
    bool getCleanTimeIsRunning() const;

    void setFilterTimeIsRunning(bool newFilterTimeIsRunning);
    bool getFilterTimeIsRunning() const;

    void setMaxCleanTime(int newMaxCleanTime);
    int getMaxCleanTime() const;

    void setMaxFilterTime(int newMaxFilterTime);
    int getMaxFilterTime() const;

    void setOutLeftCoolerDiameter(unsigned short newDiameter);
    unsigned short getOutLeftCoolerDiameter() const;

    void setOutTopCoolerDiameter(unsigned short newDiameter);
    unsigned short getOutTopCoolerDiameter() const;

    void setInRightCoolerDiameter(unsigned short newDiameter);
    unsigned short getInRightCoolerDiameter() const;

    void setInBottomCoolerDiameter(unsigned short newDiameter);
    unsigned short getInBottomCoolerDiameter() const;

    void setInSideCoolerDiameter(unsigned short newDiameter);
    unsigned short getInSideCoolerDiameter() const;

    void setOutLeftCoolerAmount(unsigned short newAmount);
    unsigned short getOutLeftCoolerAmount() const;

    void setOutTopCoolerAmount(unsigned short newAmount);
    unsigned short getOutTopCoolerAmount() const;

    void setInRightCoolerAmount(unsigned short newAmount);
    unsigned short getInRightCoolerAmount() const;

    void setInBottomCoolerAmount(unsigned short newAmount);
    unsigned short getInBottomCoolerAmount() const;

    void setInSideCoolerAmount(unsigned short newAmount);
    unsigned short getInSideCoolerAmount() const;

    void setMaxTemperature(unsigned short newMaxTemperature);
    unsigned short getMaxTemperature() const;

    void setUsageTime(unsigned long newUsageTime);
    unsigned long getUsageTime() const;

    void setMaxFanSpeed(unsigned int newMaxFanSpeed);
    unsigned int getMaxFanSpeed() const;

    void setCompIsPaused(bool newCompIsPaused);
    bool getCompIsPaused() const;

    void setDateOfFirstStart(QDateTime newDateOfFirstStart);
    QDateTime getDateOfFirstStart() const;

    void setLastMaxTime(QString newLastMaxTime);
    QString getLastMaxTime() const;

private:
    int id;
    QString ipAddress;
    QString name;
    unsigned short placeType;
    QDateTime cleanTime;
    QDateTime filterTime;
    bool cleanTimeIsRunning;
    bool filterTimeIsRunning;
    int maxCleanTime;
    int maxFilterTime;
    QString osName;

    Cooler outLeftCooler;
    Cooler outTopCooler;
    Cooler inRightCooler;
    Cooler inBottomCooler;
    Cooler inSideCooler;

    unsigned short maxTemperature;
    unsigned long usageTime;
    unsigned int maxFanSpeed;
    bool compIsPaused;
    QDateTime dateOfFirstStart;
    QString lastMaxTime;
};

#endif // COMPUTER_H
