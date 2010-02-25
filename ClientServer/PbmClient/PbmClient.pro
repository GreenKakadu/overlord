# -------------------------------------------------
# Project created by QtCreator 2009-07-12T19:20:00
# -------------------------------------------------
QT += network
TARGET = PbmClient
TEMPLATE = app
SOURCES += main.cpp \
    MainClientWindow.cpp \
    ../PbmCommon/Led.cpp \
    ../PbmCommon/FileParser.cpp \
    ../PbmCommon/PbmMessage.cpp \
    ConnectionController.cpp \
    ../PbmCommon/Common.cpp
HEADERS += MainClientWindow.h \
    ../PbmCommon/Led.h \
    ../PbmCommon/FileParser.h \
    ../PbmCommon/PbmMessage.h \
    ConnectionController.h \
    ../PbmCommon/Common.h
FORMS += MainClientWindow.ui \
    ConfigWin.ui \
    askReportDialog.ui
RESOURCES += ../PbmCommon/icons.qrc \
    localIcons.qrc
RC_FILE = PbmClient..rc
OTHER_FILES += PbmClient.rc
