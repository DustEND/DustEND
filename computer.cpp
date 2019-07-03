#include "computer.h"
#include <QDebug>
Computer::Computer()
{
    this->id = 0;
}

Computer::~Computer()
{
    qDebug() << "computer destructor 1";
    outLeftCooler.~Cooler();
    outTopCooler.~Cooler();
    inRightCooler.~Cooler();
    inBottomCooler.~Cooler();
    inSideCooler.~Cooler();
    qDebug() << "computer destructor 2";
}

Computer &Computer::operator =(const Computer &orig)
{
    this->id = orig.id;
    this->ipAddress = orig.ipAddress;
    this->name = orig.name;
    this->osName = orig.osName;
    this->placeType = orig.placeType;
    this->outLeftCooler = orig.outLeftCooler;
    this->outTopCooler = orig.outTopCooler;
    this->inRightCooler = orig.inRightCooler;
    this->inSideCooler = orig.inSideCooler;
    this->inBottomCooler = orig.inBottomCooler;
    this->maxCleanTime = orig.maxCleanTime;
    this->maxFilterTime = orig.maxFilterTime;
    this->maxFanSpeed = orig.maxFanSpeed;
    this->maxTemperature = orig.maxTemperature;
    this->cleanTime = orig.cleanTime;
    this->filterTime = orig.filterTime;
    this->cleanTimeIsRunning = orig.cleanTimeIsRunning;
    this->filterTimeIsRunning = orig.filterTimeIsRunning;
    this->compIsPaused = orig.compIsPaused;
    this->dateOfFirstStart = orig.dateOfFirstStart;
    this->lastMaxTime = orig.lastMaxTime;
    this->usageTime = orig.usageTime;
    return *this;
}

bool Computer::operator !=(const Computer &orig)
{
    if (this->id != orig.id)
        if (this->ipAddress != orig.ipAddress)
            if (this->name != orig.name)
                if (this->osName != orig.osName)
                    if (this->placeType != orig.placeType)
                        if (this->outLeftCooler != orig.outLeftCooler)
                            if (this->outTopCooler != orig.outTopCooler)
                                if (this->inRightCooler != orig.inRightCooler)
                                    if (this->inSideCooler != orig.inSideCooler)
                                        if (this->inBottomCooler != orig.inBottomCooler)
                                            if (this->maxCleanTime != orig.maxCleanTime)
                                                if (this->maxFilterTime != orig.maxFilterTime)
                                                    if (this->maxFanSpeed != orig.maxFanSpeed)
                                                        if (this->maxTemperature != orig.maxTemperature)
                                                            if (this->cleanTime != orig.cleanTime)
                                                                if (this->filterTime != orig.filterTime)
                                                                    if (this->cleanTimeIsRunning != orig.cleanTimeIsRunning)
                                                                        if (this->filterTimeIsRunning != orig.filterTimeIsRunning)
                                                                            if (this->compIsPaused != orig.compIsPaused)
                                                                                if (this->dateOfFirstStart != orig.dateOfFirstStart)
                                                                                    if (this->lastMaxTime != orig.lastMaxTime)
                                                                                        if (this->usageTime != orig.usageTime)
                                                                                            return true;
                                                                                        else
                                                                                            return false;
                                                                                    else
                                                                                        return false;
                                                                                else
                                                                                    return false;
                                                                            else
                                                                                return false;
                                                                        else
                                                                            return false;
                                                                    else
                                                                        return false;
                                                                else
                                                                    return false;
                                                            else
                                                                return false;
                                                        else
                                                            return false;
                                                    else
                                                        return false;
                                                else
                                                    return false;
                                            else
                                                return false;
                                        else
                                            return false;
                                    else
                                        return false;
                                else
                                    return false;
                            else
                                return false;
                        else
                            return false;
                    else
                        return false;
                else
                    return false;
            else
                return false;
        else
            return false;
    else
        return false;
}

void Computer::setId(int value)
{
    this->id = value;
}

int Computer::getId() const
{
    return id;
}

void Computer::setIpAddress(QString newIpAddress)
{
    this->ipAddress = newIpAddress;
}

QString Computer::getIpAddress() const
{
    return this->ipAddress;
}

void Computer::setName(QString newName)
{
    this->name = newName;
}

QString Computer::getName() const
{
    return this->name;
}

void Computer::setOsName(QString newOsName)
{
    this->osName = newOsName;
}

QString Computer::getOsName() const
{
    return this->osName;
}

void Computer::setPlaceType(unsigned short newPlaceType)
{
    if (newPlaceType == 1              // home
        || newPlaceType == 2           // office
        || newPlaceType == 3)          // factory
        this->placeType = newPlaceType;
    else
        this->placeType = 1;
}

unsigned short Computer::getPlaceType() const
{
    return this->placeType;
}

void Computer::setCleanTime(QDateTime newCleanTime)
{
    this->cleanTime = newCleanTime;
}

QDateTime Computer::getCleanTime() const
{
    return this->cleanTime;
}

void Computer::setFilterTime(QDateTime newFilterTime)
{
    this->filterTime = newFilterTime;
}

QDateTime Computer::getFilterTime() const
{
    return this->filterTime;
}

void Computer::setCleanTimeIsRunning(bool newCleanTimeIsRunning)
{
    this->cleanTimeIsRunning = newCleanTimeIsRunning;
}

bool Computer::getCleanTimeIsRunning() const
{
    return this->cleanTimeIsRunning;
}

void Computer::setFilterTimeIsRunning(bool newFilterTimeIsRunning)
{
    this->filterTimeIsRunning = newFilterTimeIsRunning;
}

bool Computer::getFilterTimeIsRunning() const
{
    return this->filterTimeIsRunning;
}

void Computer::setMaxCleanTime(int newMaxCleanTime)
{
    this->maxCleanTime = newMaxCleanTime;
}

int Computer::getMaxCleanTime() const
{
    return this->maxCleanTime;
}

void Computer::setMaxFilterTime(int newMaxFilterTime)
{
    this->maxFilterTime = newMaxFilterTime;
}

int Computer::getMaxFilterTime() const
{
    return this->maxFilterTime;
}

void Computer::setOutLeftCoolerDiameter(unsigned short newDiameter)
{
    outLeftCooler.setDiameter(&newDiameter);
}

unsigned short Computer::getOutLeftCoolerDiameter() const
{
    return outLeftCooler.getDiameter();
}

void Computer::setOutTopCoolerDiameter(unsigned short newDiameter)
{
    outTopCooler.setDiameter(&newDiameter);
}

unsigned short Computer::getOutTopCoolerDiameter() const
{
    return outTopCooler.getDiameter();
}

void Computer::setInRightCoolerDiameter(unsigned short newDiameter)
{
    inRightCooler.setDiameter(&newDiameter);
}

unsigned short Computer::getInRightCoolerDiameter() const
{
    return inRightCooler.getDiameter();
}

void Computer::setInBottomCoolerDiameter(unsigned short newDiameter)
{
    inBottomCooler.setDiameter(&newDiameter);
}

unsigned short Computer::getInBottomCoolerDiameter() const
{
    return inBottomCooler.getDiameter();
}

void Computer::setInSideCoolerDiameter(unsigned short newDiameter)
{
    inSideCooler.setDiameter(&newDiameter);
}

unsigned short Computer::getInSideCoolerDiameter() const
{
    return inSideCooler.getDiameter();
}

void Computer::setOutLeftCoolerAmount(unsigned short newAmount)
{
    outLeftCooler.setAmount(&newAmount);
}

unsigned short Computer::getOutLeftCoolerAmount() const
{
    return outLeftCooler.getAmount();
}

void Computer::setOutTopCoolerAmount(unsigned short newAmount)
{
    outTopCooler.setAmount(&newAmount);
}

unsigned short Computer::getOutTopCoolerAmount() const
{
    return outTopCooler.getAmount();
}

void Computer::setInRightCoolerAmount(unsigned short newAmount)
{
    inRightCooler.setAmount(&newAmount);
}

unsigned short Computer::getInRightCoolerAmount() const
{
    return inRightCooler.getAmount();
}

void Computer::setInBottomCoolerAmount(unsigned short newAmount)
{
    inBottomCooler.setAmount(&newAmount);
}

unsigned short Computer::getInBottomCoolerAmount() const
{
    return inBottomCooler.getAmount();
}

void Computer::setInSideCoolerAmount(unsigned short newAmount)
{
    inSideCooler.setAmount(&newAmount);
}

unsigned short Computer::getInSideCoolerAmount() const
{
    return inSideCooler.getAmount();
}

void Computer::setMaxTemperature(unsigned short newMaxTemperature)
{
    this->maxTemperature = newMaxTemperature;
}

unsigned short Computer::getMaxTemperature() const
{
    return this->maxTemperature;
}

void Computer::setUsageTime(unsigned long newUsageTime)
{
    this->usageTime = newUsageTime;
}

unsigned long Computer::getUsageTime() const
{
    return this->usageTime;
}

void Computer::setMaxFanSpeed(unsigned int newMaxFanSpeed)
{
    this->maxFanSpeed = newMaxFanSpeed;
}

unsigned int Computer::getMaxFanSpeed() const
{
    return this->maxFanSpeed;
}

void Computer::setCompIsPaused(bool newCompIsPaused)
{
    this->compIsPaused = newCompIsPaused;
}

bool Computer::getCompIsPaused() const
{
    return this->compIsPaused;
}

void Computer::setDateOfFirstStart(QDateTime newDateOfFirstStart)
{
    this->dateOfFirstStart = newDateOfFirstStart;
}

QDateTime Computer::getDateOfFirstStart() const
{
    return this->dateOfFirstStart;
}

void Computer::setLastMaxTime(QString newLastMaxTime)
{
    this->lastMaxTime = newLastMaxTime;
}

QString Computer::getLastMaxTime() const
{
    return this->lastMaxTime;
}
