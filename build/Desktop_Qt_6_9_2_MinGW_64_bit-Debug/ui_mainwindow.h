/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <ControlButton.h>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <TypingInput.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout_11;
    QSpacerItem *horizontalSpacer_2;
    QGridLayout *gridLayout_12;
    QGridLayout *gridLayout_18;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QGridLayout *gridLayout_13;
    QGridLayout *gridLayout_8;
    QLabel *nameLabel;
    ControlButton *infoButton;
    QGridLayout *gridLayout_19;
    ControlButton *settingsButton;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_17;
    QGridLayout *gridLayout_14;
    QGridLayout *gridLayout_16;
    QPushButton *learnButton;
    QPushButton *trainButton;
    QGridLayout *gridLayout_15;
    QPushButton *pushButton_10;
    QSpacerItem *verticalSpacer_2;
    TypingInput *typingInput;
    QWidget *scrollAreaWidgetContents;
    QWidget *page_2;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 622);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-color: #201112;\n"
"}\n"
""));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        page = new QWidget();
        page->setObjectName("page");
        gridLayout_4 = new QGridLayout(page);
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 0, 0, 1, 1);

        gridLayout_11 = new QGridLayout();
        gridLayout_11->setObjectName("gridLayout_11");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_11->addItem(horizontalSpacer_2, 0, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_11, 0, 2, 1, 1);

        gridLayout_12 = new QGridLayout();
        gridLayout_12->setObjectName("gridLayout_12");
        gridLayout_18 = new QGridLayout();
        gridLayout_18->setObjectName("gridLayout_18");
        pushButton_7 = new QPushButton(page);
        pushButton_7->setObjectName("pushButton_7");

        gridLayout_18->addWidget(pushButton_7, 0, 0, 1, 1);

        pushButton_8 = new QPushButton(page);
        pushButton_8->setObjectName("pushButton_8");

        gridLayout_18->addWidget(pushButton_8, 0, 1, 1, 1);

        pushButton_9 = new QPushButton(page);
        pushButton_9->setObjectName("pushButton_9");

        gridLayout_18->addWidget(pushButton_9, 0, 2, 1, 1);


        gridLayout_12->addLayout(gridLayout_18, 2, 0, 1, 1);

        gridLayout_13 = new QGridLayout();
        gridLayout_13->setObjectName("gridLayout_13");
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName("gridLayout_8");
        nameLabel = new QLabel(page);
        nameLabel->setObjectName("nameLabel");
        nameLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"        font-family: 'Helvetica Neue', 'Arial', sans-serif;\n"
"        font-size: 34px;\n"
"        font-weight: 400;\n"
"        color: #2c3e50;\n"
"        text-align: center;\n"
"        padding: 30px;\n"
"        background: transparent;\n"
"        text-transform: uppercase;\n"
"        letter-spacing: 3px;\n"
"    }"));

        gridLayout_8->addWidget(nameLabel, 0, 0, 1, 1);


        gridLayout_13->addLayout(gridLayout_8, 0, 3, 1, 1);

        infoButton = new ControlButton(page);
        infoButton->setObjectName("infoButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/res/infoButton.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        infoButton->setIcon(icon);

        gridLayout_13->addWidget(infoButton, 0, 0, 1, 1);

        gridLayout_19 = new QGridLayout();
        gridLayout_19->setObjectName("gridLayout_19");
        settingsButton = new ControlButton(page);
        settingsButton->setObjectName("settingsButton");
        settingsButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/res/settingsButton.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        settingsButton->setIcon(icon1);

        gridLayout_19->addWidget(settingsButton, 0, 0, 1, 1);


        gridLayout_13->addLayout(gridLayout_19, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_13->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        gridLayout_13->setColumnStretch(0, 1);
        gridLayout_13->setColumnStretch(1, 1);
        gridLayout_13->setColumnStretch(2, 6);
        gridLayout_13->setColumnStretch(3, 3);

        gridLayout_12->addLayout(gridLayout_13, 0, 0, 1, 1);

        gridLayout_17 = new QGridLayout();
        gridLayout_17->setObjectName("gridLayout_17");

        gridLayout_12->addLayout(gridLayout_17, 3, 0, 1, 1);

        gridLayout_14 = new QGridLayout();
        gridLayout_14->setObjectName("gridLayout_14");

        gridLayout_12->addLayout(gridLayout_14, 5, 0, 1, 1);

        gridLayout_16 = new QGridLayout();
        gridLayout_16->setObjectName("gridLayout_16");
        learnButton = new QPushButton(page);
        learnButton->setObjectName("learnButton");

        gridLayout_16->addWidget(learnButton, 0, 1, 1, 1);

        trainButton = new QPushButton(page);
        trainButton->setObjectName("trainButton");

        gridLayout_16->addWidget(trainButton, 0, 0, 1, 1);


        gridLayout_12->addLayout(gridLayout_16, 1, 0, 1, 1);

        gridLayout_15 = new QGridLayout();
        gridLayout_15->setObjectName("gridLayout_15");
        pushButton_10 = new QPushButton(page);
        pushButton_10->setObjectName("pushButton_10");

        gridLayout_15->addWidget(pushButton_10, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_15->addItem(verticalSpacer_2, 2, 0, 1, 1);

        typingInput = new TypingInput(page);
        typingInput->setObjectName("typingInput");
        typingInput->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 624, 136));
        typingInput->setWidget(scrollAreaWidgetContents);

        gridLayout_15->addWidget(typingInput, 0, 0, 1, 1);

        gridLayout_15->setRowStretch(0, 1);
        gridLayout_15->setRowStretch(1, 1);
        gridLayout_15->setRowStretch(2, 1);

        gridLayout_12->addLayout(gridLayout_15, 4, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_12, 0, 1, 1, 1);

        gridLayout_4->setColumnStretch(0, 1);
        gridLayout_4->setColumnStretch(1, 6);
        gridLayout_4->setColumnStretch(2, 1);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        gridLayout_6 = new QGridLayout(page_2);
        gridLayout_6->setObjectName("gridLayout_6");
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName("gridLayout_5");

        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        stackedWidget->addWidget(page_2);

        gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 900, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        pushButton_9->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        nameLabel->setText(QCoreApplication::translate("MainWindow", "Bebeb", nullptr));
        infoButton->setText(QString());
        settingsButton->setText(QString());
        learnButton->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\321\203\321\207\320\265\320\275\320\270\320\265", nullptr));
        trainButton->setText(QCoreApplication::translate("MainWindow", "\320\242\321\200\320\265\320\275\320\270\321\200\320\276\320\262\320\272\320\260", nullptr));
        pushButton_10->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
