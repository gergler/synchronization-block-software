#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./json_config.h"

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_open_file_triggered()
{
    ui->statusbar->setStyleSheet("color: darkBlue");
    ui->statusbar->showMessage("Open JSON file");

    // Выбираем файл
    QString openFileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open Json File"),
                                                        QString(),
                                                        tr("JSON (*.json)"));
    QFileInfo fileInfo(openFileName);   // С помощью QFileInfo
    QDir::setCurrent(fileInfo.path());  // установим текущую рабочую директорию, где будет файл
    // Создаём объект файла и открываем его на чтение
    QFile jsonFile(openFileName);
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        ui->statusbar->showMessage("ERROR");
        return;
    }

    // Считываем весь файл
    QByteArray saveData = jsonFile.readAll();
    // Создаём QJsonDocument
    QJsonDocument jsonDocument(QJsonDocument::fromJson(saveData));
    // Из которого выделяем объект в текущий рабочий QJsonObject
    _currentJsonObject = jsonDocument.object();

    ui->statusbar->showMessage("JSON file is opened");

    Parameters parameters = Parameters(_currentJsonObject);
    for (int i = 0; i < parameters.parameters_array_size; i++) {
        qDebug() << parameters.parameters_struct_array[i].parameter_name;
        qDebug() << parameters.parameters_struct_array[i].parameter_description;
        qDebug() << parameters.parameters_struct_array[i].parameter_default_val;
        qDebug() << parameters.parameters_struct_array[i].parameter_addr;
    }

    Firmware firmware = Firmware(_currentJsonObject);
    for (int i = 0; i < firmware.firmware_array_size; i++) {
        qDebug() << firmware.firmware_struct_array[i].firmware_id;
        qDebug() << firmware.firmware_struct_array[i].firmware_version;
        qDebug() << firmware.firmware_struct_array[i].firmware_addr;
    }

    Scenario scenario = Scenario(_currentJsonObject);
    for (int i = 0; i < scenario.scenario_array_size; i++) {
        qDebug() << scenario.scenario_struct_array[i].scenario_id;
        qDebug() << scenario.scenario_struct_array[i].scenario_name;
        qDebug() << scenario.scenario_struct_array[i].min_firmware_version;
        qDebug() << scenario.scenario_struct_array[i].scenario_states;
        qDebug() << scenario.scenario_struct_array[i].scenario_parameters;
    }
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
    // С помощью диалогового окна получаем имя файла с абсолютным путём
    QString saveFileName = QFileDialog::getSaveFileName(this,
                                                        tr("Save Json File"),
                                                        QString(),
                                                        tr("JSON (*.json)"));
    QFileInfo fileInfo(saveFileName);   // С помощью QFileInfo
    QDir::setCurrent(fileInfo.path());  // установим текущую рабочую директорию, где будет файл, иначе может не заработать
    // Создаём объект файла и открываем его на запись
    QFile jsonFile(saveFileName);
    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        return;
    }

    // Записываем текущий объект Json в файл
    jsonFile.write(QJsonDocument(_currentJsonObject).toJson(QJsonDocument::Indented));
    jsonFile.close();   // Закрываем файл
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "QUIT", "Are you sure you want to exit? All unsaved data will be lost.",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    } else {
        MainWindow::on_action_save_file_triggered();
    }
}


void MainWindow::on_expert_mode_checkbox_stateChanged(int arg1)
{
    QLineEdit* line_edit[] = {ui->firmware_version_line_edit, ui->scenario_line_edit, ui->current_state_line_edit};
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

void MainWindow::generate_parameters(QJsonObject jObj)
{
    Parameters parameters = Parameters(jObj);
//    for (int i = 0; i < parameters.parameters_array_size; i++) {
//        QLabel *label = new QLabel(this);
//        QSpinBox *spinbox = new QSpinBox(this);
//        label->setBuddy(spinbox);

//        label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
//        label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
//        label->setText(parameters.parameters_structs[i].parameter_name);
//        label->setToolTip(parameters.parameters_structs[i].parameter_description);

//        spinbox->setValue(parameters.parameters_structs[i].parameter_default_val);
//    }
}
