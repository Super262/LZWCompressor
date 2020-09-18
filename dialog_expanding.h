#ifndef DIALOG_EXPANDING_H
#define DIALOG_EXPANDING_H

#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include "dialog_unzipping.h"

namespace Ui {
class DialogExpanding;
}

class DialogExpanding : public QDialog
{
    Q_OBJECT

public:
    explicit DialogExpanding(QWidget *parent = nullptr);
    void setData(QString from);
    ~DialogExpanding();

private slots:
    void on_btn_cho_dir_clicked();

    void on_btn_start_clicked();

private:
    bool isText;
    Ui::DialogExpanding *ui;
    QString file_from,directory_to,file_to;
};

#endif // DIALOG_EXPANDING_H
