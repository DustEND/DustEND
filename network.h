#ifndef NETWORK_H
#define NETWORK_H

#include <QList>
#include <QPair>
#include <QObject>
#include <QThread>
#include <QSemaphore>

#include <windows.h>

#include "netcomp.h"

typedef QList<NetComp> NetCompList;
typedef NetComp NetC;

class QString;

class Network : public QThread
{
    Q_OBJECT

public:
    Network();
    void run();
    void scanNetwork();
    QList<NetComp> getNetTbl() const;

private:
    QList<NetComp> *netTbl;
    NetComp tempComp;
    bool notAdded;
    QSemaphore netSemaph;


    // Удалось ли проинициализировать WinSock
    BOOL bWSA;
    int ResFunc(NETRESOURCE *pNr,    // pNr - NETRESOURCE container'а, где смотреть содержимое
                int sp);                // sp - количество пробелов для отступа
    void addComp(QString *name, QString *ipAddr, QString *os, QString *cleanTime, QString *filterTime);

signals:
    void getNetComp(NetC);
    void scanFinished(NetCompList);

public slots:
    void wake();
    void pause();

};

#endif // NETWORK_H
