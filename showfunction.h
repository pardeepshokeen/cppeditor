#ifndef SHOWFUNCTION_H
#define SHOWFUNCTION_H

#include <QDialog>
#include <structures.h>
#include <QtGui>
#include <QtCore>
#include <QVector>
#include <QSet>
#include <QTextDocument>
#include <codeviewer.h>
#include <QSplitter>
#include <QHBoxLayout>
#include <QTreeWidgetItem>
#include <QSplitter>
#include <QGridLayout>
#include <QTreeWidget>
#include <showiterationsdialog.h>

namespace Ui {
class showFunction;
}

class showFunction : public QDialog
{
    Q_OBJECT

public:
    explicit showFunction(functionData tempData,QTextDocument *doc, QWidget *parent = 0);
    ~showFunction();
    void objectToTree(QJsonValue val1, QTreeWidgetItem *parent);
    void addObjectToTree(QJsonObject object1, QTreeWidget *parent);

private:
    Ui::showFunction *ui;
    QVector<QVector<lineInfo> > vec;
    QSet<int> itLines;
    int totalPages;
    int currentPage;
    QTextDocument *doc;
    CodeViewer *codeViewer;
    QMap<int,QVector<QPair<int,lineInfo> > > itPages;
    showIterationsDialog *itDialog;


 public slots:
    void hasDoubleClick(int);
    void cursorChanged();
    void pageShowChange();
    void onPageChangeRequest(int);

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // SHOWFUNCTION_H
