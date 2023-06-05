#include "mysocket.h"

MySocket::MySocket(qintptr ID, QObject *parent) : QObject(parent)
{
    this->socketDescriptor = ID;
    this->setObjectName(QString("Socket: %1").arg(ID));
}

void MySocket::run()
{
    qInfo() << "Thread Started" <<QThread::currentThread();

    m_socket = new QTcpSocket(this);
    if(!m_socket->setSocketDescriptor(socketDescriptor))
    {
        qInfo() << "Socket error";
        emit error(m_socket->error());
        return;
    }
    connect(m_socket,&QTcpSocket::readyRead,this,&MySocket::readyRead);
    connect(m_socket,&QTcpSocket::disconnected,this,&MySocket::disconnected);

    qInfo() << socketDescriptor << "Client connected" << QThread::currentThread();

}

void MySocket::readyRead()
{
    QTextStream ts(m_socket);
    QString msg = ts.readLine();
    qInfo() << this << "Read" << msg << "From" << m_socket->peerAddress().toString() << QThread::currentThread();
    emit newMessage(msg);

    if (msg == m_readDoneMsg) {
        return;
    }
    else {
        emit socketMessage(m_socket->socketDescriptor(),msg);
        ts << m_readDoneMsg << endl;
        ts.flush();
    }
}

void MySocket::disconnected()
{
    qInfo() << this << "Disconnected" << QThread::currentThread();
    m_socket->deleteLater();
    emit finished();
}

void MySocket::writeSocket(qintptr id, QString msg)
{
    if(id!=socketDescriptor) return;
    qInfo() << this << "write" << msg << QThread::currentThread();;
    QTextStream ts(m_socket);
    ts << msg << endl;
    ts.flush();
}

void MySocket::exitSocket()
{
    m_socket->disconnect();
}

QString MySocket::getReadDoneMsg() const
{
    return m_readDoneMsg;
}

void MySocket::setReadDoneMsg(const QString &readDoneMsg)
{
    m_readDoneMsg = readDoneMsg;
}

qintptr MySocket::getSocketDescriptor() const
{
    return socketDescriptor;
}

QTcpSocket *MySocket::getSocket() const
{
    return m_socket;
}
