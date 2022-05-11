#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./json_config.h"
#include "./client.h"
#include "addresses.h"
#include <chrono>

#include <QSpinBox>
#include <QMainWindow>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QLabel>
#include <QComboBox>
#include <QTimer>
#include <QTime>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Firmware firmware;
    Scenario scenario;
    Parameters parameters;
    Register reg;

    void closeEvent(QCloseEvent *event);
    void generate(QJsonObject jObj);
    void generate_measurements(QJsonObject jObj);
    void generate_parameters(QJsonObject jObj);

    QLabel* firmware_label;
    QLabel* scenario_label;
    QLabel* status_label;

    QCheckBox* expert_checkbox;
    QCheckBox* reload_checkbox;
    QPushButton* measure_button;
    QPushButton* param_read_button;
    QPushButton* param_write_button;
    QPushButton* scen_set_button;
    QPushButton* config_button;

    QTimer *timer;

    struct register_values_snapshot_t {
        std::chrono::time_point<std::chrono::system_clock> timestamp;
        std::map<uint32_t, uint32_t> values;
    };

    register_values_snapshot_t previous_measurements;

    struct exeprt_struct {
        QLineEdit* firmware_line;
        QLineEdit* scenario_line;
        QLineEdit* status;
        QVector<QLineEdit*> measure_lines;
        QVector<QSpinBox*> param_spinboxes;
    };

    exeprt_struct expert_struct;

    struct param_struct {
        QComboBox* firmware_combobox;
        QComboBox* scenario_combobox;
        QLineEdit* status;
        QVector<QLineEdit*> measure_lines;
        QVector<QSpinBox*> param_spinboxes;
    };

    param_struct param_struct;

    QLabel* add_label(QString name, QString description);
    QLineEdit* add_line_edit(QString text);
    QSpinBox* add_spinbox(int value);
    QCheckBox* add_checkbox(QString text);

    QJsonObject open_file_JSON(QString file_name);

    void exec_reg_command(CMD_Packet request, CMD_Packet &reply);

    uint32_t read_register(uint32_t address);
    void write_register(uint32_t address, uint32_t value);
    void configure_fpga(uint32_t address, uint32_t firmware_version);

    void read_register_values();

private slots:
    void on_action_open_file_triggered();
    void on_action_save_file_triggered();
    void on_action_configure_triggered();
    void on_action_read_registers_triggered();
    void on_action_set_scenario_triggered();
    void on_action_read_parameters_triggered();
    void on_action_wtite_parameters_triggered();
    void on_action_start_triggered();
    void on_action_stop_triggered();

    void expert_checkbox_state_changed(bool checked);
    void reload_checkbox_state_changed(bool checked);
    void timer_timeout();

private:
    Ui::MainWindow *ui;
    QJsonObject _currentJsonObject;
};
#endif // MAINWINDOW_H
