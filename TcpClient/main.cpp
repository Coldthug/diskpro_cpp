#include "tcpclient.h"
#include <QApplication>
#include "opewidget.h"
#include "online.h"
#include "friend.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpClient::getInstance().show();
//    TcpClient w;
//    w.show();

//    OpeWidget O;
//    O.show();

//    Online online;
//    online.show();


//    Friend f;
//    f.show();


//    OpeWidget opeWidget;
//    opeWidget.show();
    return a.exec();
}
