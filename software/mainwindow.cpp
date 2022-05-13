#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QComboBox>
#include <QLineEdit>
#include <QLayout>
#include <QErrorMessage>

#include <QThread>

#define CLK_COUNTER "FPGA clock counter"
#define FG_PERIOD "FastGate period"
#define PHASE_PERIOD "Phase period"

#define TIMEOUT 1000

CMD_Packet packet { 'z', 0, 0, 0 };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Synchronization block application");
    setWindowIcon(QIcon("main.png"));

    _currentJsonObject = open_file_JSON("configure.json");
    generate(_currentJsonObject);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QJsonObject MainWindow::open_file_JSON(QString file_name) {
    QFileInfo fileInfo(file_name);
    QDir::setCurrent(fileInfo.path());
    QFile jsonFile(file_name);
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        ui->statusbar->showMessage("Open JSON file error");
    }
    QByteArray saveData = jsonFile.readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(saveData));

    ui->statusbar->setStyleSheet("color: darkBlue");
    ui->statusbar->showMessage("JSON file is opened");

    return jsonDocument.object();
}

QLabel* MainWindow::add_label(QString name, QString description) {
    QLabel* label = new QLabel(this);
    label->setText(name);
    label->setToolTip(description);
    return label;
}

QLineEdit* MainWindow::add_line_edit(QString text) {
    QLineEdit* line = new QLineEdit(this);
    line->setAlignment(Qt::AlignRight);
    line->setText(text);
    line->setReadOnly(true);
    return line;
}

QSpinBox* MainWindow::add_spinbox(int value) {
    QSpinBox* spinbox = new QSpinBox(this);
    spinbox->setRange(INT32_MIN, INT32_MAX);
    spinbox->setValue(value);
    spinbox->setAlignment(Qt::AlignRight);
    return spinbox;
}

QCheckBox* MainWindow::add_checkbox(QString text) {
    QCheckBox* checkbox = new QCheckBox(this);
    checkbox->setText(text);

    return checkbox;
}

static uint32_t qstring2uint(QString &str, uint8_t bits=16) {
    return std::stoul(str.toStdString().c_str(), nullptr, bits);
}

double suffix2val (QString &str) {
    double multiplier = 1;
    if (str.toStdString().find("m") != std::string::npos)
        multiplier = 1e-3;
    if (str.toStdString().find("u") != std::string::npos)
        multiplier = 1e-6;
    if (str.toStdString().find("n") != std::string::npos)
        multiplier = 1e-9;
    if (str.toStdString().find("M") != std::string::npos)
        multiplier = 1e6;
    if (str.contains("k"))
        multiplier = 1e3;
    return multiplier;
}

QString val2suffix (uint32_t value) {
    QString suffix;
    if (value >= 1e6)
        suffix = "M";
    else if (value >= 1e3)
        suffix = "k";
//    else if ((value*1e3) >= 1)
//        suffix = "m";
//    else if ((value*1e6) >= 1)
//        suffix = "u";
//    else if ((value*1e9) >= 1)
//        suffix = "n";
    return suffix;
}

void MainWindow::generate(QJsonObject jObj) {
    firmware.firmware_init(jObj);
    scenario.scenario_init(jObj);

    expert_checkbox = add_checkbox("Expert mode");
    expert_checkbox->setChecked(true);
    ui->gridLayout->addWidget(expert_checkbox, 0, 2);
    QObject::connect(expert_checkbox, SIGNAL(clicked(bool)), this, SLOT(expert_checkbox_state_changed(bool)));

    firmware_label = add_label("Firmware", "Firmware version");
    ui->gridLayout->addWidget(firmware_label, 1, 0);
    param_struct.firmware_combobox = new QComboBox(this);
    for (int i = 0; i < firmware.array_size; ++i) {
        param_struct.firmware_combobox->addItem(firmware.struct_array[i].version);
    }
    ui->gridLayout->addWidget(param_struct.firmware_combobox, 1, 1);
    expert_struct.firmware_line = add_line_edit("0x0");
    ui->gridLayout->addWidget(expert_struct.firmware_line, 1, 2);

    config_button = new QPushButton("Configure FPGA", this);
    connect(config_button, SIGNAL(clicked()), SLOT(on_action_configure_triggered()));
    ui->gridLayout->addWidget(config_button, 2, 1, 1, -1);

    scenario_label = add_label("Scenario", "Experiment scenario");
    ui->gridLayout->addWidget(scenario_label, 3, 0);
    param_struct.scenario_combobox = new QComboBox(this);
    for (int i = 0; i < scenario.array_size; ++i) {
        param_struct.scenario_combobox->addItem(scenario.struct_array[i].name);
    }
    ui->gridLayout->addWidget(param_struct.scenario_combobox, 3, 1);
    expert_struct.scenario_line = add_line_edit("0x0");
    ui->gridLayout->addWidget(expert_struct.scenario_line, 3, 2);

    scen_set_button = new QPushButton("Set scenario", this);
    connect(scen_set_button, SIGNAL(clicked()), SLOT(on_action_set_scenario_triggered()));
    ui->gridLayout->addWidget(scen_set_button, 4, 1, 1, -1);

    generate_measurements(jObj);
    generate_parameters(jObj);
    expert_checkbox_state_changed(true);
}

void MainWindow::generate_measurements(QJsonObject jObj)
{
    reg.register_init(jObj);

    status_label = add_label(reg.struct_array[0].name, reg.struct_array[0].description);
    ui->gridLayout->addWidget(status_label, 5, 0);
    param_struct.status = add_line_edit("IDLE");
    ui->gridLayout->addWidget(param_struct.status, 5, 1);
    expert_struct.status = add_line_edit("0x0");
    ui->gridLayout->addWidget(expert_struct.status, 5, 2);

    reload_checkbox = add_checkbox("Autoreload");
    ui->gridLayout_reg->addWidget(reload_checkbox, 0, 2);
    QObject::connect(reload_checkbox, SIGNAL(clicked(bool)), this, SLOT(reload_checkbox_state_changed(bool)));

    for (int i = 1; i < reg.array_size; ++i) {
        QLabel *label = add_label(reg.struct_array[i].name, reg.struct_array[i].description);
        ui->gridLayout_reg->addWidget(label, i+1, 0);

        param_struct.measure_lines.push_back(add_line_edit("0"));
        param_struct.measure_lines[i - 1]->setReadOnly(true);
        ui->gridLayout_reg->addWidget(param_struct.measure_lines[i - 1], i+1, 1);

        expert_struct.measure_lines.push_back(add_line_edit("0x0"));
        ui->gridLayout_reg->addWidget(expert_struct.measure_lines[i - 1], i+1, 2);
    }

    measure_button = new QPushButton("Read measurement registers", this);
    connect(measure_button, SIGNAL(clicked()), SLOT(on_action_read_registers_triggered()));
    ui->gridLayout_reg->addWidget(measure_button, reg.array_size + 1, 0, 1, -1);
}

void MainWindow::generate_parameters(QJsonObject jObj)
{
    parameters.parameters_init(jObj);

    for (int i = 0; i < parameters.array_size; ++i) {
        QLabel* label = add_label(parameters.struct_array[i].name, parameters.struct_array[i].description);
        ui->gridLayout_parameters->addWidget(label, i+1, 0);

        QString def_val = parameters.struct_array[i].default_val;
        param_struct.param_spinboxes.push_back(add_spinbox(qstring2uint(def_val.split(" ")[0], 10)));
        ui->gridLayout_parameters->addWidget(param_struct.param_spinboxes[i], i+1, 1);

        if (def_val.split(" ").size() == 1)
            expert_struct.param_lines.push_back(add_line_edit("0x" + QString::number(qstring2uint(def_val.split(" ")[0]))));
        else {
            param_struct.param_spinboxes[i]->setSuffix(" " + def_val.split(" ")[1]);
            uint32_t value = qstring2uint(def_val.split(" ")[0], 10);//*suffix2val(def_val.split(" ")[1]);
            expert_struct.param_lines.push_back(add_line_edit("0x" + QString::number(value, 16)));
        }

        ui->gridLayout_parameters->addWidget(expert_struct.param_lines[i], i+1, 2);
    }

    param_read_button = new QPushButton("Read parameter registers", this);
    connect(param_read_button, SIGNAL(clicked()), SLOT(on_action_read_parameters_triggered()));
    ui->gridLayout_parameters->addWidget(param_read_button, parameters.array_size + 1, 0, 1, -1);

    param_write_button = new QPushButton("Write parameters in registers", this);
    connect(param_write_button, SIGNAL(clicked()), SLOT(on_action_wtite_parameters_triggered()));
    ui->gridLayout_parameters->addWidget(param_write_button, parameters.array_size + 2, 0, 1, -1);
}

void MainWindow::exec_reg_command(CMD_Packet request, CMD_Packet &reply) {
    QString data;

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
}

uint32_t MainWindow::read_register(uint32_t address) {
    CMD_Packet reply   {'R', address, 0, 1};
    CMD_Packet request {'R', address, 0, 1};

    exec_reg_command(request, reply);

    if (reply.status == 0)
        return reply.value;
    else
        return -1;
}

void MainWindow::write_register(uint32_t address, uint32_t value) {
    CMD_Packet reply   {'W', address, value, 1};
    CMD_Packet request {'W', address, value, 1};

    exec_reg_command(request, reply);
}

void MainWindow::configure_fpga(uint32_t address, uint32_t firmware_version) {
    CMD_Packet reply   {'C', address, firmware_version, 1};
    CMD_Packet request {'C', address, firmware_version, 1};

    exec_reg_command(request, reply);
}

void MainWindow::on_action_read_registers_triggered()
{
    expert_struct.firmware_line->setText("0x" + QString::number(read_register(SYSID_ADDR), 16));
    expert_struct.scenario_line->setText("0x" + QString::number(read_register(SCENARIO_CONTROL_ADDR), 16));
    param_struct.status->setText("0x" + QString::number(read_register(SCENARIO_STATUS_ADDR), 16));

    read_register_values();
}

void MainWindow::on_action_configure_triggered()
{
    configure_fpga(SYSID_ADDR, param_struct.firmware_combobox->currentIndex());
    QThread::msleep(100);
    uint32_t data = read_register(SYSID_ADDR);
    expert_struct.firmware_line->setText("0x" + QString::number(data, 16));
}

void MainWindow::read_register_values() {
    uint32_t control_reg_value = read_register(SCENARIO_CONTROL_ADDR);
    uint32_t status_reg_value = read_register(SCENARIO_STATUS_ADDR);

    register_values_snapshot_t new_values;
    new_values.timestamp = std::chrono::system_clock::now();

    expert_struct.status->setText("0x"+QString::number(status_reg_value, 16));
    expert_struct.scenario_line->setText("0x"+QString::number(control_reg_value, 16));

    int scen_number = (control_reg_value >> 8) & 0xFF;
    int state_number = status_reg_value & 0xFF;
    QString scen_state_str;
    if (scen_number >= 0 && scen_number < scenario.array_size) {
        if (state_number >= 0 && state_number < scenario.struct_array[scen_number].states.size())
            scen_state_str = scenario.struct_array[scen_number].states[state_number].toString();
        else
            scen_state_str = QString::number(state_number);
    }

    param_struct.status ->setText(scen_state_str);

    for (int i = 1; i < reg.array_size; ++i) {
        uint32_t reg_addr  = qstring2uint(reg.struct_array[i].address);
        uint32_t reg_value = read_register(reg_addr);

        new_values.values[reg_addr] = reg_value;

//        param_struct.measure_lines[i - 1]->setText(QString::number(reg_value));
        expert_struct.measure_lines[i - 1]->setText("0x"+QString::number(reg_value, 16));
    }

    /* ui update */
    for (int i = 1; i < reg.array_size; ++i) {
        std::string reg_name = reg.struct_array[i].name.toStdString();
        double multiplier = 0;
        if (reg_name.find("counter") == std::string::npos) {
            if (reg_name.find("period") != std::string::npos) {
                uint32_t reg_addr  = qstring2uint(reg.struct_array[i].address);
                uint32_t reg_value = new_values.values[reg_addr];
                uint32_t current_fw = param_struct.firmware_combobox->currentIndex();

                if (reg_name.find("Phase") != std::string::npos) {
                    multiplier = suffix2val(firmware.struct_array[current_fw].default_values[PHASE_PERIOD].split(" ")[1]);
                    uint32_t phase_period = qstring2uint(firmware.struct_array[current_fw].default_values[PHASE_PERIOD].split(" ")[0]);//*multiplier;
                    reg_value *= 20;
                    if ((reg_value < (reg_value - 50*multiplier)) or (reg_value > (reg_value + 50*multiplier))) {
                        ui->action_start->setDisabled(true);
                        ui->statusbar->setStyleSheet("color: red");
                        ui->statusbar->showMessage("PHASE period don't match the defaults");
                    }

                } else if (reg_name.find("FastGate") != std::string::npos) {
                    multiplier = suffix2val(firmware.struct_array[current_fw].default_values[FG_PERIOD].split(" ")[1]);
                    uint32_t fg_period = qstring2uint(firmware.struct_array[current_fw].default_values[FG_PERIOD].split(" ")[0]);//*multiplier;
                    reg_value *= 20;
                    if ((reg_value < (reg_value - 10*multiplier)) or (reg_value > (reg_value + 10*multiplier))) {
                        ui->action_start->setDisabled(true);
                        ui->statusbar->setStyleSheet("color: red");
                        ui->statusbar->showMessage("FASTGATE period don't match the defaults");
                    }
                } else
                    ui->action_start->setDisabled(false);

                param_struct.measure_lines[i - 1]->setText(QString::number(reg_value) + " " + val2suffix(multiplier) + "ns");
            }
            continue;
        }

        uint32_t reg_addr  = qstring2uint(reg.struct_array[i].address);
        uint32_t reg_value, reg_old_value;

        reg_value     = new_values.values[reg_addr];
        reg_old_value = previous_measurements.values [reg_addr];
        long diff_value = long(reg_value) - long(reg_old_value);
        if (diff_value < 0)
            diff_value += INT32_MAX;

        double frequency = diff_value * 1000000;
        frequency /= std::chrono::duration_cast<std::chrono::microseconds>( new_values.timestamp - previous_measurements.timestamp  ).count();

        QString suffix = val2suffix(frequency);
        multiplier = suffix2val(suffix);
        param_struct.measure_lines[i - 1]->setText(QString::number(frequency/multiplier) + " " + suffix + "Hz");

        if (reg_name.find("FPGA") != std::string::npos) {
            uint32_t current_fw = param_struct.firmware_combobox->currentIndex();
            QString def_val = firmware.struct_array[current_fw].default_values[CLK_COUNTER];
            multiplier = suffix2val(def_val.split(" ")[1]);
            uint32_t clk = qstring2uint(def_val.split(" ")[0], 10)*multiplier;
            if ((frequency < (clk - multiplier/10)) or (frequency > (clk + multiplier/10))) {
                ui->action_start->setDisabled(true);
                ui->statusbar->setStyleSheet("color: red");
                ui->statusbar->showMessage("FPGA CLK COUNTER don't match the defaults");
            }
        } else ui->action_start->setDisabled(false);
    }

    /* old = new */
    previous_measurements = new_values;
}

void MainWindow::on_action_set_scenario_triggered()
{
    uint32_t selected_scen = param_struct.scenario_combobox->currentIndex();

    uint32_t old_value = read_register(SCENARIO_CONTROL_ADDR);
    write_register(SCENARIO_CONTROL_ADDR, old_value | RESET_MSK);

    uint32_t new_value = selected_scen << 8;
    write_register(SCENARIO_CONTROL_ADDR, new_value | RESET_MSK);

    write_register(SCENARIO_CONTROL_ADDR, new_value & ~RESET_MSK);
}

void MainWindow::on_action_read_parameters_triggered()
{
    for (int i = 0; i < parameters.array_size; ++i) {
        uint32_t param_addr  = qstring2uint(parameters.struct_array[i].address);
        uint32_t param_value = read_register(param_addr);

        QString suffix = val2suffix(param_value);
        double multiplier = suffix2val(suffix);

        param_struct.param_spinboxes[i]->setValue(param_value/multiplier);
        if (parameters.struct_array[i].name.toStdString().find("Trigger count") == std::string::npos)
            param_struct.param_spinboxes[i]->setSuffix(suffix + "s");
        else
            param_struct.param_spinboxes[i]->setSuffix(suffix);

        expert_struct.param_lines[i]->setText("0x" + QString::number(param_value, 16));
    }
}

void MainWindow::on_action_wtite_parameters_triggered()
{
    for (int i = 0; i < parameters.array_size; ++i) {
        uint32_t param_addr  = qstring2uint(parameters.struct_array[i].address);
        uint32_t param_value;
        if (!expert_checkbox->isChecked()) {
            param_value = param_struct.param_spinboxes[i]->value();
            expert_struct.param_lines[i]->setText("0x" + QString::number(param_value, 16));
        }
        param_value = std::stoul(expert_struct.param_lines[i]->text().toStdString().c_str(), nullptr, 16);
//        param_struct.param_spinboxes[i]->setValue(param_value/20);
        write_register(param_addr, param_value);
    }
}

void MainWindow::on_action_start_triggered()
{
    uint32_t old_value = read_register(SCENARIO_CONTROL_ADDR);
    uint32_t selected_scen = (old_value >> 8) & 0xFF;

    uint32_t selected_scen_old = param_struct.scenario_combobox->currentIndex();

    param_struct.scenario_combobox->setCurrentIndex(selected_scen);

    if (selected_scen != selected_scen_old) {
        ui->statusbar->setStyleSheet("color: red");
        ui->statusbar->showMessage("Wrong scenario!");
        return;
    }

    write_register(SCENARIO_CONTROL_ADDR, old_value | START_MSK);
    QThread::usleep(1);
    write_register(SCENARIO_CONTROL_ADDR, old_value & ~START_MSK);
}

void MainWindow::on_action_stop_triggered()
{
    uint32_t old_value = read_register(SCENARIO_CONTROL_ADDR);
    write_register(SCENARIO_CONTROL_ADDR, (old_value | RESET_MSK) & ~START_MSK);
    QThread::usleep(1);
    write_register(SCENARIO_CONTROL_ADDR, old_value & ~RESET_MSK & ~START_MSK);
}

void MainWindow::timer_timeout() {
    read_register_values();
}

void MainWindow::reload_checkbox_state_changed(bool checked) {
    if (checked) {
        timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
        timer->start(TIMEOUT);

        ui->statusbar->setStyleSheet("color: green");
        ui->statusbar->showMessage("Autoreload ON");
    } else {
        timer->stop();

        ui->statusbar->setStyleSheet("color: red");
        ui->statusbar->showMessage("Autoreload OFF");
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

        expert_struct.status->setReadOnly(false);
        expert_struct.status->setStyleSheet("background: white");

        for (int i = 0; i < expert_struct.measure_lines.size(); i++) {
            expert_struct.measure_lines[i]->setReadOnly(false);
            expert_struct.measure_lines[i]->setStyleSheet("background: white");
        }

        for (int i = 0; i < expert_struct.param_lines.size(); i++) {
            expert_struct.param_lines[i]->setReadOnly(false);
            expert_struct.param_lines[i]->setStyleSheet("background: white");
        }

    } else {
        ui->statusbar->setStyleSheet("color: red");
        ui->statusbar->showMessage("Expert Mode OFF");

        expert_struct.firmware_line->setReadOnly(true);
        expert_struct.firmware_line->setStyleSheet("background: lightGray");

        expert_struct.scenario_line->setReadOnly(true);
        expert_struct.scenario_line->setStyleSheet("background: lightGray");

        expert_struct.status->setReadOnly(true);
        expert_struct.status->setStyleSheet("background: lightGray");

        for (int i = 0; i < expert_struct.measure_lines.size(); i++) {
            expert_struct.measure_lines[i]->setReadOnly(true);
            expert_struct.measure_lines[i]->setStyleSheet("background: lightGray");
        }

        for (int i = 0; i < expert_struct.param_lines.size(); i++) {
            expert_struct.param_lines[i]->setReadOnly(true);
            expert_struct.param_lines[i]->setStyleSheet("background: lightGray");
        }
    }
}

///////////////////////////////////CHANGE OR DELETE//////////////////////////////////////////////////////////////////

void MainWindow::on_action_open_file_triggered()
{
    QString open_file_name = QFileDialog::getOpenFileName(this,
                                                        tr("Open Json File"),
                                                        QString(),
                                                        tr("JSON (*.json)"));
    _currentJsonObject = open_file_JSON(open_file_name);

    generate(_currentJsonObject);
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
