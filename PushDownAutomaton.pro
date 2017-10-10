#-------------------------------------------------
#
# Project created by QtCreator 2017-09-25T08:21:05
#
# Written by: Leonardo Ospino & Leonardo Burgos
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PushDownAutomaton
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        dialog.cpp \
    graphwidget.cpp \
    edge.cpp \
    node.cpp \
    pushdownautomaton.cpp \
    state.cpp

HEADERS += \
        dialog.h \
    condition.h \
    state.h \
    pushDownAutomaton.h \
    graphwidget.h \
    edge.h \
    node.h

FORMS += \
        dialog.ui

RC_ICONS = pda.ico

QMAKE_CXXFLAGS += -std=gnu++14
