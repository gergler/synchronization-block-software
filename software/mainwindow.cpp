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
    ui->groupBox_log->hide();

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

QCheckBox* MainWindow::add_checkbox(QString text) {
    QCheckBox* checkbox = new QCheckBox(this);
    checkbox->setText(text);

    return checkbox;
}

static uint32_t qstring2uint(QString &str, uint8_t bits=0) {
    return std::stoul(str.toStdString().c_str(), nullptr, bits);
}

double suffix2val (QString &str) {
    double multiplier = 1;
    if (str.contains("m"))
        multiplier = 1e-3;
    else if (str.contains("u"))
        multiplier = 1e-6;
    else if (str.contains("n"))
        multiplier = 1e-9;
    else if (str.contains("M"))
        multiplier = 1e6;
    else if (str.contains("k"))
        multiplier = 1e3;
    return multiplier;
}

QString val2suffix (double value) {
    QString suffix = "";
    if (value >= 1e6)
        suffix = "M";
    else if (value >= 1e3)
        suffix = "k";
    else if ((value < 1) && (value*1e3) >= 1)
        suffix = "m";
    else if ((value < 1) && (value*1e6) >= 1)
        suffix = "u";
    else if ((value < 1) && (value*1e9) >= 1)
        suffix = "n";
    return suffix;
}

void MainWindow::generate(QJsonObject jObj) {
    firmware.firmware_init(jObj);
    scenario.scenario_init(jObj);

    expert_checkbox = add_checkbox("Expert mode");
    expert_checkbox->setChecked(false);
    ui->gridLayout->addWidget(expert_checkbox, 0, 2);
    QObject::connect(expert_checkbox, SIGNAL(clicked(bool)), this, SLOT(expert_checkbox_state_changed(bool)));

    firmware_label = add_label("Firmware", "Firmware version");
    ui->gridLayout->addWidget(firmware_label, 1, 0);
    param_struct.firmware_combobox = new QComboBox(this);
    for (int i = 0; i < firmware.firmware_map.size(); ++i) {
        param_struct.firmware_combobox->addItem(firmware.firmware_map[i].version);
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
    for (int i = 0; i < scenario.scenario_map.size(); ++i) {
        param_struct.scenario_combobox->addItem(scenario.scenario_map[i].name);
    }
    ui->gridLayout->addWidget(param_struct.scenario_combobox, 3, 1, 1, -1);
    expert_struct.scenario_line = add_line_edit("0x0");
    ui->gridLayout->addWidget(expert_struct.scenario_line, 4, 2);

    scen_set_button = new QPushButton("Set scenario", this);
    connect(scen_set_button, SIGNAL(clicked()), SLOT(on_action_set_scenario_triggered()));
    ui->gridLayout->addWidget(scen_set_button, 4, 1);

    generate_measurements(jObj);
    generate_parameters(jObj);
    expert_checkbox_state_changed(false);
}

void MainWindow::generate_measurements(QJsonObject jObj)
{
    measurement.measurment_init(jObj);

    status_label = add_label(measurement.measurment_map[0].name, measurement.measurment_map[0].description);
    ui->gridLayout->addWidget(status_label, 5, 0);
    param_struct.status = add_line_edit("IDLE");
    ui->gridLayout->addWidget(param_struct.status, 5, 1);
    expert_struct.status = add_line_edit("0x0");
    ui->gridLayout->addWidget(expert_struct.status, 5, 2);

    reload_checkbox = add_checkbox("AutoRefresh");
    ui->gridLayout_reg->addWidget(reload_checkbox, 0, 2);
    QObject::connect(reload_checkbox, SIGNAL(clicked(bool)), this, SLOT(reload_checkbox_state_changed(bool)));
    reload_checkbox->setChecked(true);
    reload_checkbox_state_changed(reload_checkbox->checkState());

    for (int i = 1; i < measurement.measurment_map.size(); ++i) {
        QLabel *label = add_label(measurement.measurment_map[i].name, measurement.measurment_map[i].description);
        ui->gridLayout_reg->addWidget(label, i+1, 0);

        param_struct.measure_lines.push_back(add_line_edit("0"));
        param_struct.measure_lines[i - 1]->setReadOnly(true);
        ui->gridLayout_reg->addWidget(param_struct.measure_lines[i - 1], i+1, 1);

        expert_struct.measure_lines.push_back(add_line_edit("0x0"));
        ui->gridLayout_reg->addWidget(expert_struct.measure_lines[i - 1], i+1, 2);
    }

    measure_button = new QPushButton("Read measurements", this);
    connect(measure_button, SIGNAL(clicked()), SLOT(on_action_read_registers_triggered()));
    ui->gridLayout_reg->addWidget(measure_button, 0, 0, 1, 2);
}

void MainWindow::generate_parameters(QJsonObject jObj)
{
    parameters.parameters_init(jObj);

    for (int i = 0; i < parameters.parameters_map.size(); ++i) {
        QLabel* label = add_label(parameters.parameters_map[i].name, parameters.parameters_map[i].description);
        ui->gridLayout_parameters->addWidget(label, i+1, 0);

        param_struct.param_lines.push_back(add_line_edit(""));
        ui->gridLayout_parameters->addWidget(param_struct.param_lines[i], i+1, 1);
        param_struct.param_lines[i]->setReadOnly(false);

        expert_struct.param_lines.push_back(add_line_edit(""));
        ui->gridLayout_parameters->addWidget(expert_struct.param_lines[i], i+1, 2);

        connect(param_struct.param_lines[i], SIGNAL(textEdited(const QString&)), SLOT(text_changed(const QString&)));
        connect(expert_struct.param_lines[i], SIGNAL(textEdited(const QString&)), SLOT(text_changed(const QString&)));
    }
    on_action_default_parameters_triggered();

    param_read_button = new QPushButton("Read parameters", this);
    connect(param_read_button, SIGNAL(clicked()), SLOT(on_action_read_parameters_triggered()));
    ui->gridLayout_parameters->addWidget(param_read_button, 0, 0, 1, 1);

    param_write_button = new QPushButton("Write parameters", this);
    connect(param_write_button, SIGNAL(clicked()), SLOT(on_action_write_parameters_triggered()));
    ui->gridLayout_parameters->addWidget(param_write_button, 0, 1, 1, 1);

    param_default = new QPushButton("Default parameters", this);
    connect(param_default, SIGNAL(clicked()), SLOT(on_action_default_parameters_triggered()));
    ui->gridLayout_parameters->addWidget(param_default, 0, 2, 1, 1);
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
    uint32_t sysid = read_register(SYSID_ADDR);
    int fw_number = param_struct.firmware_combobox->currentIndex();
    uint32_t expected_sysid = qstring2uint(firmware.firmware_map[fw_number].sysid);
    expert_struct.firmware_line->setText("0x" + QString::number(sysid, 16));
    if (expected_sysid != sysid && !expert_checkbox->isChecked()) {
        ui->statusbar->setStyleSheet("color: red");
        ui->statusbar->showMessage("The selected SYSTEM ID doesn't match the one stored in the FPGA");
        ui->action_start->setDisabled(true);
        ui->action_stop->setDisabled(true);
        scen_set_button->setDisabled(true);
        ui->groupBox_2->setDisabled(true);
        ui->groupBox_3->setDisabled(true);
        return;
    } else {
        ui->action_start->setDisabled(false);
        ui->action_stop->setDisabled(false);
        scen_set_button->setDisabled(false);
        ui->groupBox_2->setDisabled(false);
        ui->groupBox_3->setDisabled(false);
    }

    uint32_t control_reg_value = read_register(SCENARIO_CONTROL_ADDR);
    uint32_t status_reg_value = read_register(SCENARIO_STATUS_ADDR);

    register_values_snapshot_t new_values;
    new_values.timestamp = std::chrono::system_clock::now();

    expert_struct.status->setText("0x"+QString::number(status_reg_value, 16));
    expert_struct.scenario_line->setText("0x"+QString::number(control_reg_value, 16));

    int scen_number = (control_reg_value >> 8) & 0xFF;
    int state_number = status_reg_value & 0xFF;
    QString scen_state_str;
    if (scen_number >= 0 && scen_number < scenario.scenario_map.size()) {
        if (param_struct.scenario_combobox->currentIndex() != scen_number)
            param_struct.scenario_combobox->setObjectName(scenario.scenario_map[param_struct.scenario_combobox->currentIndex()].name);
        if (state_number >= 0 && state_number < scenario.scenario_map[scen_number].states.size())
            scen_state_str = scenario.scenario_map[scen_number].states[state_number].toString();
        else
            scen_state_str = QString::number(state_number);
    }

    param_struct.status ->setText(scen_state_str);

    for (int i = 1; i < measurement.measurment_map.size(); ++i) {
        uint32_t reg_addr  = qstring2uint(measurement.measurment_map[i].address);
        uint32_t reg_value = read_register(reg_addr);

        new_values.values[reg_addr] = reg_value;
        expert_struct.measure_lines[i - 1]->setText("0x"+QString::number(reg_value, 16));
    }

//    uint32_t current_fw = param_struct.firmware_combobox->currentIndex();
//    QStringList default_clk = firmware.firmware_map[current_fw].clock.split(" ");
//    clk_multiplier = suffix2val(default_clk[1]);
//    clk = clk_multiplier * qstring2uint(default_clk[0]);

    for (int i = 1; i < measurement.measurment_map.size(); ++i) {
        double multiplier = 0;
        QString suffix = "";
        uint32_t reg_addr  = qstring2uint(measurement.measurment_map[i].address);

        if (measurement.measurment_map[i].name.contains("period")) {
            uint32_t reg_value = new_values.values[reg_addr];
            double clk_period = 1.0/clk;
            double meter_value = reg_value * clk_period;
            suffix = val2suffix(meter_value);
            multiplier = suffix2val(suffix);
            param_struct.measure_lines[i - 1]->setText(QString::number(meter_value/multiplier) + " " + suffix + "s");

//            if ((reg_value < (reg_value - 5)) or (reg_value > (reg_value + 5))) {
//                ui->action_start->setDisabled(true);
//                param_struct.measure_lines[i - 1]->setStyleSheet("color: red");
//            } else {
//                ui->action_start->setDisabled(false);
//                param_struct.measure_lines[i - 1]->setStyleSheet("color: green");
//            }

        }

        if (measurement.measurment_map[i].name.contains("counter")) {
            uint32_t reg_value, reg_old_value;

            reg_value     = new_values.values[reg_addr];
            reg_old_value = previous_measurements.values [reg_addr];
            long diff_value = long(reg_value) - long(reg_old_value);
            if (diff_value < 0)
                diff_value += 0x100000000L;

            double frequency = diff_value * 1000000;
            frequency /= std::chrono::duration_cast<std::chrono::microseconds>( new_values.timestamp - previous_measurements.timestamp  ).count();

            suffix = val2suffix(frequency);
            multiplier = suffix2val(suffix);
            param_struct.measure_lines[i - 1]->setText(QString::number(frequency/multiplier) + " " + suffix + "Hz");

            if (measurement.measurment_map[i].name.contains("FPGA")) {
                if ((frequency < (clk - clk_multiplier/10)) or (frequency > (clk + clk_multiplier/10))) {
                    ui->action_start->setDisabled(true);
                    param_struct.measure_lines[i - 1]->setStyleSheet("color: red");
                } else {
                    ui->action_start->setDisabled(false);
                    param_struct.measure_lines[i - 1]->setStyleSheet("color: green");
                }
            } else ui->action_start->setDisabled(false);
        }
    }

    /* old = new */
    previous_measurements = new_values;
}

void MainWindow::on_action_configure_triggered()
{
    configure_fpga(SYSID_ADDR, param_struct.firmware_combobox->currentIndex());
    QThread::msleep(100);
    uint32_t data = read_register(SYSID_ADDR);
    expert_struct.firmware_line->setText("0x" + QString::number(data, 16));

    uint32_t current_fw = param_struct.firmware_combobox->currentIndex();
    QStringList default_clk = firmware.firmware_map[current_fw].clock.split(" ");
    clk_multiplier = suffix2val(default_clk[1]);
    clk = clk_multiplier * qstring2uint(default_clk[0]);
}

void MainWindow::on_action_set_scenario_triggered()
{
    uint32_t selected_scen = param_struct.scenario_combobox->currentIndex();
    param_struct.scenario_combobox->setObjectName(scenario.scenario_map[selected_scen].name + "(active)");

    uint32_t old_value = read_register(SCENARIO_CONTROL_ADDR);
    write_register(SCENARIO_CONTROL_ADDR, old_value | RESET_MSK);

    uint32_t new_value = selected_scen << 8;
    write_register(SCENARIO_CONTROL_ADDR, new_value | RESET_MSK);

    write_register(SCENARIO_CONTROL_ADDR, new_value & ~RESET_MSK);
}

void MainWindow::text_changed(const QString& str) {
    QString param_value;
    for (int i = 0; i < parameters.parameters_map.size(); ++i) {
        if (!expert_checkbox->isChecked()) {
            param_value = param_struct.param_lines[i]->text();
            auto value_unit = param_value.split(" ");
            uint32_t value = qstring2uint(value_unit[0]);
            double multiplier = 1.0;
            if (value_unit.size() > 1) {
                multiplier = suffix2val(value_unit[1]);
                if (value_unit[1].contains('s'))
                    value *= multiplier*clk;
            }
            expert_struct.param_lines[i]->setText("0x" + QString::number(value, 16));
        } else {
            param_value = expert_struct.param_lines[i]->text();
            double value = qstring2uint(param_value);

            if (parameters.parameters_map[i].default_val.split(" ").size() > 1) {
                value /= clk;
                QString suffix = val2suffix(value);
                double multiplier = suffix2val(suffix);
                param_struct.param_lines[i]->setText(QString::number(value/multiplier) + " " + suffix + "s");
            }
            else
                param_struct.param_lines[i]->setText(QString::number(value));
        }
    }
}

void MainWindow::on_action_read_parameters_triggered()
{
    for (int i = 0; i < parameters.parameters_map.size(); ++i) {
        uint32_t param_addr  = qstring2uint(parameters.parameters_map[i].address);
        uint32_t param_value = read_register(param_addr);

        expert_struct.param_lines[i]->setText("0x" + QString::number(param_value, 16));

        if (parameters.parameters_map[i].default_val.split(" ").size() > 1) {
            double value = param_value;
            value = value/clk;
            QString suffix = val2suffix(value);
            double multiplier = suffix2val(suffix);
            param_struct.param_lines[i]->setText(QString::number(value/multiplier) + " " + suffix + "s");
        }
        else
            param_struct.param_lines[i]->setText(QString::number(param_value));
    }
}

void MainWindow::on_action_write_parameters_triggered()
{
    for (int i = 0; i < parameters.parameters_map.size(); ++i) {
        uint32_t param_addr  = qstring2uint(parameters.parameters_map[i].address);
        QString param_value  = expert_struct.param_lines[i]->text();
        write_register(param_addr, qstring2uint(param_value));
    }
}

void MainWindow::on_action_default_parameters_triggered()
{
    uint32_t current_fw = param_struct.firmware_combobox->currentIndex();
    QStringList default_clk = firmware.firmware_map[current_fw].clock.split(" ");
    clk_multiplier = suffix2val(default_clk[1]);
    clk = clk_multiplier * qstring2uint(default_clk[0]);

    for (int i = 0; i < parameters.parameters_map.size(); ++i) {
        param_struct.param_lines[i]->setText(parameters.parameters_map[i].default_val);

        auto value_unit = parameters.parameters_map[i].default_val.split(" ");
        uint32_t value = qstring2uint(value_unit[0]);
        QString  unit  = (value_unit.size() > 1 ? value_unit[1] : QString());
        double multiplier = suffix2val(unit);
        if (unit.contains("s"))
            value *= multiplier*clk;
        else
            value = value;
        expert_struct.param_lines[i]->setText("0x" + QString::number(value, 16));
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
    on_action_read_registers_triggered();
}

void MainWindow::reload_checkbox_state_changed(bool checked) {
    if (checked) {
        timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
        timer->start(TIMEOUT);
    } else
        timer->stop();
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
            param_struct.param_lines[i]->setDisabled(true);
            expert_struct.param_lines[i]->setDisabled(false);
            param_struct.param_lines[i]->setReadOnly(false);
            expert_struct.param_lines[i]->setReadOnly(false);
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
            param_struct.param_lines[i]->setDisabled(false);
            expert_struct.param_lines[i]->setDisabled(true);
            param_struct.param_lines[i]->setReadOnly(false);
            expert_struct.param_lines[i]->setReadOnly(false);
        }
    }
}

///////////////////////////////////CHANGE//////////////////////////////////////////////////////////////////

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
