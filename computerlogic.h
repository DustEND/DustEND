#ifndef COMPUTERLOGIC_H
#define COMPUTERLOGIC_H

#include <vector>
#include <QString>
#include <QObject>

#include "computer.h"
#include "savecompthread.h"

#include <QVector>

class ComputerGateway;
class Computer;
//class QString;
class QDateTime;
class QHostInfo;


class ComputerLogic : public QObject
{    
    Q_OBJECT

public:
    ComputerLogic();
    ~ComputerLogic();

    void loadSettings(); 
    void addDefaultComputer();
//    QString getName(QString *ipAddress);
    QString getOsName(QString *ipAddress);
    void setOsName(QString *ipAddress, QString *newOsName);
    QString getCleanTimeText(QString *ipAddress);
    QString getFilterTimeText(QString *ipAddress);
    QString time2Text(int *time);
    void setMaxAndCurCleanTime(QString *ipAddress);
    void setMaxAndCurFilterTime(QString *ipAddress);
    void correctCleanTime(QString *ipAddress);
    void correctFilterTime(QString *ipAddress);
    void timerTick();
    QString getUsageTimeText(QString *ipAddress);
    bool getCleanTimeIsRunning(QString *ipAddress);
    bool getFilterTimeIsRunning(QString *ipAddress);
    unsigned short getPlaceType(QString *ipAddress);

    unsigned short getOutLeftCoolerAmount(QString *ipAddress);
    unsigned short getOutTopCoolerAmount(QString *ipAddress);
    unsigned short getInRightCoolerAmount(QString *ipAddress);
    unsigned short getInBottomCoolerAmount(QString *ipAddress);
    unsigned short getInSideCoolerAmount(QString *ipAddress);
    unsigned short getOutLeftCoolerDiameter(QString *ipAddress);
    unsigned short getOutTopCoolerDiameter(QString *ipAddress);
    unsigned short getInRightCoolerDiameter(QString *ipAddress);
    unsigned short getInBottomCoolerDiameter(QString *ipAddress);
    unsigned short getInSideCoolerDiameter(QString *ipAddress);

    void setOutLeftCoolerAmount(QString *ipAddress, unsigned short *amount);
    void setOutTopCoolerAmount(QString *ipAddress, unsigned short *amount);
    void setInRightCoolerAmount(QString *ipAddress, unsigned short *amount);
    void setInBottomCoolerAmount(QString *ipAddress, unsigned short *amount);
    void setInSideCoolerAmount(QString *ipAddress, unsigned short *amount);

    void setOutLeftCoolerDiameter(QString *ipAddress, unsigned short *diameter);
    void setOutTopCoolerDiameter(QString *ipAddress, unsigned short *diameter);
    void setInRightCoolerDiameter(QString *ipAddress, unsigned short *diameter);
    void setInBottomCoolerDiameter(QString *ipAddress, unsigned short *diameter);
    void setInSideCoolerDiameter(QString *ipAddress, unsigned short *diameter);

    void setCleanTimeIsRunning(QString *ipAddress, bool *value);
    void setFilterTimeIsRunning(QString *ipAddress, bool *value);
    void setPlaceType(QString *ipAddress, unsigned short *placeType/*, unsigned short *cleanOrFilter*/);
    QString getShortCleanTime(QString *ipAddress, bool *withAverageUsageTime);
    QString getShortFilterTime(QString *ipAddress, bool *withAverageUsageTime);
    unsigned int getMaxCleanTime(QString *ipAddress);
    unsigned int getMaxFilterTime(QString *ipAddress);
    QString getWarningMessage();
    bool computerWasAdded(QString *ipAddress, QVector<Computer> *compList);
    void delNetComp(QString *ipAddress);

    void addComputer(QVector<Computer> *compList, QString *ipAddress, QString *name,
                     unsigned short *placeType, QDateTime *cleanTime, QDateTime *filterTime,
                     bool *cleanTimeIsRunning, bool *filterTimeIsRunning,
                     int *maxCleanTime, int *maxFilterTime,
                     unsigned short *outLeftCoolerAmount,
                     unsigned short *outLeftCoolerDiameter,
                     unsigned short *outTopCoolerAmount,
                     unsigned short *outTopCoolerDiameter,
                     unsigned short *inRightCoolerAmount,
                     unsigned short *inRightCoolerDiameter,
                     unsigned short *inBottomCoolerAmount,
                     unsigned short *inBottomCoolerDiameter,
                     unsigned short *inSideCoolerAmount,
                     unsigned short *inSideCoolerDiameter,
                     unsigned short *maxTemperature, unsigned long *usageTime,
                     unsigned int *maxFanSpeed, QString *osName, bool *compIsPaused,
                     QDateTime *dateOfFirstStart, QString *lastMaxTime);
    bool CompIsPaused(QString *ipAddress);
    void setCompPause(QString *ipAddress, bool *pause);

    void addCooler(QString *ipAddress, unsigned short *coolerType, unsigned short *newAmount);
    int getCurrentCleanHours(QString *ipAddress);
    int getCurrentFilterHours(QString *ipAddress);
    int getMaxCleanHours();
    int getMaxFilterHours();
    QString getIpAddress(QString *ipAddress);
    void changeCoolerDiameter(QString *ipAddress, unsigned short *coolerType, unsigned short *newDiameter);
    int getIncrementForCleanTime(unsigned short *coolerDiameter);
    int getIncrementForFilterTime(unsigned short *coolerDiameter);
    void setDateOfFirstStart(QDateTime *firstStartTime, QString *ipAddress);
    QDateTime getDateOfFirstStart(QString *ipAddress);
    void setLastMaxTime(QString *ipAddress, QString *lastMaxTime);
    QString getLastMaxTime(QString *ipAddress);
    QString getLastMaxTimeHours(QString *ipAddress);

    QVector<Computer>* getAllComps();
    void setDayDot(QString value);
    void setTimeToClean(QString value);
    void setYourComp(QString value);
    void setFilterCloged(QString value);
    void saveSettingsNew();
    void saveComputers();
    void saveEntryTime(QString ipAddress);

//    SaveCompThread *saveThr;

    SaveCompThread* getSaveThr() const;
private:
    QVector<Computer> allComps;    
    QVector<Computer>::iterator firstCompI;
    ComputerGateway *compG;
    QString cleanWarningText;
    QString filterWarningText;
    QString warningText;
    QString lastWarningText;
    int warningTime;
    double averageUsageTime;
    QString dayDot;
    QString timeToClean;
    QString yourComp;
    QString filterCloged;

    SaveCompThread *saveThr;

    void loadCompList();
    QVector<Computer>::iterator findComputer(QString *ipAddress, QVector<Computer> *compList);

    void calculateUsageTime(QString *ipAddress);
    void setCleanWarningText(QString *ipAddress);
    void setFilterWarningText(QString *ipAddress);
    void makeWarningText();
    double calcAverageUsageTime(QDateTime *dateOfFirstStart, unsigned long *usageTime);
    QString getAverageUsageTimeText(QDateTime *dateOfFirstStart, unsigned long *usageTime);
    int generateId();

};

#endif // COMPUTERLOGIC_H
