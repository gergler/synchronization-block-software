#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./json_config.h"

#include "client.h"

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QComboBox>
#include <QLineEdit>
#include <QLayout>

CMD_Packet packet { 'z', 0, 0, 0 };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Synchronization block application");

    QString openFileName = "configure.json";
    QFileInfo fileInfo(openFileName);
    QDir::setCurrent(fileInfo.path());
    QFile jsonFile(openFileName);
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        ui->statusbar->showMessage("ERROR");
        return;
    }
    QByteArray saveData = jsonFile.readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(saveData));
    _currentJsonObject = jsonDocument.object();

    generate(_currentJsonObject);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_open_file_triggered()
{
    ui->statusbar->setStyleSheet("color: darkBlue");
    ui->statusbar->showMessage("Open JSON file");

    QString openFileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open Json File"),
                                                        QString(),
                                                        tr("JSON (*.json)"));
    QFileInfo fileInfo(openFileName);
    QDir::setCurrent(fileInfo.path());
    QFile jsonFile(openFileName);
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        ui->statusbar->showMessage("ERROR");
        return;
    }
    QByteArray saveData = jsonFile.readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(saveData));
    _currentJsonObject = jsonDocument.object();

    ui->statusbar->showMessage("JSON file is opened");

    generate(_currentJsonObject);
}

void MainWindow::on_action_configure_triggered()
{
    QString data;

    CMD_Packet reply;
    CMD_Packet req {'C', 0, 0, 1};
    req.number = firmware_combobox->currentIndex();

    int n = execUDPCommand(req, reply, 500000);

    if (n < 0) {
        data = strerror(errno);
    } else if (n != sizeof (CMD_Packet)) {
        data = "Error: invalid size";
    } else if (reply.status != 0) {
        data = "Error: bad reply status";
    } else {
        data = "OK: "+QString::number( reply.number );
    }

    ui->statusbar->showMessage(data);
    expert_struct.firmware_line->setText(data);
}

void MainWindow::on_action_start_triggered()
{
    ui->statusbar->setStyleSheet("color: green");
    ui->statusbar->showMessage("START signal sent successfully");

}

void MainWindow::on_action_stop_triggered()
{
    ui->statusbar->setStyleSheet("color: darkRed");
    ui->statusbar->showMessage("STOP signal sent successfully");
}


void MainWindow::on_action_save_file_triggered()
{
    QString saveFileName = QFileDialog::getSaveFileName(this,
                                                        tr("Save Json File"),
                                                        QString(),
                                                        tr("JSON (*.json)"));
    QFileInfo fileInfo(saveFileName);
    QDir::setCurrent(fileInfo.path());
    QFile jsonFile(saveFileName);
    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        return;
    }
    jsonFile.write(QJsonDocument(_currentJsonObject).toJson(QJsonDocument::Indented));
    jsonFile.close();

    ui->statusbar->setStyleSheet("color: darkBlue");
    ui->statusbar->showMessage("Save JSON file");
}

void MainWindow::closeEvent(QCloseEvent *)
{
    return; // FIXME!!

    QMessageBox::StandardButton reply = QMessageBox::question(this, "QUIT", "Are you sure you want to exit? All unsaved data will be lost.",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    } else {
        MainWindow::on_action_save_file_triggered();
    }
}

void MainWindow::expert_checkbox_state_changed(bool checked) {
    if (checked) {
        ui->statusbar->setStyleSheet("color: green");
        ui->statusbar->showMessage("Expert Mode ON");

        expert_struct.firmware_line->setReadOnly(false);
        expert_struct.firmware_line->setStyleSheet("background: white");

        expert_struct.scenario_line->setReadOnly(false);
        expert_struct.scenario_line->setStyleSheet("background: white");

        for (int i = 0; i < sizeof(expert_struct.reg_spinboxes)/sizeof(expert_struct.reg_spinboxes[0]); i++) {
            expert_struct.reg_spinboxes[i]->setReadOnly(false);
            expert_struct.reg_spinboxes[i]->setStyleSheet("background: white");
        }

        for (int i = 0; i < sizeof(expert_struct.param_spinboxes)/sizeof(expert_struct.param_spinboxes[0]); i++) {
            expert_struct.param_spinboxes[i]->setReadOnly(false);
            expert_struct.param_spinboxes[i]->setStyleSheet("background: white");
        }

    } else {
        ui->statusbar->setStyleSheet("color: red");
        ui->statusbar->showMessage("Expert Mode OFF");

        expert_struct.firmware_line->setReadOnly(true);
        expert_struct.firmware_line->setStyleSheet("background: lightGray");

        expert_struct.scenario_line->setReadOnly(true);
        expert_struct.scenario_line->setStyleSheet("background: lightGray");

        for (int i = 0; i < sizeof(expert_struct.reg_spinboxes)/sizeof(expert_struct.reg_spinboxes[0]); i++) {
            expert_struct.reg_spinboxes[i]->setReadOnly(true);
            expert_struct.reg_spinboxes[i]->setStyleSheet("background: lightGray");
        }

        for (int i = 0; i < sizeof(expert_struct.param_spinboxes)/sizeof(expert_struct.param_spinboxes[0]); i++) {
            expert_struct.param_spinboxes[i]->setReadOnly(true);
            expert_struct.param_spinboxes[i]->setStyleSheet("background: lightGray");
        }
    }
}

void MainWindow::generate(QJsonObject jObj) {
    Firmware firmware = Firmware(jObj);
    Scenario scenario = Scenario(jObj);

    expert_checkbox = new QCheckBox(this);
    expert_checkbox->setText("Expert mode");
    ui->gridLayout->addWidget(expert_checkbox, 0, 3);

    QObject::connect(expert_checkbox, SIGNAL(clicked(bool)), this, SLOT(expert_checkbox_state_changed(bool)));

    firmware_label = new QLabel(this);
    firmware_label->setText("Firmware");
    ui->gridLayout->addWidget(firmware_label, 1, 1);

    firmware_combobox = new QComboBox(this);
    for (int i = 0; i < firmware.firmware_array_size; ++i) {
        firmware_combobox->addItem(firmware.firmware_struct_array[i].firmware_version);
    }
    ui->gridLayout->addWidget(firmware_combobox, 1, 2);

    expert_struct.firmware_line = new QLineEdit(this);
    expert_struct.firmware_line->setReadOnly(true);
    expert_struct.firmware_line->setStyleSheet("background: lightGray");
    ui->gridLayout->addWidget(expert_struct.firmware_line, 1, 3);

    scenario_label = new QLabel(this);
    scenario_label->setText("Scenario");
    ui->gridLayout->addWidget(scenario_label, 2, 1);

    scenario_combobox = new QComboBox(this);
    for (int i = 0; i < scenario.scenario_array_size; ++i) {
        scenario_combobox->addItem(scenario.scenario_struct_array[i].scenario_name);
    }
    ui->gridLayout->addWidget(scenario_combobox, 2, 2);

    expert_struct.scenario_line = new QLineEdit(this);
    expert_struct.scenario_line->setReadOnly(true);
    expert_struct.scenario_line->setStyleSheet("background: lightGray");
    ui->gridLayout->addWidget(expert_struct.scenario_line, 2, 3);

    generate_reg(jObj);
    generate_parameters(jObj);
}

void MainWindow::generate_reg(QJsonObject jObj)
{
    Register reg = Register(jObj);
    QSpinBox* spinbox_array[reg.register_array_size][2];

    for (int i = 0; i < reg.register_array_size; ++i) {
        QLabel *label = new QLabel(this);
        spinbox_array[i][0] = new QSpinBox(this);
        spinbox_array[i][1] = new QSpinBox(this);

        spinbox_array[i][0]->setMaximum(100500);
        spinbox_array[i][1]->setMaximum(10000);

        spinbox_array[i][0]->setAlignment(Qt::AlignRight);
        spinbox_array[i][1]->setAlignment(Qt::AlignRight);

        label->setText(reg.register_struct_array[i].register_name);
        label->setToolTip(reg.register_struct_array[i].register_description);

        spinbox_array[i][0]->setValue(reg.register_struct_array[i].register_default_val);
        spinbox_array[i][0]->setReadOnly(true);
        spinbox_array[i][1]->setReadOnly(true);
        spinbox_array[i][1]->setStyleSheet("background: lightGray");

        spinbox_array[i][0]->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinbox_array[i][1]->setButtonSymbols(QAbstractSpinBox::NoButtons);

        ui->gridLayout_reg->addWidget(label, i+1, 0);
        ui->gridLayout_reg->addWidget(spinbox_array[i][0], i+1, 1);
        ui->gridLayout_reg->addWidget(spinbox_array[i][1], i+1, 2);

        expert_struct.reg_spinboxes[i] = spinbox_array[i][1];
    }

}

void MainWindow::generate_parameters(QJsonObject jObj)
{
    Parameters parameters = Parameters(jObj);
    QSpinBox* spinbox_array[parameters.parameters_array_size][2];

    for (int i = 0; i < parameters.parameters_array_size; ++i) {
        QLabel *label = new QLabel(this);
        spinbox_array[i][0] = new QSpinBox(this);
        spinbox_array[i][1] = new QSpinBox(this);

        spinbox_array[i][0]->setMaximum(10000);
        spinbox_array[i][1]->setMaximum(10000);

        spinbox_array[i][0]->setAlignment(Qt::AlignRight);
        spinbox_array[i][1]->setAlignment(Qt::AlignRight);

        label->setText(parameters.parameters_struct_array[i].parameter_name);
        label->setToolTip(parameters.parameters_struct_array[i].parameter_description);

        spinbox_array[i][0]->setValue(parameters.parameters_struct_array[i].parameter_default_val);
        spinbox_array[i][1]->setReadOnly(true);
        spinbox_array[i][1]->setStyleSheet("background: lightGray");

        ui->gridLayout_parameters->addWidget(label, i+1, 0);
        ui->gridLayout_parameters->addWidget(spinbox_array[i][0], i+1, 1);
        ui->gridLayout_parameters->addWidget(spinbox_array[i][1], i+1, 2);

        expert_struct.param_spinboxes[i] = spinbox_array[i][1];
    }
}

void MainWindow::on_action_read_registers_triggered()
{
    QString data;

    CMD_Packet reply;
    CMD_Packet req {'R', 0x1000, 0, 1};

    int n = execUDPCommand(req, reply);

    if (n < 0) {
        data = strerror(errno);
    } else if (n != sizeof (CMD_Packet)) {
        data = "Error: invalid size";
    } else if (reply.status != 0) {
        data = "Error: bad reply status";
    } else {
        data = "0x"+QString::number( reply.value, 16 );
    }

    //QString data = QString(reinterpret_cast<CMD_Packet&>(buffer).value);


    //ui->statusbar->showMessage("Recv data:");
    ui->statusbar->showMessage(data);
    expert_struct.firmware_line->setText(data);
}
