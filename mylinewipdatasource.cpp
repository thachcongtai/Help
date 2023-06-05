#include "mylinewipdatasource.h"

MyLineWipDataSource::MyLineWipDataSource(QObject *parent) : QObject(parent)
{

}

MyLineWipDataSource::~MyLineWipDataSource()
{
    clearLines();
}

QList<LineMornitorInfo *> MyLineWipDataSource::dataItems()
{
    return m_lines;
}

void MyLineWipDataSource::addLine(LineMornitorInfo *line)
{
    qInfo() << "add Line" << line->lineId();
    if(!lineExistsCheck(line->lineId())) {

        return;
    }
    int index = m_lines.size();
    emit preLineAdded(index);
//    connect(this,&MyLineWipDataSource::processMessage,line,&MyLineWipDataSource::processMessage); check
    m_lines.append(line);
    emit postLineAdded();
}

void MyLineWipDataSource::addLine()
{
    LineMornitorInfo *line = new LineMornitorInfo(this);
    addLine(line);
}

void MyLineWipDataSource::addLine(const int lineId)
{
    LineMornitorInfo *line = buildLineDisplay(lineId);
    addLine(line);
}

void MyLineWipDataSource::removeLine(int index)
{
    if(index>=m_lines.size()||index <0)
    {
        return;
    }
    emit preLineRemoved(index);
    LineMornitorInfo *line = m_lines.takeAt(index);
    emit postLineRemoved();
    line->deleteLater();
}

void MyLineWipDataSource::removeLastLine()
{
    removeLine(m_lines.size()-1);
}

int MyLineWipDataSource::lineCount()
{
    return m_lines.count();
}

LineMornitorInfo *MyLineWipDataSource::lineAt(int index)
{
    return m_lines[index];
}

void MyLineWipDataSource::clearLines()
{
    qDeleteAll(m_lines);
    m_lines.clear();
}

bool MyLineWipDataSource::lineExistsCheck(int lineId)
{
    if(lineId==-1) return true;
    foreach(LineMornitorInfo* line, m_lines)
    {
        if(line->lineId()==lineId)
        {
            qInfo() <<"This line id already exists!";
            return false;
        }
    }
    return true;
}

void MyLineWipDataSource::readJson(const QJsonObject &json)
{
    qInfo() << "new datasource";
    if (json.contains("lines") && json["lines"].isArray())
    {
        QJsonArray lineArray = json["lines"].toArray();
        clearLines();
        m_lines.reserve(lineArray.size());
        for (int lineIndex = 0; lineIndex < lineArray.size(); ++lineIndex)
        {
            QJsonObject lineObject = lineArray[lineIndex].toObject();
            LineMornitorInfo* line = new LineMornitorInfo ();
            line->readJson(lineObject);
//            qInfo() << "New line" << line->lineId();

            addLine(line);
        }
    }
}

void MyLineWipDataSource::writeJson(QJsonObject &json)
{
    QJsonArray lineArray;
    for (LineMornitorInfo *line : m_lines) {
        QJsonObject lineObject;
        line->writeJson(lineObject);
        lineArray.append(lineObject);
    }
    json["lines"] = lineArray;
}

LineMornitorInfo *MyLineWipDataSource::buildLineDisplay(int lineId)
{
    LineMornitorInfo* line = new LineMornitorInfo (this);
    line->setLineId(lineId);
    return line;
}
