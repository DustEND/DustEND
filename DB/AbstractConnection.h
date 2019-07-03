#ifndef ABSTRACTCONNECTION_H
#define	ABSTRACTCONNECTION_H

#include <QString>

class AbstractConnection {
public:
    AbstractConnection();
    //AbstractConnection(char* host, 
    //                   unsigned int port, 
    //                   char* dbName, 
    //                   char* login, 
    //                   char* password);
    virtual ~AbstractConnection();
    
    QString getUrlAddress();
    void setUrlAddress(QString newUrlAddress);
    QString getDBName();
    void setDBName(QString newDBName);
    QString getLogin();
    void setLogin(QString newLogin);
    QString getPassword();
    void setPassword(QString newPassword);
    virtual bool connectToDBServer() = 0;
    virtual bool connectToBase() = 0;
    virtual bool disconnectFromBase() = 0;
    
protected:
    QString urlAddress;
    QString dbName;
    QString login;
    QString password;
};


#endif	/* ABSTRACTCONNECTION_H */

