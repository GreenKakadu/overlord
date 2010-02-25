TEMPLATE = app
TARGET = PbmServer
QT += core \
    gui \
    network
HEADERS += ServerApp.h \
    PbmTcpServer.h \
    PbmConnection.h \
    ../PbmCommon/FileParser.h \
    ../PbmCommon/Led.h \
    ../PbmCommon/PbmMessage.h \
    ConnectionAgent.h \
    ../PbmCommon/Common.h \
    ServerWindow.h
SOURCES += main.cpp \
    ServerApp.cpp \
    PbmTcpServer.cpp \
    PbmConnection.cpp \
    ../PbmCommon/FileParser.cpp \
    ../PbmCommon/Led.cpp \
    ../PbmCommon/PbmMessage.cpp \
    ConnectionAgent.cpp \
    ../PbmCommon/Common.cpp \
    ServerWindow.cpp
FORMS += ServerWindow.ui
RESOURCES += ../PbmCommon/icons.qrc \
    localresources.qrc
