#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_configure_triggered()
{
    ui->statusbar->setStyleSheet("color: darkBlue");
    ui->statusbar->showMessage("Configuring fpga with the selected file");
}

void MainWindow::on_action_start_triggered()
{
    ui->statusbar->setStyleSheet("color: green");
    ui->statusbar->showMessage("START signal sent successfully");
}

void MainWindow::on_action_stop_triggered()
{
    ui->statusbar->setStyleSheet("color: darkRed");
    ui->statusbar->showMessage("RESET signal sent successfully");
}


void MainWindow::on_action_save_file_triggered()
{
    ui->statusbar->setStyleSheet("color: blue");
    ui->statusbar->showMessage("Saving the configuration file");
}

void MainWindow::on_action_help_triggered()
{
    ui->statusbar->setStyleSheet("color: darkGreen");
    ui->statusbar->showMessage("Help");
}

void MainWindow::on_action_quit_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "QUIT", "Are you sure you want to exit? All unsaved data will be lost.",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    } /*else {
        qDebug() << "Button had been pressed";
    }*/
}


void MainWindow::on_expert_mode_checkbox_stateChanged(int arg1)
{
    QLineEdit* line_edit[] = {ui->firmware_version_line_edit, ui->scenario_line_edit, ui->current_state_line_edit,
                               ui->raw_fpga_clock_counter_line_edit, ui->raw_phase_impulse_counter_line_edit,
                               ui->raw_phase_period_line_edit, ui->raw_fg_impulse_counter_line_edit,
                               ui->raw_fg_period_line_edit, ui->raw_fg_opto_to_opened_delay_line_edit,
                               ui->raw_phase_shift_line_edit, ui->raw_trigger_lenght_line_edit,
                               ui->raw_trigger_counter_line_edit, ui->raw_detector_wait_timeout_line_edit};
    if (arg1 == Qt::Checked) {
        ui->statusbar->setStyleSheet("color: green");
        ui->statusbar->showMessage("Expert Mode ON");
        for (int i = 0; i < sizeof(line_edit)/sizeof(line_edit[0]); i++) {
            line_edit[i]->setReadOnly(false);
            line_edit[i]->setStyleSheet("background: white");
        }
    } else {
        ui->statusbar->setStyleSheet("color: red");
        ui->statusbar->showMessage("Expert Mode OFF");
        for (int i = 0; i < sizeof(line_edit)/sizeof(line_edit[0]); i++) {
            line_edit[i]->setReadOnly(true);
            line_edit[i]->setStyleSheet("background: lightGray");
        }
    }
}















