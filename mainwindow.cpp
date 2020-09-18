#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignCenter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_create_clicked()
{
    create = new DialogCreate(this);
    create->exec();
}

void MainWindow::on_btn_open_clicked()
{
    QString fliter="compressed file (*.lzwt *.lzwp)";
    QString filename=QFileDialog::getOpenFileName(this,"Select source file",QDir::homePath(),fliter);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(!file.open(QFile::ReadOnly)){
            QMessageBox::warning(this,"Warning","Invalid file! ");
        }
        else{
            expand=new DialogExpanding(this);
            expand->setData(filename);
            expand->exec();
        }
    }
}
