#include "dialog_expanding.h"
#include "ui_dialog_expanding.h"

DialogExpanding::DialogExpanding(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogExpanding)
{
    ui->setupUi(this);
}

void DialogExpanding::setData(QString from)
{
    file_from=from;
    ui->label_from->setText(file_from);
    QString ext=file_from.mid(file_from.size()-4,4);//abcd.lzwt
    if(ext.compare("lzwt")==0){
        isText=true;
        ui->label_ext->setText(".txt");
    }
    else{
        isText=false;
        ui->label_ext->setText(".bmp");
    }
}

DialogExpanding::~DialogExpanding()
{
    delete ui;
}

void DialogExpanding::on_btn_cho_dir_clicked()
{
    QString directory=QFileDialog::getExistingDirectory(this,"Select target folder ...",QDir::homePath());
    if(!directory.isEmpty()){
        if(QDir(directory).exists()){
            ui->lineEdit_to->setText(directory);
        }
        else{
            QMessageBox::warning(this,"Warning","Invalid path! ");
        }
    }
}

void DialogExpanding::on_btn_start_clicked()
{
    int count=2;
    QString from, to, d_to;
    if((!ui->lineEdit_to->text().isEmpty())&&(!ui->label_from->text().isEmpty())&&(!ui->lineEdit_filename->text().isEmpty())){
        from=ui->label_from->text();
        d_to=ui->lineEdit_to->text()+"/";
        to=(d_to+(ui->lineEdit_filename->text()))+(ui->label_ext->text());
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
        if(ui->label_from->text().isEmpty()){
            QMessageBox::warning(this,"Warning","Empty source file! ");
            count--;
        }
        if(ui->lineEdit_to->text().isEmpty()|ui->lineEdit_filename->text().isEmpty()){
            QMessageBox::warning(this,"Warning","Empty target file!");
            count--;
        }
    }
    if(count==2){
        DialogUnzipping d;
        d.setData(from,d_to,to,isText);
        d.exec();
    }
}
