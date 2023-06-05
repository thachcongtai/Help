#include "linemornitorinfo.h"

LineMornitorInfo::LineMornitorInfo(QObject *parent) : QObject(parent),
    m_lineId(-1),m_active(true),m_stateFront(1),m_statePba(1),m_countFront(0),m_countPba(0),
    m_lastFrontConnected(QDateTime::currentDateTime()),m_lastPbaConnected(QDateTime::currentDateTime())
{
    m_timeStartPba = QDateTime::currentDateTime();
    m_timeStartFront = QDateTime::currentDateTime();
    timerTrigger();
}

int LineMornitorInfo::lineId() const
{
    return m_lineId;
}

bool LineMornitorInfo::active() const
{
    return m_active;
}

int LineMornitorInfo::stateFront() const
{
    return m_stateFront;
}

int LineMornitorInfo::statePba() const
{
    return m_statePba;
}

QString LineMornitorInfo::timeFront() const
{
    qint64 sec =m_timeStartFront.secsTo(QDateTime::currentDateTime());
    QString timeFront = QDateTime::fromSecsSinceEpoch(sec,Qt::UTC).toString("hh:mm:ss");
    return timeFront;
}

QString LineMornitorInfo::timePba() const
{
    QString timePba = QDateTime::fromSecsSinceEpoch(m_timeStartPba.secsTo(QDateTime::currentDateTime()),Qt::UTC).toString("hh:mm:ss");
    return timePba;
}

int LineMornitorInfo::countFront() const
{
    return m_countFront;
}

int LineMornitorInfo::countPba() const
{
    return m_countPba;
}

void LineMornitorInfo::setLineId(int lineId)
{
    if (m_lineId == lineId)
        return;

    m_lineId = lineId;
    emit lineIdChanged(m_lineId);
}

void LineMornitorInfo::setActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;
    emit activeChanged(m_active);
}

void LineMornitorInfo::setStateFront(int stateFront)
{
    if (m_stateFront == stateFront)
        return;

    m_stateFront = stateFront;
    emit stateFrontChanged(m_stateFront);
}

void LineMornitorInfo::setStatePba(int statePba)
{
    if (m_statePba == statePba)
        return;

    m_statePba = statePba;
    emit statePbaChanged(m_statePba);
}

void LineMornitorInfo::setTimeFront(QString timeFront)
{
    if (m_timeFront == timeFront)
        return;

    m_timeFront = timeFront;
    emit timeFrontChanged(m_timeFront);
}

void LineMornitorInfo::setTimePba(QString timePba)
{
    if (m_timePba == timePba)
        return;

    m_timePba = timePba;
    emit timePbaChanged(m_timePba);
}

void LineMornitorInfo::setCountFront(int countFront)
{
    if (m_countFront == countFront)
        return;

    m_countFront = countFront;
    emit countFrontChanged(m_countFront);
}

void LineMornitorInfo::setCountPba(int countPba)
{
    if (m_countPba == countPba)
        return;

    m_countPba = countPba;
    emit countPbaChanged(m_countPba);
}

void LineMornitorInfo::updateTimeFront()
{
    m_lastFrontConnected = QDateTime::currentDateTime();
}

void LineMornitorInfo::updateTimePBA()
{
    m_lastPbaConnected = QDateTime::currentDateTime();
}

void LineMornitorInfo::readJson(const QJsonObject &json)
{
    if(json.contains("id"))
    {
        m_lineId = json["id"].toInt();
    }
    if(json.contains("active"))
    {
        m_active = json["active"].toBool();
    }
    if(json.contains("stateFront"))
    {
        m_stateFront = json["stateFront"].toInt();
        if(m_stateFront==0) m_stateFront=1;
    }
    if(json.contains("statePba"))
    {
        m_statePba = json["statePba"].toInt();
        if(m_statePba==0) m_statePba=1;
    }
    if(json.contains("countFront"))
    {
        m_countFront = json["countFront"].toInt();
    }
    if(json.contains("countPba"))
    {
        m_countPba = json["countPba"].toInt();
    }
    if(json.contains("startTimeFront"))
    {
        QString strTime = json["startTimeFront"].toString();
        m_timeStartFront = QDateTime::fromString(strTime,"yyyy-MM-dd hh::mm:ss");
    }
    if(json.contains("startTimePba"))
    {
        QString strTime = json["startTimePba"].toString();
        m_timeStartPba = QDateTime::fromString(strTime,"yyyy-MM-dd hh::mm:ss");
    }
}

void LineMornitorInfo::writeJson(QJsonObject &json)
{
    json["id"] = m_lineId;
    json["active"] = m_active;
    json["stateFront"] = m_stateFront;
    json["m_statePba"] = m_statePba;
    json["countFront"] = m_countFront;
    json["countPba"] = m_countPba;
    json["startTimeFront"] = m_timeStartFront.toString("yyyy-MM-dd hh::mm:ss");
    json["startTimePba"] = m_timeStartPba.toString("yyyy-MM-dd hh::mm:ss");
}

QDateTime LineMornitorInfo::lastPbaConnected() const
{
    return m_lastPbaConnected;
}

QDateTime LineMornitorInfo::lastFrontConnected() const
{
    return m_lastFrontConnected;
}

void LineMornitorInfo::timerTrigger()
{
    if(!m_active)
    {
        emit timePbaChanged("");
        emit timeFrontChanged("");
    }
    else {
        if(m_statePba==0)
        {
            emit timePbaChanged("");
        }
        else {
            QString timePba = QDateTime::fromMSecsSinceEpoch(m_timeStartPba.secsTo(QDateTime::currentDateTime()),Qt::UTC).toString("hh:mm:ss");
            emit timePbaChanged(timePba);
        }

        if(m_stateFront==0)
        {
            emit timeFrontChanged("");
        }
        else {
            QString timeFront = QDateTime::fromMSecsSinceEpoch(m_timeStartFront.secsTo(QDateTime::currentDateTime()),Qt::UTC).toString("hh:mm:ss");
            emit timePbaChanged(timeFront);
        }
    }
}

void LineMornitorInfo::resetTimeFront()
{
    m_timeStartFront = QDateTime::currentDateTime();
    qInfo() << "Reset Front Timer" << lineId();
}

void LineMornitorInfo::resetTimePba()
{
    m_timeStartPba = QDateTime::currentDateTime();
    qInfo() << "Reset PBA Timer" << lineId();
}

QDateTime LineMornitorInfo::timeStartFront() const
{
    return m_timeStartFront;
}

void LineMornitorInfo::setTimeStartFront(const QDateTime &timeStartFront)
{
    m_timeStartFront = timeStartFront;
}

QDateTime LineMornitorInfo::timeStartPba() const
{
    return m_timeStartPba;
}

void LineMornitorInfo::setTimeStartPba(const QDateTime &timeStartPba)
{
    m_timeStartPba = timeStartPba;
}


