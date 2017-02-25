#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setCentralWidget(ui->widget);

    ui->statusBar->showMessage("Designed & Developed by -> Pardeep Shokeen");
    tabWidget = ui->tabWidget;
    this->ui->label->setText("<b>" + QString("TEST") + "</b>");
    this->ui->label->setStyleSheet("QLabel {color: blue}");
    process = new QProcess;
    // setting up directory !!
     pathToDir = QDir::homePath()+"/CpEditor";
    if(QDir(QDir::homePath()+"/CpEditor").exists()){
       // qDebug() << "directory exists";
    }else{
        QDir().mkdir(QDir::homePath()+"/CpEditor");
        // make test File
    }
    // checking for files
    if(!QFile(pathToDir+"/0.cpp").exists()){
        QFile file(pathToDir+"/0.cpp");
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "unable to create file";
        }
    }

    if(!QFile(pathToDir+"/0input.txt").exists()){
        QFile file(pathToDir+"/0input.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
    }


    if(!QFile(pathToDir+"/listOfFiles.txt").exists()){
        QFile file(pathToDir+"/listOfFiles.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
    }


    if(!QFile(pathToDir+"/settings.txt").exists()){
        QFile file(pathToDir+"/settings.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << 0 << " " << 0 << " ";
        file.close();
    }

    fileHints.push_back("Test File");

    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+R"),this);
    connect(shortcut,SIGNAL(activated()), this, SLOT(on_pushButton_clicked()));




    //  read last Files



    this->changeLastFiles(0,lastFiles, this->totalFiles);


    customTab.push_back(new CustomTab(this, 0, pathToDir));

    tabWidget->setTabsClosable(true);

    tabWidget->addTab(customTab[0],"Test");
    qDebug() << lastFiles[0];

    for(int i=1; i<=lastFiles[0] && i < lastFiles.size(); i++){
          //qDebug() << "here";
        customTab.push_back(new CustomTab(parent,lastFiles[i], pathToDir));
        tabWidget->addTab(customTab[i]," File "+QString::number(lastFiles[i]));
    }

    // TODO :->



    //



  // signal connections
    connect(this->tabWidget,SIGNAL(tabCloseRequested(int)), this, SLOT(tabClosed(int)));
    connect(this->tabWidget,SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));








}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
// run file
    int index = this->tabWidget->currentIndex();
    this->customTab[index]->runFile();

}

void MainWindow::on_pushButton_2_clicked()
{
 // new file
    if(customTab.size() >7){
        // show alert and message to close some files
        QMessageBox::information(this,"Too much tabs","Try closing some tabs",QMessageBox::Ok);
        return ;
    }
    bool ok;
    QString fileName = QInputDialog::getText(this,"New File","Enter File Name",QLineEdit::Normal,"File Name",&ok);
    if(!ok)
        return;

    this->changeFileHints(1,totalFiles+1,fileName);

    int i = customTab.size();
    customTab.push_back(new CustomTab(this,totalFiles+1, this->pathToDir));
    tabWidget->addTab(customTab[i]," File "+QString::number(totalFiles+1));
    this->totalFiles++;
    this->lastFiles[0]++;
    lastFiles.push_back(totalFiles);
    fileHints.push_back(fileName);
    this->changeLastFiles(1,this->lastFiles,this->totalFiles);
    tabWidget->setCurrentIndex(customTab.size()-1);


   // CustomTab



}



void MainWindow::changeLastFiles(int mode, std::vector <int>  &lastFiles, int &totalFiles)
{
    QFile file(pathToDir+"/settings.txt");
    // 0-> read , else-> write;

    if(mode ==0){
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream stream(&file);
            stream >> totalFiles;
            int files = -1;

            stream >> files;
            lastFiles.push_back(files);
            int temp;
            while(files--){
                stream >> temp;
                lastFiles.push_back(temp);
                this->changeFileHints(0,temp,"");
            }
            file.close();






    }else{
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out (&file);
        out << totalFiles << " ";
        out << lastFiles[0] << " ";
        for(int i=1; i<=lastFiles[0]; i++){
            qDebug() << "writing " << lastFiles[i];
            out << lastFiles[i] << " ";
        }
        file.close();
    }


}

void MainWindow::changeFileHints(int mode, int fileNumber, QString name)
{   // 0->set file number to fileHints
    // 1-> new file to list;
    if(mode==0){
        QFile file(pathToDir+"/listOfFiles.txt");
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QTextStream in(&file);
        int temp; QString tempS;
        while(!in.atEnd()){
            in >> temp;
            tempS = in.readLine();
            if(temp==fileNumber)
            {fileHints.push_back(tempS); break;}
        }
        file.close();



    }else{
        QFile file(pathToDir+"/listOfFiles.txt");
        file.open(QIODevice::Append|QIODevice::Text);
        QTextStream out(&file);
        out << fileNumber << " " << name << "\n";
    }
}

void MainWindow::tabClosed(int index)
{
  if(!index){
    QMessageBox::information(this,"ERROR","Can't remove <b>TEST</b> File",QMessageBox::Ok);
    return ;
  }

  if(QMessageBox::information(this, "Alert","Are you sure ",QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok){

      tabWidget->removeTab(index);
      delete customTab[index];
      customTab.erase(customTab.begin()+index);

      this->lastFiles.erase(lastFiles.begin()+index);
      lastFiles[0]--;
      fileHints.erase(fileHints.begin()+index);
      this->changeLastFiles(1,lastFiles,this->totalFiles);


  }else{
      return;
  }

}

void MainWindow::tabChanged(int i)
{// slot for tab changed
    if(i< fileHints.size()){
        this->ui->label->setText("<b>"+fileHints[i].toUpper()+"</b>");
        this->ui->label->setStyleSheet("QLabel {color: blue}");
    }
}



void MainWindow::on_pushButton_3_clicked()
{
    // open a file;
    openFile open;
    open.exec();
    if(!open.isRejected){
        if(open.selectedIndex !=-1){

            customTab.push_back(new CustomTab(this, open.fileIndex, this->pathToDir));
            int i = customTab.size() -1;
            tabWidget->addTab(customTab[i]," File "+QString::number(open.fileIndex));
            lastFiles.push_back(open.fileIndex);
            lastFiles[0]++;
            fileHints.push_back(open.fileName);
            qDebug() << lastFiles.size();
            this->changeLastFiles(1,lastFiles,totalFiles);
            this->tabWidget->setCurrentIndex(customTab.size()-1);
        }
    }

}
