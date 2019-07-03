#include "savecompthread.h"
#include "computer.h"
#include "computergateway.h"

#include <QDebug>

SaveCompThread::SaveCompThread()
{
}

SaveCompThread::SaveCompThread(QVector<Computer> allComps)
{
    this->allComps = allComps;
    compG = new ComputerGateway();
}

void SaveCompThread::run()
{
    saveSettingsNew();
    emit settingsSaved();
}

void SaveCompThread::setAllComps(QVector<Computer> value)
{
    this->allComps = value;
}

void SaveCompThread::setFirstAllComps(QVector<Computer> value)
{
    this->firstAllComps = value;
}

bool SaveCompThread::computerWasAdded(QString *ipAddress, QVector<Computer> *compList)
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

int SaveCompThread::generateId()
{
    QVector< Computer >::iterator compI;
    int id = 0;
    for (compI = allComps.begin(); compI != allComps.end(); ++compI) {
        if (compI->getId() > id)
            id = compI->getId();
    }
    return ++id;
}

void SaveCompThread::saveSettingsNew()
{
    QVector< Computer >::iterator cI;
    QString ip;
    for (cI = allComps.begin(); cI != allComps.end(); ++cI) {
        ip = cI->getIpAddress();        
        if (computerWasAdded(&ip, &firstAllComps)) {
            if (cI->getName() != firstCompI->getName()
                    || cI->getOsName() != firstCompI->getOsName()
                    || cI->getPlaceType() != firstCompI->getPlaceType()
                    || cI->getOutLeftCoolerAmount() != firstCompI->getOutLeftCoolerAmount()
                    || cI->getOutTopCoolerAmount() != firstCompI->getOutTopCoolerAmount()
                    || cI->getInRightCoolerAmount() != firstCompI->getInRightCoolerAmount()
                    || cI->getInBottomCoolerAmount() != firstCompI->getInBottomCoolerAmount()
                    || cI->getInSideCoolerAmount() != firstCompI->getInSideCoolerAmount()
                    || cI->getOutLeftCoolerDiameter() != firstCompI->getOutLeftCoolerDiameter()
                    || cI->getOutTopCoolerDiameter() != firstCompI->getOutTopCoolerDiameter()
                    || cI->getInRightCoolerDiameter() != firstCompI->getInRightCoolerDiameter()
                    || cI->getInBottomCoolerDiameter() != firstCompI->getInBottomCoolerDiameter()
                    || cI->getInSideCoolerDiameter() != firstCompI->getInSideCoolerDiameter()
                    || cI->getMaxCleanTime() != firstCompI->getMaxCleanTime()
                    || cI->getMaxFilterTime() != firstCompI->getMaxFilterTime()
                    || cI->getMaxFanSpeed() != firstCompI->getMaxFanSpeed()
                    || cI->getUsageTime() != firstCompI->getUsageTime()
                    || cI->getDateOfFirstStart() != firstCompI->getDateOfFirstStart()
                    || cI->getMaxTemperature() != firstCompI->getMaxTemperature()
                    || cI->getCleanTimeIsRunning() != firstCompI->getCleanTimeIsRunning()
                    || cI->getFilterTimeIsRunning() != firstCompI->getFilterTimeIsRunning()
                    || cI->getCleanTime() != firstCompI->getCleanTime()
                    || cI->getFilterTime() != firstCompI->getFilterTime()
                    || cI->getCompIsPaused() != firstCompI->getCompIsPaused()
                    || cI->getLastMaxTime() != firstCompI->getLastMaxTime()) {
                compG->modify(cI);              
            }
        }
        else
            compG->insertId(cI);
    }
//    firstAllComps.clear();
    firstAllComps = allComps;
}
