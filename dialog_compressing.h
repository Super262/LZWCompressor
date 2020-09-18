#ifndef DIALOG_COMPRESSING_H
#define DIALOG_COMPRESSING_H

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QThread>
#include <QTimer>
#include <QCloseEvent>
#include <QMessageBox>
#include "archive_thread.h"
#include "ui_dialog_compressing.h"

namespace Ui {
class DialogCompressing;
}

class DialogCompressing : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCompressing(QWidget *parent = nullptr);
    void setData(QString file_from,  QString driectory_to, QString file_to, bool isText);
    void closeEvent(QCloseEvent *event);
    ~DialogCompressing();

private slots:
    void on_btn_directory_clicked();

    void on_progressBar_valueChanged(int value);

    void readStarted();

    void readFinished();

    void archiveStarted();

    void writeStarted();

    void writeFinished();

public slots:
    void updateBar(int value);

private:
    Ui::DialogCompressing *ui;
    QString file_from, file_to, directory_to;
    bool isDone;
    ArchiveThread *worker;

};

#endif // DIALOG_COMPRESSING_H
