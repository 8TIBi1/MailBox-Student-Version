QT       += core gui
QT       += network
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    head.cpp \
    login.cpp \
    main.cpp \
    register.cpp \
    send.cpp \
    users.cpp

HEADERS += \
    head.h \
    login.h \
    register.h \
    send.h \
    users.h

FORMS += \
    login.ui \
    register.ui \
    send.ui \
    users.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    blue.qrc \
    login.qrc \
    register.qrc \
    send_picture.qrc \
    white.qrc


