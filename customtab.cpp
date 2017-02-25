#include "customtab.h"
QAbstractItemModel *CustomTab::modelFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, completer);
}

CustomTab::CustomTab(QWidget *parent, int fileIndex, QString pathToDir)
{
   // widget = new QWidget(parent);
    gridLayout = new QGridLayout(this);
    editor = new CodeEditor(this);
    // setting completer
    this->completer = new QCompleter(this);
   // completer->setModel(modelFromFile("://wordlist.txt"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    this->editor->setCompleter(completer);

    //

    inputText = new QTextEdit(this);
    outputText = new QTextEdit(this);
    inputLabel = new QLabel(this);\
    outputLabel = new QLabel(this);
    this->fileIndex = fileIndex;
    this->pathToDir = pathToDir;
    functionToDebugName = new QLineEdit(this);
    functionToDebugName->setText("main");
    debugLabel = new QLabel(this);
    debugButton = new QPushButton(this);
   /* Highlighter *highlighter = */new Highlighter(editor->document());
    debugLabel->setText("Function Name to Debug");
    debugButton->setText("Debug");
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(functionToDebugName);
    hlayout->addWidget(debugButton);
    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->addWidget(debugLabel);
    vlayout->addLayout(hlayout);



    process = new QProcess();



    outputText->setLineWrapMode(QTextEdit::NoWrap);
    gridLayout->addWidget(editor, 0,0,20,10);
    inputLabel->setText("<b>STD  INPUT</b>");
    outputLabel->setText("<b>STD OUTPUT</b>");
    gridLayout->addWidget(inputLabel,0,10,1,5);
    gridLayout->addWidget(inputText,1, 10,7,5);

    gridLayout->addWidget(outputLabel,8,10,1,5);
    gridLayout->addWidget(outputText,9,10,6,5);
    gridLayout->addLayout(vlayout,15,10,5,5);

     cppName = pathToDir+"/"+QString::number(fileIndex)+".cpp";
     inputName = pathToDir+"/"+QString::number(fileIndex)+"input.txt";
     editor->setFocus();
     outputText->setReadOnly(true);

     // splitter
//     QSplitter *hsplitter = new QSplitter(Qt::Horizontal);
//     hsplitter->addWidget(CodeEditor);
//     QSplitter *vsplitter = new QSplitter(Qt::Vertical);





     //






    initializeFiles();




    connect(this->editor , SIGNAL(textChanged()), this , SLOT(editorChanged()) );
    connect(this->inputText,SIGNAL(textChanged()), this, SLOT(inputChanged()) );
    connect(this->debugButton,SIGNAL(clicked(bool)), this, SLOT(debugStart(bool)));

}


void CustomTab::saveFile(QString fileName, QString Text){
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    out << Text;
    file.close();
    out.flush();
}

void CustomTab::editorChanged(){
    QString text = editor->document()->toPlainText();
    saveFile(cppName, text);
}

void CustomTab::inputChanged(){
    QString text = inputText->document()->toPlainText();
    saveFile(inputName,text);
}

void CustomTab::debugStart(bool e)
{   // start input
    if(this->runFile()){
        //
        QString functionName = this->functionToDebugName->text();
        // checking valid function;
        bool functionNameOk = true;
        for(int i = 0; i<functionName.length(); i++){
            QChar ch = functionName[i];
            if(!ch.isLetterOrNumber() && ch!='_'){
                functionNameOk = false;
                break;
            }
        }

        if(!functionNameOk){
            QMessageBox::information(this, "Error","Write a valid function name");
            return;
        }
      //  qDebug() << this->editor->document()->toPlainText();
        debugDialog = new DebuggerDebugging(QString(functionName), QString::number(this->fileIndex),this->editor->document());
        debugDialog->show();
     //   debugDialog->accept();




    }

}
void CustomTab::initializeFiles(){
    cppFile = new QFile(cppName);
    inputFile = new QFile(inputName);
  //  notesFile = new QFile(pathToDir+"/"+QString::number(fileIndex)+"notes.cpp");
    if(!cppFile->open(QIODevice::ReadOnly|QIODevice::Text))
        cppFile->open(QIODevice::WriteOnly|QIODevice::Text);
    cppFile->close();
    cppFile->open(QIODevice::ReadOnly|QIODevice::Text);

    QTextStream cpp(cppFile);
    editor->document()->setPlainText(cpp.readAll());
    cppFile->close();
    if(!inputFile->open(QIODevice::ReadOnly|QIODevice::Text))
            inputFile->open(QIODevice::WriteOnly|QIODevice::Text);
    inputFile->close();
    inputFile->open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream input(inputFile);
    inputText->document()->setPlainText(input.readAll());
    inputFile->close();
}

bool CustomTab::runFile(){

    bool isOk = true;
    QString fileName = QString::number(fileIndex);
     this->process->setWorkingDirectory(QDir::homePath()+"/CpEditor");


    this->process->start( " g++ -g " + fileName + ".cpp  -o " + fileName+ " ");
    this->process->waitForFinished(10000);

    //process->close();
    QString error =  this->process->readAllStandardError();
    process->close();
    if(error.length()){
        isOk = false;
        this->outputLabel->setStyleSheet("QLabel {color:red}");
        this->outputText->document()->setPlainText(error);


    }else{

        process->setStandardInputFile( this->pathToDir+"/"+fileName+"input.txt");


        process->start("./"+fileName);
        this->process->waitForFinished(4000);
        this->outputLabel->setStyleSheet("QLabel {color:green} ");
        this->outputText->document()->setPlainText(this->process->readAllStandardOutput());
        process->close();

    }
 return isOk;



}

 CustomTab::~CustomTab(){}



