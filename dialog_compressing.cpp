#include "dialog_compressing.h"

DialogCompressing::DialogCompressing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCompressing)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    isDone=false;
}

void DialogCompressing::setData(QString file_from,  QString directory_to, QString file_to,bool isText){
    this->file_from=file_from;
    this->file_to=file_to;
    this->directory_to=directory_to;
    ui->label_from->setText(file_from);
    ui->label_to->setText(file_to);
    ui->btn_directory->setEnabled(false);
    ui->progressBar->setValue(0);

    worker= new ArchiveThread(this);
    worker->setData(file_from,file_to,isText);
    connect(worker, SIGNAL(readStart()),this,SLOT(readStarted()));
    connect(worker, SIGNAL(readDone()),this,SLOT(readFinished()));
    connect(worker, SIGNAL(archiveStart()),this,SLOT(archiveStarted()));
    connect(worker,SIGNAL(progressChanged(int)),this,SLOT(updateBar(int)));
    connect(worker, SIGNAL(writeStart()),this,SLOT(writeStarted()));
    connect(worker, SIGNAL(writeDone()),this,SLOT(writeFinished()));
    connect(worker, SIGNAL(workFinished()),worker,SLOT(deleteLater()));
    worker->start();
}

DialogCompressing::~DialogCompressing()
{
    delete ui;
}

void DialogCompressing::on_btn_directory_clicked()
{
    QUrl url(directory_to);
    QDesktopServices::openUrl(url);
}

void DialogCompressing::on_progressBar_valueChanged(int value)
{
    if(value==100){
        ui->btn_directory->setEnabled(true);
    }
}

void DialogCompressing::readStarted()
{
    this->setWindowTitle("Loading ...");
    ui->status->setText("Loading ...");
    ui->progressBar->setValue(5);
}

void DialogCompressing::readFinished()
{
    ui->progressBar->setValue(25);
}

void DialogCompressing::archiveStarted()
{
    this->setWindowTitle("Compressing ...");
    ui->status->setText("Compressing ...");
}

void DialogCompressing::writeStarted()
{
    this->setWindowTitle("Writing ...");
    ui->status->setText("Writing ...");
    ui->progressBar->setValue(75);
}

void DialogCompressing::writeFinished()
{
    this->setWindowTitle("Finished! ");
    ui->status->setText("Finished! ");
    ui->progressBar->setValue(100);
    isDone=true;
}

void DialogCompressing::updateBar(int value)
{
    ui->progressBar->setValue(value);
}

void DialogCompressing::closeEvent(QCloseEvent *event){
    if(isDone){
        event->accept();
    }
    else{
        event->ignore();
        QMessageBox::information(this,"Warning","It's not done! ");
    }
}
