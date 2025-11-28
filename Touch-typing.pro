QT       += core gui svg charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    ControlButton.cpp \
    JSONParser.cpp \
    ScrollButtonWidget.cpp \
    SettingsDialog.cpp \
    StatisticsWidget.cpp \
    ToggleSwitch.cpp \  # Добавим новый файл
    TypingInput.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ControlButton.h \
    JSONParser.h \
    ScrollButtonWidget.h \
    SettingsDialog.h \
    StatisticsWidget.h \
    ToggleSwitch.h \  # Добавим новый заголовок
    TypingInput.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
