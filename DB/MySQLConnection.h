#ifndef MYSQLCONNECTION_H
#define	MYSQLCONNECTION_H

#include <QtSql/QSqlDatabase>

#include "AbstractConnection.h"

class MySQLConnection : public AbstractConnection {
public:
//    static MySQLConnection* getInstance();

//private:
    MySQLConnection();
    MySQLConnection(QString urlAddress,
                    QString dbName,
                    QString login,
                    QString password);
    ~MySQLConnection();
    MySQLConnection(const MySQLConnection& );
    MySQLConnection& operator=(const MySQLConnection& );

    bool connectToDBServer();
    bool disconnectFromBase();
    bool connectToBase();
    bool isClosed();
    void deleteVariables();
    QSqlDatabase currentDatabase() const;


    void commit();
    void rollback();

    std::string getStr(int rowNum, int colNum) const;
    int getInt(int rowNum, int colNum) const;
    double getDouble(int rowNum, int colNum) const;
    bool getBool(int rowNum, int colNum) const;



//    query->value(colNum).toChar().toLatin1();
//    query->value(colNum).toDate();
//    query->value(colNum).toList();
//    query->value(colNum).toMap();
//    query->value(colNum).toUInt();

//    query->value(colNum).toByteArray();
//    query->value(colNum).toTime();
//    query->value(colNum).toUrl();
protected: 	
    QSqlDatabase db;
    QString activeDb;
};
#endif	/* MYSQLCONNECTION_H */
