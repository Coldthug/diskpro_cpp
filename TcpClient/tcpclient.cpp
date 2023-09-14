#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include "privatechat.h"

TcpClient::TcpClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    loadConfig();

    connect(ui->log_pb,SIGNAL(clicked()),this,SLOT(on_log_pb_clicked()));
    connect(ui->reg_pb,SIGNAL(clicked()),this,SLOT(on_reg_pb_clicked()));
    connect(ui->logout_pb,SIGNAL(clicked()),this,SLOT(on_logout_pb_clicked()));

    connect(&m_tcpSocket,SIGNAL(connected()),this,SLOT(showConnect())); // **bug**  connected拼错了
    connect(&m_tcpSocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));
    // 连接服务器
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort);

}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::loadConfig()
{
    QFile file(":/client.config");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray baData = file.readAll();
        QString strData = baData.toStdString().c_str();
        file.close();

        //
        strData.replace("\r\n"," "); //去除换行

        QStringList strList = strData.split(" ");

        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        qDebug() << "ip:" << m_strIP << " port:" << m_usPort;
    }
    else
    {
        QMessageBox::critical(this, "open config", "open config failed");
    }

}

TcpClient &TcpClient::getInstance()
{
    static TcpClient instance;
    return instance;
}

QTcpSocket &TcpClient::getTcpSocket()
{
    return m_tcpSocket;
}

QString TcpClient::loginName()
{
    return m_strLoginName;
}

void TcpClient::showConnect()
{
    QMessageBox::information(this,"连接服务器","连接服务器成功");
}

void TcpClient::recvMsg()
{
    qDebug() << m_tcpSocket.bytesAvailable();
    uint uiPDULen = 0;
    m_tcpSocket.read((char*)&uiPDULen, sizeof(uint));
    uint uiMsgLen = uiPDULen-sizeof(PDU);
    PDU *pdu = mkPDU(uiMsgLen);
    m_tcpSocket.read((char*)pdu+sizeof(uint), uiPDULen-sizeof(uint));
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
            OpeWidget::getInstance().show();
            hide();
        }
        else if (0 == strcmp(pdu->caData, LOGIN_FAILED))
        {
            QMessageBox::warning(this, "登录", LOGIN_FAILED);
        }
        break;
    }
    case ENUM_MSG_TYPE_ALL_ONLINE_RESPOND:
    {
        OpeWidget::getInstance().getFriend()->showAllOnlineUsr(pdu);
        break;
    }
    case ENUM_MSG_TYPE_SEARCH_USR_RESPOND:
    {
        if (0 == strcmp(SEARCH_USR_NO, pdu->caData))
        {
            QMessageBox::information(this, "搜索", QString("%1: not exist").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
        }
        else if (0 == strcmp(SEARCH_USR_ONLINE, pdu->caData))
        {
            QMessageBox::information(this, "搜索", QString("%1: online").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
        }
        else if (0 == strcmp(SEARCH_USR_OFFLINE, pdu->caData))
        {
            QMessageBox::information(this, "搜索", QString("%1: offline").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
        }
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
    {
        char caName[32] = {'\0'};
        strncpy(caName, pdu->caData+32, 32);
        int ret = QMessageBox::information(this, "添加好友", QString("%1 want to add you as friend ?").arg(caName)
                                           , QMessageBox::Yes, QMessageBox::No);
        PDU *respdu = mkPDU(0);
        memcpy(respdu->caData, pdu->caData, 64);
        if (QMessageBox::Yes == ret)
        {
            respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGGREE;
        }
        else
        {
            respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REFUSE;
        }
        m_tcpSocket.write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:
    {
        QMessageBox::information(this, "添加好友", pdu->caData);
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGGREE:
    {
        char caPerName[32] = {'\0'};
        memcpy(caPerName, pdu->caData, 32);
        QMessageBox::information(this, "添加好友", QString("添加%1好友成功").arg(caPerName));
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE:
    {
        char caPerName[32] = {'\0'};
        memcpy(caPerName, pdu->caData, 32);
        QMessageBox::information(this, "添加好友", QString("添加%1好友失败").arg(caPerName));
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:
    {
        OpeWidget::getInstance().getFriend()->updateFriendList(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:
    {
        char caName[32] = {'\0'};
        memcpy(caName, pdu->caData, 32);
        QMessageBox::information(this, "删除好友", QString("%1 删除你作为他的好友").arg(caName));
        break;
    }
    case ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND:
    {
        QMessageBox::information(this, "删除好友", "删除好友成功");
        break;
    }
    case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST:
    {
        if (PrivateChat::getInstance().isHidden())
        {
            PrivateChat::getInstance().show();
        }
        char caSendName[32] = {'\0'};
        memcpy(caSendName, pdu->caData, 32);
        QString strSendName = caSendName;
        PrivateChat::getInstance().setChatName(strSendName);

        PrivateChat::getInstance().updateMsg(pdu);

        break;
    }
    case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST:
    {
        OpeWidget::getInstance().getFriend()->updateGroupMsg(pdu);
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
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
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
        m_strLoginName = strName;
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
        strncpy(pdu->caData, strName.toStdString().c_str(), 32);
        strncpy(pdu->caData+32, strPwd.toStdString().c_str(), 32);
        m_tcpSocket.write((char*)pdu, pdu->uiPDULen);
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
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
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
