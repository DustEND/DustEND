#include "computerlogic.h"

#include "computer.h"
#include "predicates.h"
#include "computergateway.h"

#include <QString>
#include <QDateTime>
#include <QtWidgets/QApplication>
#include <QSettings>
#include <QtNetwork/QtNetwork>
#include <algorithm>
#include <cmath>

#include <QDebug>


ComputerLogic::ComputerLogic()
{
    cleanWarningText = "";
    filterWarningText = "";
    warningText = "";
    lastWarningText = "";
    warningTime = 600;
    compG = new ComputerGateway();
}

ComputerLogic::~ComputerLogic()
{
    qDebug() << "compLogic destructor 1";
}

void ComputerLogic::addDefaultComputer()
{
    QString name, osName, lastMaxTime, curIp;
    unsigned short placeType, outLeftCoolerAmount, outLeftCoolerDiameter,
    outTopCoolerAmount, outTopCoolerDiameter, inRightCoolerAmount,
    inRightCoolerDiameter, inBottomCoolerAmount, inBottomCoolerDiameter,
    inSideCoolerAmount, inSideCoolerDiameter, maxTemperature;
    int maxCleanTime, maxFilterTime;
    unsigned int maxFanSpeed;
    unsigned long usageTime;
    bool cleanTimeIsRunning, filterTimeIsRunning, compIsPaused;
    QDateTime cleanTime, filterTime, dateOfFirstStart;

    name = "Your computer";
    curIp = "127.0.0.1";
    osName = "Windows XP";
    placeType = 1;
    outLeftCoolerAmount = 0;
    outTopCoolerAmount = 0;
    inRightCoolerAmount = 0;
    inBottomCoolerAmount = 0;
    inSideCoolerAmount = 0;
    outLeftCoolerDiameter = 120;
    outTopCoolerDiameter = 120;
    inRightCoolerDiameter = 120;
    inBottomCoolerDiameter = 120;
    inSideCoolerDiameter = 120;
    maxCleanTime = 4680000;
    lastMaxTime = "1300";
    maxFilterTime = 9360000;
    maxFanSpeed = 2000;
    usageTime = 0;
    maxTemperature = 70;
    cleanTimeIsRunning = false;
    filterTimeIsRunning = false;
    cleanTime = QDateTime::currentDateTime().addSecs(4680000);
    filterTime = QDateTime::currentDateTime().addSecs(9360000);
    compIsPaused = false;
    dateOfFirstStart = QDateTime::fromString("24.10.1795 12:12:12",
                                             "dd.MM.yyyy hh:mm:ss");

    addComputer(&allComps, &curIp, &name, &placeType, &cleanTime, &filterTime,
                &cleanTimeIsRunning, &filterTimeIsRunning, &maxCleanTime,
                &maxFilterTime, &outLeftCoolerAmount, &outLeftCoolerDiameter,
                &outTopCoolerAmount, &outTopCoolerDiameter, &inRightCoolerAmount,
                &inRightCoolerDiameter, &inBottomCoolerAmount, &inBottomCoolerDiameter,
                &inSideCoolerAmount, &inSideCoolerDiameter, &maxTemperature,
                &usageTime, &maxFanSpeed, &osName, &compIsPaused, &dateOfFirstStart,
                &lastMaxTime);
}

QVector<Computer>::iterator ComputerLogic::findComputer(QString *ipAddress, QVector<Computer> *compList)
{
    bool finded = false;
    QVector<Computer>::iterator compIter;
    for(compIter = compList->begin(); compIter != compList->end(); ++compIter) {
        if (compIter->getIpAddress() == *ipAddress) {
            finded = true;
            break;
        }
    }
    if (!finded && *ipAddress == "127.0.0.1") {
        addDefaultComputer();
        findComputer(ipAddress, &allComps);
    }
    return compIter;
}

void ComputerLogic::loadSettings()
{
    loadCompList();
}

void ComputerLogic::addComputer(QVector<Computer> *compList,
                                QString *ipAddress, QString *name, unsigned short *placeType,
                                QDateTime *cleanTime, QDateTime *filterTime,
                                bool *cleanTimeIsRunning, bool *filterTimeIsRunning,
                                int *maxCleanTime, int *maxFilterTime,
                                unsigned short *outLeftCoolerAmount,
                                unsigned short *outLeftCoolerDiameter,
                                unsigned short *outTopCoolerAmount,
                                unsigned short *outTopCoolerDiameter,
                                unsigned short *inRightCoolerAmount,
                                unsigned short *inRightCoolerDiameter,
                                unsigned short *inBottomCoolerAmount,
                                unsigned short *inBottomCoolerDiameter,
                                unsigned short *inSideCoolerAmount,
                                unsigned short *inSideCoolerDiameter,
                                unsigned short *maxTemperature, unsigned long *usageTime,
                                unsigned int *maxFanSpeed, QString *osName, bool *compIsPaused,
                                QDateTime *dateOfFirstStart, QString *lastMaxTime)
{    
    if (!computerWasAdded(ipAddress, compList)) {
        Computer newComp;
        newComp.setId(generateId());
        newComp.setIpAddress(*ipAddress);        
        newComp.setName(*name);
        newComp.setPlaceType(*placeType);
        newComp.setCleanTime(*cleanTime);
        newComp.setFilterTime(*filterTime);
        newComp.setCleanTimeIsRunning(*cleanTimeIsRunning);
        newComp.setFilterTimeIsRunning(*filterTimeIsRunning);
        newComp.setMaxCleanTime(*maxCleanTime);
        newComp.setMaxFilterTime(*maxFilterTime);
        newComp.setOutLeftCoolerAmount(*outLeftCoolerAmount);
        newComp.setOutLeftCoolerDiameter(*outLeftCoolerDiameter);
        newComp.setOutTopCoolerAmount(*outTopCoolerAmount);
        newComp.setOutTopCoolerDiameter(*outTopCoolerDiameter);
        newComp.setInRightCoolerAmount(*inRightCoolerAmount);
        newComp.setInRightCoolerDiameter(*inRightCoolerDiameter);
        newComp.setInBottomCoolerAmount(*inBottomCoolerAmount);
        newComp.setInBottomCoolerDiameter(*inBottomCoolerDiameter);
        newComp.setInSideCoolerAmount(*inSideCoolerAmount);
        newComp.setInSideCoolerDiameter(*inSideCoolerDiameter);
        newComp.setMaxTemperature(*maxTemperature);
        newComp.setUsageTime(*usageTime);
        newComp.setMaxFanSpeed(*maxFanSpeed);
        newComp.setOsName(*osName);
        newComp.setCompIsPaused(*compIsPaused);
        newComp.setDateOfFirstStart(*dateOfFirstStart);
        newComp.setLastMaxTime(*lastMaxTime);
        compList->push_back(newComp);
    }
}

//QString ComputerLogic::getName(QString *ipAddress)
//{
//    return findComputer(ipAddress, &allComps)->getName();
//}

QString ComputerLogic::getOsName(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getOsName();
}

void ComputerLogic::setOsName(QString *ipAddress, QString *newOsName)
{
    if (computerWasAdded(ipAddress, &allComps))
        findComputer(ipAddress, &allComps)->setOsName(*newOsName);
}

QString ComputerLogic::getCleanTimeText(QString *ipAddress)
{
    QVector<Computer>::iterator compIter = findComputer(ipAddress, &allComps);
    int cleanTime = compIter->getMaxCleanTime();
    return QString("%1").arg(time2Text(&cleanTime));
}

QString ComputerLogic::getFilterTimeText(QString *ipAddress)
{
    QVector<Computer>::iterator compIter = findComputer(ipAddress, &allComps);
    int filterTime = compIter->getMaxFilterTime();
    return QString("%1").arg(time2Text(&filterTime));
}

QString ComputerLogic::time2Text(int *time)
{
    int hours, mins, secs;
    QString color = "green";
    secs = *time % 3600 % 60;
    mins = *time % 3600 / 60;
    hours = *time / 3600;

    if (hours < 5)
        color = "yellow";
    if (hours < 1)
        color = "red";

    if (hours < 0) {
        mins *= -1;
        secs *= -1;
    }
    if (hours == 0 && mins < 0)
        secs *= -1;
    return QString("<font color=\"%4\">%1:%2:%3</font>").arg(hours)
                   .arg(mins).arg(secs).arg(color);
}

void ComputerLogic::setMaxAndCurCleanTime(QString *ipAddress)
{
    QVector<Computer>::iterator compIter = findComputer(ipAddress, &allComps);
    int time;
    QDateTime curDate = QDateTime::currentDateTime();
    if (compIter->getPlaceType() == 1)
        time = 4680000;                          // 1300 * 3600
    else {
        if (compIter->getPlaceType() == 2)
            time = 3960000;                      // 1100 * 3600
        else
            time = 3240000;                      // 900 * 3600
    }
    compIter->setMaxCleanTime(time);
    compIter->setCleanTime(curDate);
    curDate = compIter->getCleanTime().addSecs(compIter->getMaxCleanTime());
    compIter->setCleanTime(curDate);
}

void ComputerLogic::timerTick()
{
    QVector<Computer>::iterator compIter;
    QString curIpAddress;
    int tempTime;
    unsigned long tempUsageTime;
//    bool run = false;
    cleanWarningText = "";
    filterWarningText = "";
    warningText = "";      
    for (compIter = allComps.begin(); compIter != allComps.end(); ++compIter) {
        curIpAddress = compIter->getIpAddress();        
        tempUsageTime = compIter->getUsageTime() + 1;
        compIter->setUsageTime(tempUsageTime);
        if (compIter->getCleanTimeIsRunning()
                && !compIter->getCompIsPaused()) {
            tempTime = compIter->getMaxCleanTime() - 1;
            compIter->setMaxCleanTime(tempTime);
            if (compIter->getMaxCleanTime() <= 0)
                setCleanWarningText(&curIpAddress);                        
//            if (!compIter->getMaxCleanTime())
//                compIter->setCleanTimeIsRunning(&run);
        }
        if (compIter->getFilterTimeIsRunning()
                && !compIter->getCompIsPaused()) {
            tempTime = compIter->getMaxFilterTime() - 1;            
            compIter->setMaxFilterTime(tempTime);
            if (compIter->getMaxFilterTime() <= 0)
                setFilterWarningText(&curIpAddress);
//            if (!compIter->getMaxFilterTime())
//                compIter->setFilterTimeIsRunning(&run);
        }
//        calculateUsageTime(&curIpAddress);
    }
    makeWarningText();
}

unsigned short ComputerLogic::getOutLeftCoolerAmount(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getOutLeftCoolerAmount();
}

unsigned short ComputerLogic::getOutTopCoolerAmount(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getOutTopCoolerAmount();
}

unsigned short ComputerLogic::getInRightCoolerAmount(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getInRightCoolerAmount();
}

unsigned short ComputerLogic::getInBottomCoolerAmount(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getInBottomCoolerAmount();
}

unsigned short ComputerLogic::getInSideCoolerAmount(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getInSideCoolerAmount();
}

unsigned short ComputerLogic::getOutLeftCoolerDiameter(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getOutLeftCoolerDiameter();
}

unsigned short ComputerLogic::getOutTopCoolerDiameter(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getOutTopCoolerDiameter();
}

unsigned short ComputerLogic::getInRightCoolerDiameter(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getInRightCoolerDiameter();
}

unsigned short ComputerLogic::getInBottomCoolerDiameter(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getInBottomCoolerDiameter();
}

unsigned short ComputerLogic::getInSideCoolerDiameter(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getInSideCoolerDiameter();
}

void ComputerLogic::setMaxAndCurFilterTime(QString *ipAddress)
{
    QVector<Computer>::iterator compIter = findComputer(ipAddress, &allComps);
    int time;
    QDateTime curDate = QDateTime::currentDateTime();
    if (compIter->getPlaceType() == 1)
        time = 9360000;                          // 2600 * 3600
    else {
        if (compIter->getPlaceType() == 2)
            time = 7920000;                      // 2200 * 3600
        else
            time = 6480000;                      // 1800 * 3600
    }
    compIter->setMaxFilterTime(time);
    compIter->setFilterTime(curDate);
    curDate = compIter->getFilterTime().addSecs(compIter->getMaxFilterTime());   
    compIter->setFilterTime(curDate);
}

void ComputerLogic::correctCleanTime(QString *ipAddress)
{
    setMaxAndCurCleanTime(ipAddress);
    QVector<Computer>::iterator compIter = findComputer(ipAddress, &allComps);
    int tempTime = compIter->getMaxCleanTime();
    unsigned short outLeftCoolerDiameter = compIter->getOutLeftCoolerDiameter();
    unsigned short outTopCoolerDiameter = compIter->getOutTopCoolerDiameter();
    unsigned short inRightCoolerDiameter = compIter->getInRightCoolerDiameter();
    unsigned short inBottomCoolerDiameter = compIter->getInBottomCoolerDiameter();
    unsigned short inSideCoolerDiameter = compIter->getInSideCoolerDiameter();
    unsigned short outLeftCoolerAmount = compIter->getOutLeftCoolerAmount();
    unsigned short outTopCoolerAmount = compIter->getOutTopCoolerAmount();
    unsigned short inRightCoolerAmount = compIter->getInRightCoolerAmount();
    unsigned short inBottomCoolerAmount = compIter->getInBottomCoolerAmount();
    unsigned short inSideCoolerAmount = compIter->getInSideCoolerAmount();
    unsigned int time2Del;    
    time2Del = 0;
    if (outLeftCoolerAmount > 0) {
        if (outLeftCoolerDiameter == 80)
            time2Del = 72000;                                   // 20 * 3600
        if (outLeftCoolerDiameter == 120)
            time2Del = 108000;                                  // 30 * 3600
        if (outLeftCoolerDiameter == 140)
            time2Del = 144000;                                  // 40 * 3600
        tempTime -= (time2Del * outLeftCoolerAmount);
    }

    if (outTopCoolerAmount > 0) {
        if (outTopCoolerDiameter == 80)
            time2Del = 72000;
        if (outTopCoolerDiameter == 120)
            time2Del = 108000;
        if (outTopCoolerDiameter == 140)
            time2Del = 144000;
        if (outTopCoolerDiameter == 200)
            time2Del = 180000;                                  // 50 * 3600
        if (outTopCoolerDiameter == 230)
            time2Del = 216000;                                  // 60 * 3600
        tempTime -= (time2Del * outTopCoolerAmount);
    }

    if (inRightCoolerAmount > 0) {
        if (inRightCoolerDiameter == 80)
            time2Del = 72000;
        if (inRightCoolerDiameter == 120)
            time2Del = 108000;
        if (inRightCoolerDiameter == 140)
            time2Del = 144000;
        if (inRightCoolerDiameter == 200)
            time2Del = 180000;
        if (inRightCoolerDiameter == 230)
            time2Del = 216000;
        tempTime -= (time2Del * inRightCoolerAmount);
    }

    if (inBottomCoolerAmount > 0) {
        if (inBottomCoolerDiameter == 80)
            time2Del = 72000;
        if (inBottomCoolerDiameter == 120)
            time2Del = 108000;
        if (inBottomCoolerDiameter == 140)
            time2Del = 144000;
        tempTime -= (time2Del * inBottomCoolerAmount);
    }

    if (inSideCoolerAmount > 0) {
        if (inSideCoolerDiameter == 80)
            time2Del = 72000;
        if (inSideCoolerDiameter == 120)
            time2Del = 108000;
        if (inSideCoolerDiameter == 140)
            time2Del = 144000;
        if (inSideCoolerDiameter == 200)
            time2Del = 180000;
        if (inSideCoolerDiameter == 230)
            time2Del = 216000;
        tempTime -= (time2Del * inSideCoolerAmount);
    }
    compIter->setMaxCleanTime(tempTime);

    QDateTime curDate = QDateTime::currentDateTime();
    compIter->setCleanTime(curDate);
    curDate = compIter->getCleanTime().addSecs(compIter->getMaxCleanTime());
    compIter->setCleanTime(curDate);
}

void ComputerLogic::correctFilterTime(QString *ipAddress)
{
    setMaxAndCurFilterTime(ipAddress);
    QVector<Computer>::iterator compIter = findComputer(ipAddress, &allComps);
    int tempTime = compIter->getMaxFilterTime();
    unsigned short outLeftCoolerDiameter = compIter->getOutLeftCoolerDiameter();
    unsigned short outTopCoolerDiameter = compIter->getOutTopCoolerDiameter();
    unsigned short outLeftCoolerAmount = compIter->getOutLeftCoolerAmount();
    unsigned short outTopCoolerAmount = compIter->getOutTopCoolerAmount();
    unsigned short inRightCoolerAmount = compIter->getInRightCoolerAmount();
    unsigned short inBottomCoolerAmount = compIter->getInBottomCoolerAmount();
    unsigned short inSideCoolerAmount = compIter->getInSideCoolerAmount();
    unsigned int time2Del;    
    time2Del = 0;
    if (inRightCoolerAmount || inBottomCoolerAmount || inSideCoolerAmount)
        tempTime -= 180000;

    if (outLeftCoolerAmount) {
        if (outLeftCoolerDiameter == 80)
            time2Del = 72000;                                   // 20 * 3600
        if (outLeftCoolerDiameter == 120)
            time2Del = 108000;                                  // 30 * 3600
        if (outLeftCoolerDiameter == 140)
            time2Del = 144000;                                  // 40 * 3600
        tempTime -= (time2Del * outLeftCoolerAmount);
    }

    if (outTopCoolerAmount) {
        if (outTopCoolerDiameter == 80)
            time2Del = 72000;
        if (outTopCoolerDiameter == 120)
            time2Del = 108000;
        if (outTopCoolerDiameter == 140)
            time2Del = 144000;
        if (outTopCoolerDiameter == 200)
            time2Del = 180000;                                  // 50 * 3600
        if (outTopCoolerDiameter == 230)
            time2Del = 216000;                                  // 60 * 3600
        tempTime -= (time2Del * outTopCoolerAmount);
    }
    compIter->setMaxFilterTime(tempTime);

    QDateTime curDate = QDateTime::currentDateTime();
    compIter->setFilterTime(curDate);
    curDate = compIter->getFilterTime().addSecs(compIter->getMaxFilterTime());
    compIter->setFilterTime(curDate);
}

void ComputerLogic::calculateUsageTime(QString *ipAddress)
{
    QVector<Computer>::iterator compIter = findComputer(ipAddress, &allComps);
    if (compIter->getCleanTimeIsRunning()
            || compIter->getFilterTimeIsRunning()) {
        unsigned long curUsageTime = compIter->getUsageTime();
        ++curUsageTime;
        compIter->setUsageTime(curUsageTime);
    }
}

QString ComputerLogic::getUsageTimeText(QString *ipAddress)
{
    unsigned long time = findComputer(ipAddress, &allComps)->getUsageTime();

    int hours, mins, days, secs;
    secs = time % 86400 % 3600 % 60;
    mins = time % 86400 % 3600 / 60;
    hours = time % 86400 / 3600;
    days = time / 86400;
    return QString("%1 %5 %2:%3:%4").arg(days).arg(hours).arg(mins).arg(secs)
            .arg(dayDot);
}

bool ComputerLogic::getCleanTimeIsRunning(QString *ipAddress)
{    
    if (computerWasAdded(ipAddress, &allComps))
        return findComputer(ipAddress, &allComps)->getCleanTimeIsRunning();
    else
        return false;
}

bool ComputerLogic::getFilterTimeIsRunning(QString *ipAddress)
{
    if (computerWasAdded(ipAddress, &allComps))
        return findComputer(ipAddress, &allComps)->getFilterTimeIsRunning();
    else
        return false;
}

unsigned short ComputerLogic::getPlaceType(QString *ipAddress)
{    
    return findComputer(ipAddress, &allComps)->getPlaceType();
}

void ComputerLogic::setOutLeftCoolerAmount(QString *ipAddress, unsigned short *amount)
{
    findComputer(ipAddress, &allComps)->setOutLeftCoolerAmount(*amount);
}

void ComputerLogic::setOutTopCoolerAmount(QString *ipAddress, unsigned short *amount)
{
    findComputer(ipAddress, &allComps)->setOutTopCoolerAmount(*amount);
}

void ComputerLogic::setInRightCoolerAmount(QString *ipAddress, unsigned short *amount)
{
    findComputer(ipAddress, &allComps)->setInRightCoolerAmount(*amount);
}

void ComputerLogic::setInBottomCoolerAmount(QString *ipAddress, unsigned short *amount)
{
    findComputer(ipAddress, &allComps)->setInBottomCoolerAmount(*amount);
}

void ComputerLogic::setInSideCoolerAmount(QString *ipAddress, unsigned short *amount)
{
    findComputer(ipAddress, &allComps)->setInSideCoolerAmount(*amount);
}

void ComputerLogic::setOutLeftCoolerDiameter(QString *ipAddress, unsigned short *diameter)
{
    findComputer(ipAddress, &allComps)->setOutLeftCoolerDiameter(*diameter);
}

void ComputerLogic::setOutTopCoolerDiameter(QString *ipAddress, unsigned short *diameter)
{
    findComputer(ipAddress, &allComps)->setOutTopCoolerDiameter(*diameter);
}

void ComputerLogic::setInRightCoolerDiameter(QString *ipAddress, unsigned short *diameter)
{
    findComputer(ipAddress, &allComps)->setInRightCoolerDiameter(*diameter);
}

void ComputerLogic::setInBottomCoolerDiameter(QString *ipAddress, unsigned short *diameter)
{
    findComputer(ipAddress, &allComps)->setInBottomCoolerDiameter(*diameter);
}

void ComputerLogic::setInSideCoolerDiameter(QString *ipAddress, unsigned short *diameter)
{
    findComputer(ipAddress, &allComps)->setInSideCoolerDiameter(*diameter);
}

void ComputerLogic::setCleanTimeIsRunning(QString *ipAddress, bool *value)
{
    findComputer(ipAddress, &allComps)->setCleanTimeIsRunning(*value);
}

void ComputerLogic::setFilterTimeIsRunning(QString *ipAddress, bool *value)
{
    findComputer(ipAddress, &allComps)->setFilterTimeIsRunning(*value);
}

void ComputerLogic::setPlaceType(QString *ipAddress, unsigned short *placeType/*, unsigned short *cleanOrFilter*/)
{
    QVector<Computer>::iterator curComp = findComputer(ipAddress, &allComps);
    if (*placeType == 1 || *placeType == 2 || *placeType == 3 )
        curComp->setPlaceType(*placeType);

    int cleanTime2Add = 0;
    int filterTime2Add = 0;

    if (*placeType == 3) {
        filterTime2Add = 800 * 3600;
        cleanTime2Add = 400 * 3600;
    }
    if (*placeType == 1) {
        filterTime2Add = -800 * 3600;
        cleanTime2Add = -400 * 3600;
    }
    int curTime = curComp->getMaxFilterTime();
    curTime -= filterTime2Add;
    curComp->setMaxFilterTime(curTime);

    QDateTime curDate = QDateTime::currentDateTime();
    curComp->setFilterTime(curDate);
    curDate = curComp->getFilterTime().addSecs(curComp->getMaxFilterTime());
    curComp->setFilterTime(curDate);

    curTime = curComp->getMaxCleanTime();
    curTime -= cleanTime2Add;
    curComp->setMaxCleanTime(curTime);

    curDate = QDateTime::currentDateTime();
    curComp->setCleanTime(curDate);
    curDate = curComp->getCleanTime().addSecs(curComp->getMaxCleanTime());
    curComp->setCleanTime(curDate);
}

QString ComputerLogic::getShortCleanTime(QString *ipAddress, bool *withAverageUsageTime)
{
    if (computerWasAdded(ipAddress, &allComps)) {

        int time = findComputer(ipAddress, &allComps)->getMaxCleanTime();
        int hours, mins, secs;
        QString hoursStr, minsStr, secsStr;
        secs = time  % 3600 % 60;
        mins = time % 3600 / 60;
        hours = time / 3600;

        if (hours < 0) {
            mins *= -1;
            secs *= -1;
        }
        if (hours == 0 && mins < 0)
            secs *= -1;
        secsStr = QString("%1").arg(secs);
        if (secs < 10 && secs > 0)
            secsStr =  QString("0%1").arg(secsStr);
        minsStr = QString("%1").arg(mins);
        if (mins < 10 && mins > 0)
            minsStr =  QString("0%1").arg(minsStr);
        hoursStr = QString("%1").arg(hours);
        if (hours < 10 && hours > 0)
            hoursStr =  QString("000%1").arg(hoursStr);
        if (hours < 100 && hours > 0)
            hoursStr =  QString("00%1").arg(hoursStr);
        if (hours < 1000 && hours > 0)
            hoursStr =  QString("0%1").arg(hoursStr);
        if (*withAverageUsageTime) {
            QDateTime curDateOfFirstStart = findComputer(ipAddress, &allComps)->getDateOfFirstStart();            
            unsigned long curUsageTime = findComputer(ipAddress, &allComps)->getUsageTime();
            return QString("%1:%2:%3 / %4").arg(hoursStr).arg(minsStr).arg(secsStr)
                    .arg(getAverageUsageTimeText(&curDateOfFirstStart, &curUsageTime));
        }
        else
            return QString("%1:%2:%3").arg(hoursStr).arg(minsStr).arg(secsStr);
    }
    else
        return QString("");
}

QString ComputerLogic::getShortFilterTime(QString *ipAddress, bool *withAverageUsageTime)
{
    if (computerWasAdded(ipAddress, &allComps)) {
        int time = findComputer(ipAddress, &allComps)->getMaxFilterTime();
        int hours, mins, secs;
        QString hoursStr, minsStr, secsStr;
        secs = time  % 3600 % 60;
        mins = time % 3600 / 60;
        hours = time / 3600;

        if (hours < 0) {
            mins *= -1;
            secs *= -1;
        }
        if (hours == 0 && mins < 0)
            secs *= -1;
        secsStr = QString("%1").arg(secs);
        if (secs < 10)
            secsStr =  QString("0%1").arg(secsStr);
        minsStr = QString("%1").arg(mins);
        if (mins < 10)
            minsStr =  QString("0%1").arg(minsStr);
        hoursStr = QString("%1").arg(hours);
        if (hours < 10)
            hoursStr =  QString("000%1").arg(hoursStr);
        if (hours < 100)
            hoursStr =  QString("00%1").arg(hoursStr);
        if (hours < 1000)
            hoursStr =  QString("0%1").arg(hoursStr);
        if (*withAverageUsageTime) {
            QDateTime curDateOfFirstStart = findComputer(ipAddress, &allComps)->getDateOfFirstStart();
            unsigned long curUsageTime = findComputer(ipAddress, &allComps)->getUsageTime();
            return QString("%1:%2:%3 / %4").arg(hoursStr).arg(minsStr).arg(secsStr)
                    .arg(getAverageUsageTimeText(&curDateOfFirstStart, &curUsageTime));
        }
        else
            return QString("%1:%2:%3").arg(hoursStr).arg(minsStr).arg(secsStr);
    }
    else
        return QString("");
}

unsigned int ComputerLogic::getMaxCleanTime(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getMaxCleanTime();
}

unsigned int ComputerLogic::getMaxFilterTime(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getMaxFilterTime();
}

QString ComputerLogic::getWarningMessage()
{
    return warningText;
}

void ComputerLogic::setCleanWarningText(QString *ipAddress)
{
    if (!cleanWarningText.length())
        cleanWarningText = timeToClean + " ";
    if (*ipAddress == QApplication::tr("127.0.0.1"))
        cleanWarningText += yourComp;
    else
        cleanWarningText += QString(QApplication::tr("\n ip=%1")).arg(*ipAddress);
}

void ComputerLogic::setFilterWarningText(QString *ipAddress)
{
    if (!filterWarningText.length())
        filterWarningText = filterCloged + " ";
    if (*ipAddress == QApplication::tr("127.0.0.1"))
        filterWarningText += yourComp;
    else
        filterWarningText += QString(QApplication::tr("\n ip=%1")).arg(*ipAddress);
}

void ComputerLogic::makeWarningText()
{
    if (cleanWarningText == "" && filterWarningText == "")
        warningText = "";
    else {
        warningText = cleanWarningText + "\n" + filterWarningText;
        if (cleanWarningText == "")
            warningText = filterWarningText;
        if (filterWarningText == "")
            warningText = cleanWarningText;
    }
}

double ComputerLogic::calcAverageUsageTime(QDateTime *dateOfFirstStart, unsigned long *usageTime)
{    
    QDateTime d1 = *dateOfFirstStart;
    d1.setUtcOffset(0);
//    int daysAfterStart = d1.daysTo(QDateTime::currentDateTimeUtc()) + 1;
//    double curAverUsageTime = (double)*usageTime / 3600 / (double)daysAfterStart;
    int daysAfterStart = d1.secsTo(QDateTime::currentDateTimeUtc()) + 1;
    double curAverUsageTime = (double)*usageTime / (double)daysAfterStart;
    curAverUsageTime = roundDouble(curAverUsageTime, 4);
    return curAverUsageTime;
}

QString ComputerLogic::getAverageUsageTimeText(QDateTime *dateOfFirstStart, unsigned long *usageTime)
{
    double averUsTime = calcAverageUsageTime(dateOfFirstStart, usageTime);
    double intPart, realPart;    
    realPart = modf(averUsTime, &intPart);
    QString timeText = QString("%1").arg(intPart);
    int mins = 60 * realPart;
    timeText = QString("%1:%2").arg(timeText).arg(mins);   
    return timeText;
}

int ComputerLogic::generateId()
{
    QVector< Computer >::iterator compI;
    int id = 0;
    for (compI = allComps.begin(); compI != allComps.end(); ++compI) {
        if (compI->getId() > id)
            id = compI->getId();
    }
    return ++id;
}

bool ComputerLogic::computerWasAdded(QString *ipAddress, QVector<Computer> *compList)
{
    QVector<Computer>::iterator compIter;
    bool here = false;
    for(compIter = compList->begin(); compIter != compList->end(); ++compIter)
        if (compIter->getIpAddress() == *ipAddress) {
            here = true;
            firstCompI = compIter;
            break;
        }
    return here;
}

void ComputerLogic::delNetComp(QString *ipAddress)
{
    if (*ipAddress != QObject::tr("127.0.0.1"))
        if (computerWasAdded(ipAddress, &allComps))
            allComps.erase(std::remove_if(allComps.begin(),
                                          allComps.end(),
                                          ComputersEqualByIp(*ipAddress)),
                           allComps.end());
}

bool ComputerLogic::CompIsPaused(QString *ipAddress)
{
    if (computerWasAdded(ipAddress, &allComps))
        return findComputer(ipAddress, &allComps)->getCompIsPaused();
    else
        return false;
}

void ComputerLogic::setCompPause(QString *ipAddress, bool *pause)
{
    if (computerWasAdded(ipAddress, &allComps))
        findComputer(ipAddress, &allComps)->setCompIsPaused(*pause);
}

void ComputerLogic::addCooler(QString *ipAddress, unsigned short *coolerType, unsigned short *newAmount)
{
    int time2Add, curCleanTime, curFilterTime;
    unsigned short coolersDiameter, oldAmountOfCoolers;
    short amountOfAddedCoolers;
    time2Add = 0;
    curCleanTime = 0;
    curFilterTime = 0;
    coolersDiameter = 0;
    oldAmountOfCoolers = 0;
    amountOfAddedCoolers = 0;
    QVector< Computer >::iterator curComp = findComputer(ipAddress, &allComps);

    if (*coolerType == 1) {       // out left cooler
        oldAmountOfCoolers = curComp->getOutLeftCoolerAmount();
        coolersDiameter = curComp->getOutLeftCoolerDiameter();        
        findComputer(ipAddress, &allComps)->setOutLeftCoolerAmount(*newAmount);
    }
    if (*coolerType == 2) {       // out top cooler
        oldAmountOfCoolers = curComp->getOutTopCoolerAmount();
        coolersDiameter = curComp->getOutTopCoolerDiameter();
        findComputer(ipAddress, &allComps)->setOutTopCoolerAmount(*newAmount);
    }
    if (*coolerType == 3) {       // in right cooler
        oldAmountOfCoolers = curComp->getInRightCoolerAmount();
        coolersDiameter = curComp->getInRightCoolerDiameter();
        findComputer(ipAddress, &allComps)->setInRightCoolerAmount(*newAmount);
    }
    if (*coolerType == 4) {       // in bottom cooler
        oldAmountOfCoolers = curComp->getInBottomCoolerAmount();
        coolersDiameter = curComp->getInBottomCoolerDiameter();
        findComputer(ipAddress, &allComps)->setInBottomCoolerAmount(*newAmount);
    }
    if (*coolerType == 5) {       // in side cooler
        oldAmountOfCoolers = curComp->getInSideCoolerAmount();
        coolersDiameter = curComp->getInSideCoolerDiameter();
        findComputer(ipAddress, &allComps)->setInSideCoolerAmount(*newAmount);
    }

    amountOfAddedCoolers = *newAmount - oldAmountOfCoolers;

    time2Add = amountOfAddedCoolers * getIncrementForCleanTime(&coolersDiameter);

    curCleanTime = curComp->getMaxCleanTime();
    curCleanTime -= time2Add;
    curComp->setMaxCleanTime(curCleanTime);
    time2Add = 0;
    if ((*coolerType == 3 || *coolerType == 4 || *coolerType == 5)
         && (oldAmountOfCoolers != *newAmount)) {
        if (oldAmountOfCoolers == 0)
            time2Add = 180000;        
        else {
            if (*newAmount == 0)
                time2Add = -180000;                
            else
                time2Add = 0;
        }
    }
    else
        time2Add = amountOfAddedCoolers * getIncrementForFilterTime(&coolersDiameter);

    curFilterTime = curComp->getMaxFilterTime();
    curFilterTime -= time2Add;
    curComp->setMaxFilterTime(curFilterTime);


    QDateTime curDate = QDateTime::currentDateTime();
    curComp->setCleanTime(curDate);
    curDate = curComp->getCleanTime().addSecs(curComp->getMaxCleanTime());
    curComp->setCleanTime(curDate);

    curComp->setFilterTime(curDate);
    curDate = curComp->getFilterTime().addSecs(curComp->getMaxFilterTime());
    curComp->setFilterTime(curDate);
}

int ComputerLogic::getCurrentCleanHours(QString *ipAddress)
{
    int hours, time;
    hours = getMaxCleanHours();
    time = 0;

    time = findComputer(ipAddress, &allComps)->getMaxCleanTime();
    hours = time / 3600;

    return hours;
}

int ComputerLogic::getCurrentFilterHours(QString *ipAddress)
{
    int hours, time;
    hours = getMaxFilterHours();
    time = 0;

    time = findComputer(ipAddress, &allComps)->getMaxFilterTime();
    hours = time / 3600;

    return hours;
}

int ComputerLogic::getMaxCleanHours()
{
    return 1300;
}

int ComputerLogic::getMaxFilterHours()
{
    return 2600;
}

QString ComputerLogic::getIpAddress(QString *ipAddress)
{
    if (computerWasAdded(ipAddress, &allComps))
        return findComputer(ipAddress, &allComps)->getIpAddress();
    else
        return QString("");
}

void ComputerLogic::changeCoolerDiameter(QString *ipAddress,
                                         unsigned short *coolerType,
                                         unsigned short *newDiameter)
{
    QVector<Computer>::iterator curComp = findComputer(ipAddress, &allComps);
    unsigned short coolerAmount = 0;
    unsigned short oldCoolerDiameter = 0;
    int oldAddedTime, newAddedTime, time2Add, curCleanTime, curFilterTime;
    oldAddedTime = 0;
    newAddedTime = 0;
    time2Add = 0;
    curFilterTime = 0;

    if (*coolerType == 1) {
        coolerAmount = curComp->getOutLeftCoolerAmount();
        oldCoolerDiameter = curComp->getOutLeftCoolerDiameter();
        setOutLeftCoolerDiameter(ipAddress, newDiameter);
    }
    if (*coolerType == 2) {
        coolerAmount = curComp->getOutTopCoolerAmount();
        oldCoolerDiameter = curComp->getOutTopCoolerDiameter();
        setOutTopCoolerDiameter(ipAddress, newDiameter);
    }
    if (*coolerType == 3) {
        coolerAmount = curComp->getInRightCoolerAmount();
        oldCoolerDiameter = curComp->getInRightCoolerDiameter();
        setInRightCoolerDiameter(ipAddress, newDiameter);
    }
    if (*coolerType == 4) {
        coolerAmount = curComp->getInBottomCoolerAmount();
        oldCoolerDiameter = curComp->getInBottomCoolerDiameter();
        setInBottomCoolerDiameter(ipAddress, newDiameter);
    }
    if (*coolerType == 5) {
        coolerAmount = curComp->getInSideCoolerAmount();
        oldCoolerDiameter = curComp->getInSideCoolerDiameter();
        setInSideCoolerDiameter(ipAddress, newDiameter);
    }

    oldAddedTime = coolerAmount * getIncrementForCleanTime(&oldCoolerDiameter);
    newAddedTime = coolerAmount * getIncrementForCleanTime(newDiameter);
    time2Add = newAddedTime - oldAddedTime;

    curCleanTime = curComp->getMaxCleanTime();
    curCleanTime -= time2Add;
    curComp->setMaxCleanTime(curCleanTime);

    oldAddedTime = coolerAmount * getIncrementForFilterTime(&oldCoolerDiameter);
    newAddedTime = coolerAmount * getIncrementForFilterTime(newDiameter);
    time2Add = newAddedTime - oldAddedTime;

    if (*coolerType == 3 || *coolerType == 4 || *coolerType == 5)
        time2Add = 0;

    curFilterTime = curComp->getMaxFilterTime();
    curFilterTime -= time2Add;
    curComp->setMaxFilterTime(curFilterTime);
}

int ComputerLogic::getIncrementForCleanTime(unsigned short *coolerDiameter)
{
    if (*coolerDiameter == 80)
        return 72000;
    else
        if (*coolerDiameter == 120)
            return 108000;
        else
            if (*coolerDiameter == 140)
                return 144000;
            else
                if (*coolerDiameter == 200)
                    return 180000;
                else
                    if (*coolerDiameter == 230)
                        return 216000;
                    else
                        return 0;
}

int ComputerLogic::getIncrementForFilterTime(unsigned short *coolerDiameter)
{
    if (*coolerDiameter == 80)
        return 0;
    else
        if (*coolerDiameter == 120)
            return 10 * 3600;
        else
            if (*coolerDiameter == 140)
                return 20 * 3600;
            else
                if (*coolerDiameter == 200)
                    return 30 * 3600;
                else
                    if (*coolerDiameter == 230)
                        return 40 * 3600;
                    else
                        return 0;
}

void ComputerLogic::setDateOfFirstStart(QDateTime *firstStartTime, QString *ipAddress)
{
    QDateTime dateOf1Start = QDateTime::fromString(findComputer(ipAddress, &allComps)->getDateOfFirstStart().toString("dd.MM.yyyy hh:mm:ss"),
                                         "dd.MM.yyyy hh:mm:ss");
    if (dateOf1Start == QDateTime::fromString("24.10.1795 12:12:12", "dd.MM.yyyy hh:mm:ss")
            || dateOf1Start == QDateTime::fromString("")) {
        findComputer(ipAddress, &allComps)->setDateOfFirstStart(*firstStartTime);
    }
}

QDateTime ComputerLogic::getDateOfFirstStart(QString *ipAddress)
{
    return findComputer(ipAddress, &allComps)->getDateOfFirstStart();
}

void ComputerLogic::setLastMaxTime(QString *ipAddress, QString *lastMaxTime)
{
    if (computerWasAdded(ipAddress, &allComps))
        findComputer(ipAddress, &allComps)->setLastMaxTime(*lastMaxTime);
}

QString ComputerLogic::getLastMaxTime(QString *ipAddress)
{
    if (computerWasAdded(ipAddress, &allComps))
        return findComputer(ipAddress, &allComps)->getLastMaxTime();
    else
        return "1300";

}

QString ComputerLogic::getLastMaxTimeHours(QString *ipAddress)
{
    if (computerWasAdded(ipAddress, &allComps)) {
        QString lastMaxTime = findComputer(ipAddress, &allComps)->getLastMaxTime();
        QString tempTime;
        for (int i = 0; i < lastMaxTime.length(); ++i) {
            if (lastMaxTime[i] == ':')
                break;
            else
                tempTime += lastMaxTime[i];

        }
        return tempTime;
    }
    return "1300";
}

QVector<Computer> *ComputerLogic::getAllComps()
{
    return &allComps;
}

void ComputerLogic::setDayDot(QString value)
{
    dayDot = value;
}

void ComputerLogic::setTimeToClean(QString value)
{
    timeToClean = value;
}

void ComputerLogic::setYourComp(QString value)
{
    yourComp = value;
}

void ComputerLogic::setFilterCloged(QString value)
{
    filterCloged = value;
}

void ComputerLogic::loadCompList()
{
    allComps = compG->findAll();
    if (allComps.size() == 0) {
        Computer newComp;
        newComp.setId(1);
        newComp.setIpAddress("127.0.0.1");
        newComp.setName("Your computer");
        newComp.setOsName("Windows XP");
        newComp.setPlaceType(1);
        newComp.setOutLeftCoolerAmount(0);
        newComp.setOutTopCoolerAmount(0);
        newComp.setInRightCoolerAmount(0);
        newComp.setInBottomCoolerAmount(0);
        newComp.setInSideCoolerAmount(0);
        newComp.setOutLeftCoolerDiameter(120);
        newComp.setOutTopCoolerDiameter(120);
        newComp.setInRightCoolerDiameter(120);
        newComp.setInBottomCoolerDiameter(120);
        newComp.setInSideCoolerDiameter(120);
        newComp.setMaxCleanTime(4680000);
        newComp.setMaxFilterTime(9360000);
        newComp.setMaxFanSpeed(2000);
        newComp.setUsageTime(0);
        newComp.setDateOfFirstStart(QDateTime::fromString("24.10.1795 12:12:12",
                                                          "dd.MM.yyyy hh:mm:ss"));
        newComp.setMaxTemperature(70);
        newComp.setCleanTimeIsRunning(false);
        newComp.setFilterTimeIsRunning(false);
        newComp.setCleanTime(QDateTime::currentDateTime().addSecs(4680000));
        newComp.setFilterTime(QDateTime::currentDateTime().addSecs(9360000));
        newComp.setCompIsPaused(false);
        newComp.setLastMaxTime("1300");
        allComps.push_back(newComp);
    }
    saveThr = new SaveCompThread(allComps);
}

void ComputerLogic::saveSettingsNew()
{
    saveThr->setAllComps(allComps);
    saveThr->start();
}

void ComputerLogic::saveComputers()
{
    QVector< Computer >::iterator cI;
    for (cI = allComps.begin(); cI != allComps.end(); ++cI) {
        if (cI->getId() == 0) {
            cI->setId(generateId());
            compG->insertId(cI);
        }
        else
            compG->modify(cI);
    }
}

void ComputerLogic::saveEntryTime(QString ipAddress)
{
}

SaveCompThread* ComputerLogic::getSaveThr() const
{
    return saveThr;
}
