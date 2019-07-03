#include "MySQLConnection.h"

#include <QtSql/QtSql>
#include <QtWidgets/QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

//MySQLConnection* MySQLConnection::getInstance(){
//    static MySQLConnection instance;
//    return &instance;
//}

MySQLConnection::MySQLConnection(){
    this->urlAddress = "";
    this->dbName = (char*)"";
    this->login = (char*)"";
    this->password = (char*)"";

    qDebug() << "MySQLConnection" ;
}

MySQLConnection::MySQLConnection(QString urlAddress,
                                 QString dbName,
                                 QString login,
                                 QString password){
    this->urlAddress = urlAddress;    
    this->dbName = dbName;
    this->login = login;
    this->password = password;

    qDebug() <<  "MySQLConnection with params" ;
}

MySQLConnection::~MySQLConnection(){
    qDebug() << "~MySQLConnection" ;
    disconnectFromBase();
    deleteVariables();
}

bool MySQLConnection::connectToDBServer(){
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setHostName(urlAddress);
    db.setDatabaseName(dbName);
    db.setUserName(login);
    db.setPassword(password);

    if (!db.open()){
        qDebug()  << db.lastError().text();
        return false;
    }
    else{
        qDebug() << "Connected..." ;        
        activeDb = db.connectionName();            
        return true;
    }
}

QSqlDatabase MySQLConnection::currentDatabase() const{
    return QSqlDatabase::database(activeDb);
}

bool MySQLConnection::connectToBase(){
   if (db.isOpen()){
       return true;
   }
   else
       return false;
}

bool MySQLConnection::disconnectFromBase(){
    db.close();
    if (isClosed()){
        qDebug() << "MySQLConnection have been closed";
        return true;
    }
    else
        return false;    
}

bool MySQLConnection::isClosed(){    
    return !db.isOpen()?true:false;
}

void MySQLConnection::commit(){
    if (db.commit())
        qDebug() << "Saved";
    else
        qDebug() << "Error";
}

void MySQLConnection::rollback(){
        db.rollback();
}

void MySQLConnection::deleteVariables(){
}
