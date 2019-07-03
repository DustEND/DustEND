#ifndef SAVECOMPTHREAD_H
#define SAVECOMPTHREAD_H

#include <QThread>
#include <QObject>
#include <QVector>

class ComputerGateway;
class Computer;

class SaveCompThread : public QThread
{
    Q_OBJECT
public:
    SaveCompThread();
    SaveCompThread(QVector<Computer> allComps);
    void run();
    void setAllComps(QVector<Computer> allComps);
    void setFirstAllComps(QVector<Computer> firstAllComps);

private:
    QVector<Computer> allComps;
    QVector<Computer> firstAllComps;
    QVector<Computer>::iterator firstCompI;
    ComputerGateway *compG;

    void saveSettingsNew();
    bool computerWasAdded(QString *ipAddress, QVector<Computer> *compList);
    int generateId();

signals:
    void settingsSaved();
};

#endif // SAVECOMPTHREAD_H
