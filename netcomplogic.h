#ifndef NETCOMPLOGIC_H
#define NETCOMPLOGIC_H

#include "netcomp.h"
#include <QList>

class NetCompLogic
{
public:
    NetCompLogic();
    void sortByName(QList<NetComp> &mas, bool ascending = false);
    void sortByTime(QList<NetComp> &mas, bool ascending = false);
    void sortByCleanTime(QList<NetComp> &mas, bool ascending = false);
    void sortByFilterTime(QList<NetComp> &mas, bool ascending = false);
    void sortByOs(QList<NetComp> &mas, bool ascending = false);
    void sortByIpAddress(QList<NetComp> &mas, bool ascending = false);
};

#endif // NETCOMPLOGIC_H
