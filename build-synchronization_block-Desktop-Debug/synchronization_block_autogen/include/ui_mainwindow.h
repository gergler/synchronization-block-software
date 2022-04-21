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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
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
    QAction *action_read_registers;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_reg;
    QGridLayout *gridLayout_parameters;
    QMenuBar *menubar;
    QMenu *menuSynchronization_block_software;
    QMenu *menuExperiment;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(381, 475);
        MainWindow->setMinimumSize(QSize(0, 0));
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
        action_read_registers = new QAction(MainWindow);
        action_read_registers->setObjectName(QString::fromUtf8("action_read_registers"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/rec/img/read.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_read_registers->setIcon(icon5);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetNoConstraint);

        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        gridLayout_reg = new QGridLayout();
        gridLayout_reg->setObjectName(QString::fromUtf8("gridLayout_reg"));
        gridLayout_reg->setSizeConstraint(QLayout::SetNoConstraint);

        gridLayout_2->addLayout(gridLayout_reg, 1, 0, 1, 1);

        gridLayout_parameters = new QGridLayout();
        gridLayout_parameters->setObjectName(QString::fromUtf8("gridLayout_parameters"));
        gridLayout_parameters->setSizeConstraint(QLayout::SetNoConstraint);

        gridLayout_2->addLayout(gridLayout_parameters, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 381, 22));
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
        menuExperiment->addAction(action_read_registers);
        toolBar->addAction(action_read_registers);
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
#ifndef QT_NO_TOOLTIP
        action_open_file->setToolTip(QApplication::translate("MainWindow", "Open JSON file", nullptr));
#endif // QT_NO_TOOLTIP
        action_read_registers->setText(QApplication::translate("MainWindow", "Read registers", nullptr));
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