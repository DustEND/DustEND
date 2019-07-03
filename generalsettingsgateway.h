#ifndef GENERALSETTINGSGATEWAY_H
#define GENERALSETTINGSGATEWAY_H

#include "DB/QueryObj.h"
#include "DB/Criteria.h"
#include "generalsettings.h"

#include <QVector>
#include <QList>

class NewSqlTableField;

class GeneralSettingsGateway {
public:
    GeneralSettingsGateway();
    GeneralSettingsGateway(const GeneralSettingsGateway& orig);
    virtual ~GeneralSettingsGateway();

    QVector< GeneralSettings > findAll();
    GeneralSettings* find(int id);
    QVector< GeneralSettings > findWhere(QString findCriteria);
    void insertAll();
    void insertId(QVector< GeneralSettings >::iterator genSetI);
    void modify(QVector< GeneralSettings >::iterator genSetIt);
    void del(int id);
    void commit();

private:
    QVector< GeneralSettings >* genSets;
    QString tableName;
    QList< QString > selectFields;
    QList< QString > insertFields;
    QList< NewSqlTableField > newFields;
    QString selectCriteria;
    QString modifyCriteria;

    QueryObj* genSetQuery;
    Criteria* genSetCriteria;

    void createGenSetVec();
    void addFields2Select();
    void addFields2Insert();
    void createTable();
    void addFields2Create();
    void appendField2NewFields(QString fieldName, QString type,
                               QString defaultValue, bool notNull,
                               bool primaryKey, bool autoIncrement);
};

#endif // GENERALSETTINGSGATEWAY_H
