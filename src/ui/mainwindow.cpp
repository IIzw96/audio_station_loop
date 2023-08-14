#include "mainwindow.h"
#include "../ui_mainwindow.h"

MainWindow::MainWindow(UI &gui, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), gui(gui)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    gui.load_loop(arg1);
    ui->lcdNumber->display(arg1);
}


void MainWindow::on_checkBox_stateChanged()
{
    gui.arm_track_1();
}

void MainWindow::on_checkBox_2_stateChanged()
{
    gui.arm_track_2();
}

void MainWindow::on_checkBox_3_stateChanged()
{
    gui.arm_track_3();
}

void MainWindow::on_checkBox_4_stateChanged()
{
    gui.arm_track_4();
}

void MainWindow::on_pushButton_5_clicked()
{
    spinboxvalue = ui->spinBox->value();
    gui.save_loop(spinboxvalue);
}

void MainWindow::on_recButton_1_clicked()
{
    if (gui.get_track_1()) {
        std::cout << "ON" << std::endl;
        // TODO once sound card recognized -> gui.start_recording();
    }

    if(!gui.get_track_1()) {
        std::cout << "OFF" << std::endl;
        // TODO once sound card recognized -> gui.stop_recording();
    }
}

