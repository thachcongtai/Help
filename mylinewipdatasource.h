#ifndef MYLINEWIPDATASOURCE_H
#define MYLINEWIPDATASOURCE_H

#include <QObject>
#include "linemornitorinfo.h"
#include <QJsonArray>
#include <QJsonObject>

class MyLineWipDataSource : public QObject
{
    Q_OBJECT
public:
    explicit MyLineWipDataSource(QObject *parent = nullptr);
    ~MyLineWipDataSource();

    QList<LineMornitorInfo*> dataItems();
    enum LineState{
        NotConnect =0,
        LineEmpty,
        AGVonGoing,
        LineFull
    };

    void addLine(LineMornitorInfo *line);
    Q_INVOKABLE void addLine();
    Q_INVOKABLE void addLine(const int lineId);
    Q_INVOKABLE void removeLine(int index);
    Q_INVOKABLE void removeLastLine();

    int lineCount();
    LineMornitorInfo *lineAt(int index);
    void clearLines();
    bool lineExistsCheck(int lineId);

    // save Data
    void readJson(const QJsonObject &json);
    void writeJson(QJsonObject &json);


signals:
    void preLineAdded(int index);
    void postLineAdded();
    void preLineRemoved(int index);
    void postLineRemoved();
    void processMessage(int agv, QString tag);
    void dataChanged(int index);

public slots:


private:
    QList<LineMornitorInfo*> m_lines;
    LineMornitorInfo * buildLineDisplay(int lineId);
};

#endif // MYLINEWIPDATASOURCE_H
