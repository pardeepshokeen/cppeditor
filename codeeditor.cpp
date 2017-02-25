/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "codeeditor.h"

//![constructor]

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent), c(0)
{
    this->setTabStopWidth(8* fontMetrics().width(' '));
    QFont font;
    //font.setFamily("TimesNewRoman");
    font.setWordSpacing(1.5);
    font.setLetterSpacing(QFont::AbsoluteSpacing,0.8);
    font.setStretch(120);


    this->setFont(font);
    lineNumberArea = new LineNumberArea(this);
    //this->cursor = new  QTextCursor(this->textCursor());
    //this->cursor =  &(this->textCursor());

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

}

//![constructor]

//![extraAreaWidth]

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::setCompleter(QCompleter *c)
{
    if (c)
        QObject::disconnect(c, 0, this, 0);

    //c = completer;
    this->c = c;

    if (!c)
        return;


    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));

}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}
//![slotUpdateRequest]

//! [slot cursor changed]
void CodeEditor::cursorChanged()
{
    // qDebug() << "changed";

    QList<QTextEdit::ExtraSelection> extras;
    QTextEdit::ExtraSelection extra;
    QTextCursor cur = this->textCursor();

    extra.cursor = this->textCursor();
    extra.format.setForeground(Qt::green);
    extra.format.setBackground(Qt::lightGray);
    extra.format.setFontWeight(100);

    QChar charN = this->document()->characterAt(cur.position());
    QChar charP = this->document()->characterAt(cur.position()-1);

    int p1 = cur.position()-1;
    int p2 = this->findComplementBracket(charP, -1);

    int n1 = cur.position();
    int n2 = this->findComplementBracket(charN, 1);
    qDebug() << charP << " " << p2;
    if(p2>=0){

        extra.cursor.setPosition(p2);
        extra.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

        extras<< extra;
        extra.cursor.setPosition(p1);
        extra.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        extras<< extra;
    }
    if(n2>=0){
        extra.cursor.setPosition(n2);
        extra.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

        extras<< extra;
        extra.cursor.setPosition(n1);
        extra.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        extras<< extra;
    }

    this->setExtraSelections(extras);


}


//![slot cursor changed]

void CodeEditor::insertCompletion(const QString &completion)
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

int CodeEditor::findComplementBracket(QChar ch, int toward)
{
    QChar toFind;



    if(toward == 1){
            if(ch == '{'){
                toFind = '}';
            }else if(ch == '('){
                toFind = ')';
            }else if(ch == '['){
                toFind = ']';
            }else{
                return -1;
            }
           int count = 0;
            QString string = this->document()->toPlainText();
            int pos = this->textCursor().position();
            pos++;
            bool found = false;
            while(pos<string.length()){
                if(string[pos]==ch)
                    count++;
                else if(string[pos] == toFind){
                    if(count)
                        count--;
                    else
                        return pos;
                }
                pos++;
            }

            return -2;




    }else{
        if(ch == '}'){
            toFind = '{';
        }else if(ch == ')'){
            toFind = '(';
        }else if(ch == ']'){
            toFind = '[';
        }else{
            return -1;
        }


        int count = 0;
         QString string = this->document()->toPlainText();
         int pos = this->textCursor().position();
         pos-=2;
         bool found = false;
         while(pos>=0){
             if(string[pos]==ch)
                 count++;
             else if(string[pos] == toFind){
                 if(count)
                     count--;
                 else
                     return pos;
             }
             pos--;
         }

         return -2;



    }

}


QString CodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}



//![resizeEvent]

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}
//![resizeEvent]


//![keyPressEvent]
void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    QPlainTextEdit::keyPressEvent(event);

    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (event->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            event->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }


    QString string = this->document()->toPlainText();
    int pos = this->textCursor().position();



    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){

        int t = pos-2;
        bool isExtraLine = (pos-2 > 0 && string[pos-2] == '{' && string[pos] == '}'?true:false);
        while(t>=0 && string[t]!='\n' ) t--;
        int countTab = 0; int countSpace = 0; t++;
        while(string[t]=='\t' || string[t] == ' '){
            (string[t]=='\t'?countTab++:countSpace++);
            t++;
        }



        this->textCursor().insertText(QString(countTab,'\t'));
        this->textCursor().insertText(QString(countSpace,' '));

        if(isExtraLine){
            this->textCursor().insertText(QString("\t\n"));
            int posForCursor = this->textCursor().position()-1;
            this->textCursor().insertText(QString(countTab,'\t'));
            this->textCursor().insertText(QString(countSpace,' '));
            QTextCursor tempCur(this->document());
            tempCur.setPosition(posForCursor);
            this->setTextCursor(tempCur);
        }

        return;
    }



    QChar pairList[][2] = {
                           {'(',')'},
                           {'\'','\''},
                           {'"','"'},
                           {'{','}'},
                           {'[',']'}
                           };

    for(int i=0; i<5; i++){

        if((char)event->key() == pairList[i][1] && (this->document()->characterAt(this->textCursor().position()) == pairList[i][1])){



            this->textCursor().deleteChar();
//            QTextCursor tempCur = this->textCursor();
//            tempCur.setPosition(tempCur.position()+1);
//            this->setTextCursor(tempCur);

            return;
        }
        if((char)event->key() == pairList[i][0]){


            this->textCursor().insertText(QString(pairList[i][1]));
            QTextCursor tempCur = this->textCursor();
            tempCur.setPosition(tempCur.position()-1);
            this->setTextCursor(tempCur);

            return;
        }
    }


    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
//    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (event->text().isEmpty() || completionPrefix.length() < 3
                      || eow.contains(event->text().right(1))) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr);



}

void CodeEditor::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

//![keyPressEvent]


//![cursorPositionChanged]

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

//![cursorPositionChanged]

//![extraAreaPaintEvent_0]

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
//![extraAreaPaintEvent_2]

