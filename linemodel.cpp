#include "linemodel.h"

LineModel::LineModel(QObject *parent) : QAbstractListModel(parent)
{
    initDefaultDataSource();
}

MyLineWipDataSource *LineModel::dataSource() const
{
    return m_dataSource;
}

QQmlListProperty<LineMornitorInfo> LineModel::lines()
{
    return QQmlListProperty<LineMornitorInfo>(this,this,
                                              &LineModel::appendLine,
                                              &LineModel::lineCount,
                                              &LineModel::line,
                                              &LineModel::clearLines);
}

void LineModel::setDataSource(MyLineWipDataSource *dataSource)
{
    beginResetModel();
    if(m_dataSource&&m_signalConnected)
    {
        m_dataSource->disconnect(this);
    }
    //    beginInsertRows()

    m_dataSource = dataSource;
    //    connect(this,&LineModel::processMessage,m_dataSource,&myAgvTagDataSource::processMessage);
    connect(m_dataSource,&MyLineWipDataSource::preLineAdded,[=](int index){
        beginInsertRows(QModelIndex(),index,index);
    });
    connect(m_dataSource,&MyLineWipDataSource::postLineAdded,[=](){
        endInsertRows();
    });
    connect(m_dataSource,&MyLineWipDataSource::preLineRemoved,[=](int index){
        beginRemoveRows(QModelIndex(),index,index);
    });
    connect(m_dataSource,&MyLineWipDataSource::postLineRemoved,[=](){
        endRemoveRows();
    });
    m_signalConnected = true;
    emit dataSourceChanged(m_dataSource);
    endResetModel();

}

void LineModel::readJson(const QJsonObject &json)
{

    if (json.contains("dataSource"))
    {
        if(m_dataSource) delete m_dataSource;
        m_dataSource = new MyLineWipDataSource (this);
        QJsonObject dataSource = json["dataSource"].toObject();

        m_dataSource->readJson(dataSource);
        //        connect(this,&TagModel::processMessage,ds,&myAgvTagDataSource::processMessage);
        setDataSource(m_dataSource);
    }

}

void LineModel::writeJson(QJsonObject &json)
{
    QJsonObject dataSource;
    if(m_dataSource)
    {
        m_dataSource->writeJson(dataSource);
    }
    json["dataSource"] = dataSource;
}

void LineModel::resetTimeFront(int mindex)
{
    m_dataSource->lineAt(mindex)->resetTimeFront();
    m_dataSource->lineAt(mindex)->updateTimeFront();
    dataChanged(index(mindex),index(mindex),QVector<int>()<<timeFrontRole);
}

void LineModel::resetTimePba(int mindex)
{
    m_dataSource->lineAt(mindex)->resetTimePba();
    m_dataSource->lineAt(mindex)->updateTimePBA();
    dataChanged(index(mindex),index(mindex),QVector<int>()<<timePbaRole);
}

void LineModel::resetCountFront(int mindex)
{
    setData(index(mindex),0,countFrontRole);
}

void LineModel::resetCountPba(int mindex)
{
    setData(index(mindex),0,countPbaRole);
}

void LineModel::clearCount()
{
    for (int i=0;i<rowCount();i++) {
        setData(index(i),0,countFrontRole);
        setData(index(i),0,countPbaRole);
    }
}

/**
* @brief LineModel::checkLineIsConnected
* check all lineid in model, set disconnect if time out
*/
void LineModel::checkLineIsConnected()
{
    for(int i=0;i<rowCount();i++)
    {
        if(m_dataSource->lineAt(i)->active()==true)
        {
            int stateFront = m_dataSource->lineAt(i)->stateFront();
            if(stateFront!=LineMornitorInfo::AGV_RUN&&stateFront!=LineMornitorInfo::IDLE)
            {
                qint64 timeFront = m_dataSource->lineAt(i)->lastFrontConnected().secsTo(QDateTime::currentDateTime());
                if(timeFront>=30)
                {
                    m_dataSource->lineAt(i)->setStateFront(LineMornitorInfo::IDLE);
                    m_dataSource->lineAt(i)->updateTimeFront();
                    emit dataChanged(index(i),index(i),QVector<int>()<<stateFrontRole<<timeFrontRole);
                }
            }
            int statePba = m_dataSource->lineAt(i)->statePba();
            if(statePba!=LineMornitorInfo::AGV_RUN&&statePba!=LineMornitorInfo::IDLE)
            {
                qint64 timePba = m_dataSource->lineAt(i)->lastPbaConnected().secsTo(QDateTime::currentDateTime());
                if(timePba>=30)
                {
                    m_dataSource->lineAt(i)->setStatePba(LineMornitorInfo::IDLE);
                    m_dataSource->lineAt(i)->updateTimePBA();
                    emit dataChanged(index(i),index(i),QVector<int>()<<statePbaRole<<timePbaRole);
                }
            }
        }
    }
}

int LineModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return  m_dataSource->dataItems().size();
}

QVariant LineModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_dataSource->dataItems().size())
    {
        return QVariant();
    }
    //        LinesRoles mRole = LinesRoles(role);
    LineMornitorInfo *line = m_dataSource->dataItems().at(index.row());
    switch (role) {
    case LineModel::LineIdRole:
        return line->lineId();
    case LineModel::activeRole:
        return line->active();
    case LineModel::stateFrontRole:
        return line->stateFront();
    case LineModel::statePbaRole:
        return line->statePba();
    case LineModel::timeFrontRole:
        return line->timeFront();
    case LineModel::timePbaRole:
        return line->timePba();
    case LineModel::countFrontRole:
        return line->countFront();
    case LineModel::countPbaRole:
        return line->countPba();

    default:
        return QVariant();

    }
}

bool LineModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= m_dataSource->dataItems().size())
        return false;
    bool somethingChanged = false;
    LineMornitorInfo *line = m_dataSource->dataItems().at(index.row());
    switch (role) {
    case LineModel::LineIdRole:
    {
        if(line->lineId()!=value.toInt()&&m_dataSource->lineExistsCheck(value.toInt()))
        {
            line->setLineId(value.toInt());
            somethingChanged = true;
        }
        break;
    }
    case LineModel::activeRole:
    {
        if(line->active()!=value.toBool())
        {
            line->setActive(value.toBool());
            somethingChanged = true;
        }
        break;
    }
    case LineModel::stateFrontRole:
    {
        if(line->stateFront()!=value.toInt())
        {
            line->setStateFront(value.toInt());
            somethingChanged = true;
        }
        break;
    }
    case LineModel::statePbaRole:
    {
        if(line->statePba()!=value.toInt())
        {
            line->setStatePba(value.toInt());
            somethingChanged = true;
        }
        break;
    }
    case LineModel::timeFrontRole:
    {
        line->resetTimeFront();
        somethingChanged=true;
        break;
    }
    case LineModel::timePbaRole:
    {
        line->resetTimePba();
        somethingChanged=true;
        break;
    }
    case LineModel::countFrontRole:
    {
        if(line->countFront()!=value.toInt())
        {
            line->setCountFront(value.toInt());
            somethingChanged = true;
        }
        break;
    }
    case LineModel::countPbaRole:
    {
        if(line->countPba()!=value.toInt())
        {
            line->setCountPba(value.toInt());
            somethingChanged = true;
        }
        break;
    }
    }

    if( somethingChanged){
        emit dataChanged(index,index,QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags LineModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> LineModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[LineIdRole] = "mLineID";
    roles[activeRole] = "mActive";
    roles[stateFrontRole] = "mStateFront";
    roles[statePbaRole] = "mStatePba";
    roles[timeFrontRole] = "mTimeFront";
    roles[timePbaRole] = "mTimePba";
    roles[countFrontRole] = "mCountFront";
    roles[countPbaRole] = "mCountPba";
    return roles;
}

void LineModel::appendLine(QQmlListProperty<LineMornitorInfo> *list, LineMornitorInfo *line)
{
    reinterpret_cast<LineModel*>(list->data)->dataSource()->addLine(line);
}

int LineModel::lineCount(QQmlListProperty<LineMornitorInfo> *list)
{
    return reinterpret_cast<LineModel*>(list->data)->rowCount();
}

LineMornitorInfo *LineModel::line(QQmlListProperty<LineMornitorInfo> *list, int index)
{
    return reinterpret_cast<LineModel*>(list->data)->dataSource()->lineAt(index);
}

void LineModel::clearLines(QQmlListProperty<LineMornitorInfo> *list)
{
    reinterpret_cast<LineModel*>(list->data)->dataSource()->clearLines();
}

void LineModel::initDefaultDataSource()
{
    m_dataSource = new MyLineWipDataSource (this);
    for (int id =1;id<=40 ;id++) {
        m_dataSource->addLine(id);
    }
    setDataSource(m_dataSource);
}

void LineModel::setLineStatus(int line, LineModel::LineType type, LineModel::LineStatus status)
{
    QModelIndex index = getIndexByLineId(line);
    if(!index.isValid())
    {
        qInfo() << "Index not valid";
        return;
    }
    if(data(index,activeRole).toBool()==false)
    {
        qInfo() << "Line not active yet";
        return;
    }

    switch (type) {
    case LineModel::Front:
    {
        // updated last msg time
        m_dataSource->lineAt(index.row())->updateTimeFront();
        int currentState = m_dataSource->lineAt(index.row())->stateFront();

        switch (status) {
        case LineModel::LineFull:
        {
            if(currentState!=LineMornitorInfo::AGV_RUN)
            {
                // set line state to notReady
                if(currentState!=LineMornitorInfo::Line_State::FULLBOX)
                {
                    m_dataSource->lineAt(index.row())->setStateFront(LineMornitorInfo::FULLBOX);
                    m_dataSource->lineAt(index.row())->setTimeStartFront(QDateTime::currentDateTime());
                    emit dataChanged(index,index,QVector<int>()<<stateFrontRole<<timeFrontRole);
                }

            }
            break;
        }
        case LineModel::LineEmpty:
        {
            if(currentState!=LineMornitorInfo::AGV_RUN)
            {
                // set line state to Empty
                if(currentState!=LineMornitorInfo::Line_State::WAITING)
                {
                    m_dataSource->lineAt(index.row())->setStateFront(LineMornitorInfo::WAITING);
                    m_dataSource->lineAt(index.row())->setTimeStartFront(QDateTime::currentDateTime());
                    emit dataChanged(index,index,QVector<int>()<<stateFrontRole<<timeFrontRole);
                }

            }
            break;
        }
        case LineModel::AgvOnGoing:
        {
            // set line state to Empty
            if(currentState!=LineMornitorInfo::AGV_RUN)
            {
                m_dataSource->lineAt(index.row())->setStateFront(LineMornitorInfo::AGV_RUN);
                m_dataSource->lineAt(index.row())->setTimeStartFront(QDateTime::currentDateTime());
                emit dataChanged(index,index,QVector<int>()<<stateFrontRole<<timeFrontRole);
            }
            break;
        }
        case LineModel::AgvFinished:
        {
            // set line state to Empty
            if(currentState==LineMornitorInfo::AGV_RUN)
            {
                m_dataSource->lineAt(index.row())->setStateFront(LineMornitorInfo::FULLBOX);
                m_dataSource->lineAt(index.row())->setTimeStartFront(QDateTime::currentDateTime());
                m_dataSource->lineAt(index.row())->setCountFront(m_dataSource->lineAt(index.row())->countFront()+1);
                emit dataChanged(index,index,QVector<int>()<<stateFrontRole<<timeFrontRole<<countFrontRole);
            }
            break;
        }
        case LineModel::LineDisconnected:
        {
            // Don't set, this is automatic check by time
            break;
        }
        }
        break;
    }

    case LineModel::PBA:
    {
        // updated last msg time
        m_dataSource->lineAt(index.row())->updateTimePBA();
        int currentState = m_dataSource->lineAt(index.row())->statePba();

        switch (status) {
        case LineModel::LineFull:
        {
            if(currentState!=LineMornitorInfo::AGV_RUN)
            {
                // set line state to notReady
                if(currentState!=LineMornitorInfo::FULLBOX)
                {
                    m_dataSource->lineAt(index.row())->setStatePba(LineMornitorInfo::FULLBOX);
                    m_dataSource->lineAt(index.row())->setTimeStartPba(QDateTime::currentDateTime());
                    emit dataChanged(index,index,QVector<int>()<<statePbaRole<<timePbaRole);
                }

            }
            break;
        }
        case LineModel::LineEmpty:
        {
            if(currentState!=LineMornitorInfo::AGV_RUN)
            {
                // set line state to Empty
                if(currentState!=LineMornitorInfo::WAITING)
                {
                    m_dataSource->lineAt(index.row())->setStatePba(LineMornitorInfo::WAITING);
                    m_dataSource->lineAt(index.row())->setTimeStartPba(QDateTime::currentDateTime());
                    emit dataChanged(index,index,QVector<int>()<<statePbaRole<<timePbaRole);
                }

            }
            break;
        }
        case LineModel::AgvOnGoing:
        {
            // set line state to Empty
            if(currentState!=LineMornitorInfo::AGV_RUN)
            {
                m_dataSource->lineAt(index.row())->setStatePba(LineMornitorInfo::AGV_RUN);
                m_dataSource->lineAt(index.row())->setTimeStartPba(QDateTime::currentDateTime());
                emit dataChanged(index,index,QVector<int>()<<statePbaRole<<timePbaRole);
            }
            break;
        }
        case LineModel::AgvFinished:
        {
            // set line state to Empty
            if(currentState==LineMornitorInfo::AGV_RUN)
            {
                m_dataSource->lineAt(index.row())->setStatePba(LineMornitorInfo::FULLBOX);
                m_dataSource->lineAt(index.row())->setTimeStartPba(QDateTime::currentDateTime());
                m_dataSource->lineAt(index.row())->setCountPba(m_dataSource->lineAt(index.row())->countPba()+1);
                emit dataChanged(index,index,QVector<int>()<<statePbaRole<<timePbaRole<<countPbaRole);
            }
            break;
        }
        case LineModel::LineDisconnected:
        {
            // Don't set, this is automatic check by time
            break;
        }
        }
        break;
    }

    }
}

QModelIndex LineModel::getIndexByLineId(int lineid)
{
    QModelIndexList list=  match(index(0),LineIdRole,QVariant::fromValue(lineid));
    if(!list.isEmpty()) return list.at(0);
    else {
        return QModelIndex();
    }
}

void LineModel::socketMessage(qintptr id,QString msg)
{
    Q_UNUSED(id)
    if(QRegExp("front-\\d+-\\d+").exactMatch(msg))
    {
        int line = msg.split('-').at(1).toInt();
        QString sensor = msg.split('-').at(2);
        if(sensor.toInt()>0)
        {
            setLineStatus(line,Front,LineEmpty);
        }
        else
        {
            setLineStatus(line,Front,LineFull);
        }

    }
    else if(QRegExp("pba-\\d+-\\d+").exactMatch(msg))
    {
        int line = msg.split('-').at(1).toInt();
        QString sensor = msg.split('-').at(2);
        if(sensor.toInt()>0)
        {
            setLineStatus(line,PBA,LineEmpty);
        }
        else
        {
            setLineStatus(line,PBA,LineFull);
        }

    }
    else if(QRegExp("Finishedpba-\\d+").exactMatch(msg))
    {
        int line = msg.split('-').at(1).toInt();
        setLineStatus(line,PBA,AgvOnGoing);
    }
    else if(QRegExp("Finishedloadpba-\\d+").exactMatch(msg))
    {
        int line = msg.split('-').at(1).toInt();
        setLineStatus(line,PBA,AgvFinished);;
    }
    else if(QRegExp("Finishedfront-\\d+").exactMatch(msg))
    {
        int line = msg.split('-').at(1).toInt();
        setLineStatus(line,Front,AgvOnGoing);
    }
    else if(QRegExp("Finishedloadfront-\\d+").exactMatch(msg))
    {
        int line = msg.split('-').at(1).toInt();
        setLineStatus(line,Front,AgvFinished);
    }
    //    else if(QRegExp("callLiftPBA-\\d+").exactMatch(msg))
    //    {
    //        int line = msg.split('-').at(1).toInt();
    //        emit updatedStatus(line,FromNewLine::pba,LabelDisplayTime::Status::lift_called);
    //    }
    //    else if(QRegExp("liftTransferPBA-\\d+").exactMatch(msg))
    //    {
    //        int line = msg.split('-').at(1).toInt();
    //        emit updatedStatus(line,FromNewLine::pba,LabelDisplayTime::Status::lift_transfer);
    //    }
    //    else if(QRegExp("callACSPBA-\\d+").exactMatch(msg))
    //    {
    //        int line = msg.split('-').at(1).toInt();
    //        emit updatedStatus(line,FromNewLine::pba,LabelDisplayTime::Status::AGV_called);
    //    }
}
