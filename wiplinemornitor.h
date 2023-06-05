#ifndef WIPLINEMORNITOR_H
#define WIPLINEMORNITOR_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "linemodel.h"
#include "Server/myserver.h"
#include <QDir>
#define DEFAULT_SAVE_MORNITOR_PATH QDir::currentPath() + "/MornitorWip.json"

class WipLineMornitor : public QObject
{
    Q_OBJECT
public:
    explicit WipLineMornitor(QObject *parent = nullptr);
    ~WipLineMornitor();
    // Qml use function
    Q_INVOKABLE bool saveData(QString path =DEFAULT_SAVE_MORNITOR_PATH);
    Q_INVOKABLE bool loadData(QString path);
    // init Mornitor
    bool initQml();
    // save Data
    void readJson(const QJsonObject &json);
    void writeJson(QJsonObject &json);

signals:
    void errorString(QString errorString);
public slots:

private:
    QString m_savePath = DEFAULT_SAVE_MORNITOR_PATH;
    QQmlApplicationEngine m_engine;
    LineModel m_model;
    MyServer m_server;
};

#endif // WIPLINEMORNITOR_H
