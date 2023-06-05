#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

class MySocket : public QObject
{
    Q_OBJECT
public:
    explicit MySocket(qintptr ID,QObject *parent = nullptr);


    QTcpSocket *getSocket() const;
    qintptr getSocketDescriptor() const;
    QString getReadDoneMsg() const;
    void setReadDoneMsg(const QString &readDoneMsg);

signals:
    void error(QTcpSocket::SocketError socketerror);
    void newMessage(QString data);
    void finished();
    void socketMessage(qintptr id,QString msg);

public slots:
    void run();
    void readyRead();
    void disconnected();
    void writeSocket(qintptr id,QString msg);
    void exitSocket();

private:
    QTcpSocket *m_socket;
    qintptr socketDescriptor;
    QString m_readDoneMsg = "read";
};

#endif // MYSOCKET_H
