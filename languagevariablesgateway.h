#ifndef LANGUAGEVARIABLESGATEWAY_H
#define LANGUAGEVARIABLESGATEWAY_H

#include "DB/QueryObj.h"
#include "DB/Criteria.h"
#include "languagevariables.h"

#include <QVector>
#include <QList>

class NewSqlTableField;

class LanguageVariablesGateway {
public:
    LanguageVariablesGateway();
    LanguageVariablesGateway(const LanguageVariablesGateway& orig);
    virtual ~LanguageVariablesGateway();

    QVector< LanguageVariables > findAll();
    LanguageVariables* find(QString varName);
    QVector< LanguageVariables > findWhere(QString findCriteria);
    void insertAll();
    void insertId(QVector< LanguageVariables >::iterator langI);
    void modify(QVector< LanguageVariables >::iterator langI);
    void del(QString varName);
    void commit();

private:
    QVector< LanguageVariables >* langVars;
    QString tableName;
    QList< QString > selectFields;
    QList< QString > insertFields;
    QList< QString > modifyFields;
    QList< NewSqlTableField > newFields;
    QString selectCriteria;
    QString modifyCriteria;

    QueryObj* langQuery;
    Criteria* langCriteria;

    void createLangVarVec();
    void addFields2Select();
    void addFields2Insert();
    void addFields2Modify();
    void createTable();
    void addFields2Create();
    void appendField2NewFields(QString fieldName, QString type,
                               QString defaultValue, bool notNull,
                               bool primaryKey, bool autoIncrement);
};

#endif // LANGUAGEVARIABLESGATEWAY_H
