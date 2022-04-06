#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./json_config.h"
#include "./client.h"
#include "addresses.h"

#include <QSpinBox>
#include <QMainWindow>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QLabel>
#include <QComboBox>

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
    void generate_reg(QJsonObject jObj);
    void generate_parameters(QJsonObject jObj);

    QLabel* firmware_label;
    QLabel* scenario_label;
    QLabel* state_label;

    struct expert_param_struct {
        QCheckBox* expert_checkbox;
        QLineEdit* firmware_line;
        QLineEdit* scenario_line;
        QLineEdit* current_state;
        QSpinBox* reg_spinboxes[4];
        QSpinBox* param_spinboxes[5];
    };

    expert_param_struct expert_struct;

    struct param_struct {
        QComboBox* firmware_combobox;
        QComboBox* scenario_combobox;
        QLineEdit* current_state;
        QSpinBox* reg_spinboxes[4];
        QSpinBox* param_spinboxes[5];
    };

    param_struct param_struct;

    QLabel* add_label(QString name, QString description);
    QLineEdit* add_line_edit(QString text, int expert_mode=0);
    QSpinBox* add_spinbox(int value, int maximum=10000, int arrow=1, int expert_mode=0);
    QCheckBox* add_checkbox(QString text);

    QJsonObject open_file_JSON(QString file_name);

    void exec_reg_command(CMD_Packet request, CMD_Packet &reply);

    uint32_t read_register(uint32_t address);
    void write_register(uint32_t value, uint32_t address);
    void configure_register(uint32_t firmware_version, uint32_t address=0x1000);

    void read_register_values();

private slots:
    void on_action_open_file_triggered();
    void on_action_save_file_triggered();

    void on_action_configure_triggered();

    void on_action_start_triggered();
    void on_action_stop_triggered();

    void expert_checkbox_state_changed(bool checked);

    void on_action_read_registers_triggered();

private:
    Ui::MainWindow *ui;
    QJsonObject _currentJsonObject;
};
#endif // MAINWINDOW_H
