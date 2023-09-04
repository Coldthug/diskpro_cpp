#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include "protocol.h"
#include "opedb.h"

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    QString getName();
private:
    QString m_strName;

signals:
    void offline(MyTcpSocket *mysocket);
public slots:
    void recvMsg();
    void clientOffline();
};

#endif // MYTCPSOCKET_H
