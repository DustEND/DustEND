#include "netcomp.h"

NetComp::NetComp()
{
    initParams();
}

NetComp::NetComp(QString newName, QString newIpAddress, QString newOsName,
                 QString newCleanTime, QString newFilterTime)
{
    initParams();

    this->name = newName;
    this->ipAddress = newIpAddress;
    this->osName = newOsName;
    this->cleanTime = newCleanTime;
    this->filterTime = newFilterTime;
}

void NetComp::setName(QString *newName)
{
    this->name = *newName;
}

QString NetComp::getName() const
{
    return name;
}

void NetComp::setIpAddress(QString *newIpAddress)
{
    this->ipAddress = *newIpAddress;
}

QString NetComp::getIpAddress() const
{
    return ipAddress;
}

void NetComp::setOsName(QString *newOsName)
{
    this->osName = *newOsName;
}

QString NetComp::getOsName() const
{
    return osName;
}

NetComp & NetComp::operator=(const NetComp &orig)
{
    this->name = orig.name;
    this->osName = orig.osName;
    this->ipAddress = orig.ipAddress;
    this->cleanTime = orig.cleanTime;
    this->filterTime = orig.filterTime;
    return *this;
}

bool NetComp::operator==(const NetComp &orig)
{
    if (this->name == orig.name) {
        if (this->ipAddress == orig.ipAddress)
            return true;
        else
            return false;
    }
    else
        return false;
}

void NetComp::setCleanTime(QString *newCleanTime)
{
    this->cleanTime = *newCleanTime;
}

QString NetComp::getCleanTime() const
{
    return cleanTime;
}

void NetComp::setFilterTime(QString *newFilterTime)
{
    this->filterTime = *newFilterTime;
}

QString NetComp::getFilterTime() const
{
    return filterTime;
}

void NetComp::initParams()
{    
    name = "";
    ipAddress = "";
    osName = "";
    cleanTime = "";
    filterTime = "";
}
