#include "AbstractConnection.h"
#include <QString>

AbstractConnection::AbstractConnection(){
//    this->host = "";
//    this->port = 0;
//    this->dbName = "";
//    this->login = "";
//    this->password = "";
}

//AbstractConnection::AbstractConnection(char* host, 
//                       unsigned int port, 
//                       char* dbName, 
//                       char* login, 
//                       char* password){
//    this->host = host;
//    this->port = port;
//    this->dbName = dbName;
//    this->login = login;
//    this->password = password;
//}

AbstractConnection::~AbstractConnection(){
//    disconnectFromBase();
}

QString AbstractConnection::getUrlAddress(){
    return urlAddress;
}

QString AbstractConnection::getDBName(){
    return dbName;
}

QString AbstractConnection::getLogin(){
    return login;
}

QString AbstractConnection::getPassword(){
    return password;
}

void AbstractConnection::setUrlAddress(QString newUrlAddress){
    this->urlAddress = newUrlAddress;
}

void AbstractConnection::setDBName(QString newDBName){
    this->dbName = newDBName;
}

void AbstractConnection::setLogin(QString newLogin){
    this->login = newLogin;
}

void AbstractConnection::setPassword(QString newPassword){
    this->password = newPassword;
}
