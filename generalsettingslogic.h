#ifndef GENERALSETTINGSLOGIC_H
#define GENERALSETTINGSLOGIC_H

#include <QVector>

class GeneralSettings;
class GeneralSettingsGateway;

class GeneralSettingsLogic
{
public:
    GeneralSettingsLogic();

    QString getLocale() const;
    void setLocale(QString *newLocale);
    bool getName() const;
    void setName(bool *newName);
    bool getIpAddress() const;
    void setIpAddress(bool *newIpAddress);
    bool getCleanTime() const;
    void setCleanTime(bool *newCleanTime);
    bool getFilterTime() const;
    void setFilterTime(bool *newFilterTime);
    bool getOs() const;
    void setOs(bool *newOs);
    bool getTime() const;
    void setTime(bool *newTime);
    bool getStatus() const;
    void setStatus(bool *newStatus);
    int getWarningInterval() const;
    void setWarningInterval(unsigned int *newWarningInterval);
    bool getSendMail() const;
    void setSendMail(bool *newSendMail);
    QString getEmailAddress() const;
    void setEmailAddress(QString *newEmailAddress);
    bool getAutoStartApp() const;
    void setAutoStartApp(bool *newAutoStartApp);

private:
    GeneralSettingsGateway* genSetG;
    QVector< GeneralSettings > genSets;

    void loadGenSetList();
    void saveGenSets();
};

#endif // GENERALSETTINGSLOGIC_H
