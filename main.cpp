#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "wiplinemornitor.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    WipLineMornitor monitor;

    if(!monitor.initQml())
    {
        return -1;
    }


    return app.exec();
}
