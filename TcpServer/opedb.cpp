#include "opedb.h"
#include <QSqlError>
#include <QMessageBox>
#include<QDebug>

OpeDB::OpeDB(QObject *parent) : QObject(parent)
{
    m_db=QSqlDatabase::addDatabase("QSQLITE");
}

OpeDB &OpeDB::getInstance()
{
    static OpeDB instance;
    return instance;
}

void OpeDB::init()
{
    m_db.setHostName("localhost");
    m_db.setDatabaseName("C:\\Users\\Coldthug\\Desktop\\Diskpro.cpp\\TcpServer\\cloud.db");
//    qDebug()<<m_db.open();
    if(m_db.open()){
        QSqlQuery query;
        query.exec("select * from usrInfo");

        //检查错误，结果为路径错误
//        qDebug()<<query.next();
//        if (query.lastError().isValid()) {
//            qDebug() << "Database error: " << query.lastError().text();
//        } else {
//            qDebug() << "Query executed successfully!";
//        }
        while (query.next())
        {
            QString data = QString("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
            qDebug() << data;
        }

    }
    else {
        QMessageBox::critical(NULL,"打开数据库","打开数据库失败");
    }
}

OpeDB::~OpeDB()
{
    m_db.close();
}

bool OpeDB::handleRegist(const char *name, const char *pwd)
{
    if (NULL == name || NULL == pwd)
    {
        qDebug() << "name | pwd is NULL";
        return false;
    }
    QString data = QString("insert into usrInfo(name, pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    qDebug() << data;
    QSqlQuery query;
    return query.exec(data);
}

bool OpeDB::handleLogin(const char *name, const char *pwd)
{
    if (NULL == name || NULL == pwd)
    {
        qDebug() << "name | pwd is NULL";
        return false;
    }
    QString data = QString("select * from usrInfo where name=\'%1\' and pwd = \'%2\' and online=0").arg(name).arg(pwd);
    qDebug() << data;
    QSqlQuery query;
    query.exec(data);
    if (query.next())
    {
        data = QString("update usrInfo set online=1 where name=\'%1\' and pwd = \'%2\'").arg(name).arg(pwd);
        qDebug() << data;
        QSqlQuery query;
        query.exec(data);

        return true;
    }
    else
    {
        return false;
    }

}

void OpeDB::handleOffline(const char *name)
{
    if (NULL == name)
    {
        qDebug() << "name is NULL";
        return;
    }
    QString data = QString("update usrInfo set online=0 where name=\'%1\'").arg(name);

    QSqlQuery query;
    query.exec(data);
}


