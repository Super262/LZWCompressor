#ifndef DIALOG_CREATE_H
#define DIALOG_CREATE_H

#include "dialog_compressing.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDialog>

namespace Ui {
class DialogCreate;
}

class DialogCreate : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCreate(QWidget *parent = nullptr);
    ~DialogCreate();

private slots:
    void on_btn_start_clicked();

    void on_btn_choose1_clicked();

    void on_rbtn_txt_clicked();

    void on_rbtn_pic_clicked();

    void on_btn_choose2_clicked();

private:
    Ui::DialogCreate *ui;
    bool isTextFrom=true;
};

#endif // DIALOG_CREATE_H
