#ifndef SHOWFUNCTIONSDIALOG_H
#define SHOWFUNCTIONSDIALOG_H

#include <QDialog>
#include <structures.h>
#include <QTreeWidget>
#include <QVector>
#include <QTreeWidgetItem>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QTextDocument>
#include <showfunction.h>
namespace Ui {
class showFunctionsDialog;
}

class showFunctionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit showFunctionsDialog(QVector<functionData> dataTemp ,QTextDocument *doc,QString ,QWidget *parent = 0);
    ~showFunctionsDialog();
    QString functionToString(functionData function, int index);


private slots:
    void on_buttonBox_accepted();
public slots:
    void testSlot(QTreeWidgetItem* t, QTreeWidgetItem*  t1);

private:
    Ui::showFunctionsDialog *ui;
    QVector<functionData> data;
    QMap<int, QString> map;
    QTextDocument *doc;
    showFunction *showfunc;
    QString functionName;

};

#endif // SHOWFUNCTIONSDIALOG_H
