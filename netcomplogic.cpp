#include "netcomplogic.h"

#include <QDebug>

NetCompLogic::NetCompLogic()
{
}

void NetCompLogic::sortByName(QList<NetComp> &mas, bool ascending)
{
    for(int i = 0; i < mas.size(); ++i) {
        int minPos = i;
        for(int j = i + 1; j < mas.size(); ++j) {
            if (ascending) {
                if(mas.at(minPos).getName() > mas.at(j).getName())
                    minPos = j;
            }
            else
                if(mas.at(minPos).getName() < mas.at(j).getName())
                    minPos = j;

        }
        mas.swap(minPos, i);
    }
}

void NetCompLogic::sortByTime(QList<NetComp> &mas, bool ascending)
{
    QString minPosTime, jTime;
    minPosTime = "";
    jTime = "";
    for(int i = 0; i < mas.size(); ++i) {
        int minPos = i;
        for(int j = i + 1; j < mas.size(); ++j) {
            if (mas.at(minPos).getCleanTime() != "")
                minPosTime = mas.at(minPos).getCleanTime();
            else
                minPosTime = mas.at(minPos).getFilterTime();
            if (mas.at(j).getCleanTime() != "")
                jTime = mas.at(j).getCleanTime();
            else
                jTime = mas.at(j).getFilterTime();

            if (ascending) {
                if(minPosTime > jTime)
                    minPos = j;
            }
            else
                if(minPosTime < jTime)
                    minPos = j;
        }
        mas.swap(minPos, i);
    }
}

void NetCompLogic::sortByCleanTime(QList<NetComp> &mas, bool ascending)
{
    for(int i = 0; i < mas.size(); ++i) {
        int minPos = i;
        for(int j = i + 1; j < mas.size(); ++j) {
            if (ascending) {
                if(mas.at(minPos).getCleanTime() > mas.at(j).getCleanTime())
                    minPos = j;
            }
            else
                if(mas.at(minPos).getCleanTime() < mas.at(j).getCleanTime())
                    minPos = j;

        }
        mas.swap(minPos, i);
    }
}

void NetCompLogic::sortByFilterTime(QList<NetComp> &mas, bool ascending)
{
    for(int i = 0; i < mas.size(); ++i) {
        int minPos = i;
        for(int j = i + 1; j < mas.size(); ++j) {
            if (ascending) {
                if(mas.at(minPos).getFilterTime() > mas.at(j).getFilterTime())
                    minPos = j;
            }
            else
                if(mas.at(minPos).getFilterTime() < mas.at(j).getFilterTime())
                    minPos = j;

        }
        mas.swap(minPos, i);
    }
}

void NetCompLogic::sortByIpAddress(QList<NetComp> &mas, bool ascending)
{
    for(int i = 0; i < mas.size(); ++i) {
        int minPos = i;
        for(int j = i + 1; j < mas.size(); ++j) {
            if (ascending) {
                if(mas.at(minPos).getIpAddress() > mas.at(j).getIpAddress())
                    minPos = j;
            }
            else
                if(mas.at(minPos).getIpAddress() < mas.at(j).getIpAddress())
                    minPos = j;

        }
        mas.swap(minPos, i);
    }
}

void NetCompLogic::sortByOs(QList<NetComp> &mas, bool ascending)
{
    for(int i = 0; i < mas.size(); ++i) {
        int minPos = i;
        for(int j = i + 1; j < mas.size(); ++j) {
            if (ascending) {
                if(mas.at(minPos).getOsName() > mas.at(j).getOsName())
                    minPos = j;
            }
            else
                if(mas.at(minPos).getOsName() < mas.at(j).getOsName())
                    minPos = j;

        }
        mas.swap(minPos, i);
    }
}
