#ifndef MAILTHREAD_H
#define MAILTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>

class MailThread : public QThread
{
    Q_OBJECT
public:
    MailThread();
    void run();

    void setWarningMsg(QString *value);
    void setSubject(QString *value);
    void setEmailAddress(QString *value);
    
private:
    QString warningMsg;
    QString subject;
    QString emailAddress;

    void sendEMail();
    
};

#endif // MAILTHREAD_H
