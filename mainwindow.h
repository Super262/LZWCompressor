#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QMainWindow>
#include <QLayout>
#include "dialog_create.h"
#include "dialog_expanding.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_btn_create_clicked();

    void on_btn_open_clicked();

private:
    Ui::MainWindow *ui;
    DialogCreate *create;
    DialogExpanding *expand;
};

#endif // MAINWINDOW_H
