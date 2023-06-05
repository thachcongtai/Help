#ifndef LINEMODEL_H
#define LINEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QQmlListProperty>
#include "mylinewipdatasource.h"
#include "linemornitorinfo.h"

class LineModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(MyLineWipDataSource * dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)
    Q_PROPERTY(QQmlListProperty<LineMornitorInfo> lines READ lines NOTIFY linesChanged)
    Q_CLASSINFO("DefaultProperty","lines")

    enum LinesRoles{
        LineIdRole = Qt::UserRole + 1,
        activeRole,
        stateFrontRole,
        statePbaRole,
        timeFrontRole,
        timePbaRole,
        countFrontRole,
        countPbaRole
    };
    enum LineStatus{
        LineFull,
        LineEmpty,
        AgvOnGoing,
        AgvFinished,
        LineDisconnected
    };
    enum LineType{
        Front,
        PBA
    };


public:
    explicit LineModel(QObject *parent = nullptr);

    MyLineWipDataSource * dataSource() const;
    QQmlListProperty<LineMornitorInfo> lines();
    void setDataSource(MyLineWipDataSource * dataSource);

    // save Data
    void readJson(const QJsonObject &json);
    void writeJson(QJsonObject &json);

    // Properties
    Q_INVOKABLE void resetTimeFront(int mindex);
    Q_INVOKABLE void resetTimePba(int mindex);
    Q_INVOKABLE void resetCountFront(int mindex);
    Q_INVOKABLE void resetCountPba(int mindex);
    Q_INVOKABLE void clearCount();
    Q_INVOKABLE void checkLineIsConnected();

signals:

    void dataSourceChanged(MyLineWipDataSource * dataSource);
    void linesChanged(QQmlListProperty<LineMornitorInfo> lines);

public slots:
    // interface with server
    void socketMessage(qintptr id,QString msg);


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    static void appendLine(QQmlListProperty<LineMornitorInfo>* list, LineMornitorInfo* line);
    static int lineCount(QQmlListProperty<LineMornitorInfo>* list);
    static LineMornitorInfo* line(QQmlListProperty<LineMornitorInfo>*list, int index);
    static void clearLines(QQmlListProperty<LineMornitorInfo>*list);
    void initDefaultDataSource();

    //Helped method
    void setLineStatus(int line,LineType type, LineStatus status);
    QModelIndex getIndexByLineId(int lineid);
    void lineDisconnect(int);

    MyLineWipDataSource * m_dataSource = nullptr;
    QQmlListProperty<LineMornitorInfo> m_lines;
    bool m_signalConnected = false;
};

#endif // LINEMODEL_H
