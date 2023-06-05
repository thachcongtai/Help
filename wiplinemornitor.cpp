#include "wiplinemornitor.h"

WipLineMornitor::WipLineMornitor(QObject *parent) : QObject(parent)
{    
    m_server.setPort(6006);
    m_server.startServer();
    // connect server and model
    connect(&m_server,&MyServer::socketMessage,&m_model,&LineModel::socketMessage);
    // end connect
    loadData(m_savePath);
}

WipLineMornitor::~WipLineMornitor()
{
    saveData(m_savePath);
}

bool WipLineMornitor::saveData(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        QString error ="could not open save file!";
        qWarning() << error;
        emit errorString(error);
        return false;
    }

    QJsonObject json;
    writeJson(json);
    file.write(QJsonDocument(json).toJson());
    file.flush();
    file.close();
    return true;
}

bool WipLineMornitor::loadData(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        QString error ="could not open load file!";
        qWarning() << error;
        emit errorString(error);
        return false;
    }
    QByteArray saveData = file.readAll();
    QJsonDocument loadDoc = QJsonDocument::fromJson(saveData);
    readJson(loadDoc.object());
    return true;
}

bool WipLineMornitor::initQml()
{
    qmlRegisterType<LineMornitorInfo>("com.thai.class",1,0,"LineMornitor");
    qmlRegisterType<LineModel>("com.thai.class",1,0,"LineModel");
    qmlRegisterType<WipLineMornitor>("com.thai.class",1,0,"WipMornitor");
    qmlRegisterUncreatableType<MyLineWipDataSource>("com.thai.class",1,0,"MyLineWipDataSource","Cant create this type");

    m_engine.rootContext()->setContextProperty("lineModel",&m_model);
    m_engine.rootContext()->setContextProperty("mornitor",this);
    m_engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (m_engine.rootObjects().isEmpty())
        return false;
//    m_engine.

    return true;
}

void WipLineMornitor::readJson(const QJsonObject &json)
{
    if (json.contains("model"))
    {
        QJsonObject model = json["model"].toObject();
        m_model.readJson(model);
    }
}

void WipLineMornitor::writeJson(QJsonObject &json)
{
    QJsonObject model;
    m_model.writeJson(model);
    json["model"]=model;

}
