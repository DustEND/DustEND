#ifndef NEWSQLTABLEFIELD_H
#define NEWSQLTABLEFIELD_H

#include <QString>

class NewSqlTableField
{
public:
    NewSqlTableField();
    QString getFieldName() const;
    void setFieldName(QString value);
    QString getType() const;
    void setType(QString value);
    bool getPrimaryKey() const;
    void setPrimaryKey(bool value);
    bool getAutoIncrement() const;
    void setAutoIncrement(bool value);
    bool getNotNull() const;
    void setNotNull(bool value);
    QString getDefaultValue() const;
    void setDefaultValue(QString value);

private:
    QString fieldName;
    QString type;
    bool primaryKey;
    bool autoIncrement;
    bool notNull;
    QString defaultValue;
};

#endif // NEWSQLTABLEFIELD_H
