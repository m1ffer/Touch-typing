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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <ScrollButtonWidget.h>
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
    QGridLayout *gridLayout_14;
    QStackedWidget *modesStackedWidget;
    QWidget *inputMode;
    QGridLayout *gridLayout_9;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_10;
    TypingInput *typingInput;
    QGridLayout *gridLayout_17;
    ControlButton *resetButton;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QLabel *timerLabel;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer;
    QWidget *choseMode;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout_15;
    QGridLayout *gridLayout_18;
    ControlButton *closeChoseButton;
    QSpacerItem *horizontalSpacer_6;
    QGridLayout *gridLayout_21;
    ScrollButtonWidget *choseWidget;
    QGridLayout *gridLayout_13;
    QGridLayout *gridLayout_8;
    QLabel *nameLabel;
    ControlButton *infoButton;
    QGridLayout *gridLayout_19;
    ControlButton *settingsButton;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_16;
    QPushButton *learnButton;
    QPushButton *trainButton;
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
        gridLayout_14 = new QGridLayout();
        gridLayout_14->setObjectName("gridLayout_14");
        modesStackedWidget = new QStackedWidget(page);
        modesStackedWidget->setObjectName("modesStackedWidget");
        inputMode = new QWidget();
        inputMode->setObjectName("inputMode");
        gridLayout_9 = new QGridLayout(inputMode);
        gridLayout_9->setObjectName("gridLayout_9");
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName("gridLayout_7");
        gridLayout_10 = new QGridLayout();
        gridLayout_10->setObjectName("gridLayout_10");
        typingInput = new TypingInput(inputMode);
        typingInput->setObjectName("typingInput");

        gridLayout_10->addWidget(typingInput, 0, 0, 1, 1);

        gridLayout_17 = new QGridLayout();
        gridLayout_17->setObjectName("gridLayout_17");
        resetButton = new ControlButton(inputMode);
        resetButton->setObjectName("resetButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/res/resetButton.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        resetButton->setIcon(icon);

        gridLayout_17->addWidget(resetButton, 1, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_17->addItem(horizontalSpacer_4, 1, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_17->addItem(horizontalSpacer_5, 1, 2, 1, 1);

        timerLabel = new QLabel(inputMode);
        timerLabel->setObjectName("timerLabel");
        timerLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: #ffffff; \n"
"    font-size: 16px; \n"
"    font-family: 'Consolas'; \n"
"    background-color: #333333; \n"
"    padding: 5px; \n"
"    border-radius: 3px;\n"
"}"));

        gridLayout_17->addWidget(timerLabel, 0, 1, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_17->addItem(horizontalSpacer_7, 0, 2, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_17->addItem(horizontalSpacer_8, 0, 0, 1, 1);

        gridLayout_17->setColumnStretch(0, 10);
        gridLayout_17->setColumnStretch(1, 1);
        gridLayout_17->setColumnStretch(2, 10);

        gridLayout_10->addLayout(gridLayout_17, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_10->addItem(verticalSpacer, 2, 0, 1, 1);

        gridLayout_10->setRowStretch(0, 5);

        gridLayout_7->addLayout(gridLayout_10, 0, 0, 1, 1);

        gridLayout_7->setRowStretch(0, 1);

        gridLayout_9->addLayout(gridLayout_7, 0, 0, 1, 1);

        modesStackedWidget->addWidget(inputMode);
        choseMode = new QWidget();
        choseMode->setObjectName("choseMode");
        horizontalLayout = new QHBoxLayout(choseMode);
        horizontalLayout->setObjectName("horizontalLayout");
        gridLayout_15 = new QGridLayout();
        gridLayout_15->setObjectName("gridLayout_15");
        gridLayout_18 = new QGridLayout();
        gridLayout_18->setObjectName("gridLayout_18");
        closeChoseButton = new ControlButton(choseMode);
        closeChoseButton->setObjectName("closeChoseButton");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/res/closeButton.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        closeChoseButton->setIcon(icon1);

        gridLayout_18->addWidget(closeChoseButton, 0, 1, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_18->addItem(horizontalSpacer_6, 0, 0, 1, 1);

        gridLayout_18->setColumnStretch(0, 10);
        gridLayout_18->setColumnStretch(1, 1);

        gridLayout_15->addLayout(gridLayout_18, 0, 0, 1, 1);

        gridLayout_21 = new QGridLayout();
        gridLayout_21->setObjectName("gridLayout_21");
        choseWidget = new ScrollButtonWidget(choseMode);
        choseWidget->setObjectName("choseWidget");

        gridLayout_21->addWidget(choseWidget, 0, 0, 1, 1);


        gridLayout_15->addLayout(gridLayout_21, 1, 0, 1, 1);

        gridLayout_15->setRowStretch(0, 1);
        gridLayout_15->setRowStretch(1, 15);

        horizontalLayout->addLayout(gridLayout_15);

        modesStackedWidget->addWidget(choseMode);

        gridLayout_14->addWidget(modesStackedWidget, 0, 0, 1, 1);


        gridLayout_12->addLayout(gridLayout_14, 2, 0, 1, 1);

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
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/res/infoButton.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        infoButton->setIcon(icon2);

        gridLayout_13->addWidget(infoButton, 0, 0, 1, 1);

        gridLayout_19 = new QGridLayout();
        gridLayout_19->setObjectName("gridLayout_19");
        settingsButton = new ControlButton(page);
        settingsButton->setObjectName("settingsButton");
        settingsButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/res/settingsButton.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        settingsButton->setIcon(icon3);

        gridLayout_19->addWidget(settingsButton, 0, 0, 1, 1);


        gridLayout_13->addLayout(gridLayout_19, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_13->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        gridLayout_13->setColumnStretch(0, 1);
        gridLayout_13->setColumnStretch(1, 1);
        gridLayout_13->setColumnStretch(2, 6);
        gridLayout_13->setColumnStretch(3, 3);

        gridLayout_12->addLayout(gridLayout_13, 0, 0, 1, 1);

        gridLayout_16 = new QGridLayout();
        gridLayout_16->setObjectName("gridLayout_16");
        learnButton = new QPushButton(page);
        learnButton->setObjectName("learnButton");

        gridLayout_16->addWidget(learnButton, 0, 1, 1, 1);

        trainButton = new QPushButton(page);
        trainButton->setObjectName("trainButton");

        gridLayout_16->addWidget(trainButton, 0, 0, 1, 1);


        gridLayout_12->addLayout(gridLayout_16, 1, 0, 1, 1);


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
        modesStackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        resetButton->setText(QString());
        timerLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        closeChoseButton->setText(QString());
        nameLabel->setText(QCoreApplication::translate("MainWindow", "Bebeb", nullptr));
        infoButton->setText(QString());
        settingsButton->setText(QString());
        learnButton->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\321\203\321\207\320\265\320\275\320\270\320\265", nullptr));
        trainButton->setText(QCoreApplication::translate("MainWindow", "\320\242\321\200\320\265\320\275\320\270\321\200\320\276\320\262\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
