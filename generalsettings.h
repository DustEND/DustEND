#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QString>

class GeneralSettings
{
public:
    GeneralSettings();

    void setId(int newId);
    int getId() const;
    void setLocale(QString newLocale);
    QString getLocale() const;
    void setName(bool newName);
    bool getName() const;
    void setIpAddress(bool newIpAddress);
    bool getIpAddress() const;
    void setCleanTime(bool newCleanTime);
    bool getCleanTime() const;
    void setFilterTime(bool newFilterTime);
    bool getFilterTime() const;
    void setOs(bool newOs);
    bool getOs() const;
    void setTime(bool newTime);
    bool getTime() const;
    void setStatus(bool newStatus);
    bool getStatus() const;
    void setWarningInterval(int newWarningInterval);
    int getWarningInterval() const;
    void setSendMail(bool newSendMail);
    bool getSendMail() const;
    void setEmailAddress(QString newEmailAddress);
    QString getEmailAddress() const;
    void setAutoStartApp(bool newAutoStartApp);
    bool getAutoStartApp() const;

private:
    int id;
    QString locale;
    bool name;
    bool ipAddress;
    bool cleanTime;
    bool filterTime;
    bool os;
    bool time;
    bool status;
    int warningInterval;
    bool sendMail;
    QString emailAddress;
    bool autoStartApp;
};

#endif // GENERALSETTINGS_H
