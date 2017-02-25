#include "showfunction.h"
#include "ui_showfunction.h"

showFunction::showFunction(functionData tempData, QTextDocument *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showFunction)
{


    ui->setupUi(this);
    this->doc = doc;
    QVector<lineInfo> tempForVec;
    if(tempData.lineList.size())
        this->vec.push_back(tempForVec);


    int pLine = -1;
    // for iterator lines;
    for(int i =0; i<tempData.lineList.size(); i++){
        if(tempData.lineList[i].lineNo < pLine)
            this->itLines.insert(tempData.lineList[i].lineNo);
        pLine = tempData.lineList[i].lineNo;
    }



     pLine  = INT_MIN;
    int j = 0;

    for(int i =0; i<tempData.lineList.size(); i++){
        lineInfo line = tempData.lineList[i];
        if(line.lineNo < pLine){
            vec.push_back(tempForVec);
            j++;

        }
        if(this->itLines.contains(line.lineNo)){
            if(this->itPages.contains(line.lineNo))
                this->itPages[line.lineNo].push_back( QPair<int,lineInfo>(j,line));
            else{
                QVector <QPair<int,lineInfo> > tempVec;
                tempVec.push_back(QPair<int,lineInfo>(j,line));
                this->itPages[line.lineNo] = tempVec;

            }
        }
        vec[j].push_back(line);
        pLine = line.lineNo;
    }
    this->totalPages = j;
    this->currentPage = 0;


//    for(QMap<int, QVector<QPair<int,lineInfo> > >::iterator it = this->itPages.begin(); it!=this->itPages.end(); it++){
//        qDebug() << it.key();
//        qDebug() << it.value()[0].first;
//    }


    //
    this->ui->pageLabel->setStyleSheet("QLabel{color:red;font:bold}");

    codeViewer = new CodeViewer(doc,this);








    //font.setBold(true);
    ui->treeWidgetAfter->setStyleSheet("QTreeWidget{color:black}");
    ui->treeWidgetBefore->setStyleSheet("QTreeWidget{color:black}");
    ui->treeWidgetAfter->setColumnCount(2);
    ui->treeWidgetAfter->setHeaderLabels(QStringList() << "Name" << "Value");
    ui->treeWidgetBefore->setColumnCount(2);
    ui->treeWidgetBefore->setHeaderLabels(QStringList() << "Name" << "Value");

    QWidget *tempWidget  = new QWidget();
    tempWidget->setLayout(ui->controlLayout);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(codeViewer,0,0,6,1);
    mainLayout->addWidget(tempWidget,6,0,1,1 );
    mainLayout->addWidget(ui->varSplitter,7,0,3,1 );
    setLayout(mainLayout);







connect(this->codeViewer, SIGNAL(doubleClickOnLine(int)), this, SLOT(hasDoubleClick(int)));
connect(this->codeViewer,SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
connect(this->codeViewer,SIGNAL(pageShowChange()), this, SLOT(pageShowChange()));











codeViewer->showPage(this->vec[this->currentPage], this->itLines);
}

showFunction::~showFunction()
{
    delete ui;
}

void showFunction::objectToTree(QJsonValue val, QTreeWidgetItem *parent)
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

void showFunction::addObjectToTree(QJsonObject object, QTreeWidget *parent)
{
    QJsonObject::iterator it = object.begin();
    for(; it!=object.end(); it++){
          QTreeWidgetItem *child  = new QTreeWidgetItem(parent);
          child->setText(0,it.key());
          this->objectToTree(it.value(), child);
    }

}

void showFunction::hasDoubleClick(int line)
{   if(this->itLines.contains(line)){

        itDialog = new showIterationsDialog(this->itPages[line],this->doc->findBlockByLineNumber(line-1).text(), this);
        connect(this->itDialog, SIGNAL(changePage(int)), this, SLOT(onPageChangeRequest(int)));
        itDialog->show();
    }
}

void showFunction::cursorChanged()
{
    ui->treeWidgetAfter->clear();
    ui->treeWidgetBefore->clear();
    int line = this->codeViewer->textCursor().blockNumber()+1;
    for(int i = 0; i < this->vec[this->currentPage].size(); i++){
        lineInfo tempLine = vec[this->currentPage][i];
        if(tempLine.lineNo == line){
           addObjectToTree(tempLine.prev, ui->treeWidgetBefore);
           addObjectToTree(tempLine.next, ui->treeWidgetAfter);
            return;
        }
    }

    QJsonObject tempObj ;
    tempObj.insert("**", QJsonValue("No Data Available"));
    addObjectToTree(tempObj, ui->treeWidgetBefore);
    addObjectToTree(tempObj, ui->treeWidgetAfter);

}

void showFunction::pageShowChange()
{

    this->ui->pageLabel->setText(QString::number(this->currentPage) + "/" + QString::number(this->totalPages));


}

void showFunction::on_pushButton_3_clicked()
{
    // next Page
    if(this->currentPage < this->totalPages)
    codeViewer->showPage(this->vec[++(this->currentPage)], this->itLines);
}

void showFunction::on_pushButton_4_clicked()
{
    // prev Page
    if(this->currentPage > 0)
    codeViewer->showPage(this->vec[--(this->currentPage)], this->itLines);
}

void showFunction::on_pushButton_2_clicked()
{
    // last Page

    codeViewer->showPage(this->vec[this->totalPages], this->itLines);
    this->currentPage = this->totalPages;
}

void showFunction::on_pushButton_clicked()
{
    // first page
    codeViewer->showPage(this->vec[0], this->itLines);
    this->currentPage = 0;
}

void showFunction::onPageChangeRequest(int page)
{
    // change page;
    qDebug() << "here" << page;
    this->currentPage = page;
    this->codeViewer->showPage(this->vec[page], this->itLines);
}
