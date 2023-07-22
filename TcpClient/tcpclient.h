#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QFile>
#include <QTcpSocket>
#include"protocol.h"
namespace Ui {
class TcpClient;
}

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = nullptr);
    ~TcpClient();

    //加载配置文件
    void loadConfig();

public slots:
    void  showConnect();

private slots:
    void on_send_pb_clicked();

private:
    Ui::TcpClient *ui;

    //Ip地址
    QString m_strIP;

    //端口号
    quint16 m_usPort;

    QTcpSocket m_tcpScoket;
};

#endif // TCPCLIENT_H
