#ifndef DATABASE_H
#define	DATABASE_H

#include "MySQLConnection.h"

#include <boost/shared_ptr.hpp>

class DataBase {
public:
    DataBase();
    DataBase(QString urlAddress,
             QString dbName,
             QString login,
             QString password);
    DataBase(const DataBase& orig);
    virtual ~DataBase();
    bool disconnectFromBase();
    bool connectToBase();
    void setAutoCommit(bool value);

    void setQuery(QString* sql);

    void commit();
    void rollback();
    DataBase getDB();
    void host() const;
    QString urlAddress() const;
    QString dbName() const;
    QString login() const;
    QString password() const;
    
private:
    boost::shared_ptr< MySQLConnection > myConn;
    QString* m_urlAddress;
    QString* m_dbName;
    QString* m_login;
    QString* m_password;
    
    int rowCount;
};

#endif	/* DATABASE_H */
