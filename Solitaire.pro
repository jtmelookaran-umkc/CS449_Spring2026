QT       += core gui widgets

TARGET   = Solitaire
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    src/Board.cpp \
    src/BoardTypes.cpp \
    src/Game.cpp \
    gui/MainWindow.cpp \
    gui/BoardWidget.cpp \
    main_gui.cpp

HEADERS += \
    src/Board.h \
    src/BoardTypes.h \
    src/Game.h \
    src/GameRecorder.h \
    gui/MainWindow.h \
    gui/BoardWidget.h

INCLUDEPATH += src
