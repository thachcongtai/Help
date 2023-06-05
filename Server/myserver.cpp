#include "myserver.h"

MyServer::MyServer(QObject *parent) : QTcpServer(parent)
{

}

MyServer::~MyServer()
{
    emit quit();
    foreach(QObject* obj, children())
    {
        QThread *thread = qobject_cast<QThread*>(obj);
        if(!thread) continue;
        thread->quit();
        thread->wait();
    }
}

quint16 MyServer::port() const
{
    return m_port;
}

void MyServer::setPort(const quint16 &port)
{
    m_port = port;
}

void MyServer::startServer()
{
    if(!this->listen(QHostAddress::Any,m_port))
    {
        qInfo() << "Could not start server";
    }
    else
    {
        qInfo() << "Listening to port" << m_port << "...";
    }
}

void MyServer::incomingConnection(qintptr handle)
{
    qInfo() << handle << "Connecting...";
    QThread *thread = new QThread (this);
    MySocket *socket = new MySocket(handle,nullptr);

    connect(thread,&QThread::started,socket,&MySocket::run,Qt::QueuedConnection);
    connect(socket,&MySocket::finished,thread,&QThread::quit,Qt::QueuedConnection);
    connect(socket,&MySocket::socketMessage,this,&MyServer::socketMessage,Qt::QueuedConnection);
    connect(socket,&MySocket::newMessage,this,&MyServer::newMessage,Qt::QueuedConnection);
    connect(socket,&MySocket::finished,thread,&QThread::quit,Qt::QueuedConnection);
    connect(this,&MyServer::writeSocket,socket,&MySocket::writeSocket,Qt::QueuedConnection);

    socket->moveToThread(thread);
    thread->start();
}




