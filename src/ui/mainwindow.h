#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <string>
#include "./ui.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(UI &gui, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_checkBox_2_stateChanged();

    void on_checkBox_stateChanged();

    void on_checkBox_3_stateChanged();

    void on_checkBox_4_stateChanged();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    UI &gui;
    int spinboxvalue;
};

#endif // MAINWINDOW_H
