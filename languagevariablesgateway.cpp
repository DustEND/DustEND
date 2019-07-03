#include "languagevariablesgateway.h"

#include "DB/newsqltablefield.h"
#include <QDebug>

LanguageVariablesGateway::LanguageVariablesGateway(const LanguageVariablesGateway& orig) {
    this->tableName = orig.tableName;
    this->selectFields = orig.selectFields;
    this->selectCriteria = orig.selectCriteria;
}

LanguageVariablesGateway::LanguageVariablesGateway(){
    langQuery = new QueryObj();
    langCriteria = new Criteria();

    langVars = new QVector< LanguageVariables >;
    tableName = "language_variables";
    langQuery->setTableName(&tableName);
}


LanguageVariablesGateway::~LanguageVariablesGateway() {
}

QVector< LanguageVariables > LanguageVariablesGateway::findAll(){
    addFields2Select();
    selectCriteria = "";
    langQuery->setSelect(&selectFields, selectCriteria);
    if (langQuery->select())
        createLangVarVec();
    else
        createTable();
    return *langVars;
}

void LanguageVariablesGateway::createLangVarVec(){
    int rows = langQuery->getRowCount();
    if (!langVars->empty()) {
        langVars->clear();
    }

    LanguageVariables tempLangVar;
    for (int i = 0; i < rows; ++i){
        tempLangVar.setVarName(langQuery->getString(0, i));
        tempLangVar.setEn(langQuery->getString(1, i));
        tempLangVar.setRu(langQuery->getString(2, i));

        langVars->push_back(tempLangVar);
    }
}

void LanguageVariablesGateway::addFields2Select(){
    selectFields.clear();
    selectFields.push_back("variable_name");
    selectFields.push_back("en");
    selectFields.push_back("ru");
}

void LanguageVariablesGateway::addFields2Insert(){
    insertFields.clear();
    insertFields.push_back("variable_name");
    insertFields.push_back("en");
    insertFields.push_back("ru");
}

void LanguageVariablesGateway::addFields2Modify(){
    modifyFields.clear();
    modifyFields.push_back("en");
    modifyFields.push_back("ru");
}

void LanguageVariablesGateway::createTable()
{
    addFields2Create();
    langQuery->setCreate(&newFields);
    langQuery->create();
    langQuery->commit();
}

void LanguageVariablesGateway::addFields2Create()
{
    appendField2NewFields("variable_name", "VARCHAR(50)", "", false, true, false);
    appendField2NewFields("en", "VARCHAR(1000)", "", false, false, false);
    appendField2NewFields("ru", "VARCHAR(1000)", "", false, false, false);
}

void LanguageVariablesGateway::appendField2NewFields(QString fieldName, QString type,
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

QVector< LanguageVariables > LanguageVariablesGateway::findWhere( QString findCriteria ) {
    addFields2Select();
    langQuery->setSelect(&selectFields, findCriteria);
    langQuery->select();
    createLangVarVec();
    return *langVars;
}

void LanguageVariablesGateway::insertAll() {
    addFields2Insert();
    QList< QString > values;
    QVector< LanguageVariables >::iterator langI;
    if (langQuery->getRowCount() > 0){
        for (langI = langVars->begin(); langI != langVars->end(); ++langI){
            values.push_back(langI->getVarName());
            values.push_back(langI->getEn());
            values.push_back(langI->getRu());
            langQuery->setInsert(&insertFields, &values);
            langQuery->insert();
            values.clear();
        }
    }
}

void LanguageVariablesGateway::insertId(QVector< LanguageVariables >::iterator langI){
    addFields2Insert();
    QList< QString > values;
    values.push_back(langI->getVarName());
    values.push_back(langI->getEn());
    values.push_back(langI->getRu());
    langQuery->setInsert(&insertFields, &values);
    langQuery->insert();
    values.clear();
}

void LanguageVariablesGateway::modify(QVector< LanguageVariables >::iterator langI ){
    addFields2Modify();
    Criteria* modCr = new Criteria();
    QString field = "variable_name";
    QString sign = "=";
    QString val = langI->getVarName();
    modCr->addCriteria(&field, &val, &sign);
    modifyCriteria = modCr->getCriteria();

    QList< QString > values;
    values.push_back(langI->getEn());
    values.push_back(langI->getRu());
    langQuery->setModify(&modifyFields, modifyCriteria, &values);
    langQuery->modify();
    values.clear();
}

void LanguageVariablesGateway::del( QString varName ){
    Criteria* modCr = new Criteria();
    QString field = "variable_name";
    QString sign = "=";
    QString val = varName;
    modCr->addCriteria(&field, &val, &sign);
    modifyCriteria = modCr->getCriteria();

    QVector< LanguageVariables >::iterator langI;
    if (langQuery->getRowCount() > 0){
        for (langI = langVars->begin(); langI != langVars->end(); ++langI){
            if (varName == langI->getVarName()){
                langQuery->setDelete(&modifyCriteria);
                langQuery->del();
                break;
            }
        }
    }
}

void LanguageVariablesGateway::commit(){
    langQuery->commit();
}

