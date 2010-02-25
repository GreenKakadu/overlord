/****************************************************************************
                         main.cpp
    Entry point for Pbm Server Application
                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#include <QtCore>
#include <QApplication>
#include "ServerWindow.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ServerWindow serverApp;
    serverApp.show();
    return app.exec();
}
