#ifndef OPEWIDGET_H
#define OPEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include "friend.h"
#include "book.h"

class OpeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpeWidget(QWidget *parent = nullptr);
private:
    QListWidget *m_pListW;
    QStackedWidget *m_pSW;

    Friend *m_pFriend;
    Book *m_pBook;
signals:

public slots:
};

#endif // OPEWIDGET_H
