#ifndef DEBUGGERDEBUGGING_H
#define DEBUGGERDEBUGGING_H

#include <QDialog>
#include <QJsonObject>
#include <QRegExp>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QTextBlock>
#include <QTextDocument>
#include<QStack>
#include <QProcess>
#include <QtGui>
#include <QtCore>
#include <QDir>
#include <structures.h>
#include <showfunctionsdialog.h>

namespace Ui {
class DebuggerDebugging;
}

class DebuggerDebugging : public QDialog
{
    Q_OBJECT

public:
    explicit DebuggerDebugging(QString functionToDebug,QString fileName, QTextDocument *sourceDocument ,QWidget *parent = 0);
    ~DebuggerDebugging();

private:
    Ui::DebuggerDebugging *ui;
    QProcess *process;
    QStringList *strlist;
    QJsonObject *object;
    bool programRunning;

    QVector<QPair<int,QJsonObject> > vec;
    QString funcToDebug;
    QVector<lineInfo> processOutput(QVector<QPair<int,QJsonObject> > &vec);
    lineInfo processLineInfo(QJsonObject &next, QJsonObject &prev, int lineN);
    QTextDocument *doc;
    bool isStringContains(QString &str, QString &var);
    //DebugWindow *win;
    QVector<functionData> debugData;
    QStack <int> functionStack;
    //showFunctionsDialog *showFunctions;
    int currentFunction;
    QJsonObject strToObject(QString str);
    QJsonValue parseValue(QString str);
   std::pair<QString, QJsonObject> parseGdbOutput(QString str);
   void debugBrain(std::pair<QString, QJsonObject> p);
   showFunctionsDialog *functionDialog;


public slots:
    void readOutput();

    void onDebuggerDebugComplete();


signals:
     void debuggerDebugComplete();



private slots:
     void on_pushButton_clicked();
};

#endif // DEBUGGERDEBUGGING_H
