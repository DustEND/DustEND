#include "mailthread.h"
#include "email/SmtpMime"

#include <QDebug>

MailThread::MailThread()
{
}

void MailThread::run()
{
    sendEMail();
}

void MailThread::setWarningMsg(QString *value)
{
    this->warningMsg = *value;
}

void MailThread::setSubject(QString *value)
{
    this->subject = *value;
}

void MailThread::setEmailAddress(QString *value)
{
    this->emailAddress = *value;
}

void MailThread::sendEMail()
{
    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);
    smtp.setUser("support@dustend.com");
    smtp.setPassword("Vm2&6j<*Jk>");

    MimeMessage message;
    message.setSender(new EmailAddress("support@dustend.com", "ComputerDustControl"));
    message.addRecipient(new EmailAddress(emailAddress, ""));
    message.setSubject(subject);

    MimeText text;
    text.setText(warningMsg);
    message.addPart(&text);

    smtp.connectToHost();
    smtp.login();
    smtp.sendMail(message);
    smtp.quit();
}
