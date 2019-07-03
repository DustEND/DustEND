#ifndef QUERYOBJ_H
#define	QUERYOBJ_H

#include <QtSql/QSqlQuery>

class NewSqlTableField;

class QueryObj {
public:  
    QueryObj();
    QueryObj(const QueryObj& orig);
    virtual ~QueryObj();
	
    void setTableName(QString* tableName);
    void setSelect(QList< QString >* selectFields, QString selectCriteria);
    void setInsert(QList< QString >* insertFields, QList< QString >* values);
    void setModify(QList< QString >* modifyFields, QString modifyCriteria,
                   QList< QString >* values);
    void setDelete(QString* deleteCriteria);
    void setCreate(QList< NewSqlTableField >* fields);
    int getRowCount();
    
    bool select();
    void insert();
    void modify();
    void del();    
    void commit();
    bool create();

    QString getString(int fieldNumber, int recordNumber);
    int getInt(int fieldNumber, int recordNumber);
    double getDouble(int fieldNumber, int recordNumber);
    bool getBool(int fieldNumber, int recordNumber);
    //BLOB getBlob(int fieldNumber, int recordNumber);


private:
    QList< QString >* selectFields;
    QList< QString >* insertFields;
    QList< QString >* modifyFields;
    QString* selectCriteria;
    QString* modifyCriteria;
    QString* deleteCriteria;

    QString selectSql;
    QString insertSql;
    QString modifySql;
    QString deleteSql;
    QString tableName;
    QString createSql;

    QSqlQuery* query;
};

#endif	/* QUERYOBJ_H */

