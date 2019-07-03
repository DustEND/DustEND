#include "generalsettingsgateway.h"
#include "DB/newsqltablefield.h"


GeneralSettingsGateway::GeneralSettingsGateway(const GeneralSettingsGateway& orig) {
    this->tableName = orig.tableName;
    this->selectFields = orig.selectFields;
    this->selectCriteria = orig.selectCriteria;
}

GeneralSettingsGateway::GeneralSettingsGateway(){
    genSetQuery = new QueryObj();
    genSetCriteria = new Criteria();

    genSets = new QVector< GeneralSettings >;
    tableName = "general_settings";
    genSetQuery->setTableName(&tableName);
}


GeneralSettingsGateway::~GeneralSettingsGateway() {
}

QVector< GeneralSettings > GeneralSettingsGateway::findAll(){
    addFields2Select();
    selectCriteria = "";
    genSetQuery->setSelect(&selectFields, selectCriteria);
    if (genSetQuery->select())
        createGenSetVec();
    else
        createTable();
    return *genSets;
}

void GeneralSettingsGateway::createGenSetVec(){
    int rows = genSetQuery->getRowCount();
    if (!genSets->empty())
        genSets->clear();

    GeneralSettings tempGenSet;
    for (int i = 0; i < rows; ++i){        
        tempGenSet.setId(genSetQuery->getInt(0, i));
        tempGenSet.setLocale(genSetQuery->getString(1, i));
        tempGenSet.setName(genSetQuery->getBool(2, i));
        tempGenSet.setIpAddress(genSetQuery->getBool(3, i));
        tempGenSet.setCleanTime(genSetQuery->getBool(4, i));
        tempGenSet.setFilterTime(genSetQuery->getBool(5, i));
        tempGenSet.setOs(genSetQuery->getBool(6, i));
        tempGenSet.setTime(genSetQuery->getBool(7, i));
        tempGenSet.setStatus(genSetQuery->getBool(8, i));
        tempGenSet.setWarningInterval(genSetQuery->getInt(9, i));
        tempGenSet.setSendMail(genSetQuery->getBool(10, i));
        tempGenSet.setEmailAddress(genSetQuery->getString(11, i));        
        tempGenSet.setAutoStartApp(genSetQuery->getBool(12, i));

        genSets->push_back(tempGenSet);
    }
}

void GeneralSettingsGateway::addFields2Select(){
    selectFields.clear();
    selectFields.push_back("id");
    selectFields.push_back("locale");
    selectFields.push_back("name");
    selectFields.push_back("ip_address");
    selectFields.push_back("clean_time");
    selectFields.push_back("filter_time");
    selectFields.push_back("os");
    selectFields.push_back("time");
    selectFields.push_back("status");
    selectFields.push_back("warning_interval");
    selectFields.push_back("send_mail");
    selectFields.push_back("email_address");
    selectFields.push_back("autostart_app");
}

void GeneralSettingsGateway::addFields2Insert(){
    insertFields.clear();
    insertFields.push_back("locale");
    insertFields.push_back("name");
    insertFields.push_back("ip_address");
    insertFields.push_back("clean_time");
    insertFields.push_back("filter_time");
    insertFields.push_back("os");
    insertFields.push_back("time");
    insertFields.push_back("status");
    insertFields.push_back("warning_interval");
    insertFields.push_back("send_mail");
    insertFields.push_back("email_address");
    insertFields.push_back("autostart_app");
}

void GeneralSettingsGateway::createTable()
{
    addFields2Create();
    genSetQuery->setCreate(&newFields);
    genSetQuery->create();
    genSetQuery->commit();
}

void GeneralSettingsGateway::addFields2Create()
{
    appendField2NewFields("id", "INTEGER", "", false, true, true);
    appendField2NewFields("locale", "VARCHAR(3)", "en", false, false, false);
    appendField2NewFields("name", "BOOLEAN", "true", false, false, false);
    appendField2NewFields("ip_address", "BOOLEAN", "false", false, false, false);
    appendField2NewFields("clean_time", "BOOLEAN", "false", false, false, false);
    appendField2NewFields("filter_time", "BOOLEAN", "false", false, false, false);
    appendField2NewFields("os", "BOOLEAN", "false", false, false, false);
    appendField2NewFields("time", "BOOLEAN", "true", false, false, false);
    appendField2NewFields("status", "BOOLEAN", "true", false, false, false);
    appendField2NewFields("warning_interval", "INTEGER", "30", false, false, false);
    appendField2NewFields("send_mail", "BOOLEAN", "false", false, false, false);
    appendField2NewFields("email_address", "VARCHAR(50)", "", false, false, false);
    appendField2NewFields("autostart_app", "BOOLEAN", "false", false, false, false);
}

void GeneralSettingsGateway::appendField2NewFields(QString fieldName, QString type,
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

QVector< GeneralSettings > GeneralSettingsGateway::findWhere( QString findCriteria ){
    addFields2Select();
    genSetQuery->setSelect(&selectFields, findCriteria);
    genSetQuery->select();
    createGenSetVec();
    return *genSets;
}

void GeneralSettingsGateway::insertAll(){
    addFields2Insert();
    QList< QString > values;
    QVector< GeneralSettings >::iterator uI;
    if (genSetQuery->getRowCount() > 0){
        for (uI = genSets->begin(); uI != genSets->end(); ++uI){
            values.push_back(uI->getLocale());
            uI->getName() ? values.push_back("true") : values.push_back("false");
            uI->getIpAddress() ? values.push_back("true") : values.push_back("false");
            uI->getCleanTime() ? values.push_back("true") : values.push_back("false");
            uI->getFilterTime() ? values.push_back("true") : values.push_back("false");
            uI->getOs() ? values.push_back("true") : values.push_back("false");
            uI->getTime() ? values.push_back("true") : values.push_back("false");
            uI->getStatus() ? values.push_back("true") : values.push_back("false");
            values.push_back(QString::number(uI->getWarningInterval()));
            uI->getSendMail() ? values.push_back("true") : values.push_back("false");
            values.push_back(uI->getEmailAddress());
            uI->getAutoStartApp() ? values.push_back("true") : values.push_back("false");
            genSetQuery->setInsert(&insertFields, &values);
            genSetQuery->insert();
            values.clear();
        }
    }
}

void GeneralSettingsGateway::insertId(QVector< GeneralSettings >::iterator genSetIt){
    addFields2Insert();
    QList< QString > values;
    values.push_back(genSetIt->getLocale());
    genSetIt->getName() ? values.push_back("true") : values.push_back("false");
    genSetIt->getIpAddress() ? values.push_back("true") : values.push_back("false");
    genSetIt->getCleanTime() ? values.push_back("true") : values.push_back("false");
    genSetIt->getFilterTime() ? values.push_back("true") : values.push_back("false");
    genSetIt->getOs() ? values.push_back("true") : values.push_back("false");
    genSetIt->getTime() ? values.push_back("true") : values.push_back("false");
    genSetIt->getStatus() ? values.push_back("true") : values.push_back("false");
    values.push_back(QString::number(genSetIt->getWarningInterval()));
    genSetIt->getSendMail() ? values.push_back("true") : values.push_back("false");
    values.push_back(genSetIt->getEmailAddress());
    genSetIt->getAutoStartApp() ? values.push_back("true") : values.push_back("false");
    genSetQuery->setInsert(&insertFields, &values);
    genSetQuery->insert();
    values.clear();
}

void GeneralSettingsGateway::modify(QVector< GeneralSettings >::iterator genSetIt ){
    addFields2Insert();
    Criteria* modCr = new Criteria();
    QString field = "id";
    QString sign = "=";
    QString val = QString::number(genSetIt->getId());
    modCr->addCriteria(&field, &val, &sign);
    modifyCriteria = modCr->getCriteria();

    QList< QString > values;
    values.push_back(genSetIt->getLocale());
    values.push_back(QString::number(genSetIt->getName()));
    values.push_back(QString::number(genSetIt->getIpAddress()));
    values.push_back(QString::number(genSetIt->getCleanTime()));
    values.push_back(QString::number(genSetIt->getFilterTime()));
    values.push_back(QString::number(genSetIt->getOs()));
    values.push_back(QString::number(genSetIt->getTime()));
    values.push_back(QString::number(genSetIt->getStatus()));
    values.push_back(QString::number(genSetIt->getWarningInterval()));
    values.push_back(QString::number(genSetIt->getSendMail()));
    values.push_back(genSetIt->getEmailAddress());
    values.push_back(QString::number(genSetIt->getAutoStartApp()));
    genSetQuery->setModify(&insertFields, modifyCriteria, &values);
    genSetQuery->modify();
    values.clear();
}

void GeneralSettingsGateway::del( int id ){
    Criteria* modCr = new Criteria();
    QString field = "id";
    QString sign = "=";
    QString val = QString::number(id);
    modCr->addCriteria(&field, &val, &sign);
    modifyCriteria = modCr->getCriteria();

    QVector< GeneralSettings >::iterator cI;
    if (genSetQuery->getRowCount() > 0){
        for (cI = genSets->begin(); cI != genSets->end(); ++cI){
            if (id == cI->getId()){
                genSetQuery->setDelete(&modifyCriteria);
                genSetQuery->del();
                break;
            }
        }
    }
}

void GeneralSettingsGateway::commit(){
    genSetQuery->commit();
}
