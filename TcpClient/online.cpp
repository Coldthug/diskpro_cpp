#include "online.h"
#include "tcpclient.h"
#include "ui_online.h"

Online::Online(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Online)
{
    ui->setupUi(this);
}

Online::~Online()
{
    delete ui;
}


// Bug : 每点击一次显示在线用户按钮，都会触发，重复添加在线用户，需判断，如果列表已存在在线用户，不再添加
void Online::showUsr(PDU *pdu)
{
    if (NULL == pdu)
    {
        return;
    }
    uint uiSize = pdu->uiMsgLen/32;
    char caTmp[32];
    for (uint i=0; i<uiSize; i++)
    {
        memcpy(caTmp, (char*)(pdu->caMsg)+i*32, 32);
        qDebug() << caTmp;
        ui->online_lw->addItem(caTmp);
    }
}

void Online::on_addFriend_pb_clicked()
{
    QListWidgetItem *pItem = ui->online_lw->currentItem();
    QString strPerUsrName = pItem->text();
    QString strLoginName = TcpClient::getInstance().loginName();
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    memcpy(pdu->caData, strPerUsrName.toStdString().c_str(), strPerUsrName.size());
    memcpy(pdu->caData+32, strLoginName.toStdString().c_str(), strLoginName.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}
