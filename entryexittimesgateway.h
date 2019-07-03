#ifndef ENTRYEXITTIMESGATEWAY_H
#define ENTRYEXITTIMESGATEWAY_H

#include "entryexittimes.h"

#include "DB/QueryObj.h"
#include "DB/Criteria.h"

#include <QVector>
#include <QList>

class NewSqlTableField;

class EntryExitTimesGateway
{
public:
    EntryExitTimesGateway();
    EntryExitTimesGateway(const EntryExitTimesGateway& orig);
    virtual ~EntryExitTimesGateway();

    QVector< EntryExitTimes > findAll();
    EntryExitTimes* find(QString ipAddress);
    QVector< EntryExitTimes > findWhere(QString findCriteria);
    void insertAll();
    void insertId(QVector< EntryExitTimes >::iterator entExtTimesI);
    void modify(QVector< EntryExitTimes >::iterator entExtTimesI);
    void del(QString ipAddress);
    void commit();

private:
    QVector< EntryExitTimes >* entExtTimes;
    QString tableName;
    QList< QString > selectFields;
    QList< QString > insertFields;
    QList< QString > modifyFields;
    QList< NewSqlTableField > newFields;
    QString selectCriteria;
    QString modifyCriteria;

    QueryObj* entrExtTmsQuery;
    Criteria* entrExtTmsCriteria;

    void createEntExtTimesVec();
    void addFields2Select();
    void addFields2Insert();
    void addFields2Modify();
    void createTable();
    void addFields2Create();
    void appendField2NewFields(QString fieldName, QString type,
                               QString defaultValue, bool notNull,
                               bool primaryKey, bool autoIncrement);
};

#endif // ENTRYEXITTIMESGATEWAY_H
