#ifndef NETCOMP_H
#define NETCOMP_H

#include <QString>

class NetComp
{
public:
    NetComp();
    NetComp(QString newName, QString newIpAddress, QString newOsName,
            QString newCleanTime, QString newFilterTime);
    NetComp& operator=(const NetComp& orig);
    bool operator==(const NetComp& orig);

    void setName(QString *newName);
    QString getName() const;
    void setIpAddress(QString *newIpAddress);
    QString getIpAddress() const;
    void setOsName(QString *newOsName);
    QString getOsName() const;
    void setCleanTime(QString *newCleanTime);
    QString getCleanTime() const;
    void setFilterTime(QString *newFilterTime);
    QString getFilterTime() const;

private:
    QString name;
    QString ipAddress;
    QString osName;
    QString cleanTime;
    QString filterTime;

    void initParams();
};

#endif // NETCOMP_H
