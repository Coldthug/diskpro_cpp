#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "mytcpserver.h"
#include <QTcpServer>
#include <QWidget>
#include <QFile>

namespace Ui {
class TcpServer;
}

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    explicit TcpServer(QWidget *parent = nullptr);
    ~TcpServer();

    void loadConfig();

private:
    Ui::TcpServer *ui;

    //Ip地址
    QString m_strIP;

    //端口号
    quint16 m_usPort;
};

#endif // TCPSERVER_H
