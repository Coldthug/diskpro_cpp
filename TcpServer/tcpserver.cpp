#include "tcpserver.h"
#include "ui_tcpserver.h"
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>


TcpServer::TcpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpServer)
{
    ui->setupUi(this);

    loadConfig();

    MyTcpServer::getInstance().listen(QHostAddress(m_strIP),m_usPort);
}

TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::loadConfig()
{
    QFile file(":/server.config");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray baData= file.readAll();
        QString strData = baData.toStdString().c_str();

        //
        strData.replace("\r\n"," "); //去除换行

        QStringList strList=strData.split(" "); // 分隔字符串
        for (int i=0;i<strList.size();i++) {
            qDebug()<<strList[i];
        }
        m_strIP=strList.at(0);
        m_usPort=strList.at(1).toUShort();
        qDebug()<<"ip:"<<m_strIP<<" port:"<<m_usPort;

        file.close();
    }else {
        QMessageBox::critical(this,"open config","open config failed");
    }
}
