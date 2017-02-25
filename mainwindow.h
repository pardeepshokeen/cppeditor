#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qsci/qsciscintilla.h>
#include <QtGui>
#include <QtCore>
#include <highlighter.h>
#include <codeeditor.h>
#include <customtab.h>
#include <QDir>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QInputDialog>
#include <openfile.h>
#include <QShortcut>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
   // QTextEdit *textEdit;
    QProcess *process;

    QTabWidget *tabWidget;

    //CustomTab* customTab[10];
    std::vector <CustomTab*> customTab;

    QDir *dir;
    QString pathToDir;
    std::vector <int>  lastFiles;
    int totalFiles;
    std::vector <QString> fileHints;


    void changeLastFiles(int , std::vector<int> &, int &);
    void changeFileHints(int, int, QString);
private slots:
    void tabClosed(int );
    void tabChanged(int );



    void on_pushButton_3_clicked();
};

#endif // MAINWINDOW_H
