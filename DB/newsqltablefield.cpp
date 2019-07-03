#include "newsqltablefield.h"

NewSqlTableField::NewSqlTableField()
{
}

QString NewSqlTableField::getFieldName() const
{
    return fieldName;
}

void NewSqlTableField::setFieldName(QString value)
{
    this->fieldName = value;
}

QString NewSqlTableField::getType() const
{
    return type;
}

void NewSqlTableField::setType(QString value)
{
    this->type = value;
}

bool NewSqlTableField::getPrimaryKey() const
{
    return primaryKey;
}

void NewSqlTableField::setPrimaryKey(bool value)
{
    this->primaryKey = value;
}

bool NewSqlTableField::getAutoIncrement() const
{
    return autoIncrement;
}

void NewSqlTableField::setAutoIncrement(bool value)
{
    this->autoIncrement = value;
}

bool NewSqlTableField::getNotNull() const
{
    return notNull;
}

void NewSqlTableField::setNotNull(bool value)
{
    this->notNull = value;
}

QString NewSqlTableField::getDefaultValue() const
{
    return defaultValue;
}

void NewSqlTableField::setDefaultValue(QString value)
{
    this->defaultValue = value;
}
