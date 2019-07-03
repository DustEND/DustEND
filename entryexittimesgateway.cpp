#include "entryexittimesgateway.h"

#include "DB/newsqltablefield.h"
#include <QDebug>

EntryExitTimesGateway::EntryExitTimesGateway(const EntryExitTimesGateway& orig) {
    this->tableName = orig.tableName;
    this->selectFields = orig.selectFields;
    this->selectCriteria = orig.selectCriteria;
}

EntryExitTimesGateway::EntryExitTimesGateway(){
    entrExtTmsQuery = new QueryObj();
    entrExtTmsCriteria = new Criteria();

    entExtTimes = new QVector< EntryExitTimes >;
    tableName = "entry_exit_times";
    entrExtTmsQuery->setTableName(&tableName);
}


EntryExitTimesGateway::~EntryExitTimesGateway() {
}

QVector< EntryExitTimes > EntryExitTimesGateway::findAll(){
    addFields2Select();
    selectCriteria = "";
    entrExtTmsQuery->setSelect(&selectFields, selectCriteria);
    if (entrExtTmsQuery->select())
        createEntExtTimesVec();
    else
        createTable();
    return *entExtTimes;
}

void EntryExitTimesGateway::createEntExtTimesVec(){
    int rows = entrExtTmsQuery->getRowCount();
    if (!entExtTimes->empty()) {
        entExtTimes->clear();
    }

    EntryExitTimes tempEntExtTms;
    for (int i = 0; i < rows; ++i){
        tempEntExtTms.setIpAddress(entrExtTmsQuery->getString(0, i));
        tempEntExtTms.setEntryTime(QDateTime::fromString(entrExtTmsQuery->getString(1, i), "dd.MM.yyyy hh:mm:ss"));
        tempEntExtTms.setExitTime(QDateTime::fromString(entrExtTmsQuery->getString(2, i), "dd.MM.yyyy hh:mm:ss"));
        tempEntExtTms.setTime(entrExtTmsQuery->getInt(3, i));

        entExtTimes->push_back(tempEntExtTms);
    }
}

void EntryExitTimesGateway::addFields2Select(){
    selectFields.clear();
    selectFields.push_back("ip_address");
    selectFields.push_back("entry_time");
    selectFields.push_back("exit_time");
    selectFields.push_back("time");
}

void EntryExitTimesGateway::addFields2Insert(){
    insertFields.clear();
    insertFields.push_back("ip_address");
    insertFields.push_back("entry_time");
    insertFields.push_back("exit_time");
    insertFields.push_back("time");
}

void EntryExitTimesGateway::addFields2Modify(){
    modifyFields.clear();
    modifyFields.push_back("ip_address");
    modifyFields.push_back("entry_time");
    modifyFields.push_back("exit_time");
    modifyFields.push_back("time");
}

void EntryExitTimesGateway::createTable()
{
    addFields2Create();
    entrExtTmsQuery->setCreate(&newFields);
    entrExtTmsQuery->create();
    entrExtTmsQuery->commit();
}

void EntryExitTimesGateway::addFields2Create()
{
    appendField2NewFields("ip_address", "VARCHAR(15)", "127.0.0.1", false, true, false);
    appendField2NewFields("entry_time", "DATETIME", "", false, false, false);
    appendField2NewFields("exit_time", "DATETIME", "", false, false, false);
    appendField2NewFields("time", "INTEGER", "0", false, false, false);
}

void EntryExitTimesGateway::appendField2NewFields(QString fieldName, QString type,
                                            QString defaultValue, bool notNull,
                                            bool primaryKey, bool autoIncrement)
{
    NewSqlTableField newIt;
    newIt.setFieldName(fieldName);
    newIt.setType(type);
    newIt.setDefaultValue(defaultValue);
    newIt.setNotNull(notNull);
    newIt.setPrimaryKey(primaryKey);
    newIt.setAutoIncrement(autoIncrement);
    newFields.push_back(newIt);
}

QVector< EntryExitTimes > EntryExitTimesGateway::findWhere( QString findCriteria ) {
    addFields2Select();
    entrExtTmsQuery->setSelect(&selectFields, findCriteria);
    entrExtTmsQuery->select();
    createEntExtTimesVec();
    return *entExtTimes;
}

void EntryExitTimesGateway::insertAll() {
    addFields2Insert();
    QList< QString > values;
    QVector< EntryExitTimes >::iterator entrExtTmsI;
    if (entrExtTmsQuery->getRowCount() > 0){
        for (entrExtTmsI = entExtTimes->begin(); entrExtTmsI != entExtTimes->end(); ++entrExtTmsI){
            values.push_back(entrExtTmsI->getIpAddress());
            values.push_back(entrExtTmsI->getEntryTime().toString("dd.MM.yyyy hh:mm:ss"));
            values.push_back(entrExtTmsI->getExitTime().toString("dd.MM.yyyy hh:mm:ss"));
            values.push_back(QString::number(entrExtTmsI->getTime()));
            entrExtTmsQuery->setInsert(&insertFields, &values);
            entrExtTmsQuery->insert();
            values.clear();
        }
    }
}

void EntryExitTimesGateway::insertId(QVector< EntryExitTimes >::iterator entrExtTmsI){
    addFields2Insert();
    QList< QString > values;
    values.push_back(entrExtTmsI->getIpAddress());
    values.push_back(entrExtTmsI->getEntryTime().toString("dd.MM.yyyy hh:mm:ss"));
    values.push_back(entrExtTmsI->getExitTime().toString("dd.MM.yyyy hh:mm:ss"));
    values.push_back(QString::number(entrExtTmsI->getTime()));
    entrExtTmsQuery->setInsert(&insertFields, &values);
    entrExtTmsQuery->insert();
    values.clear();
}

void EntryExitTimesGateway::modify(QVector< EntryExitTimes >::iterator entrExtTmsI ){
    addFields2Modify();
    Criteria* modCr = new Criteria();
    QString field = "entry_exit_times";
    QString sign = "=";
    QString val = entrExtTmsI->getIpAddress();
    modCr->addCriteria(&field, &val, &sign);
    modifyCriteria = modCr->getCriteria();

    QList< QString > values;
    values.push_back(entrExtTmsI->getIpAddress());
    values.push_back(entrExtTmsI->getEntryTime().toString("dd.MM.yyyy hh:mm:ss"));
    values.push_back(entrExtTmsI->getExitTime().toString("dd.MM.yyyy hh:mm:ss"));
    values.push_back(QString::number(entrExtTmsI->getTime()));
    entrExtTmsQuery->setModify(&modifyFields, modifyCriteria, &values);
    entrExtTmsQuery->modify();
    values.clear();
}

void EntryExitTimesGateway::del( QString ipAddress ){
    Criteria* modCr = new Criteria();
    QString field = "entry_exit_times";
    QString sign = "=";
    QString val = ipAddress;
    modCr->addCriteria(&field, &val, &sign);
    modifyCriteria = modCr->getCriteria();

    QVector< EntryExitTimes >::iterator entrExtTmsI;
    if (entrExtTmsQuery->getRowCount() > 0){
        for (entrExtTmsI = entExtTimes->begin(); entrExtTmsI != entExtTimes->end(); ++entrExtTmsI){
            if (ipAddress == entrExtTmsI->getIpAddress()){
                entrExtTmsQuery->setDelete(&modifyCriteria);
                entrExtTmsQuery->del();
                break;
            }
        }
    }
}

void EntryExitTimesGateway::commit(){
    entrExtTmsQuery->commit();
}
