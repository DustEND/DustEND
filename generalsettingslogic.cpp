#include "generalsettingslogic.h"
#include "generalsettingsgateway.h"

#include <QDebug>

GeneralSettingsLogic::GeneralSettingsLogic()
{
    genSetG = new GeneralSettingsGateway();
    loadGenSetList();
}

QString GeneralSettingsLogic::getLocale() const
{
    return genSets.at(0).getLocale();
}

void GeneralSettingsLogic::setLocale(QString *newLocale)
{
    QVector< GeneralSettings >::iterator genSetI = genSets.begin();
    genSetI->setLocale(*newLocale);
    saveGenSets();
    genSetG->commit();
}

bool GeneralSettingsLogic::getName() const
{
    return genSets.at(0).getName();
}

void GeneralSettingsLogic::setName(bool *newName)
{
    QVector< GeneralSettings >::iterator genSetI = genSets.begin();
    genSetI->setName(*newName);
    saveGenSets();
    genSetG->commit();
}

bool GeneralSettingsLogic::getIpAddress() const
{
    return genSets.at(0).getIpAddress();
}

void GeneralSettingsLogic::setIpAddress(bool *newIpAddress)
{
    QVector< GeneralSettings >::iterator genSetI = genSets.begin();
    genSetI->setIpAddress(*newIpAddress);
    saveGenSets();
    genSetG->commit();
}

bool GeneralSettingsLogic::getCleanTime() const
{
    return genSets.at(0).getCleanTime();
}

void GeneralSettingsLogic::setCleanTime(bool *newCleanTime)
{
    QVector< GeneralSettings >::iterator genSetI = genSets.begin();
    genSetI->setCleanTime(*newCleanTime);
    saveGenSets();
    genSetG->commit();
}

bool GeneralSettingsLogic::getFilterTime() const
{
    return genSets.at(0).getFilterTime();
}

void GeneralSettingsLogic::setFilterTime(bool *newFilterTime)
{
    QVector< GeneralSettings >::iterator genSetI = genSets.begin();
    genSetI->setFilterTime(*newFilterTime);
    saveGenSets();
    genSetG->commit();
}

bool GeneralSettingsLogic::getOs() const
{
    return genSets.at(0).getOs();
}

void GeneralSettingsLogic::setOs(bool *newOs)
{
    QVector< GeneralSettings >::iterator genSetI = genSets.begin();
    genSetI->setOs(*newOs);
    saveGenSets();
    genSetG->commit();
}

bool GeneralSettingsLogic::getTime() const
{
    return genSets.at(0).getTime();
}

void GeneralSettingsLogic::setTime(bool *newTime)
{
    QVector< GeneralSettings >::iterator genSetI = genSets.begin();
    genSetI->setTime(*newTime);
    saveGenSets();
    genSetG->commit();
}

bool GeneralSettingsLogic::getStatus() const
{
    return genSets.at(0).getStatus();
}

void GeneralSettingsLogic::setStatus(bool *newStatus)
{
    QVector< GeneralSettings >::iterator genSetI = genSets.begin();
    genSetI->setStatus(*newStatus);
    saveGenSets();
    genSetG->commit();
}

int GeneralSettingsLogic::getWarningInterval() const
{
    return genSets.at(0).getWarningInterval();
}

void GeneralSettingsLogic::setWarningInterval(unsigned int *newWarningInterval)
{
    QVector< GeneralSettings >::iterator genSetI = genSets.begin();
    genSetI->setWarningInterval(*newWarningInterval);
    saveGenSets();
    genSetG->commit();
}

bool GeneralSettingsLogic::getSendMail() const
{
    return genSets.at(0).getSendMail();
}

void GeneralSettingsLogic::setSendMail(bool *newSendMail)
{
    QVector< GeneralSettings >::iterator genSetI = genSets.begin();
    genSetI->setSendMail(*newSendMail);
    saveGenSets();
    genSetG->commit();
}

QString GeneralSettingsLogic::getEmailAddress() const
{
    return genSets.at(0).getEmailAddress();
}

void GeneralSettingsLogic::setEmailAddress(QString *newEmailAddress)
{
    QVector< GeneralSettings >::iterator genSetI = genSets.begin();
    genSetI->setEmailAddress(*newEmailAddress);
    saveGenSets();
    genSetG->commit();
}

bool GeneralSettingsLogic::getAutoStartApp() const
{
    return genSets.at(0).getAutoStartApp();
}

void GeneralSettingsLogic::setAutoStartApp(bool *newAutoStartApp)
{
    QVector< GeneralSettings >::iterator genSetI = genSets.begin();
    genSetI->setAutoStartApp(*newAutoStartApp);
    saveGenSets();
    genSetG->commit();
}

void GeneralSettingsLogic::loadGenSetList()
{
    genSets = genSetG->findAll();
    if (genSets.size() == 0) {
        GeneralSettings newGenSet;
        newGenSet.setId(0);
        newGenSet.setLocale("en");
        newGenSet.setName(true);
        newGenSet.setIpAddress(false);
        newGenSet.setCleanTime(false);
        newGenSet.setFilterTime(false);
        newGenSet.setOs(false);
        newGenSet.setTime(true);
        newGenSet.setStatus(true);
        newGenSet.setWarningInterval(30);
        newGenSet.setSendMail(false);
        newGenSet.setEmailAddress("");
        newGenSet.setAutoStartApp(false);
        genSets.push_back(newGenSet);
    }
}

void GeneralSettingsLogic::saveGenSets()
{
    QVector< GeneralSettings>::iterator genSetI = genSets.begin();
    genSetG->modify(genSetI);

    if (genSetI->getId() == 0)
        genSetG->insertId(genSetI);
}
