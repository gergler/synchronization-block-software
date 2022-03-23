/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_save_file;
    QAction *action_configure;
    QAction *action_start;
    QAction *action_stop;
    QAction *action_open_file;
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QComboBox *firmware_version_combobox;
    QSpinBox *spinBox_8;
    QLabel *fg_impulse_counter_label;
    QSpinBox *spinBox_7;
    QLineEdit *scenario_line_edit;
    QSpinBox *spinBox_9;
    QSpinBox *spinBox_10;
    QLabel *trigger_counter_label;
    QSpinBox *expert_detector_wait_timeout_spinBox;
    QSpinBox *spinBox_17;
    QLabel *fg_period_label;
    QSpinBox *spinBox_4;
    QLabel *scenario_label;
    QLabel *fpga_clock_counter_label;
    QSpinBox *spinBox_16;
    QSpinBox *spinBox_2;
    QLabel *phase_impulse_counter_label;
    QLineEdit *firmware_version_line_edit;
    QComboBox *scenario_combobox;
    QLabel *phase_shift_label;
    QSpinBox *spinBox_6;
    QSpinBox *spinBox_12;
    QLabel *trigger_lenght_label;
    QSpinBox *spinBox_18;
    QLabel *current_state_label;
    QSpinBox *spinBox_15;
    QLineEdit *current_state_line_edit;
    QLabel *phase_period_label;
    QSpinBox *spinBox_5;
    QLabel *firmware_version_label;
    QSpinBox *spinBox_13;
    QSpinBox *detector_wait_timeout_spinBox;
    QSpinBox *spinBox_20;
    QSpinBox *spinBox_19;
    QLabel *fg_opto_to_open_delay_label;
    QSpinBox *spinBox_14;
    QSpinBox *spinBox_3;
    QCheckBox *expert_mode_checkbox;
    QLabel *detector_wait_timeout_label;
    QLineEdit *current_state_line_edit_2;
    QMenuBar *menubar;
    QMenu *menuSynchronization_block_software;
    QMenu *menuExperiment;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(452, 658);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(452, 658));
        MainWindow->setMaximumSize(QSize(16777215, 16777215));
        MainWindow->setContextMenuPolicy(Qt::NoContextMenu);
        action_save_file = new QAction(MainWindow);
        action_save_file->setObjectName(QString::fromUtf8("action_save_file"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/rec/img/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_save_file->setIcon(icon);
        action_configure = new QAction(MainWindow);
        action_configure->setObjectName(QString::fromUtf8("action_configure"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/rec/img/configure.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_configure->setIcon(icon1);
        action_start = new QAction(MainWindow);
        action_start->setObjectName(QString::fromUtf8("action_start"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/rec/img/start.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_start->setIcon(icon2);
        action_stop = new QAction(MainWindow);
        action_stop->setObjectName(QString::fromUtf8("action_stop"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/rec/img/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_stop->setIcon(icon3);
        action_open_file = new QAction(MainWindow);
        action_open_file->setObjectName(QString::fromUtf8("action_open_file"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/rec/img/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_open_file->setIcon(icon4);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 431, 571));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        firmware_version_combobox = new QComboBox(layoutWidget);
        firmware_version_combobox->addItem(QString());
        firmware_version_combobox->setObjectName(QString::fromUtf8("firmware_version_combobox"));

        gridLayout->addWidget(firmware_version_combobox, 2, 1, 1, 2);

        spinBox_8 = new QSpinBox(layoutWidget);
        spinBox_8->setObjectName(QString::fromUtf8("spinBox_8"));
        spinBox_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_8->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout->addWidget(spinBox_8, 12, 1, 1, 2);

        fg_impulse_counter_label = new QLabel(layoutWidget);
        fg_impulse_counter_label->setObjectName(QString::fromUtf8("fg_impulse_counter_label"));

        gridLayout->addWidget(fg_impulse_counter_label, 13, 0, 1, 1);

        spinBox_7 = new QSpinBox(layoutWidget);
        spinBox_7->setObjectName(QString::fromUtf8("spinBox_7"));
        spinBox_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_7->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout->addWidget(spinBox_7, 13, 1, 1, 2);

        scenario_line_edit = new QLineEdit(layoutWidget);
        scenario_line_edit->setObjectName(QString::fromUtf8("scenario_line_edit"));

        gridLayout->addWidget(scenario_line_edit, 5, 3, 1, 1);

        spinBox_9 = new QSpinBox(layoutWidget);
        spinBox_9->setObjectName(QString::fromUtf8("spinBox_9"));
        spinBox_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_9->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout->addWidget(spinBox_9, 11, 1, 1, 2);

        spinBox_10 = new QSpinBox(layoutWidget);
        spinBox_10->setObjectName(QString::fromUtf8("spinBox_10"));
        spinBox_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_10->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout->addWidget(spinBox_10, 10, 1, 1, 2);

        trigger_counter_label = new QLabel(layoutWidget);
        trigger_counter_label->setObjectName(QString::fromUtf8("trigger_counter_label"));

        gridLayout->addWidget(trigger_counter_label, 18, 0, 1, 1);

        expert_detector_wait_timeout_spinBox = new QSpinBox(layoutWidget);
        expert_detector_wait_timeout_spinBox->setObjectName(QString::fromUtf8("expert_detector_wait_timeout_spinBox"));
        expert_detector_wait_timeout_spinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        expert_detector_wait_timeout_spinBox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

        gridLayout->addWidget(expert_detector_wait_timeout_spinBox, 19, 3, 1, 1);

        spinBox_17 = new QSpinBox(layoutWidget);
        spinBox_17->setObjectName(QString::fromUtf8("spinBox_17"));
        spinBox_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_17->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

        gridLayout->addWidget(spinBox_17, 13, 3, 1, 1);

        fg_period_label = new QLabel(layoutWidget);
        fg_period_label->setObjectName(QString::fromUtf8("fg_period_label"));

        gridLayout->addWidget(fg_period_label, 14, 0, 1, 1);

        spinBox_4 = new QSpinBox(layoutWidget);
        spinBox_4->setObjectName(QString::fromUtf8("spinBox_4"));
        spinBox_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(spinBox_4, 16, 1, 1, 2);

        scenario_label = new QLabel(layoutWidget);
        scenario_label->setObjectName(QString::fromUtf8("scenario_label"));

        gridLayout->addWidget(scenario_label, 5, 0, 1, 1);

        fpga_clock_counter_label = new QLabel(layoutWidget);
        fpga_clock_counter_label->setObjectName(QString::fromUtf8("fpga_clock_counter_label"));

        gridLayout->addWidget(fpga_clock_counter_label, 10, 0, 1, 1);

        spinBox_16 = new QSpinBox(layoutWidget);
        spinBox_16->setObjectName(QString::fromUtf8("spinBox_16"));
        spinBox_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_16->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

        gridLayout->addWidget(spinBox_16, 14, 3, 1, 1);

        spinBox_2 = new QSpinBox(layoutWidget);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(spinBox_2, 17, 1, 1, 2);

        phase_impulse_counter_label = new QLabel(layoutWidget);
        phase_impulse_counter_label->setObjectName(QString::fromUtf8("phase_impulse_counter_label"));

        gridLayout->addWidget(phase_impulse_counter_label, 11, 0, 1, 1);

        firmware_version_line_edit = new QLineEdit(layoutWidget);
        firmware_version_line_edit->setObjectName(QString::fromUtf8("firmware_version_line_edit"));

        gridLayout->addWidget(firmware_version_line_edit, 2, 3, 1, 1);

        scenario_combobox = new QComboBox(layoutWidget);
        scenario_combobox->addItem(QString());
        scenario_combobox->addItem(QString());
        scenario_combobox->setObjectName(QString::fromUtf8("scenario_combobox"));

        gridLayout->addWidget(scenario_combobox, 5, 1, 1, 2);

        phase_shift_label = new QLabel(layoutWidget);
        phase_shift_label->setObjectName(QString::fromUtf8("phase_shift_label"));

        gridLayout->addWidget(phase_shift_label, 16, 0, 1, 1);

        spinBox_6 = new QSpinBox(layoutWidget);
        spinBox_6->setObjectName(QString::fromUtf8("spinBox_6"));
        spinBox_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_6->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout->addWidget(spinBox_6, 14, 1, 1, 2);

        spinBox_12 = new QSpinBox(layoutWidget);
        spinBox_12->setObjectName(QString::fromUtf8("spinBox_12"));
        spinBox_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_12->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

        gridLayout->addWidget(spinBox_12, 18, 3, 1, 1);

        trigger_lenght_label = new QLabel(layoutWidget);
        trigger_lenght_label->setObjectName(QString::fromUtf8("trigger_lenght_label"));

        gridLayout->addWidget(trigger_lenght_label, 17, 0, 1, 1);

        spinBox_18 = new QSpinBox(layoutWidget);
        spinBox_18->setObjectName(QString::fromUtf8("spinBox_18"));
        spinBox_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_18->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

        gridLayout->addWidget(spinBox_18, 12, 3, 1, 1);

        current_state_label = new QLabel(layoutWidget);
        current_state_label->setObjectName(QString::fromUtf8("current_state_label"));

        gridLayout->addWidget(current_state_label, 6, 0, 1, 1);

        spinBox_15 = new QSpinBox(layoutWidget);
        spinBox_15->setObjectName(QString::fromUtf8("spinBox_15"));
        spinBox_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_15->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

        gridLayout->addWidget(spinBox_15, 15, 3, 1, 1);

        current_state_line_edit = new QLineEdit(layoutWidget);
        current_state_line_edit->setObjectName(QString::fromUtf8("current_state_line_edit"));

        gridLayout->addWidget(current_state_line_edit, 6, 3, 1, 1);

        phase_period_label = new QLabel(layoutWidget);
        phase_period_label->setObjectName(QString::fromUtf8("phase_period_label"));

        gridLayout->addWidget(phase_period_label, 12, 0, 1, 1);

        spinBox_5 = new QSpinBox(layoutWidget);
        spinBox_5->setObjectName(QString::fromUtf8("spinBox_5"));
        spinBox_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(spinBox_5, 15, 1, 1, 2);

        firmware_version_label = new QLabel(layoutWidget);
        firmware_version_label->setObjectName(QString::fromUtf8("firmware_version_label"));

        gridLayout->addWidget(firmware_version_label, 2, 0, 1, 1);

        spinBox_13 = new QSpinBox(layoutWidget);
        spinBox_13->setObjectName(QString::fromUtf8("spinBox_13"));
        spinBox_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_13->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

        gridLayout->addWidget(spinBox_13, 17, 3, 1, 1);

        detector_wait_timeout_spinBox = new QSpinBox(layoutWidget);
        detector_wait_timeout_spinBox->setObjectName(QString::fromUtf8("detector_wait_timeout_spinBox"));
        detector_wait_timeout_spinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(detector_wait_timeout_spinBox, 19, 1, 1, 2);

        spinBox_20 = new QSpinBox(layoutWidget);
        spinBox_20->setObjectName(QString::fromUtf8("spinBox_20"));
        spinBox_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_20->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

        gridLayout->addWidget(spinBox_20, 10, 3, 1, 1);

        spinBox_19 = new QSpinBox(layoutWidget);
        spinBox_19->setObjectName(QString::fromUtf8("spinBox_19"));
        spinBox_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_19->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

        gridLayout->addWidget(spinBox_19, 11, 3, 1, 1);

        fg_opto_to_open_delay_label = new QLabel(layoutWidget);
        fg_opto_to_open_delay_label->setObjectName(QString::fromUtf8("fg_opto_to_open_delay_label"));

        gridLayout->addWidget(fg_opto_to_open_delay_label, 15, 0, 1, 1);

        spinBox_14 = new QSpinBox(layoutWidget);
        spinBox_14->setObjectName(QString::fromUtf8("spinBox_14"));
        spinBox_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_14->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

        gridLayout->addWidget(spinBox_14, 16, 3, 1, 1);

        spinBox_3 = new QSpinBox(layoutWidget);
        spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));
        spinBox_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(spinBox_3, 18, 1, 1, 2);

        expert_mode_checkbox = new QCheckBox(layoutWidget);
        expert_mode_checkbox->setObjectName(QString::fromUtf8("expert_mode_checkbox"));
        expert_mode_checkbox->setCheckable(true);
        expert_mode_checkbox->setChecked(true);

        gridLayout->addWidget(expert_mode_checkbox, 0, 3, 1, 1);

        detector_wait_timeout_label = new QLabel(layoutWidget);
        detector_wait_timeout_label->setObjectName(QString::fromUtf8("detector_wait_timeout_label"));

        gridLayout->addWidget(detector_wait_timeout_label, 19, 0, 1, 1);

        current_state_line_edit_2 = new QLineEdit(layoutWidget);
        current_state_line_edit_2->setObjectName(QString::fromUtf8("current_state_line_edit_2"));

        gridLayout->addWidget(current_state_line_edit_2, 6, 1, 1, 2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 452, 22));
        menuSynchronization_block_software = new QMenu(menubar);
        menuSynchronization_block_software->setObjectName(QString::fromUtf8("menuSynchronization_block_software"));
        menuExperiment = new QMenu(menubar);
        menuExperiment->setObjectName(QString::fromUtf8("menuExperiment"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuSynchronization_block_software->menuAction());
        menubar->addAction(menuExperiment->menuAction());
        menuSynchronization_block_software->addAction(action_open_file);
        menuSynchronization_block_software->addAction(action_save_file);
        menuExperiment->addAction(action_configure);
        menuExperiment->addAction(action_start);
        menuExperiment->addAction(action_stop);
        toolBar->addAction(action_open_file);
        toolBar->addAction(action_save_file);
        toolBar->addAction(action_configure);
        toolBar->addAction(action_start);
        toolBar->addAction(action_stop);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        action_save_file->setText(QApplication::translate("MainWindow", "Save file", nullptr));
        action_configure->setText(QApplication::translate("MainWindow", "Configure FPGA", nullptr));
#ifndef QT_NO_TOOLTIP
        action_configure->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>FPGA reprogramming with entered data</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        action_start->setText(QApplication::translate("MainWindow", "Start signal", nullptr));
#ifndef QT_NO_TOOLTIP
        action_start->setToolTip(QApplication::translate("MainWindow", "Experiment start signal", nullptr));
#endif // QT_NO_TOOLTIP
        action_stop->setText(QApplication::translate("MainWindow", "Stop signal", nullptr));
#ifndef QT_NO_TOOLTIP
        action_stop->setToolTip(QApplication::translate("MainWindow", "Experiment stop signal", nullptr));
#endif // QT_NO_TOOLTIP
        action_open_file->setText(QApplication::translate("MainWindow", "Open file", nullptr));
        firmware_version_combobox->setItemText(0, QApplication::translate("MainWindow", "2022v1.01", nullptr));

        spinBox_8->setSuffix(QApplication::translate("MainWindow", " ns", nullptr));
        fg_impulse_counter_label->setText(QApplication::translate("MainWindow", "Fast gate impulse\n"
"counter", nullptr));
        spinBox_7->setSuffix(QString());
        spinBox_9->setSuffix(QApplication::translate("MainWindow", " ns", nullptr));
        spinBox_10->setSuffix(QApplication::translate("MainWindow", " Mhz", nullptr));
        trigger_counter_label->setText(QApplication::translate("MainWindow", "Trigger counter", nullptr));
        expert_detector_wait_timeout_spinBox->setSuffix(QString());
        spinBox_17->setSuffix(QApplication::translate("MainWindow", " Hz", nullptr));
        fg_period_label->setText(QApplication::translate("MainWindow", "Fast gate period", nullptr));
        spinBox_4->setSuffix(QApplication::translate("MainWindow", " ns", nullptr));
        scenario_label->setText(QApplication::translate("MainWindow", "Scenario selected", nullptr));
        fpga_clock_counter_label->setText(QApplication::translate("MainWindow", "FPGA Clock counter", nullptr));
        spinBox_16->setSuffix(QString());
        spinBox_2->setSuffix(QApplication::translate("MainWindow", " ns", nullptr));
        phase_impulse_counter_label->setText(QApplication::translate("MainWindow", "Phase impulse\n"
"counter", nullptr));
        scenario_combobox->setItemText(0, QApplication::translate("MainWindow", "EXPERIMENT", nullptr));
        scenario_combobox->setItemText(1, QApplication::translate("MainWindow", "CALIBRATION", nullptr));

        phase_shift_label->setText(QApplication::translate("MainWindow", "Phase shift", nullptr));
        spinBox_6->setSuffix(QApplication::translate("MainWindow", " ms", nullptr));
        spinBox_12->setSuffix(QString());
        trigger_lenght_label->setText(QApplication::translate("MainWindow", "Trigger lenght", nullptr));
        spinBox_18->setSuffix(QString());
        current_state_label->setText(QApplication::translate("MainWindow", "Current state", nullptr));
        spinBox_15->setSuffix(QString());
        phase_period_label->setText(QApplication::translate("MainWindow", "Phase period ", nullptr));
        spinBox_5->setSuffix(QApplication::translate("MainWindow", " ms", nullptr));
        firmware_version_label->setText(QApplication::translate("MainWindow", "Firmware version", nullptr));
        spinBox_13->setSuffix(QString());
        detector_wait_timeout_spinBox->setSuffix(QApplication::translate("MainWindow", " ns", nullptr));
        spinBox_20->setSuffix(QString());
        spinBox_19->setSuffix(QApplication::translate("MainWindow", " Khz", nullptr));
        fg_opto_to_open_delay_label->setText(QApplication::translate("MainWindow", "Fast gate opto to\n"
"opened delay", nullptr));
        spinBox_14->setSuffix(QString());
        spinBox_3->setSuffix(QString());
        expert_mode_checkbox->setText(QApplication::translate("MainWindow", "Expert Mode", nullptr));
        detector_wait_timeout_label->setText(QApplication::translate("MainWindow", "Detector wait\n"
"timeout", nullptr));
        menuSynchronization_block_software->setTitle(QApplication::translate("MainWindow", "Main", nullptr));
        menuExperiment->setTitle(QApplication::translate("MainWindow", "Experiment", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
