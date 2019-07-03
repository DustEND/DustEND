#include "generalsettings.h"

GeneralSettings::GeneralSettings()
{
}

void GeneralSettings::setId(int newId)
{
    this->id = newId;
}

int GeneralSettings::getId() const
{
    return this->id;
}

void GeneralSettings::setLocale(QString newLocale)
{
    this->locale = newLocale;
}

QString GeneralSettings::getLocale() const
{
    return this->locale;
}
void GeneralSettings::setName(bool newName)
{
    this->name = newName;
}

bool GeneralSettings::getName() const
{
    return this->name;
}
void GeneralSettings::setIpAddress(bool newIpAddress)
{
    this->ipAddress = newIpAddress;
}

bool GeneralSettings::getIpAddress() const
{
    return this->ipAddress;
}
void GeneralSettings::setCleanTime(bool newCleanTime)
{
    this->cleanTime = newCleanTime;
}

bool GeneralSettings::getCleanTime() const
{
    return this->cleanTime;
}
void GeneralSettings::setFilterTime(bool newFilterTime)
{
    this->filterTime = newFilterTime;
}

bool GeneralSettings::getFilterTime() const
{
    return this->filterTime;
}
void GeneralSettings::setOs(bool newOs)
{
    this->os = newOs;
}

bool GeneralSettings::getOs() const
{
    return this->os;
}
void GeneralSettings::setTime(bool newTime)
{
    this->time = newTime;
}

bool GeneralSettings::getTime() const
{
    return this->time;
}
void GeneralSettings::setStatus(bool newStatus)
{
    this->status = newStatus;
}

bool GeneralSettings::getStatus() const
{
    return this->status;
}
void GeneralSettings::setWarningInterval(int newWarningInterval)
{
    this->warningInterval = newWarningInterval;
}

int GeneralSettings::getWarningInterval() const
{
    return this->warningInterval;
}
void GeneralSettings::setSendMail(bool newSendMail)
{
    this->sendMail = newSendMail;
}

bool GeneralSettings::getSendMail() const
{
    return this->sendMail;
}
void GeneralSettings::setEmailAddress(QString newEmailAddress)
{
    this->emailAddress = newEmailAddress;
}

QString GeneralSettings::getEmailAddress() const
{
    return this->emailAddress;
}
void GeneralSettings::setAutoStartApp(bool newAutoStartApp)
{
    this->autoStartApp = newAutoStartApp;
}

bool GeneralSettings::getAutoStartApp() const
{
    return this->autoStartApp;
}
