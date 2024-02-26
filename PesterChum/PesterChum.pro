QT       += core gui
QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += thread

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    auth.cpp \
    client.cpp \
    coloredmessagedelegate.cpp \
    main.cpp \
    mainwindow.cpp \
    messagemodel.cpp \
    messagethread.cpp \
    userchat.cpp

HEADERS += \
    auth.h \
    client.h \
    coloredmessagedelegate.h \
    mainwindow.h \
    messagemodel.h \
    messagethread.h \
    userchat.h

FORMS += \
    auth.ui \
    mainwindow.ui \
    userchat.ui

LIBS += -lws2_32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    fonts/ChixaDemibold.ttf \
    fonts/vcrosdmonorusbyd.ttf \
    vcrosdmonorusbyd.ttf

RESOURCES += \
    fonts.qrc
