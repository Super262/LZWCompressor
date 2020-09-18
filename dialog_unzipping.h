#ifndef DIALOG_UNZIPPING_H
#define DIALOG_UNZIPPING_H

#include "ui_dialog_unzipping.h"
#include "unzip_thread.h"
#include <QDialog>
#include <QDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QThread>
#include <QTimer>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class DialogUnzipping;
}

class DialogUnzipping : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUnzipping(QWidget *parent = nullptr);
    void setData(QString file_from,  QString driectory_to, QString file_to, bool isText);
    void closeEvent(QCloseEvent *event);
    ~DialogUnzipping();

private slots:

    void readStarted();

    void readFinished();

    void unzipStarted();

    void writeStarted();

    void writeFinished();

    void on_progressBar_valueChanged(int value);

    void on_btn_directory_clicked();

public slots:
    void updateBar(int value);

private:
    Ui::DialogUnzipping *ui;
    QString file_from, file_to, directory_to;
    bool isDone;
    UnzipThread *worker;


};

#endif // DIALOG_UNZIPPING_H
