#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include "mysocket.h"

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr);
    ~MyServer() override;

    quint16 port() const;
    void setPort(const quint16 &port);
    void startServer();

signals:
    void quit();
    void writeSocket(qintptr id,QString msg);
    void newMessage(QString data);
    void socketMessage(qintptr id,QString msg);
public slots:

private:
    quint16 m_port = 8000;

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) override;
};

#endif // MYSERVER_H
