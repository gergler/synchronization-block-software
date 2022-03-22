#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_save_file_triggered();

    void on_action_help_triggered();

    void on_action_quit_triggered();

    void on_action_configure_triggered();

    void on_action_start_triggered();

    void on_action_stop_triggered();

    void on_expert_mode_checkbox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
