#include "dialog_create.h"
#include "ui_dialog_create.h"
DialogCreate::DialogCreate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCreate)
{
    ui->setupUi(this);
}

DialogCreate::~DialogCreate()
{
    delete ui;
}

void DialogCreate::on_btn_start_clicked()
{
    int count=2;
    QString from, to, d_to;
    if((!ui->file_to->text().isEmpty())&&(!ui->directory_from->text().isEmpty())&&(!ui->directory_to->text().isEmpty())){
        from=ui->directory_from->text();
        d_to=ui->directory_to->text()+"/";
        to=(d_to+(ui->file_to->text()))+(ui->label_sort->text());
        QFile file_from(from);
        QFile file_to(to);
        if(!file_from.open(QFile::ReadOnly)){
            QMessageBox::warning(this,"Warning","Invalid source file! ");
            count--;
        }
        if(!file_to.open(QFile::WriteOnly)){
            QMessageBox::warning(this,"Warning","Writing failed! ");
            count--;
        }
    }
    else{
        if(ui->directory_from->text().isEmpty()){
            QMessageBox::warning(this,"Warning","Empty source file! ");
            count--;
        }
        if(ui->directory_to->text().isEmpty()|ui->file_to->text().isEmpty()){
            QMessageBox::warning(this,"Warning","Empty target file!");
            count--;
        }
    }
    if(count==2){
        DialogCompressing d;
        d.setData(from,d_to,to,isTextFrom);
        d.exec();
    }
}

void DialogCreate::on_rbtn_txt_clicked()
{
    isTextFrom=true;
    ui->label_sort->setText(".lzwt");

}

void DialogCreate::on_rbtn_pic_clicked()
{
    isTextFrom=false;
    ui->label_sort->setText(".lzwp");
}

void DialogCreate::on_btn_choose1_clicked()
{
    QString fliter;
    if(isTextFrom){
        fliter="text(*.txt)";
    }
    else{
        fliter="bitmap(*.bmp)";
    }
    QString filename=QFileDialog::getOpenFileName(this,"Select source file",QDir::homePath(),fliter);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(!file.open(QFile::ReadOnly)){
            QMessageBox::warning(this,"Warning","Invalid file! ");
        }
        else{
            ui->directory_from->setText(filename);
        }
    }
}

void DialogCreate::on_btn_choose2_clicked()
{
    QString directory=QFileDialog::getExistingDirectory(this,"Choose target folder",QDir::homePath());
    if(!directory.isEmpty()){
        if(QDir(directory).exists()){
            ui->directory_to->setText(directory);
        }
        else{
            QMessageBox::warning(this,"Warning","Invalid path! ");
        }
    }
}
