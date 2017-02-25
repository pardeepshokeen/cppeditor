#ifndef CUSTOMTAB_H
#define CUSTOMTAB_H

#include<QtGui>
#include<QtCore>
#include<QGridLayout>
#include<codeeditor.h>
#include<highlighter.h>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include <debuggerdebugging.h>
#include <QCompleter>
#include <QAbstractItemView>
#include <QAbstractItemModel>
#include <QApplication>
class CustomTab  : public QWidget
{
   Q_OBJECT
private:
    DebuggerDebugging *debugDialog;
    QCompleter *completer;
    QAbstractItemModel *modelFromFile(const QString& fileName);

public:
    QProcess *process;
    CustomTab(QWidget *, int, QString);
    QGridLayout *gridLayout;
    CodeEditor *editor;
    QTextEdit *inputText;
    QTextEdit *outputText;
    QLabel *inputLabel;
    QLabel *outputLabel;
    int fileIndex;
   QString pathToDir;
    QFile *cppFile;
    QFile *inputFile;

    QLineEdit *functionToDebugName;
    QPushButton *debugButton;
    QLabel *debugLabel;


    QString cppName;
    QString inputName;

    virtual ~CustomTab();


    void saveFile(QString, QString );
    void initializeFiles();
    bool runFile();
public slots:
    void editorChanged();
    void inputChanged();
    void debugStart(bool e);


};

#endif // CUSTOMTAB_H
