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

QSpinBox* MainWindow::add_spinbox(int value, QString suffix, int maximum, int arrow, int expert_mode) {
    QSpinBox* spinbox = new QSpinBox(this);
    spinbox->setMaximum(maximum);
    spinbox->setValue(value);
    spinbox->setSuffix(suffix);
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

    expert_checkbox = add_checkbox("Expert mode");
    ui->gridLayout->addWidget(expert_checkbox, 0, 3);
    QObject::connect(expert_checkbox, SIGNAL(clicked(bool)), this, SLOT(expert_checkbox_state_changed(bool)));

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

    status_label = add_label(reg.register_struct_array[0].register_name, reg.register_struct_array[0].register_description);
    ui->gridLayout->addWidget(status_label, 3, 1);
    param_struct.status = add_line_edit("IDLE");
    ui->gridLayout->addWidget(param_struct.status, 3, 2);
    expert_struct.status = add_line_edit("", 1);
    ui->gridLayout->addWidget(expert_struct.status, 3, 3);

    reload_checkbox = add_checkbox("Autoreload");
    ui->gridLayout_reg->addWidget(reload_checkbox, 0, 2);
    QObject::connect(reload_checkbox, SIGNAL(clicked(bool)), this, SLOT(reload_checkbox_state_changed(bool)));

    for (int i = 1; i < reg.register_array_size; ++i) {
        QLabel *label = add_label(reg.register_struct_array[i].register_name, reg.register_struct_array[i].register_description);
        ui->gridLayout_reg->addWidget(label, i+1, 0);

        param_struct.reg_spinboxes[i - 1] = add_spinbox(reg.register_struct_array[i].register_default_val, reg.register_struct_array[i].register_suffix, INT_MAX, 0);
        param_struct.reg_spinboxes[i - 1]->setReadOnly(true);
        ui->gridLayout_reg->addWidget(param_struct.reg_spinboxes[i - 1], i+1, 1);

        expert_struct.reg_spinboxes[i - 1] = add_spinbox(0, "", INT_MAX, 0, 1);
        expert_struct.reg_spinboxes[i - 1]->setMinimum(INT32_MIN);
        expert_struct.reg_spinboxes[i - 1]->setDisplayIntegerBase(16);
        ui->gridLayout_reg->addWidget(expert_struct.reg_spinboxes[i - 1], i+1, 2);
    }
}

void MainWindow::generate_parameters(QJsonObject jObj)
{
    parameters.parameters_init(jObj);

    for (int i = 0; i < parameters.parameters_array_size; ++i) {
        QLabel* label = add_label(parameters.parameters_struct_array[i].parameter_name, parameters.parameters_struct_array[i].parameter_description);
        ui->gridLayout_parameters->addWidget(label, i+1, 0);

        param_struct.param_spinboxes[i] = add_spinbox(parameters.parameters_struct_array[i].parameter_default_val, parameters.parameters_struct_array[i].parameter_suffix);
        ui->gridLayout_parameters->addWidget(param_struct.param_spinboxes[i], i+1, 1);

        expert_struct.param_spinboxes[i] = add_spinbox(0, "", 10000, 1, 1);
        expert_struct.param_spinboxes[i]->setRange(INT32_MIN, INT32_MAX);
        ui->gridLayout_parameters->addWidget(expert_struct.param_spinboxes[i], i+1, 2);
    }
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

//    for (int i = 0; i < parameters.parameters_array_size; ++i) {
//        expert_struct.param_spinboxes[i]->setValue(read_register((parameters.parameters_struct_array[i].parameter_addr).toUInt()));
//    }
}

void MainWindow::on_action_configure_triggered()
{
    configure_fpga(SYSID_ADDR, param_struct.firmware_combobox->currentIndex());
    QThread::msleep(100);
    uint32_t data = read_register(SYSID_ADDR);
    expert_struct.firmware_line->setText("0x" + QString::number(data, 16));
}

static uint32_t qstring2uint(QString &str) {
    return std::stoul(str.toStdString().c_str(), nullptr, 16);
}

void MainWindow::read_register_values() {
    uint32_t control_reg_value = read_register(SCENARIO_CONTROL_ADDR);
    uint32_t status_reg_value = read_register(SCENARIO_STATUS_ADDR);

    register_values_snapshot_t new_values; // = {std::chrono::system_clock::now(), {0, expert_struct.reg_spinboxes[0]->value()}};
    new_values.timestamp = std::chrono::system_clock::now();

    expert_struct.status->setText("0x"+QString::number( status_reg_value, 16 ));
    expert_struct.scenario_line->setText("0x"+QString::number( control_reg_value, 16 ));

#if 0
    param_struct.status ->setText(QString::number( status_reg_value ));
#else
    int scen_number = (control_reg_value >> 8) & 0xFF;
    int state_number = status_reg_value & 0xFF;
    QString scen_state_str;
    if (scen_number >= 0 && scen_number < scenario.scenario_array_size) {
        if (state_number >= 0 && state_number < scenario.scenario_struct_array[scen_number].scenario_states.size())
            scen_state_str = scenario.scenario_struct_array[scen_number].scenario_states[state_number].toString();
        else
            scen_state_str = QString::number(state_number);
    }


    param_struct.status ->setText(scen_state_str);
#endif

    for (int i = 1; i < reg.register_array_size; ++i) {
        printf("%d json_addr %s\n", i, reg.register_struct_array[i].register_addr.toStdString().c_str() );

        uint32_t reg_addr  = qstring2uint(reg.register_struct_array[i].register_addr);
        uint32_t reg_value = read_register(reg_addr);

        new_values.values [reg_addr] = reg_value;

        printf("%d Addr = %04x, Value = %x\n", i, reg_addr, reg_value);
        param_struct.reg_spinboxes[i - 1]->setValue(reg_value);
        expert_struct.reg_spinboxes[i - 1]->setValue(reg_value);
    }

    /* ui update */

    for (int i = 1; i < reg.register_array_size; ++i) {
        std::string reg_name = reg.register_struct_array[i].register_name.toStdString();
        printf("reg_name = %s \n", reg_name.c_str());

        if (reg_name.find("counter") == std::string::npos)
            continue;

        uint32_t reg_addr  = qstring2uint(reg.register_struct_array[i].register_addr);
        uint32_t reg_value, reg_old_value;

        reg_value     = new_values.values [reg_addr];
        reg_old_value = previous_measurements.values [reg_addr];
        printf("reg_addr = %x, old_value = %d, new_value = %d\n", reg_addr, reg_old_value, reg_value);

        //printf("%d Addr = %04x, Value = %x\n", i, reg_addr, reg_value);
        //param_struct.reg_spinboxes[i - 1]->setValue(reg_value);
        long diff_value = long(reg_value) - long(reg_old_value);
        if (diff_value < 0)
            diff_value += INT32_MAX;

        double frequency = diff_value * 1000000;
        frequency /= std::chrono::duration_cast<std::chrono::microseconds>( new_values.timestamp - previous_measurements.timestamp  ).count();
        printf("frequency = %e\n", frequency);

        param_struct.reg_spinboxes[i - 1]->setValue(frequency);
        param_struct.reg_spinboxes[i - 1]->setSuffix(" Hz");
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

//    uint32_t data = read_register(SCENARIO_STATUS_ADDR);
//    if ((data << 24) == 0) {
//        printf("Addr = %04x, Value = %x\n", SCENARIO_CONTROL_ADDR, data << 24);
//        write_register(SCENARIO_CONTROL_ADDR, old_value & ~RESET_MSK);
//    }

    write_register(SCENARIO_CONTROL_ADDR, new_value & ~RESET_MSK);
}

void MainWindow::on_action_read_parameters_triggered()
{
    for (int i = 0; i < parameters.parameters_array_size; ++i) {
        printf("%d json_addr %s\n", i, parameters.parameters_struct_array[i].parameter_addr.toStdString().c_str() );

        uint32_t param_addr  = qstring2uint(parameters.parameters_struct_array[i].parameter_addr);
        uint32_t param_value = read_register(param_addr);

        printf("%d Addr = %04x, Value = %x\n", i, param_addr, param_value);
        param_struct.param_spinboxes[i]->setValue(param_value);
        expert_struct.param_spinboxes[i]->setValue(param_value);
    }
}

void MainWindow::on_action_wtite_parameters_triggered()
{
    for (int i = 0; i < parameters.parameters_array_size; ++i) {
        printf("%d json_addr %s\n", i, parameters.parameters_struct_array[i].parameter_addr.toStdString().c_str() );

        uint32_t param_addr  = qstring2uint(parameters.parameters_struct_array[i].parameter_addr);
        uint32_t param_value = expert_struct.param_spinboxes[i]->value();
        write_register(param_addr, param_value);

        printf("%d Addr = %04x, Value = %x\n", i, param_addr, param_value);
//        param_struct.param_spinboxes[i]->setValue(param_value);
        //expert_struct.param_spinboxes[i]->setValue(param_value);
    }
}

void MainWindow::on_action_start_triggered()
{
    uint32_t old_value = read_register(SCENARIO_CONTROL_ADDR);
    uint32_t selected_scen = (old_value >> 8) & 0xFF;

    uint32_t selected_scen_old = param_struct.scenario_combobox->currentIndex();

    param_struct.scenario_combobox->setCurrentIndex(selected_scen);

    if (selected_scen != selected_scen_old) {
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

        expert_struct.status->setReadOnly(true);
        expert_struct.status->setStyleSheet("background: lightGray");

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
