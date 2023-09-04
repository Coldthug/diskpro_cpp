#include "tcpclient.h"
#include "ui_tcpclient.h"
#include "protocol.h"
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>
#include <QMessageBox>

TcpClient::TcpClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    loadConfig();

//    connect(ui->log_pb,SIGNAL(clicked()),this,SLOT(on_log_pb_clicked()));
//    connect(ui->reg_pb,SIGNAL(clicked()),this,SLOT(on_reg_pb_clicked()));
//    connect(ui->logout_pb,SIGNAL(clicked()),this,SLOT(on_logout_pb_clicked()));

    connect(&m_tcpScoket,SIGNAL(connected()),this,SLOT(showConnect())); // **bug**  connected拼错了
    connect(&m_tcpScoket,SIGNAL(readyRead()),this,SLOT(recvMsg()));
    // 连接服务器
    m_tcpScoket.connectToHost(QHostAddress(m_strIP),m_usPort);

}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::loadConfig()
{
    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray baData= file.readAll();
        QString strData = baData.toStdString().c_str();
        file.close();

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

void TcpClient::showConnect()
{
    QMessageBox::information(this,"连接服务器","连接服务器成功");
}

void TcpClient::recvMsg()
{
    qDebug()<< m_tcpScoket.bytesAvailable();

    uint uiPDULen=0;
    m_tcpScoket.read((char*)&uiPDULen,sizeof(uint));

    uint uiMsgLen=uiPDULen-sizeof(uint);
    PDU *pdu=mkPDU(uiMsgLen);
    m_tcpScoket.read((char*)pdu+sizeof (uint),uiPDULen-sizeof(uint));
    //qDebug() << pdu->uiMsgType << (char*)pdu->caMsg ;

    switch (pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_RESPOND:
    {
        if (0 == strcmp(pdu->caData, REGIST_OK))
        {
            QMessageBox::information(this, "注册", REGIST_OK);
        }
        else if (0 == strcmp(pdu->caData, REGIST_FAILED))
        {
            QMessageBox::warning(this, "注册", REGIST_FAILED);
        }
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND:
    {
        if (0 == strcmp(pdu->caData, LOGIN_OK))
        {
            QMessageBox::information(this, "登录", LOGIN_OK);
        }
        else if (0 == strcmp(pdu->caData, LOGIN_FAILED))
        {
            QMessageBox::warning(this, "登录", LOGIN_FAILED);
        }
        break;
    }
    default:
        break;
    }

    free(pdu);
    pdu = NULL;
}

#if 0
void TcpClient::on_send_pb_clicked()
{
    QString strMsg=ui->lineEdit->text();
    if(!strMsg.isEmpty()){
        PDU *pdu=mkPDU(strMsg.size()+1);
        pdu->uiMsgType=123;
        memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.size());
        qDebug()<<(char*)(pdu->caMsg);
        m_tcpScoket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }else {
        QMessageBox::warning(this,"发送信息","发送的信息不能为空");
    }
}
#endif

void TcpClient::on_log_pb_clicked()
{
    QString strName=ui->name_le->text();
    QString strPwd=ui->pwd_le->text();


    if(!strName.isEmpty() && !strPwd.isEmpty())
    {
//        QMessageBox::critical(NULL,"输入合法","");
        PDU *pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_REQUEST;
        strncpy(pdu->caData,strName.toStdString().c_str(),32);
        strncpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
        m_tcpScoket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
//        QMessageBox::critical(NULL,"输入合法","");
    }
    else
    {
        QMessageBox::critical(this, "登录", "登录失败:用户名或者密码为空");
    }
}

void TcpClient::on_reg_pb_clicked()
{
    QString strName=ui->name_le->text();
    QString strPwd=ui->pwd_le->text();


    if(!strName.isEmpty() && !strPwd.isEmpty())
    {
//        QMessageBox::critical(NULL,"输入合法","");
        PDU *pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caData,strName.toStdString().c_str(),32);
        strncpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
        m_tcpScoket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
//        QMessageBox::critical(NULL,"输入合法","");
    }
    else
    {
        QMessageBox::critical(this, "注册", "注册失败:用户名或者密码为空");
    }
}

void TcpClient::on_logout_pb_clicked()
{

}
