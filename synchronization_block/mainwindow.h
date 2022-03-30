#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./json_config.h"
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

    void closeEvent(QCloseEvent *event);
    void generate(QJsonObject jObj);
    void generate_reg(QJsonObject jObj);
    void generate_parameters(QJsonObject jObj);

    QCheckBox* expert_checkbox;

    QLabel* firmware_label;
    QComboBox* firmware_combobox;

    QLabel* scenario_label;
    QComboBox* scenario_combobox;

    struct expert_param_struct {
        QLineEdit* firmware_line;
        QLineEdit* scenario_line;
        QSpinBox* reg_spinboxes[5];
        QSpinBox* param_spinboxes[5];
    };

    expert_param_struct expert_struct;

private slots:
    void on_action_open_file_triggered();
    void on_action_save_file_triggered();

    void on_action_configure_triggered();

    void on_action_start_triggered();
    void on_action_stop_triggered();

    void expert_checkbox_state_changed(bool checked);

private:
    Ui::MainWindow *ui;
    QJsonObject _currentJsonObject;
};
#endif // MAINWINDOW_H
