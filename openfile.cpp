#include "openfile.h"
#include "ui_openfile.h"
#include <QFile>
#include <QDir>
#include <QtGui>
#include <QtCore>
bool cmp(std::pair<int, QString> &p1,std::pair<int, QString> &p2){
    return p1<p2;
}

openFile::openFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openFile)
{   this->isRejected = true;
    this->selectedIndex = -1;
    ui->setupUi(this);
    openFile::setWindowTitle("Open File");
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    QFile file(QDir::homePath()+"/CpEditor/listOfFiles.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in(&file);
    int temp; QString tempS;
    while(!in.atEnd()){
        in >> temp;
        tempS = in.readLine();
        list.push_back(std::make_pair(temp,tempS));
    }
    std::sort(list.begin(), list.end(), cmp);
    for(int i=0; i<list.size(); i++){
        ui->listWidget->addItem(QString::number(list[i].first)+"  :  "+list[i].second);
    }



}

openFile::~openFile()
{
    delete ui;
    list.clear();

}

void openFile::on_buttonBox_accepted()
{
    // ok
     this->selectedIndex = ui->listWidget->currentRow();
    isRejected = false;
    this->fileIndex = list[selectedIndex].first;
    this->fileName = list[selectedIndex].second;
    list.clear();

}

void openFile::on_buttonBox_rejected()
{
    isRejected = true;
    list.clear();
}
