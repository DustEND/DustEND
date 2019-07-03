#ifndef COMPUTERGATEWAY_H
#define COMPUTERGATEWAY_H

#include "DB/QueryObj.h"
#include "DB/Criteria.h"
#include "computer.h"

#include <QVector>
#include <QList>

class NewSqlTableField;

class ComputerGateway {
public:
    ComputerGateway();
    ComputerGateway(const ComputerGateway& orig);
    virtual ~ComputerGateway();

    QVector< Computer > findAll();
    Computer* find(int id);
    QVector< Computer > findWhere(QString findCriteria);
    void insertAll();
    void insertId(QVector< Computer >::iterator compI);
    void modify(QVector< Computer >::iterator compI);
    void del(int id);
    void commit();

private:
    QVector< Computer >* comps;
    QString tableName;
    QList< QString > selectFields;
    QList< QString > insertFields;
    QList< QString > modifyFields;
    QList< NewSqlTableField > newFields;
    QString selectCriteria;
    QString modifyCriteria;

    QueryObj* compQuery;
    Criteria* compCriteria;

    void createCompVec();
    void addFields2Select();
    void addFields2Insert();
    void addFields2Modify();
    void createTable();
    void addFields2Create();
    void appendField2NewFields(QString fieldName, QString type,
                               QString defaultValue, bool notNull,
                               bool primaryKey, bool autoIncrement);
};

#endif // COMPUTERGATEWAY_H
