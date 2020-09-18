#include "dialog_unzipping.h"

DialogUnzipping::DialogUnzipping(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUnzipping)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    isDone=false;
}

void DialogUnzipping::setData(QString file_from,  QString directory_to, QString file_to, bool isText){
    this->file_from=file_from;
    this->file_to=file_to;
    this->directory_to=directory_to;
    ui->file_from->setText(file_from);
    ui->file_to->setText(file_to);
    ui->btn_directory->setEnabled(false);
    ui->progressBar->setValue(0);

    worker= new UnzipThread(this);
    worker->setData(file_from,file_to,isText);
    connect(worker, SIGNAL(readStart()),this,SLOT(readStarted()));
    connect(worker, SIGNAL(readDone()),this,SLOT(readFinished()));
    connect(worker, SIGNAL(unzipStart()),this,SLOT(unzipStarted()));
    connect(worker,SIGNAL(progressChanged(int)),this,SLOT(updateBar(int)));
    connect(worker, SIGNAL(writeStart()),this,SLOT(writeStarted()));
    connect(worker, SIGNAL(writeDone()),this,SLOT(writeFinished()));
    connect(worker, SIGNAL(workFinished()),worker,SLOT(deleteLater()));
    worker->start();
}

void DialogUnzipping::closeEvent(QCloseEvent *event)
{
    if(isDone){
        event->accept();
    }
    else{
        event->ignore();
        QMessageBox::information(this,"Please wait. ","It's not done! ");
    }
}

DialogUnzipping::~DialogUnzipping()
{
    delete ui;
}

void DialogUnzipping::readStarted()
{
    this->setWindowTitle("Loading ...");
    ui->status->setText("Loading ...");
    ui->progressBar->setValue(5);
}

void DialogUnzipping::readFinished()
{
    ui->progressBar->setValue(25);
}

void DialogUnzipping::unzipStarted()
{
    this->setWindowTitle("Decompressing ...");
    ui->status->setText("Decompressing ...");
}

void DialogUnzipping::writeStarted()
{
    this->setWindowTitle("Writing ...");
    ui->status->setText("Writing ...");
    ui->progressBar->setValue(75);
}

void DialogUnzipping::writeFinished()
{
    this->setWindowTitle("Finished! ");
    ui->status->setText("Finished! ");
    ui->progressBar->setValue(100);
    isDone=true;
}

void DialogUnzipping::on_progressBar_valueChanged(int value)
{
    if(value==100){
        ui->btn_directory->setEnabled(true);
    }
}

void DialogUnzipping::on_btn_directory_clicked()
{
    QUrl url(directory_to);
    QDesktopServices::openUrl(url);
}

void DialogUnzipping::updateBar(int value)
{
    ui->progressBar->setValue(value);
}
