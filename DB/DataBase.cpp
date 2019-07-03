#include "DataBase.h"
#include "MySQLConnection.h"

DataBase::DataBase() { 
     myConn.reset(new MySQLConnection());
}

DataBase::DataBase(QString urlAddress,
                   QString dbName,
                   QString login,
                   QString password)
    : m_urlAddress(&urlAddress),
      m_dbName(&dbName),
      m_login(&login),
      m_password(&password){
     myConn.reset(new MySQLConnection(/*(char*)*/urlAddress,
                                      dbName,
                                      login,
                                      password));    
}

DataBase::DataBase(const DataBase& orig) {
    this->myConn = orig.myConn;
}

DataBase::~DataBase() {
    delete m_urlAddress;
    delete m_dbName;
    delete m_login;
    delete m_password;
}

bool DataBase::connectToBase(){    
    if (myConn->connectToDBServer())
        return true;
    else
        return false;
}

bool DataBase::disconnectFromBase(){
    if (myConn->disconnectFromBase())
        return true;
    else
        return false;
}

void DataBase::commit(){
	myConn->commit();
}

void DataBase::rollback(){
	myConn->rollback();
}

DataBase DataBase::getDB(){
    return *this;
}

QString DataBase::urlAddress() const{
    return *(this->m_urlAddress);
}

QString DataBase::dbName() const{
    return *(this->m_dbName);
}

QString DataBase::login() const{
    return *(this->m_login);
}

QString DataBase::password() const{
    return *(this->m_password);
}
