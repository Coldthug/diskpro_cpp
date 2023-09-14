#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QFile>
#include <QTcpSocket>
#include"protocol.h"
#include "opewidget.h"
namespace Ui {
class TcpClient;
}

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = 0);
    ~TcpClient();

    //加载配置文件
    void loadConfig();

    static TcpClient &getInstance();
    QTcpSocket &getTcpSocket();
    QString loginName();
public slots:
    void  showConnect();
    void recvMsg();

private slots:
//    void on_send_pb_clicked();

    void on_log_pb_clicked();

    void on_reg_pb_clicked();

    void on_logout_pb_clicked();

private:
    Ui::TcpClient *ui;

    //Ip地址
    QString m_strIP;

    //端口号
    quint16 m_usPort;

    QTcpSocket m_tcpSocket;
    QString m_strLoginName;
};

#endif // TCPCLIENT_H
