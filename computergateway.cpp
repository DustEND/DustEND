#include "computergateway.h"
#include "DB/newsqltablefield.h"
#include <QDebug>

ComputerGateway::ComputerGateway(const ComputerGateway& orig) {
    this->tableName = orig.tableName;
    this->selectFields = orig.selectFields;
    this->selectCriteria = orig.selectCriteria;
}

ComputerGateway::ComputerGateway(){
    compQuery = new QueryObj();
    compCriteria = new Criteria();

    comps = new QVector< Computer >;
    tableName = "computer";
    compQuery->setTableName(&tableName);
}


ComputerGateway::~ComputerGateway() {
    qDebug() << "compGateway destructor 1";
}

QVector< Computer > ComputerGateway::findAll(){
    addFields2Select();
    selectCriteria = "";
    compQuery->setSelect(&selectFields, selectCriteria);
    if (compQuery->select())
        createCompVec();
    else
        createTable();
    return *comps;
}

void ComputerGateway::createCompVec(){
    int rows = compQuery->getRowCount();
    if (!comps->empty()) {
        comps->clear();
//        comps->erase(std::remove(comps->begin(), comps->end()), comps->end());
    }

    Computer tempComp;
    for (int i = 0; i < rows; ++i){
        tempComp.setId(compQuery->getInt(0, i));
        tempComp.setIpAddress(compQuery->getString(1, i));
        tempComp.setName(compQuery->getString(2, i));
        tempComp.setOsName(compQuery->getString(3, i));
        tempComp.setPlaceType(compQuery->getInt(4, i));
        tempComp.setOutLeftCoolerAmount(compQuery->getInt(5, i));
        tempComp.setOutTopCoolerAmount(compQuery->getInt(6, i));
        tempComp.setInRightCoolerAmount(compQuery->getInt(7, i));
        tempComp.setInBottomCoolerAmount(compQuery->getInt(8, i));
        tempComp.setInSideCoolerAmount(compQuery->getInt(9, i));
        tempComp.setOutLeftCoolerDiameter(compQuery->getInt(10, i));
        tempComp.setOutTopCoolerDiameter(compQuery->getInt(11, i));
        tempComp.setInRightCoolerDiameter(compQuery->getInt(12, i));
        tempComp.setInBottomCoolerDiameter(compQuery->getInt(13, i));
        tempComp.setInSideCoolerDiameter(compQuery->getInt(14, i));
        tempComp.setMaxCleanTime(compQuery->getInt(15, i));
        tempComp.setMaxFilterTime(compQuery->getInt(16, i));
        tempComp.setMaxFanSpeed(compQuery->getInt(17, i));
        tempComp.setUsageTime(compQuery->getInt(18, i));
        tempComp.setDateOfFirstStart(QDateTime::fromString(compQuery->getString(19, i), "dd.MM.yyyy hh:mm:ss"));
        tempComp.setMaxTemperature(compQuery->getInt(20, i));
        tempComp.setCleanTimeIsRunning(compQuery->getBool(21, i));
        tempComp.setFilterTimeIsRunning(compQuery->getBool(22, i));
        tempComp.setCleanTime(QDateTime::fromString(compQuery->getString(23, i)));
        tempComp.setFilterTime(QDateTime::fromString(compQuery->getString(24, i)));
        tempComp.setCompIsPaused(compQuery->getBool(25, i));
        tempComp.setLastMaxTime(compQuery->getString(26, i));

        comps->push_back(tempComp);
    }
}

void ComputerGateway::addFields2Select(){
    selectFields.clear();
    selectFields.push_back("id");
    selectFields.push_back("ip_address");
    selectFields.push_back("name");
    selectFields.push_back("os_name");
    selectFields.push_back("place_type");
    selectFields.push_back("out_left_cooler_amount");
    selectFields.push_back("out_top_cooler_amount");
    selectFields.push_back("in_right_cooler_amount");
    selectFields.push_back("in_bottom_cooler_amount");
    selectFields.push_back("in_side_cooler_amount");
    selectFields.push_back("out_left_cooler_diameter");
    selectFields.push_back("out_top_cooler_diameter");
    selectFields.push_back("in_right_cooler_diameter");
    selectFields.push_back("in_bottom_cooler_diameter");
    selectFields.push_back("in_side_cooler_diameter");
    selectFields.push_back("max_clean_time");
    selectFields.push_back("max_filter_time");
    selectFields.push_back("max_fan_speed");
    selectFields.push_back("usage_time");
    selectFields.push_back("date_of_first_start");
    selectFields.push_back("max_temperature");
    selectFields.push_back("clean_time_is_running");
    selectFields.push_back("filter_time_is_running");
    selectFields.push_back("clean_time");
    selectFields.push_back("filter_time");
    selectFields.push_back("comp_is_paused");
    selectFields.push_back("last_max_time");
}

void ComputerGateway::addFields2Insert(){
    insertFields.clear();
    insertFields.push_back("id");
    insertFields.push_back("ip_address");
    insertFields.push_back("name");
    insertFields.push_back("os_name");
    insertFields.push_back("place_type");
    insertFields.push_back("out_left_cooler_amount");
    insertFields.push_back("out_top_cooler_amount");
    insertFields.push_back("in_right_cooler_amount");
    insertFields.push_back("in_bottom_cooler_amount");
    insertFields.push_back("in_side_cooler_amount");
    insertFields.push_back("out_left_cooler_diameter");
    insertFields.push_back("out_top_cooler_diameter");
    insertFields.push_back("in_right_cooler_diameter");
    insertFields.push_back("in_bottom_cooler_diameter");
    insertFields.push_back("in_side_cooler_diameter");
    insertFields.push_back("max_clean_time");
    insertFields.push_back("max_filter_time");
    insertFields.push_back("max_fan_speed");
    insertFields.push_back("usage_time");
    insertFields.push_back("date_of_first_start");
    insertFields.push_back("max_temperature");
    insertFields.push_back("clean_time_is_running");
    insertFields.push_back("filter_time_is_running");
    insertFields.push_back("clean_time");
    insertFields.push_back("filter_time");
    insertFields.push_back("comp_is_paused");
    insertFields.push_back("last_max_time");
}

void ComputerGateway::addFields2Modify(){
    modifyFields.clear();
    modifyFields.push_back("ip_address");
    modifyFields.push_back("name");
    modifyFields.push_back("os_name");
    modifyFields.push_back("place_type");
    modifyFields.push_back("out_left_cooler_amount");
    modifyFields.push_back("out_top_cooler_amount");
    modifyFields.push_back("in_right_cooler_amount");
    modifyFields.push_back("in_bottom_cooler_amount");
    modifyFields.push_back("in_side_cooler_amount");
    modifyFields.push_back("out_left_cooler_diameter");
    modifyFields.push_back("out_top_cooler_diameter");
    modifyFields.push_back("in_right_cooler_diameter");
    modifyFields.push_back("in_bottom_cooler_diameter");
    modifyFields.push_back("in_side_cooler_diameter");
    modifyFields.push_back("max_clean_time");
    modifyFields.push_back("max_filter_time");
    modifyFields.push_back("max_fan_speed");
    modifyFields.push_back("usage_time");
    modifyFields.push_back("date_of_first_start");
    modifyFields.push_back("max_temperature");
    modifyFields.push_back("clean_time_is_running");
    modifyFields.push_back("filter_time_is_running");
    modifyFields.push_back("clean_time");
    modifyFields.push_back("filter_time");
    modifyFields.push_back("comp_is_paused");
    modifyFields.push_back("last_max_time");
}

void ComputerGateway::createTable()
{
    addFields2Create();
    compQuery->setCreate(&newFields);
    compQuery->create();
    compQuery->commit();
}

void ComputerGateway::addFields2Create()
{
    appendField2NewFields("id", "INTEGER", "", false, true, true);
    appendField2NewFields("ip_address", "VARCHAR(15)", "127.0.0.1", false, false, false);
    appendField2NewFields("name", "VARCHAR(50)", "Your computer", false, false, false);
    appendField2NewFields("os_name", "VARCHAR(50)", "Windows XP", false, false, false);
    appendField2NewFields("place_type", "INTEGER", "1", false, false, false);
    appendField2NewFields("out_left_cooler_amount", "INTEGER", "0", false, false, false);
    appendField2NewFields("out_top_cooler_amount", "INTEGER", "0", false, false, false);
    appendField2NewFields("in_right_cooler_amount", "INTEGER", "0", false, false, false);
    appendField2NewFields("in_bottom_cooler_amount", "INTEGER", "0", false, false, false);
    appendField2NewFields("in_side_cooler_amount", "INTEGER", "0", false, false, false);
    appendField2NewFields("out_left_cooler_diameter", "INTEGER", "120", false, false, false);
    appendField2NewFields("out_top_cooler_diameter", "INTEGER", "120", false, false, false);
    appendField2NewFields("in_right_cooler_diameter", "INTEGER", "120", false, false, false);
    appendField2NewFields("in_bottom_cooler_diameter", "INTEGER", "120", false, false, false);
    appendField2NewFields("in_side_cooler_diameter", "INTEGER", "120", false, false, false);
    appendField2NewFields("max_clean_time", "LARGEINT", "4680000", false, false, false);
    appendField2NewFields("max_filter_time", "LARGEINT", "9360000", false, false, false);
    appendField2NewFields("max_fan_speed", "INTEGER", "2000", false, false, false);
    appendField2NewFields("usage_time", "LARGEINT", "0", false, false, false);
    appendField2NewFields("date_of_first_start", "DATETIME", "24.10.1795 12:12:12", false, false, false);
    appendField2NewFields("max_temperature", "INTEGER", "70", false, false, false);
    appendField2NewFields("clean_time_is_running", "BOOLEAN", "false", false, false, false);
    appendField2NewFields("filter_time_is_running", "BOOLEAN", "false", false, false, false);
    appendField2NewFields("clean_time", "DATETIME", "", false, false, false);
    appendField2NewFields("filter_time", "DATETIME", "", false, false, false);
    appendField2NewFields("comp_is_paused", "BOOLEAN", "false", false, false, false);
    appendField2NewFields("last_max_time", "VARCHAR(30)", "1300", false, false, false);
}

void ComputerGateway::appendField2NewFields(QString fieldName, QString type,
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

QVector< Computer > ComputerGateway::findWhere( QString findCriteria ) {
    addFields2Select();
    compQuery->setSelect(&selectFields, findCriteria);
    compQuery->select();
    createCompVec();
    return *comps;
}

void ComputerGateway::insertAll() {
    addFields2Insert();
    QList< QString > values;
    QVector< Computer >::iterator compI;
    if (compQuery->getRowCount() > 0){
        for (compI = comps->begin(); compI != comps->end(); ++compI){
            values.push_back(compI->getIpAddress());
            values.push_back(compI->getName());
            values.push_back(compI->getOsName());
            values.push_back(QString::number(compI->getPlaceType()));
            values.push_back(QString::number(compI->getOutLeftCoolerAmount()));
            values.push_back(QString::number(compI->getOutTopCoolerAmount()));
            values.push_back(QString::number(compI->getInRightCoolerAmount()));
            values.push_back(QString::number(compI->getInBottomCoolerAmount()));
            values.push_back(QString::number(compI->getInSideCoolerAmount()));
            values.push_back(QString::number(compI->getOutLeftCoolerDiameter()));
            values.push_back(QString::number(compI->getOutTopCoolerDiameter()));
            values.push_back(QString::number(compI->getInRightCoolerDiameter()));
            values.push_back(QString::number(compI->getInBottomCoolerDiameter()));
            values.push_back(QString::number(compI->getInSideCoolerDiameter()));
            values.push_back(QString::number(compI->getMaxCleanTime()));
            values.push_back(QString::number(compI->getMaxFilterTime()));
            values.push_back(QString::number(compI->getMaxFanSpeed()));
            values.push_back(QString::number(compI->getUsageTime()));
            values.push_back(compI->getDateOfFirstStart().toString("dd.MM.yyyy hh:mm:ss"));
            values.push_back(QString::number(compI->getMaxTemperature()));
            compI->getCleanTimeIsRunning() ? values.push_back("true") : values.push_back("false");
            compI->getFilterTimeIsRunning() ? values.push_back("true") : values.push_back("false");
            values.push_back(compI->getCleanTime().toString("dd.MM.yyyy hh:mm:ss"));
            values.push_back(compI->getFilterTime().toString("dd.MM.yyyy hh:mm:ss"));
            compI->getCompIsPaused() ? values.push_back("true") : values.push_back("false");
            values.push_back(compI->getLastMaxTime());
            compQuery->setInsert(&insertFields, &values);
            compQuery->insert();
            values.clear();
        }
    }
}

void ComputerGateway::insertId(QVector< Computer >::iterator compI){
    addFields2Insert();
    QList< QString > values;
    values.push_back(QString::number(compI->getId()));
    values.push_back(compI->getIpAddress());
    values.push_back(compI->getName());
    values.push_back(compI->getOsName());
    values.push_back(QString::number(compI->getPlaceType()));
    values.push_back(QString::number(compI->getOutLeftCoolerAmount()));
    values.push_back(QString::number(compI->getOutTopCoolerAmount()));
    values.push_back(QString::number(compI->getInRightCoolerAmount()));
    values.push_back(QString::number(compI->getInBottomCoolerAmount()));
    values.push_back(QString::number(compI->getInSideCoolerAmount()));
    values.push_back(QString::number(compI->getOutLeftCoolerDiameter()));
    values.push_back(QString::number(compI->getOutTopCoolerDiameter()));
    values.push_back(QString::number(compI->getInRightCoolerDiameter()));
    values.push_back(QString::number(compI->getInBottomCoolerDiameter()));
    values.push_back(QString::number(compI->getInSideCoolerDiameter()));
    values.push_back(QString::number(compI->getMaxCleanTime()));
    values.push_back(QString::number(compI->getMaxFilterTime()));
    values.push_back(QString::number(compI->getMaxFanSpeed()));
    values.push_back(QString::number(compI->getUsageTime()));
    values.push_back(compI->getDateOfFirstStart().toString("dd.MM.yyyy hh:mm:ss"));
    values.push_back(QString::number(compI->getMaxTemperature()));    
    compI->getCleanTimeIsRunning() ? values.push_back("true") : values.push_back("false");
    compI->getFilterTimeIsRunning() ? values.push_back("true") : values.push_back("false");
    values.push_back(compI->getCleanTime().toString("dd.MM.yyyy hh:mm:ss"));
    values.push_back(compI->getFilterTime().toString("dd.MM.yyyy hh:mm:ss"));
    compI->getCompIsPaused() ? values.push_back("true") : values.push_back("false");
    values.push_back(compI->getLastMaxTime());
    compQuery->setInsert(&insertFields, &values);
    compQuery->insert();
    values.clear();
}

void ComputerGateway::modify(QVector< Computer >::iterator compI ){
    addFields2Modify();
    Criteria* modCr = new Criteria();
    QString field = "id";
    QString sign = "=";
    QString val = QString::number(compI->getId());
    modCr->addCriteria(&field, &val, &sign);
    modifyCriteria = modCr->getCriteria();

    QList< QString > values;
    values.push_back(compI->getIpAddress());
    values.push_back(compI->getName());
    values.push_back(compI->getOsName());
    values.push_back(QString::number(compI->getPlaceType()));
    values.push_back(QString::number(compI->getOutLeftCoolerAmount()));
    values.push_back(QString::number(compI->getOutTopCoolerAmount()));
    values.push_back(QString::number(compI->getInRightCoolerAmount()));
    values.push_back(QString::number(compI->getInBottomCoolerAmount()));
    values.push_back(QString::number(compI->getInSideCoolerAmount()));
    values.push_back(QString::number(compI->getOutLeftCoolerDiameter()));
    values.push_back(QString::number(compI->getOutTopCoolerDiameter()));
    values.push_back(QString::number(compI->getInRightCoolerDiameter()));
    values.push_back(QString::number(compI->getInBottomCoolerDiameter()));
    values.push_back(QString::number(compI->getInSideCoolerDiameter()));
    values.push_back(QString::number(compI->getMaxCleanTime()));
    values.push_back(QString::number(compI->getMaxFilterTime()));
    values.push_back(QString::number(compI->getMaxFanSpeed()));
    values.push_back(QString::number(compI->getUsageTime()));
    values.push_back(compI->getDateOfFirstStart().toString("dd.MM.yyyy hh:mm:ss"));
    values.push_back(QString::number(compI->getMaxTemperature()));
    compI->getCleanTimeIsRunning() ? values.push_back("true") : values.push_back("false");
    compI->getFilterTimeIsRunning() ? values.push_back("true") : values.push_back("false");
    values.push_back(compI->getCleanTime().toString("dd.MM.yyyy hh:mm:ss"));
    values.push_back(compI->getFilterTime().toString("dd.MM.yyyy hh:mm:ss"));
    compI->getCompIsPaused() ? values.push_back("true") : values.push_back("false");
    values.push_back(compI->getLastMaxTime());
    compQuery->setModify(&modifyFields, modifyCriteria, &values);
    compQuery->modify();
    values.clear();
}

void ComputerGateway::del( int id ){
    Criteria* modCr = new Criteria();
    QString field = "id";
    QString sign = "=";
    QString val = QString::number(id);
    modCr->addCriteria(&field, &val, &sign);
    modifyCriteria = modCr->getCriteria();

    compQuery->setDelete(&modifyCriteria);
    compQuery->del();
}

void ComputerGateway::commit(){
    compQuery->commit();
}
