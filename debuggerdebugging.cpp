#include "debuggerdebugging.h"
#include "ui_debuggerdebugging.h"

DebuggerDebugging::DebuggerDebugging(QString functionToDebug, QString fileName, QTextDocument *sourceDocument , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebuggerDebugging)
{
    ui->setupUi(this);



    this->funcToDebug = functionToDebug;
//
    this->doc = sourceDocument;



    //

    this->process = new QProcess();
    programRunning = false;
//    this->strlist = new QStringList();
    process->setWorkingDirectory(QDir::homePath()+"/CpEditor/");
   QString stringForDebug = "file " + fileName + "\n -break-insert "+functionToDebug+"\n r < " + fileName+"input.txt\n";
    process->start("gdb -interpreter=mi");
    process->write(stringForDebug.toUtf8());



   // process->write("quit\n");
//    QString text = "for(int i=0; i<TestVar; i++";
//    QString var = "i";
//    qDebug() << isStringContains(text,var);

    connect(this->process, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(this,SIGNAL(debuggerDebugComplete()), this, SLOT(onDebuggerDebugComplete()));












}

DebuggerDebugging::~DebuggerDebugging()
{
    delete ui;
}

QVector<lineInfo> DebuggerDebugging::processOutput(QVector<QPair<int, QJsonObject> > &vec)
{
    QVector <lineInfo> lineInfoVector;
    if(!vec.size()) return lineInfoVector ;
    QVector <int> itLines;
    int size = vec.size();
    int prevLine = 0;
    lineInfo line;
    QJsonObject nullObj;

  if(vec.size() == 1){
    line = this->processLineInfo(nullObj, vec[0].second, vec[0].first);
    lineInfoVector.push_back(line);
    return lineInfoVector;
  }


    for(int i=0; i<size-1; i++){
        //line.lineNo = vec[i].first;
        line = this->processLineInfo(vec[i+1].second, vec[i].second, vec[i].first);
        lineInfoVector.push_back(line);
    }

        line = this->processLineInfo(nullObj,vec[size-1].second,vec[size-1].first);
        lineInfoVector.push_back(line);


//    for(int i=0; i<lineInfoVector.size(); i++)
//        qDebug() << lineInfoVector[i].lineNo << lineInfoVector[i].next << lineInfoVector[i].prev;

return lineInfoVector;


}

lineInfo DebuggerDebugging::processLineInfo(QJsonObject &next, QJsonObject &prev, int lineN)
{
    lineInfo line;
   // static int count = 0;
    line.next.empty();
    line.prev.empty();
   // qDebug() << count++;
    line.lineNo = lineN;
    QString lineText = this->doc->findBlockByLineNumber(lineN-1).text();
    QJsonArray variables = next["variables"].toArray();
    for(int i=0; i<variables.size(); i++){

        QString varName = variables[i].toObject()["name"].toString();
        if(this->isStringContains(lineText, varName )){
            //qDebug() << varName << " $$ in $$ " <<  lineText;
            line.next.insert(varName, QJsonValue(this->parseValue(variables[i].toObject()["value"].toString())));
           // QJsonValue jsonval = this->parseValue(variables[i].toObject()["value"].toString());
          //  qDebug() << varName << " <=> " << jsonval;
           // line.next.insert(varName, jsonval);
          //  qDebug() << line.next;
        }
    }
    //qDebug() << line.next;

     variables = prev["variables"].toArray();
    for(int i=0; i<variables.size(); i++){
        QString varName = variables[i].toObject()["name"].toString();
        if(this->isStringContains(lineText, varName )){
            line.prev.insert(varName, QJsonValue(this->parseValue(variables[i].toObject()["value"].toString())));
        }
    }

return line;
}

bool DebuggerDebugging::isStringContains(QString &str, QString &var)
{
    int strLen = str.length();
    int varLen = var.length();
    bool notFound = true;
    for(int i=0; i<strLen; i++){
        notFound = false;
        for(int j=0; j<varLen; j++){
            if(i+j >= strLen) break;
            if(str[i+j] != var[j]){
                notFound = true;
                break;
            }
        }
        if(!notFound){
            if(i>0 && (str[i-1].isLetterOrNumber() || (str[i-1]=='_') ) ){
                notFound = true;
            }
            if(str[i+varLen].isLetterOrNumber() ||  (str[i+varLen]=='_')){
                notFound = true;
            }

        }
        if(!notFound)
           return true;
    }
    return false;
}

QJsonObject DebuggerDebugging::strToObject(QString str)
{
    //qDebug() << str;
       str = str.split('\n')[0];
       //qDebug() << str;

   //    QFile file(QDir::homePath()+"/Desktop/gdbTest/gdbOutput.txt");
   //    file.open(QIODevice::Append | QIODevice::Text);
   //    QTextStream stream(&file);
   //    stream<<str<<"\n";

       int pos;
       while((pos=str.indexOf("std::"))!=-1){
           pos;
           int temp = pos;
           while(str[temp]!='{' && str[temp]!='[' && str[temp]!='\"') temp++;
           str.remove(pos, temp-pos);
       }
       bool isOpened = false;
       for(int i=0; i<str.length(); i++){
          //qDebug() << i;

           if(str[i]=='\\' && str[i+1]=='"'){
               i++;
               continue;
           }
           if((str[i].isLetterOrNumber() || str[i]=='-') && !isOpened){
            //   qDebug()<< "here";
               int t = i;
               while( str[t]!='=' && str[t]!='\0' ) t++;
               str.insert(i,'"');
               str.insert(t+1,'"');
               i = t+1;
               continue;
           } else if(str[i]=='"'){

               isOpened=!isOpened;
           }

       }
       for(int i=0; i<str.length(); i++){
           if(str[i]=='='){
               str.replace(i,1,':');
           }
       }
      // qDebug()<< "this";
      // qDebug() << str;
      str.insert(0,"{");
      str+="}";




   //    this->ui->plainTextEdit->document()->setPlainText(str);
      if(str.indexOf("\"stack\":[\"frame\"") !=-1)
        {
          str.insert(10,'{');
          str.insert(str.length()-2,'}');
      }


       QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
       //if(doc.isEmpty()) qDebug() << str;
       QJsonObject obj = doc.object();
      // qDebug() << obj;





       return obj;
}

QJsonValue DebuggerDebugging::parseValue(QString str)
{
    int pos = 0;
    while(str[pos]==' ') pos++;
    QStringList list;
    if(str[pos]=='{'){
        int openCount = 1;
        for(int i =pos+1; i<str.length(); i++){
            //qDebug() << pos << " " << i ;
            QChar ch = str[i];
            if(ch=='{'){
                openCount++;
                continue;
            }
            if((ch==','||ch=='}') && openCount==1){
                list.push_back(str.mid(pos+1,i-pos-1));
               // qDebug() << str.mid(pos+1,i-pos-1);
                pos = i;

            }
            if(ch=='}') openCount--;

        }

        QJsonArray arr;
       // qDebug () << list.length();
      for(int i=0; i<list.length(); i++){
        //  qDebug() << "hre " << list[i];
          arr.push_back(QJsonValue(this->parseValue(list[i])));
      }
       return QJsonValue(QJsonArray(arr));
      //return val;


    }else{
        int temp;
        if((temp = str.indexOf(':'))!=-1){
            QJsonObject obj;
            //qDebug() << str;
            //qDebug() << temp;
            //qDebug() << str.left(temp) << " +++ " << str.mid(temp+1);
            obj.insert(str.left(temp), this->parseValue(str.mid(temp+1)));
            return QJsonValue(QJsonObject(obj));

        }else{
            return  QJsonValue(str);
        }
    }
    //        return QJsonValue(str);
}

std::pair<QString, QJsonObject> DebuggerDebugging::parseGdbOutput(QString str)
{

    std::pair<QString, QJsonObject> p;
    QStringList list = str.split('\n');
    //qDebug() << list.size();

    for(int i=0; i<list.length(); i++){
     QString listi = list[i];
     if(listi[0]!='*' && listi[0] != '^'){
         list.removeAt(i);
         i--;
     }
    }


    for(int i=0; i<list.length(); i++){
        if(list[i][0] == '*'){
            if(list[i][1] == 's' &&
               list[i][2] == 't' &&
               list[i][3] == 'o' &&
               list[i][4] == 'p' &&
               list[i][5] == 'p' &&
               list[i][6] == 'e' &&
               list[i][7] == 'd' &&
               list[i][8] == ','
               ){
                p.first = "*stopped";
                p.second = this->strToObject(list[i].mid(9));
                return p;

            }
        }else
            if(
                list[i][1] == 'd' &&
                list[i][2] == 'o' &&
                list[i][3] == 'n' &&
                list[i][4] == 'e' &&
                list[i][5] == ','
                    ){
                p.first = "^done";
                p.second = this->strToObject(list[i].mid(6));
                return p;
            }
        else if(list[i][1] == 'e' &&
                 list[i][2] == 'r' &&
                 list[i][3] == 'r' &&
                 list[i][4] == 'o' &&
                 list[i][5] == 'r' &&
                 list[i][6] == ','
                 ){
                p.first = "^error";
                p.second = this->strToObject(list[i].mid(7));
                return p;
        }
    }


        p.first = "noData";
        p.second = QJsonObject();
        return p;
}

void DebuggerDebugging::debugBrain(std::pair<QString, QJsonObject> p)
{
    static int count = 0;


    static int lineNumber;
    static bool inRecursion = false;
//    if(inRecursion) qDebug() << "recurse";
    static int recursionFrames ;
   static QJsonArray arguments;
 //   QJsonObject prevObj;

     QString token = p.first;
     QJsonObject obj = p.second;
 //    QFile file(QDir::homePath()+"/Desktop/gdbTest/gdbOutput.txt");
 //    file.open(QIODevice::Append | QIODevice::Text);
 //    QTextStream out(&file);
 //    out << token << ":" << QString(QJsonDocument(obj).toBinaryData());



    // qDebug() << token;
     //qDebug() << obj;
     //vec.push_back(obj);
     static  QString lastCommand = "continue";
     //qDebug() << obj["frame"].toObject()["func"].toString();
     if(token == "noData"){
         return;
     }else{
         /// Continue
         if(lastCommand=="continue"){

             if(token == "*stopped"){
                 QString reason = obj["reason"].toString();

                 if(reason == "exited-normally"){


//                     qDebug() << "program closed";
                     this->process->write("quit\n");
                     this->process->close();
                     emit debuggerDebugComplete();
                     return;
                 }else if(reason == "breakpoint-hit"){
                     //TODO: create new function..
                     arguments = obj["frame"].toObject()["args"].toArray();


                     lastCommand = "viewParentFrame";
                     this->process->write("-stack-list-frames 1 1 \n");
                     lineNumber = obj["frame"].toObject()["line"].toString().toInt();

                     return;

                 }

             }






         /// next Step
         }else if(lastCommand == "nextStep"){
             lineNumber = obj["frame"].toObject()["line"].toString().toInt();

             if(token == "*stopped"){
                 QString reason = obj["reason"].toString();
                 if(reason == "end-stepping-range"){


                    // /// TODO:  continue the program
                     if(obj["frame"].toObject()["func"].toString() != this->funcToDebug){
                         lastCommand = "continue";
                         this->process->write("-exec-continue\n");
                         return;
                     }else{
                         if(inRecursion){
                             lastCommand = "checkDepth";
                             this->process->write("-stack-info-depth\n");
                           //  prevObj = obj;
                             return;



                         }else{
                             //debugData[this->currentFunction].vec.push_back(QPair<int,obj>);
                             lastCommand = "viewVariables";
                             this->process->write("-stack-list-variables 1\n");
                             lineNumber = obj["frame"].toObject()["line"].toString().toInt();
                             return;


                         }


                     }

                     lastCommand = "viewVariables";
                     this->process->write("-stack-list-variables 1\n");
                     lineNumber = obj["frame"].toObject()["line"].toString().toInt();
                 }else if(reason == "breakpoint-hit"){
                     // new function;
                     arguments = obj["frame"].toObject()["args"].toArray();
 //                    qDebug() << obj["frame"].toObject()["args"];
                     //qDebug() << arguments;
                     lastCommand = "viewParentFrame";
                     this->process->write("-stack-list-frames 1 1\n");
                    // qDebug() << lastCommand;
                     lineNumber = obj["frame"].toObject()["line"].toString().toInt();

                     return;

                 }else if(reason == "exited-normally"){
                     this->process->write("quit\n");
                     this->process->close();
                     emit debuggerDebugComplete();
                     return;
                 }


             }


         /// view Variables
         }else if(lastCommand == "viewVariables"){

             this->ui->label->setText(QString::number((count++)));
             if(token=="^done"){
                 //this->vec.push_back(QPair<int,QJsonObject>(lineNumber,obj));
                 debugData[this->currentFunction].vec.push_back(QPair<int, QJsonObject>(lineNumber, obj));
                 lastCommand = "nextStep";
                 this->process->write("-exec-next\n");
                 return;
             }




         /// view stack depth
         }else if(lastCommand == "viewStackDepth"){

             if(token=="^done"){
                 int previousRecursiveDepth = recursionFrames;
                 recursionFrames = obj["depth"].toString().toInt();

                 if(previousRecursiveDepth == recursionFrames){
                     if(inRecursion){

                         functionStack.pop();
                         int tempCurrentFunction = functionStack.top();
                         functionStack.pop();
                         functionStack.push(this->currentFunction);
                         this->debugData[tempCurrentFunction].recurseFunctions.push_back(this->currentFunction);
                     }
                 }
                 lastCommand = "viewVariables";
                 this->process->write("-stack-list-variables 1\n");
                 return;

             }




         /// view Parent Frame
         }else if(lastCommand == "viewParentFrame"){
             if(token == "^error"){
                 functionData data;
                 data.arguments.insert("args",QJsonValue(arguments));
                 debugData.push_back(data);
                 this->currentFunction = debugData.size() - 1;
                 this->functionStack.push(this->currentFunction);
                 lastCommand = "viewVariables";
                 this->process->write("-stack-list-variables 1\n");
                 inRecursion = false;
                 return;


             }else if(token == "^done"){
//                 qDebug() << "here--";
 //                qDebug() << obj;
                 QJsonObject parentFrame = obj["stack"].toArray()[0].toObject()["frame"].toObject();
 //                qDebug() << parentFrame;
                 if(parentFrame["func"].toString() == this->funcToDebug){
                    // qDebug() << "here";
                     // this is recursion
                     inRecursion = true;

                     functionData data;
                     data.arguments.insert("args",QJsonValue(arguments));
//                     qDebug() << data.arguments;
                     debugData.push_back(data);
                     // tempCurrentFunction = this->currentFunction;

                     this->currentFunction = debugData.size() - 1;
                     functionStack.push(this->currentFunction);


                     lastCommand = "viewStackDepth";
                     process->write("-stack-info-depth\n");
 //                    process->write("-stack-list-variables 1\n");
 //                    lastCommand = "viewVariables";
                     return;


                 }else{

                     inRecursion = false;
                    functionData data;
                    data.arguments.insert("args",QJsonArray(arguments));
                   debugData.push_back(data);
                   this->currentFunction = debugData.size() -1;
                   functionStack.push(this->currentFunction);
                   lastCommand = "viewVariables";
                   process->write("-stack-list-variables 1\n");
                   return;

                 }

             }





         }else if(lastCommand == "checkDepth"){
             /// check depth in case of recursion
             int depth = obj["depth"].toString().toInt();
//             qDebug() << "here" << depth << " " << recursionFrames;
             if(depth < recursionFrames){

                 functionStack.pop();
                 this->currentFunction = functionStack.top();
//                 qDebug() << "here " << recursionFrames;
                 recursionFrames--;
             }

//             if(functionStack.size() == 1){
//                 inRecursion = false;
//             }
           //  lineNumber = prevObj["frame"].toObject()["line"].toString().toInt();
             lastCommand = "viewVariables";
             this->process->write("-stack-list-variables 1\n");
             return;
         }





       //  qDebug() << "here";

     }
}

void DebuggerDebugging::readOutput()
{

 if(programRunning==false){
      programRunning = true;
      return;
 }

 QString output = this->process->readAllStandardOutput();
  // qDebug() << "output =  " << output ;
  std::pair<QString, QJsonObject> result = this->parseGdbOutput(output);

  if(result.first !="noData")
      debugBrain(result);


}

void DebuggerDebugging::onDebuggerDebugComplete()
{
    this->process->close();
    int sizeFunctions = this->debugData.size();
    for(int i=0; i<sizeFunctions; i++){
        this->debugData[i].lineList = this->processOutput(this->debugData[i].vec);
        this->debugData[i].vec.clear();
    }

    functionDialog = new showFunctionsDialog(this->debugData, this->doc, this->funcToDebug);
    functionDialog->show();
    this->accept();




}


void DebuggerDebugging::on_pushButton_clicked()
{
    // close debugging()
    this->process->write("quit\n");
    this->process->close();
    emit debuggerDebugComplete();
}
