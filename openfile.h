#ifndef OPENFILE_H
#define OPENFILE_H

#include <QDialog>

namespace Ui {
class openFile;
}

class openFile : public QDialog
{
    Q_OBJECT

public:
    explicit openFile(QWidget *parent = 0);
    ~openFile();
    bool isRejected;
    std:: vector < std::pair<int, QString> > list;

    int selectedIndex;
    QString fileName;
    int fileIndex;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::openFile *ui;

};

#endif // OPENFILE_H
