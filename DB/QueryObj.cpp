#include "QueryObj.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QDebug>
#include <QDateTime>
#include "newsqltablefield.h"

QueryObj::QueryObj() {
    query = new QSqlQuery;
}

QueryObj::QueryObj(const QueryObj& orig) {
    this->tableName = orig.tableName;
    this->selectSql = orig.selectSql;
    this->selectFields = orig.selectFields;
    this->selectCriteria = orig.selectCriteria;
}

QueryObj::~QueryObj() {
}

void QueryObj::setTableName(QString* tableName){
	this->tableName = *tableName;
}

void QueryObj::setSelect(QList< QString >* selectFields,
                        QString selectCriteria){
    selectSql = "SELECT ";
    QList< QString >::iterator selectIter;
    for( selectIter = selectFields->begin(); 
            selectIter != selectFields->end();
            ++selectIter ){
        selectSql += *selectIter;
        ++selectIter;
        if(selectIter != selectFields->end())
            selectSql += ", ";
        --selectIter;
    }
    selectSql += " FROM ";
    selectSql += tableName;
    if (selectCriteria != ""){
        selectSql += " WHERE ";
        selectSql += selectCriteria;  
    }
    selectSql += " ";
}

void QueryObj::setInsert(QList< QString >* insertFields,
                                QList< QString >* values){
    insertSql = "INSERT INTO ";
    insertSql += tableName;
    insertSql += " ( ";
    QList< QString >::iterator insertIter;
    for( insertIter = insertFields->begin();
         insertIter != insertFields->end();
         ++insertIter){
        insertSql += *insertIter;
        ++insertIter;
        if(insertIter != insertFields->end())
                insertSql += ", ";
        --insertIter;
    }
    insertSql += " ) values ( ";
    for( insertIter = values->begin();
            insertIter != values->end();
            ++insertIter){
		insertSql += "'";
        insertSql += *insertIter;
		insertSql += "'";
        ++insertIter;
        if(insertIter != values->end())
                insertSql += ", ";
        --insertIter;
    }
    insertSql += " ) ";
}

void QueryObj::setModify(QList<QString>* modifyFields,
                         QString modifyCriteria,
                         QList< QString >* values){
    modifySql = "UPDATE ";
    modifySql += tableName;
    modifySql += " SET ";
    QList< QString >::iterator modifyIter;
    QList< QString >::iterator valuesIter = values->begin();
    for( modifyIter = modifyFields->begin(); 
         modifyIter != modifyFields->end();
         ++modifyIter ){
        modifySql += *modifyIter;
        modifySql += " = '";
        modifySql += *valuesIter;
        modifySql += "'";
        ++modifyIter;
        if(modifyIter != modifyFields->end())
            modifySql += ", ";
        --modifyIter;
        ++valuesIter;
    }    
    modifySql += " WHERE ";
    modifySql += modifyCriteria;  
    modifySql += " ";
}

void QueryObj::setDelete(QString* deleteCriteria){
    deleteSql = "DELETE FROM ";
    deleteSql += tableName;
    deleteSql += " WHERE ";
    deleteSql += *deleteCriteria;
    deleteSql += " ";
}

void QueryObj::setCreate(QList< NewSqlTableField > *fields)
{
    createSql = QString("CREATE TABLE [%1] ( ").arg(tableName);
    QList< NewSqlTableField >::iterator fieldsIt;
    for(fieldsIt = fields->begin(); fieldsIt != fields->end(); ++fieldsIt) {
        createSql += QString("[%1] %2 ").arg(fieldsIt->getFieldName()).arg(fieldsIt->getType());
        if (fieldsIt->getDefaultValue() != "")
            createSql += QString("DEFAULT '%1' ").arg(fieldsIt->getDefaultValue());

        if (fieldsIt->getPrimaryKey())
            createSql += "PRIMARY KEY ";
        if (fieldsIt->getAutoIncrement())
            createSql += "AUTOINCREMENT ";
        if (fieldsIt->getNotNull())
            createSql += "NOT NULL ";
        else
            createSql += "NULL ";
        ++fieldsIt;
        if (fieldsIt != fields->end())
            createSql += ", ";
        --fieldsIt;
    }
    createSql += ") ";
}

bool QueryObj::select(){
    return query->exec(selectSql);
}

void QueryObj::commit(){	
}

void QueryObj::insert(){
    query->exec(insertSql);
//    qDebug() << "insert error: " << query->lastError();
}

void QueryObj::modify(){
    query->exec(modifySql);    
//    qDebug() << "modify error: " << query->lastError();
}

void QueryObj::del(){
    query->exec(deleteSql);
//    qDebug() << "del error: " << query->lastError();
}

bool QueryObj::create(){
    return query->exec(createSql);
}

int QueryObj::getRowCount(){
    int rows = 0;
    if (query->first()){
        do{
            ++rows;
        }
        while(query->next());
    }
    return rows;
}

QString QueryObj::getString(int fieldNumber, int recordNumber) {
    if (query->isActive()
        && query->isSelect()) {
        query->seek(recordNumber);
        return query->value(fieldNumber).toString();
    }
    else
        return QString("");
}

int QueryObj::getInt(int fieldNumber, int recordNumber){
    if (query->isActive()
            && query->isSelect()){
        query->seek(recordNumber);
        return query->value(fieldNumber).toInt();
    }
    else
        return 0;
}

double QueryObj::getDouble(int fieldNumber, int recordNumber){
    if (query->isActive()
        && query->isSelect()){
        query->seek(recordNumber);
        return query->value(fieldNumber).toDouble();
    }
    else
        return 0.0;
}

bool QueryObj::getBool(int fieldNumber, int recordNumber){
    if (query->isActive()
        && query->isSelect()){
        query->seek(recordNumber);
        return query->value(fieldNumber).toBool();
    }
    else
        return false;
}
