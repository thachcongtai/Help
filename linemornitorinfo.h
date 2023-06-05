#ifndef LINEMORNITORINFO_H
#define LINEMORNITORINFO_H

#include <QObject>
#include <QDateTime>
#include <QJsonObject>
#include <QTimer>
#include <QDebug>



class LineMornitorInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int lineId READ lineId WRITE setLineId NOTIFY lineIdChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(int stateFront READ stateFront WRITE setStateFront NOTIFY stateFrontChanged)
    Q_PROPERTY(int statePba READ statePba WRITE setStatePba NOTIFY statePbaChanged)
    Q_PROPERTY(QString timeFront READ timeFront WRITE setTimeFront NOTIFY timeFrontChanged)
    Q_PROPERTY(QString timePba READ timePba WRITE setTimePba NOTIFY timePbaChanged)
    Q_PROPERTY(int countFront READ countFront WRITE setCountFront NOTIFY countFrontChanged)
    Q_PROPERTY(int countPba READ countPba WRITE setCountPba NOTIFY countPbaChanged)
    Q_PROPERTY(QDateTime lastFrontConnected READ lastFrontConnected)
    Q_PROPERTY(QDateTime lastPbaConnected READ lastPbaConnected)

public:
    explicit LineMornitorInfo(QObject *parent = nullptr);
    enum Line_State{IDLE=0,WAITING,AGV_RUN,FULLBOX};

    // interface
    int lineId() const;
    bool active() const;
    int stateFront() const;
    int statePba() const;
    QString timeFront() const;
    QString timePba() const;
    int countFront() const;
    int countPba() const;
    QDateTime lastFrontConnected() const;
    QDateTime lastPbaConnected() const;
    QDateTime timeStartPba() const;
    void setTimeStartPba(const QDateTime &timeStartPba);
    QDateTime timeStartFront() const;
    void setTimeStartFront(const QDateTime &timeStartFront);

    void setLineId(int lineId);
    void setActive(bool active);
    void setStateFront(int stateFront);
    void setStatePba(int statePba);
    void setTimeFront(QString timeFront);
    void setTimePba(QString timePba);
    void setCountFront(int countFront);
    void setCountPba(int countPba);
    void updateTimeFront();
    void updateTimePBA();

    // save Data
    void readJson(const QJsonObject &json);
    void writeJson(QJsonObject &json);



signals:
    void lineIdChanged(int lineId);
    void activeChanged(bool active);
    void stateFrontChanged(int stateFront);
    void statePbaChanged(int statePba);
    void timeFrontChanged(QString timeFront);
    void timePbaChanged(QString timePba);
    void countFrontChanged(int countFront);
    void countPbaChanged(int countPba);

public slots:
    void timerTrigger();
    void resetTimeFront();
    void resetTimePba();

private:
    int m_lineId;
    bool m_active;
    int m_stateFront;
    int m_statePba;
    QString m_timeFront;
    QString m_timePba;
    int m_countFront;
    int m_countPba;
    QDateTime m_timeStartPba;
    QDateTime m_timeStartFront;    
    QDateTime m_lastFrontConnected;
    QDateTime m_lastPbaConnected;
};

#endif // LINEMORNITORINFO_H
