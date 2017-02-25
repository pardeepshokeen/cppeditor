
#ifndef STRUCTURES
#define STRUCTURES

#include <QVector>
#include <QJsonObject>
#include <QPair>

struct lineInfo{
    int lineNo;
    QJsonObject prev;
    QJsonObject next;
};

struct functionData{
    QString functionName;
    QJsonObject arguments;
    QVector<lineInfo> lineList;
    QVector<int> recurseFunctions;
    QVector<QPair<int, QJsonObject> > vec;

};


#endif // STRUCTURES

