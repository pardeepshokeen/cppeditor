#include "showfunctionsdialog.h"
#include "ui_showfunctionsdialog.h"



showFunctionsDialog::showFunctionsDialog(QVector<functionData> dataTemp, QTextDocument *doc,QString functionName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showFunctionsDialog)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setHeaderLabels(QStringList()<< "Index" << "Function");
    setLayout(ui->verticalLayout);
    this->doc = doc;
    this->functionName = functionName;
    this->ui->treeWidget->setSelectionMode(QTreeWidget::SingleSelection);
    qDebug() << dataTemp.size();
    for(int i =0; i <dataTemp.size(); i++) data.push_back(dataTemp[i]);

    for(int i = 0;i < data.size(); i++){


        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0,QString::number(i));
        item->setText(1,functionToString(data[i], i));

        for(int j =0; j<data[i].recurseFunctions.size(); j++){
             QTreeWidgetItem *childItem = new  QTreeWidgetItem(item);
             childItem->setText(0,QString::number(data[i].recurseFunctions[j]));
             childItem->setText(1,functionToString(data[data[i].recurseFunctions[j]], data[i].recurseFunctions[j]));
        }






    }

    if(dataTemp.size()==1){
        showfunc = new showFunction(this->data[0],this->doc);
        showfunc->showMaximized();
    }


//connect(this->ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(testSlot(QTreeWidgetItem*,QTreeWidgetItem*)));

}

showFunctionsDialog::~showFunctionsDialog()
{
    delete ui;
}

QString showFunctionsDialog::functionToString(functionData function, int index)
{
    if(map.contains(index)){
        return map[index];
    }
    QString s;
    s = this->functionName + "(";
    QJsonArray arr = function.arguments["args"].toArray();
    QTreeWidgetItem item;

    for(int i =0; i<arr.size(); i++){
        s = s + arr[i].toObject()["name"].toString() + " = " + arr[i].toObject()["value"].toString() + ",";
    }
    if(arr.size())
    s.remove(s.length()-1,1);
    s = s + ")";
    map[index] = s;
    return s;


}



void showFunctionsDialog::on_buttonBox_accepted()
{
    //

    int index = this->ui->treeWidget->currentItem()->text(0).toInt();
    if(index!=-1){
        showfunc = new showFunction(this->data[index],this->doc);
        showfunc->showMaximized();
    }
}

void showFunctionsDialog::testSlot(QTreeWidgetItem *t, QTreeWidgetItem *t1)
{

    qDebug() << t->text(0);
}
