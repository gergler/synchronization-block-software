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
    setWindowIcon(QIcon("main.png"));

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

    ui->statusbar->setStyleSheet("color: darkBlue");
    ui->statusbar->showMessage("JSON file is opened");

    generate(_currentJsonObject);
}

void MainWindow::on_action_configure_triggered()
{
    QString data;

    CMD_Packet reply;
    CMD_Packet req {'C', 0, 0, 1};
    req.number = param_struct.firmware_combobox->currentIndex();

    int n = exec_UDP_command(req, reply, 500000);

    if (n < 0) {
        data = strerror(errno);
    } else if (n != sizeof (CMD_Packet)) {
        data = "Error: invalid size";
    } else if (reply.status != 0) {
        data = "Error: bad reply status";
    } else {
        data = "OK: " + QString::number(reply.number);
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

        expert_struct.current_state->setReadOnly(false);
        expert_struct.current_state->setStyleSheet("background: white");

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

        expert_struct.current_state->setReadOnly(true);
        expert_struct.current_state->setStyleSheet("background: lightGray");

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

QLabel* MainWindow::add_label(QString name, QString description) {
    QLabel* label = new QLabel(this);
    label->setText(name);
    label->setToolTip(description);
    return label;
}

QLineEdit* MainWindow::add_line_edit(QString text, int expert_mode) {
    QLineEdit* line = new QLineEdit(this);
    line->setAlignment(Qt::AlignRight);
    line->setText(text);
    line->setReadOnly(true);
    if (expert_mode) {
        line->setStyleSheet("background: lightGray");
    }
    return line;
}

QSpinBox* MainWindow::add_spinbox(int value, int maximum, int arrow, int expert_mode) {
    QSpinBox* spinbox = new QSpinBox(this);
    spinbox->setMaximum(maximum);
    spinbox->setValue(value);
    spinbox->setAlignment(Qt::AlignRight);
    if (expert_mode) {
        spinbox->setReadOnly(true);
        spinbox->setStyleSheet("background: lightGray");
    }
    if (!arrow) {
        spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    }
    return spinbox;
}

QCheckBox* MainWindow::add_checkbox(QString text) {
    QCheckBox* checkbox = new QCheckBox(this);
    checkbox->setText(text);

    return checkbox;
}

void MainWindow::generate(QJsonObject jObj) {
    firmware.firmware_init(jObj);
    scenario.scenario_init(jObj);

    expert_struct.expert_checkbox = add_checkbox("Expert mode");
    ui->gridLayout->addWidget(expert_struct.expert_checkbox, 0, 3);
    QObject::connect(expert_struct.expert_checkbox, SIGNAL(clicked(bool)), this, SLOT(expert_checkbox_state_changed(bool)));

    firmware_label = add_label("Firmware", "Firmware version");
    ui->gridLayout->addWidget(firmware_label, 1, 1);

    param_struct.firmware_combobox = new QComboBox(this);
    for (int i = 0; i < firmware.firmware_array_size; ++i) {
        param_struct.firmware_combobox->addItem(firmware.firmware_struct_array[i].firmware_version);
    }
    ui->gridLayout->addWidget(param_struct.firmware_combobox, 1, 2);

    expert_struct.firmware_line = add_line_edit(" ", 1);
    ui->gridLayout->addWidget(expert_struct.firmware_line, 1, 3);

    scenario_label = add_label("Scenario", "Experiment scenario");
    ui->gridLayout->addWidget(scenario_label, 2, 1);

    param_struct.scenario_combobox = new QComboBox(this);
    for (int i = 0; i < scenario.scenario_array_size; ++i) {
        param_struct.scenario_combobox->addItem(scenario.scenario_struct_array[i].scenario_name);
    }
    ui->gridLayout->addWidget(param_struct.scenario_combobox, 2, 2);

    expert_struct.scenario_line = add_line_edit(" ", 1);
    ui->gridLayout->addWidget(expert_struct.scenario_line, 2, 3);

    generate_reg(jObj);
    generate_parameters(jObj);
}

void MainWindow::generate_reg(QJsonObject jObj)
{
    reg.register_init(jObj);

    state_label = add_label(reg.register_struct_array[0].register_name, reg.register_struct_array[0].register_description);
    ui->gridLayout_reg->addWidget(state_label, 1, 0);

    param_struct.current_state = add_line_edit("IDLE");
    ui->gridLayout_reg->addWidget(param_struct.current_state, 1, 1);

    expert_struct.current_state = add_line_edit(" ", 1);
    ui->gridLayout_reg->addWidget(expert_struct.current_state, 1, 2);

    for (int i = 1; i < reg.register_array_size; ++i) {
        QLabel *label = add_label(reg.register_struct_array[i].register_name, reg.register_struct_array[i].register_description);
        ui->gridLayout_reg->addWidget(label, i+1, 0);

        param_struct.reg_spinboxes[i - 1] = add_spinbox(reg.register_struct_array[i].register_default_val, 100500, 0);
        param_struct.reg_spinboxes[i - 1]->setReadOnly(true);
        ui->gridLayout_reg->addWidget(param_struct.reg_spinboxes[i - 1], i+1, 1);

        expert_struct.reg_spinboxes[i - 1] = add_spinbox(0, 10000, 0, 1);
        ui->gridLayout_reg->addWidget(expert_struct.reg_spinboxes[i - 1], i+1, 2);
    }
}

void MainWindow::generate_parameters(QJsonObject jObj)
{
    parameters.parameters_init(jObj);

    for (int i = 0; i < parameters.parameters_array_size; ++i) {
        QLabel* label = add_label(parameters.parameters_struct_array[i].parameter_name, parameters.parameters_struct_array[i].parameter_description);
        ui->gridLayout_parameters->addWidget(label, i+1, 0);

        param_struct.param_spinboxes[i] = add_spinbox(parameters.parameters_struct_array[i].parameter_default_val);
        ui->gridLayout_parameters->addWidget(param_struct.param_spinboxes[i], i+1, 1);

        expert_struct.param_spinboxes[i] = add_spinbox(0, 10000, 1, 1);
        ui->gridLayout_parameters->addWidget(expert_struct.param_spinboxes[i], i+1, 2);
    }
}

uint32_t MainWindow::read_register(uint32_t address) {
    QString data;

    CMD_Packet reply;
    CMD_Packet request {'R', address, 0, 1};

    int n = exec_UDP_command(request, reply);

    if (n < 0) {
        data = strerror(errno);
    } else if (n != sizeof (CMD_Packet)) {
        data = "Error: invalid size";
    } else if (reply.status != 0) {
        data = "Error: bad reply status";
    } else {
        data = "0x"+QString::number(reply.value, 16);
    }
    ui->statusbar->showMessage(data);

    return reply.value;
}

void MainWindow::on_action_read_registers_triggered()
{
    uint32_t data = read_register(0x1000);
    expert_struct.firmware_line->setText(QString::number(data, 16));
}
