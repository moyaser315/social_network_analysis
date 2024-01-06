QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    helpers.cpp \
    jsonwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    ../parsing/parse.cpp \
    ../to_json/to_json.cpp \
    ../error_handling/handling.cpp \
    ../compression/huffman.cpp \
    ../xml_tree/tree.cpp \
    ../network_analysis/network.cpp \
    netwindow.cpp \
    postswindow.cpp

HEADERS += \
    helpers.h \
    jsonwindow.h \
    mainwindow.h \
    ../parsing/parse.h \
    ../to_json/to_json.h \
    ../error_handling/handling.h \
    ../compression/huffman.h \
    ../xml_tree/tree.h \
    ../network_analysis/network.h \
    netwindow.h \
    postswindow.h

FORMS += \
    jsonwindow.ui \
    mainwindow.ui \
    netwindow.ui \
    postswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
