#ifndef SHOWITERATIONSDIALOG_H
#define SHOWITERATIONSDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QJsonValue>
#include <QVector>
#include <QPair>
#include <structures.h>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QJsonArray>
#include <QDebug>

namespace Ui {
class showIterationsDialog;
}

class showIterationsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit showIterationsDialog(QVector<QPair<int, lineInfo> > data, QString line, QWidget *parent = 0);
    ~showIterationsDialog();

    void objectToTree(QJsonValue val1, QTreeWidgetItem *parent);
    void addObjectToTree(QJsonObject object1, QTreeWidget *parent);

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void listItemChanged(int);


private:
    Ui::showIterationsDialog *ui;
    QVector<QPair<int, lineInfo> > data;
signals:
   void changePage(int);

};

#endif // SHOWITERATIONSDIALOG_H
