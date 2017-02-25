#include "showiterationsdialog.h"
#include "ui_showiterationsdialog.h"

#include <QTextDocument>

showIterationsDialog::showIterationsDialog(QVector<QPair<int,lineInfo> >tempData, QString lineText, QWidget *parent):
    QDialog(parent),
    ui(new Ui::showIterationsDialog)
{
    ui->setupUi(this);
    ui->treeWidgetAfter->setStyleSheet("QTreeWidget{color:black}");
    ui->treeWidgetBefore->setStyleSheet("QTreeWidget{color:black}");
    ui->treeWidgetAfter->setColumnCount(2);
    ui->treeWidgetAfter->setHeaderLabels(QStringList() << "Name" << "Value");
    ui->treeWidgetBefore->setColumnCount(2);
    ui->treeWidgetBefore->setHeaderLabels(QStringList() << "Name" << "Value");
    ui->listWidget->setSelectionMode(QListWidget::SingleSelection);




   for(int i =0; i<tempData.size(); i++){
       data.push_back(tempData[i]);
       ui->listWidget->addItem(QString(lineText));
   }






    connect(this->ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(listItemChanged(int)));






}

showIterationsDialog::~showIterationsDialog()
{
    delete ui;
}

void showIterationsDialog::objectToTree(QJsonValue val, QTreeWidgetItem *parent)
{
    if(val.isObject()){
        //QTreeWidgetItem *tree = new QTreeWidgetItem(parent);
        QJsonObject obj = val.toObject();
        QJsonObject::iterator it = obj.begin();
        parent->setText(0,it.key());
        this->objectToTree(it.value(),parent);

    }else if(val.isArray()){
       // QTreeWidgetItem *tree = new QTreeWidgetItem(parent);
        QJsonArray arr = val.toArray();
         parent->setText(1,QString::number(arr.size()));
        if(arr[0].isObject() || arr[0].isArray()){
            for(int i =0; i<arr.size(); i++){
                QTreeWidgetItem *child = new QTreeWidgetItem(parent);
                this->objectToTree(arr[i], child);
            }



        }else{
            for(int i =0; i<arr.size(); i++){
                QTreeWidgetItem *child = new QTreeWidgetItem(parent);
                child->setText(0, "[" + QString::number(i) + "]");
                child->setText(1, arr[i].toString());
            }


        }


    } else if(val.isString()){
        parent->setText(1,val.toString());
    }
}

void showIterationsDialog::addObjectToTree(QJsonObject object, QTreeWidget *parent)
{
    QJsonObject::iterator it = object.begin();
    for(; it!=object.end(); it++){
          QTreeWidgetItem *child  = new QTreeWidgetItem(parent);
          child->setText(0,it.key());
          this->objectToTree(it.value(), child);
    }
}

void showIterationsDialog::on_buttonBox_accepted()
{
   // line accepted;
    int index = this->ui->listWidget->currentRow();
    if(index!=-1){

        emit changePage(this->data[index].first);

    }

}

void showIterationsDialog::on_pushButton_2_clicked()
{
    // go to last Page
    emit changePage(this->data[this->data.size()-1].first);
    this->accept();
}

void showIterationsDialog::on_pushButton_clicked()
{
    // go to first page;
    emit changePage(this->data[0].first);
    this->accept();
}

void showIterationsDialog::listItemChanged(int line)
{
    ui->treeWidgetAfter->clear();
    ui->treeWidgetBefore->clear();
    addObjectToTree(this->data[line].second.prev, ui->treeWidgetBefore);
    addObjectToTree(this->data[line].second.next, ui->treeWidgetAfter);
}
